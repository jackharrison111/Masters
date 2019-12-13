#include <TStyle.h>
#include <TCanvas.h>
#include <TH2.h>
#include <vector>
#include <math.h>

Double_t CrystalBall(Double_t *x, Double_t *par){
	Double_t f, A, B, C, D, N;
	A = pow((par[1]/abs(par[0])),par[1])*exp(-pow(abs(par[0]),2)/2);
	B = par[1]/abs(par[0])-abs(par[0]);
	C = par[1]/abs(par[0])*1/(par[1]-1)*exp(-pow(abs(par[0]),2)/2);
	D = sqrt(M_PI/2)*(1+erf(abs(par[0])/sqrt(2)));
	N = 1/(par[3]*(C+D));
	if((x[0]-par[2])/par[3]>-par[0]){
		f = par[4]*N*exp(-(pow(x[0]-par[2],2))/(2*pow(par[3],2)));
	}else{
		f = par[4]*N*A*pow(B-(x[0]-par[2])/par[3],-par[1]);
	}
	return f;
}

Double_t Gaussian(Double_t *x, Double_t *par){
	Double_t g1 = par[2]*exp(-pow(((x[0]-par[0])/par[1]),2)/2);
	Double_t g2 = par[5]*exp(-pow(((x[0]-par[3])/par[4]),2)/2);
	return g1+g2;
}

//lorentzian fit for resonance
Double_t Lorentz(Double_t *x, Double_t *par){
	Double_t L1 = par[1]*par[2]/(pow(x[0]-par[0],2) + pow(0.5*par[1],2));
	//Double_t L2 = par[4]*par[5]/(pow(x[0]-par[3],2) + pow(0.5*par[4],2));
	return L1;//+L2;
}

//fit to resonance background (quadratic/cubic)
Double_t Background(Double_t *x, Double_t *par, Int_t order){
	Double_t B = 0;
	if(order==3){   
		B = par[3] + par[2]*x[0] + par[1]*pow(x[0],2) + par[0]*pow(x[0],3);
	}else if(order==2){
		B = par[2] + par[1]*x[0] + par[0]*pow(x[0],2);
	}else if(order==1){
		B = par[1] + par[0]*x[0];
	}else{
		return 0;
	}
	return B;
}

Int_t order = 1; //global?
//combine
Double_t Fit(Double_t *x, Double_t *par){
	return Gaussian(x, par) + Background(x,&par[6],order);
}

Double_t BackFit(Double_t *x, Double_t *par){
	return Background(x,par,order);

}

Double_t zMass{91.2};
Double_t L_int{10.064}; //fb
Double_t Br_lep{0.03454};


void plot(string product, string histType){
	
	
	vector<string> productNames;
	productNames.push_back("1lep1tau");
	productNames.push_back("2lep");

	//gROOT->SetStyle("ATLAS");
	gStyle->SetOptStat(0);

	TCanvas *c = new TCanvas("c", "c");	
	TLegend *legend = new TLegend(1,0.5);
	TH1D *totalHist = new TH1D("totalHist","Totals",200,0,160);
	

	TFile *f = new TFile("rootOutput/mc_output_Zee_allMC_12-12.root");	//("rootOutput/mc_output.root");
	if(!f->IsOpen()){
		std::cout << "Couldn't open mc_output.root" << std::endl;
	}
	
	//TODO:: ADD EFFICIENCY READING FROM THE PRODUCT DIRECTORY (IE 2lep)

	TVectorD *Eff_Vector= (TVectorD*)f->Get((product+"/Efficiency/efficiency").c_str());
	Double_t mc_Eff;
	if(Eff_Vector != NULL){	
		mc_Eff = (*Eff_Vector)[0];
	}

	string Zlep = "invMassZmumu";
	//for(vector<string>::iterator it = productNames.begin(); it != productNames.end(); it++){
		f->cd((product +"/" + histType).c_str());
		//gDirectory->pwd();
		TIter next(gDirectory->GetListOfKeys());
		TKey *aKey;

		//TH1D *chosenHist = new TH1D;
		
		string signalFile = "mc15_13TeV.363490.Sh_221_NNPDF30NNLO_llll.1lep1tau_raw.root"; 
		int counter{0};
		while((aKey = (TKey*)next())){
			TClass *myClass = gROOT->GetClass(aKey->GetClassName());
			if(!myClass->InheritsFrom("TH1D")){
				std::cout << "Skipping..." << std::endl;
				continue;
			}
			TH1D *myHist = new TH1D;
			myHist = (TH1D*)aKey->ReadObj();
			string histName = myHist->GetName();
			string printChoice = product + "_" + histType + "_" + signalFile;
			myHist->SetDirectory(0);
			totalHist->Add(myHist);
			counter++;
		}
//	}
	f->Close();
	
	Double_t lowerMass=110;
	Double_t higherMass=140;
	TF1 *fit;

	fit = new TF1("fit",BackFit,lowerMass,higherMass,2);
	fit->SetParameters(1,1);
	totalHist->SetTitle(";M_{inv}/GeV;counts/0.8GeV");
	totalHist->Fit("fit","+RN");
	totalHist->Draw("hist");
	Double_t m = fit->GetParameter(0);
	Double_t cons = fit->GetParameter(1);

	Double_t x[200], y[200];
	for(Int_t i{0}; i<200; i++){
		x[i]=0.8*i;
		y[i]=m*x[i]+cons;
	}
	TGraph *g = new TGraph(200,x,y);
	g->SetLineColor(kRed);
	g->SetLineWidth(2);
	g->Draw("same");
	
	Double_t efficiency = mc_Eff;

	Double_t err;
	//Double_t I = totalHist->IntegralAndError(86/0.8,96/0.8,err,"");
	Double_t I = 0;
	Double_t B = 0;
	for(Int_t i{0}; i<200; i++){
		if(0.8*i>=80&&0.8*i<=100){
			I+=totalHist->GetBinContent(i);
			B+=y[i];
		}
		//efficiency-=1/n;
	}
	Double_t N = I-B;
	std::cout << efficiency << " = eff before" << std::endl;	
	
	efficiency = N*efficiency/I; //scale by the background reduction
	
	//efficiency = 0.005; NEEDED TO GET THE RIGHT CROSS SECTION
	Double_t sigma = Br_lep*N/(efficiency*L_int);//fb
	sigma/=1e6;//nb
	
	Double_t I2 = totalHist->Integral(80/0.8,100/0.8);
	
	std::cout<<"I="<<I<<", B="<<B<<"N="<<N<<", eff="<<efficiency<<std::endl;
	std::cout<<"sigma="<<sigma<<" nb"<<std::endl;
	std::cout << "sigma in infofile= 1.95 nb" << std::endl;
	
	
	
	
	//Double_t I = signalHist->IntegralAndError(80/0.8,100/0.8,err,"");
	//Double_t err_tot;
	//Double_t I_tot = totalHist->IntegralAndError(0,200,err_tot,"");
	//Double_t I_tot = signalHist->IntegralAndError(0,200,err_tot,"");
	//Double_t N_sig = (2*I-I_tot)/2;
	//Double_t sigma = pow(2*Br_lep,2)*(N_sig-backIntegral)/(efficiency*L_int);
	//Double_t sigma = pow(Br_lep,1)*N_sig/(efficiency*L_int);
	//Double_t sigma_sigma = sigma*sqrt((pow(err,2)+pow(err_tot,2)/4+pow(background_err,2))/pow(N_sig-backIntegral,2));
	//Double_t sigma_sigma = sigma*sqrt((pow(err,2)+pow(err_tot,2)/4+pow(background_err,2))/pow(N_sig,2));
	//std::cout<<"eff="<<efficiency<<", Br_lep="<<Br_lep<<", N_sig="<<N_sig<<", bkgnd="<<backIntegral<<", L_int="<<L_int<<std::endl;
	//std::cout<<"sigma = "<<sigma/1e3<<" +- "<<sigma_sigma/1e3<<" pb"<<std::endl;
		
	//totalHist->SetTitle(";M_{inv}/GeV; counts/0.8GeV");
	//totalHist->Draw("hist");
}

int plotterZee(){
	plot("2lep","invMassZee");
	return 0;
}
