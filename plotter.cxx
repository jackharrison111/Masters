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
	return g1;//+g2;
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

Int_t order = 2; //global?
//combine
Double_t Fit(Double_t *x, Double_t *par){
	return Gaussian(x, par) + Background(x,&par[3],order);
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
	c->SetTickx();
	c->SetTicky();
	c->SetGridx();
	c->SetGridy();
	TLegend *legend = new TLegend(0.3,0.2);
	legend->SetHeader("Pairings","c");
	legend->SetBorderSize(4);
	legend->SetShadowColor(1);
	legend->SetDrawOption("br");
	TH1D *totalHist1 = new TH1D("totalHist1","",160,0,160);
	TH1D *totalHist2 = new TH1D("totalHist2","",160,0,160);
	TH1D *totalHist3 = new TH1D("totalHist3","",160,0,160);
	

	TFile *f = new TFile("rootOutput/mc_output_all2lep_15-12.root");	//("rootOutput/mc_output.root");
	if(!f->IsOpen()){
		std::cout << "Couldn't open mc_output.root" << std::endl;
	}
	
	//TODO:: ADD EFFICIENCY READING FROM THE PRODUCT DIRECTORY (IE 2lep)

	TVectorD *Eff_Vector= (TVectorD*)f->Get((product+"/Efficiency/efficiency").c_str());
	Double_t mc_Eff;
	if(Eff_Vector != NULL){	
		mc_Eff = (*Eff_Vector)[0];
	}

	//f->cd((product +"/"+histType).c_str());
	f->cd((product +"/"+histType).c_str());
	TIter next1(gDirectory->GetListOfKeys());
	TKey *aKey1;
	while((aKey1 = (TKey*)next1())){
		TClass *myClass = gROOT->GetClass(aKey1->GetClassName());
		if(!myClass->InheritsFrom("TH1D")){
			std::cout << "Skipping..." << std::endl;
			continue;
		}
		TH1D *myHist = new TH1D;
		myHist = (TH1D*)aKey1->ReadObj();
		myHist->SetDirectory(0);
		totalHist1->Add(myHist);
	}

	/*f->cd((product +"/invMass2lR").c_str());
	TIter next2(gDirectory->GetListOfKeys());
	TKey *aKey2;
	while((aKey2 = (TKey*)next2())){
		TClass *myClass = gROOT->GetClass(aKey2->GetClassName());
		if(!myClass->InheritsFrom("TH1D")){
			std::cout << "Skipping..." << std::endl;
			continue;
		}
		TH1D *myHist = new TH1D;
		myHist = (TH1D*)aKey2->ReadObj();
		myHist->SetDirectory(0);
		totalHist2->Add(myHist);
	}

	f->cd((product +"/invMass2lG").c_str());
	TIter next3(gDirectory->GetListOfKeys());
	TKey *aKey3;
	while((aKey3 = (TKey*)next3())){
		TClass *myClass = gROOT->GetClass(aKey3->GetClassName());
		if(!myClass->InheritsFrom("TH1D")){
			std::cout << "Skipping..." << std::endl;
			continue;
		}
		TH1D *myHist = new TH1D;
		myHist = (TH1D*)aKey3->ReadObj();
		myHist->SetDirectory(0);
		totalHist3->Add(myHist);
	}*/
	f->Close();

	Double_t lowerMass=40;
	Double_t higherMass=70;
	Double_t lowerIntegral=81;
	Double_t upperIntegral=101;
	TF1 *fit;

	fit = new TF1("fit",BackFit,lowerMass,higherMass,3);
	fit->SetParameters(1,1,1);
	totalHist1->SetTitle(";M_{ll} [GeV];N / [GeV]");
	totalHist1->SetZTitle("\nN / [GeV^{2}]");
	totalHist1->SetTitleOffset(1.25,"Z");
	totalHist1->Fit("fit","+RN");
	//totalHist1->SetLineColor(kBlack);
	legend->AddEntry(totalHist1,"minimum |M_{ll} - M_{Z}|","l");
	totalHist1->Draw("hist");
	/*totalHist2->SetLineColor(kRed);
	legend->AddEntry(totalHist2,"at least one 86-96 GeV","l");
	totalHist2->Draw("histsame");
	totalHist3->SetLineColor(kGreen);
	legend->AddEntry(totalHist3,"2e2#mu","l");
	totalHist3->Draw("histsame");
	legend->Draw("same");*/

	Double_t a = fit->GetParameter(0);
	Double_t m = fit->GetParameter(1);
	Double_t cons = fit->GetParameter(2);

	Double_t x[160], y[160];
	for(Int_t i{0}; i<160; i++){
		x[i]=1*i;
		y[i]=a*pow(x[i],2)+m*x[i]+cons;
	}
	TGraph *g = new TGraph(160,x,y);
	g->SetLineColor(kRed);
	g->SetLineWidth(2);
	g->Draw("same");
	
	/*Double_t efficiency = mc_Eff;

	Double_t err;
	//Double_t I = totalHist1->IntegralAndError(86/1,96/1,err,"");
	Double_t I = 0;
	Double_t B = 0;
	for(Int_t i{0}; i<160; i++){
		if(1*i>=lowerIntegral&&1*i<=upperIntegral){
			I+=totalHist1->GetBinContent(i);
			B+=y[i];
		}
		//efficiency-=1/n;
	}
	Double_t N = I-B;
	std::cout << efficiency << " = eff before" << std::endl;
	
	efficiency = N*efficiency/I; //scale by the background reduction
	
	//efficiency = 0.005; NEEDED TO GET THE RIGHT CROSS SECTION
	Double_t sigma = N /(efficiency*L_int);//fb
	sigma/=1e6;//nb
	
	Double_t I2 = totalHist1->Integral(lowerIntegral/1,upperIntegral/1);
	
	std::cout<<"I="<<I<<", B="<<B<<"N="<<N<<", eff="<<efficiency<<std::endl;
	std::cout<<"sigma="<<sigma<<" nb"<<std::endl;
	std::cout << "sigma in infofile= 1.9505 nb" << std::endl;
	
	
	
	
	//Double_t I = signalHist->IntegralAndError(80/1,100/1,err,"");
	//Double_t err_tot;
	//Double_t I_tot = totalHist->IntegralAndError(0,160,err_tot,"");
	//Double_t I_tot = signalHist->IntegralAndError(0,160,err_tot,"");
	//Double_t N_sig = (2*I-I_tot)/2;
	//Double_t sigma = pow(2*Br_lep,2)*(N_sig-backIntegral)/(efficiency*L_int);
	//Double_t sigma = pow(Br_lep,1)*N_sig/(efficiency*L_int);
	//Double_t sigma_sigma = sigma*sqrt((pow(err,2)+pow(err_tot,2)/4+pow(background_err,2))/pow(N_sig-backIntegral,2));
	//Double_t sigma_sigma = sigma*sqrt((pow(err,2)+pow(err_tot,2)/4+pow(background_err,2))/pow(N_sig,2));
	//std::cout<<"eff="<<efficiency<<", Br_lep="<<Br_lep<<", N_sig="<<N_sig<<", bkgnd="<<backIntegral<<", L_int="<<L_int<<std::endl;
	//std::cout<<"sigma = "<<sigma/1e3<<" +- "<<sigma_sigma/1e3<<" pb"<<std::endl;
		
	//totalHist->SetTitle(";M_{inv}/GeV; counts/1GeV");
	//totalHist->Draw("hist");*/
}

int plotter(){
	plot("2lep","invMass2l");
	return 0;
}
