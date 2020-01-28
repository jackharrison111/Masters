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
	gROOT->ForceStyle();

	TCanvas *c = new TCanvas("c", "c");
	c->SetTickx();
	c->SetTicky();
	//c->SetGridx();
	//c->SetGridy();
	TLegend *legend = new TLegend(1,0.5);
	/*legend->SetHeader("need to set header","c");
	legend->SetBorderSize(4);
	legend->SetShadowColor(1);
	legend->SetDrawOption("br");*/
	TH1D *totalHist = new TH1D("totalHist","",160,0,160);
	


	TFile *f = new TFile("rootOutput/re_output_tau_28-12.root");	//("rootOutput/mc_output.root");
	if(!f->IsOpen()){
		std::cout << "Couldn't open mc_output.root" << std::endl;
	}
	
	//TODO:: ADD EFFICIENCY READING FROM THE PRODUCT DIRECTORY (IE 2lep)

	TVectorD *Eff_Vector= (TVectorD*)f->Get((product+"/Efficiency/efficiency").c_str());
	Double_t mc_Eff;
	if(Eff_Vector != NULL){	
		mc_Eff = (*Eff_Vector)[0];
	}

	f->cd((product +"/" + histType).c_str());
	//gDirectory->pwd();
	TIter next(gDirectory->GetListOfKeys());
	TKey *aKey;

		
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
			
				myHist->SetDirectory(0);
				myHist->SetLineWidth(1);
				totalHist->Add(myHist);
			
		counter++;
		}
	f->Close();
	
	totalHist->Draw("hist");

	
	Int_t lowerMass=20;
	Int_t higherMass=140;
	TF1 *fit;

	if(order==1){
		fit = new TF1("fit",Fit,lowerMass,higherMass,8);
		fit->SetParameters(91,4,0.75,85,15,0.1,-0.1,0.01);
		fit->SetParNames("mu1","sigma1","A1","mu2","sigma2","A2","m","c");
		fit->SetParLimits(0,89,93);
		fit->SetParLimits(3,60,88);
	}else if(order==2){
		fit = new TF1("fit",Fit,lowerMass,higherMass,9);
		fit->SetParameters(91,5,1,80,5,1,1,1,1);
	}
	totalHist->Draw("hist");
	totalHist->Fit("fit","+RN");
	totalHist->SetTitle(";M_{ll,#tau#tau} [GeV];N / [GeV]");
	
	Int_t nBins=higherMass-lowerMass+1;
	Double_t x[nBins], y1[nBins], y2[nBins], y3[nBins];
	for(Int_t i=lowerMass; i<=higherMass; i++){
		x[i-lowerMass]=i;
		y1[i-lowerMass]=fit->GetParameter(2)*exp(-pow((x[i-lowerMass]-fit->GetParameter(0))/fit->GetParameter(1),2)/2);
		y2[i-lowerMass]=fit->GetParameter(5)*exp(-pow((x[i-lowerMass]-fit->GetParameter(3))/fit->GetParameter(4),2)/2);
		if(order==1){
			y3[i-lowerMass]=fit->GetParameter(6)*x[i-lowerMass]+fit->GetParameter(7);
		}else if(order==2){
			y3[i-lowerMass]=fit->GetParameter(6)*pow(x[i-lowerMass],2)+fit->GetParameter(7)*x[i-lowerMass]+fit->GetParameter(8);
		}
	}
	TGraph *g1 = new TGraph(nBins,x,y1);
	g1->SetLineColor(kBlue);
	g1->SetLineWidth(2);
	//g1->Draw("same");
	TGraph *g2 = new TGraph(nBins,x,y2);
	g2->SetLineColor(kGreen);
	g2->SetLineWidth(2);
	//g2->Draw("same");
	TGraph *g3 = new TGraph(nBins,x,y3);
	g3->SetLineColor(kRed);
	g3->SetLineWidth(2);
	//g3->Draw("same");

	Double_t B=0;
	Double_t err_B=0;
	for(Int_t i=lowerMass; i<=higherMass; i++){
		Double_t xx = i;
		if(order==1){
			Double_t mu, Emu, ep, Eep, A, EA;
			Double_t m = fit->GetParameter(6);
			Double_t Em = fit->GetParError(6);
			Double_t c = fit->GetParameter(7);
			Double_t Ec = fit->GetParError(7);
			if(abs(fit->GetParameter(0)-zMass)<abs(fit->GetParameter(3)-zMass)){
				mu = fit->GetParameter(0);
				Emu = fit->GetParError(0);
				ep = fit->GetParameter(1);
				Eep = fit->GetParError(1);
				A = fit->GetParameter(2);
				EA = fit->GetParError(2);
			}else{
				mu = fit->GetParameter(3);
				Emu = fit->GetParError(3);
				ep = fit->GetParameter(4);
				Eep = fit->GetParError(4);
				A = fit->GetParameter(5);
				EA = fit->GetParError(5);
			}
			B += /*A*exp(-pow((xx-mu)/ep,2)/2) +*/ m*xx + c;
			err_B += /*pow(-A/(2*ep)*pow((xx-mu)/ep,3)*exp(-pow((xx-mu)/ep,2)/2)*Emu,2) + pow(-A/(2*ep)*pow((xx-mu)/ep,4)*exp(-pow((xx-mu)/ep,2)/2)*Eep,2) + pow(exp(-pow((xx-mu)/ep,2)/2)*EA,2) + */pow(xx*Em,2) + pow(Ec,2); //propagating errors
		}else std::cout<<"order==2"<<std::endl;
	}
	err_B=sqrt(err_B);
	
	TH1D *signalHist = new TH1D("signalHist","signalHist",160,0,160);
	for(Int_t i=0; i<=160; i++){
		signalHist->SetBinContent(i+1,totalHist->GetBinContent(i+1)-y2[i]-y3[i]);
		if(signalHist->GetBinContent(i+1)<0) signalHist->SetBinContent(i+1,0);
	}
	
	Double_t efficiency = mc_Eff;

		
	//stat
	Double_t err_I;
	Double_t I = totalHist->IntegralAndError(lowerMass+1,higherMass+1,err_I,"");
	Double_t N = I/2;
	Double_t err_N = N*err_I/I;
	Double_t err_Eff = efficiency*err_N/N;
	Double_t stat_sigma = sqrt(pow(err_N/N,2)+pow(err_Eff/efficiency,2));

	//sys
	//N -= B/2;
	Double_t sys_sigma = err_B/B;
	
	//lumi
	Double_t lumi_sigma = 0.017; //1.7%

	Double_t sigma = N/(efficiency*L_int);
	sigma/=1e3;//pb
	stat_sigma *= sigma;
	sys_sigma *= sigma;
	lumi_sigma *= sigma;
	std::cout<<"I="<<I<<", B="<<B<<", N="<<N<< ", err_N = " << err_N << ", eff="<<efficiency<<std::endl;
	std::cout<<"sigma="<<sigma<<" += "<<stat_sigma<<" (stat) +- "<<sys_sigma<<" (sys) +- "<<lumi_sigma<<" (lumi) pb"<<std::endl;
		
		
	}


	int plottertau(){
		plot("1lep1tau","invMass3lep1tau");
		return 0;
	}
