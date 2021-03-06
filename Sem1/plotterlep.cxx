#include <TStyle.h>
#include <TCanvas.h>
#include <TH2.h>
#include <vector>




Double_t Gaussian(Double_t *x, Double_t *par){
	Double_t g = par[2]*exp(-pow(((x[0]-par[0])/par[1]),2)/2);
	return g;
}

//lorentzian fit for resonance
Double_t Lorentz(Double_t *x, Double_t *par){
	Double_t L = par[1]*par[2]/(pow(x[0]-par[0],2) + pow(0.5*par[1],2));
	return L;
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
	return Lorentz(x, par) + Background(x,&par[3],order);
}

Double_t BackFit(Double_t *x, Double_t *par){
	return Background(x,par,order);

}

Double_t L_int{10.064}; //fb
Double_t Br_lep{0.03454};


void plot(string product, string histType){
	
	
	vector<string> productNames;
	productNames.push_back("1fatjet1lep");	
	productNames.push_back("1lep");
	productNames.push_back("1lep1tau");
	productNames.push_back("1tau");
	productNames.push_back("2lep");
	productNames.push_back("2tau");
	productNames.push_back("GamGam");

	//gROOT->SetStyle("ATLAS");
	gStyle->SetOptStat(0);

	TCanvas *c = new TCanvas("c", "c");
	c->SetTickx();
	c->SetTicky();
	//c->SetGridx();
	//c->SetGridy();
	TLegend *legend = new TLegend(1,0.5);
	//legend->SetHeader("ZZllll","c");
	//legend->SetBorderSize(4);
	//legend->SetShadowColor(1);
	//legend->SetDrawOption("br");
	TH1D *totalHist = new TH1D("totalHist", "Totals", 160, 0, 160);
	
	
	TFile *f = new TFile("rootOutput/mc_output_2lep_15-12.root");	//("rootOutput/mc_output.root");
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
		
		string signalFile = "mc15_13TeV.361106.PwPy8EG_AZNLOCTEQ6L1_Zee.2lep_raw.root"; 
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
			//TODO: dont delete - how to choose a specific hist
			//std::cout<<histName << std::endl;
			
			string printChoice = product + "_" + histType + "_" + signalFile;
			//if(histName == printChoice){
				myHist->SetDirectory(0);
				myHist->SetTitle(";M_{inv}/GeV; Counts/0.8GeV");
				//myHist->SetLineColor(kBlue-4);
				//myHist->SetLineWidth(1);
				//legend->AddEntry(myHist,"Signal channel","l");
				//myHist->SetCanExtend(TH1::kYaxis);
				//myHist->Draw("hist");
				//myHist->Draw("hist");
				//chosenHist = myHist;	
			//}
			//myHist->SetLineColor(4 - counter);
			//myHist->SetDirectory(0);
			
			myHist->SetDirectory(0);
			totalHist->Add(myHist);
			//std::cout << histName << std::endl;

		counter++;
		}
//	}
	/*totalHist->SetLineColor(kBlue);	
	totalHist->SetLineColor(kBlue);	
	totalHist->SetDirectory(0);	
	totalHist->Draw("hist");
	
	chosenHist->SetLineColor(kRed);
	chosenHist->Draw("histsame");
	std::cout << chosenHist->GetMinimum() << std::endl;
	*/
	f->Close();

	TH1D *re_totalHist = new TH1D("re_totalHist", "", 160, 0, 160);
	//re_totalHist->SetTitle(";M_{inv}/GeV;counts/0.8GeV");
	TFile *f2 = new TFile("rootOutput/re_output_2lep_15-12.root");
	if(!f2->IsOpen()){
		std::cout << "Couldn't open re_output.root" << std::endl;
	}
		
	/*TVectorD *Re_Eff_Vector= (TVectorD*)f2->Get((product+"/Efficiency/efficiency").c_str());
	Double_t re_Eff;
	Double_t double_counts;
	if(Re_Eff_Vector != NULL){	
		re_Eff = (*Re_Eff_Vector)[0];
		double_counts = (*Re_Eff_Vector)[1];
	}*/

	f2->cd((product + "/" + histType).c_str());
	TIter re_next(gDirectory->GetListOfKeys());
	TKey *reKey;

	while((reKey = (TKey*)re_next())){
		
		TClass *realClass = gROOT->GetClass(reKey->GetClassName());
		if(!realClass->InheritsFrom("TH1D")){
		std::cout << "Skipping..." << std::endl;
		continue;
		}
		TH1D *reHist = new TH1D;
		reHist = (TH1D*)reKey->ReadObj();
		string name = reHist->GetName();
		reHist->SetDirectory(0);
				
		re_totalHist->Add(reHist);
		/*for(Int_t i = 1; i <= reHist->GetNbinsX(); i++){		
			re_totalHist->Add(reHist);
		}*/
	}

	delete f2;
	/*
	TAxis* a = totalHist->GetXaxis();
	a->SetNdivisions(-504);
	//a->ChangeLabel(1,-1,-1,-1,-1,-1,"");
	a->ChangeLabel(-1,-1,-1,-1,-1,-1,"2#pi");
	a->ChangeLabel(2,-1,-1,-1,-1,-1,"#frac{#pi}{2}");
	a->ChangeLabel(3,-1,-1,-1,-1,-1,"#pi");
	a->ChangeLabel(4,-1,-1,-1,-1,-1,"#frac{3#pi}{2}");
	a->SetLabelOffset(0.015);
	a->SetTitleOffset(1.2);
	totalHist->SetTitle(";#Delta#phi/rad; counts/[#pi/100rad]");
	totalHist->SetDirectory(0);
	totalHist->Draw("hist");

	*/

	/*

	//ADD LEGEND AND TITLES TO NEW HISTOGRAM
	legend->SetHeader("ZZ#rightarrow4l", "C");
	//legend->AddEntry(totalHist, "MC", "l");


	totalHist->SetLineColor(kRed);
	//legend->AddEntry(re_totalHist, "Real", "l");
	totalHist->SetDirectory(0);
	totalHist->SetTitle(";M_{inv} /GeV; Counts /0.8GeV");
	//totalHist->Draw("hist");
	re_totalHist->SetDirectory(0);
	re_totalHist->SetZTitle("Counts/(1.6GeV)^{2}");
	*/
	
	//legend->Draw();

	re_totalHist->SetTitle(";M_{ll} [GeV];N / [GeV]");
	//re_totalHist->SetLineColor(kBlack);
	legend->AddEntry(re_totalHist,"10 fb^{-1} real","l");
	re_totalHist->Draw("histsame");
	//totalHist->SetTitle(";M_{ll} [GeV];N / [GeV]");
	//totalHist->SetLineColor(kRed);
	//legend->AddEntry(totalHist,"MC","l");
	//totalHist->Draw("histsame");
	//legend->Draw("same");




	/*Int_t upperFit{140};
	Int_t lowerFit{50};
	TF1 *invMassFit = new TF1("invMassFit",Fit,lowerFit,upperFit,order+4); //hardcoded
	invMassFit->SetParNames("#mu","#gamma","A","a","b");
	invMassFit->SetParameters(90,5,1,1,1,1);
	invMassFit->SetParLimits(0,86,96);
	invMassFit->SetParLimits(1,0,20);
	invMassFit->SetParLimits(2,0,200);
	//invMassFit->SetParLimits(3,-2,-0.01);
	invMassFit->SetParLimits(4,-160*invMassFit->GetParameter(3),10);
	invMassFit->SetLineColor(kRed);
	re_totalHist->Fit("invMassFit","+RN");

	

	//////////////////////////////////
	//	Background fitting 	//
	
	
	Double_t lower_range{110};
	Double_t upper_range{125};
	
	
	TF1 *backFit = new TF1("backFit",BackFit,lower_range,upper_range,order+1); //hardcoded
	backFit->SetParNames("a","b");
	//backFit->SetParameters(2,20000);
	backFit->SetParameters(1,1);
	backFit->SetParameters(0,-1);
	totalHist->Fit("backFit", "+RN");

	
	Double_t m_err = backFit->GetParError(0);
	Double_t c_err = backFit->GetParError(1);


	Double_t x[200], y[200];
	Double_t background_err{0};
	for(Int_t i=0; i<200; i++){
		x[i]=160*i/200;
		//y[i]=invMassFit->GetParameter(1)*invMassFit->GetParameter(2)/(pow(x[i]-invMassFit->GetParameter(0),2)+pow(0.5*invMassFit->GetParameter(1),2));
		y[i] = backFit->GetParameter(1) + backFit->GetParameter(0)*x[i];// + backFit->GetParameter(0)*pow(x[i],2);
		if(x[i]>=80&&x[i]<=100){
			background_err += pow(x[i]*m_err,2) + pow(c_err,2);
		}
	}
	background_err = sqrt(background_err);

	TGraph *g = new TGraph(200,x,y);
	g->SetLineColor(kRed);
	g->SetLineWidth(2);
	//g->Draw("same");
	legend->AddEntry(g,"MC Background","l");
	
	Double_t backIntegral = backFit->Integral(80/0.8,100/0.8);
	*/
	/////////////////////////////
	//	Re_ background     //
	//	fitting		   //

	
	Double_t lower_range1{25};
	Double_t upper_range1{65};

	TF1 *re_backFit = new TF1("re_backFit",BackFit,lower_range1,upper_range1,order+1); //hardcoded
	re_backFit->SetParNames("a","b");
	//backFit->SetParameters(2,20000);
	re_backFit->SetParameters(1,1);
	re_backFit->SetParameters(0,-1);
	re_totalHist->Fit("re_backFit", "+RN");

	
	Double_t re_m_err = re_backFit->GetParError(0);
	Double_t re_c_err = re_backFit->GetParError(1);


	Double_t x1[200], y1[200];
	Double_t background_err1{0};
	for(Int_t i=0; i<200; i++){
		x1[i]=160*i/200;
		//y[i]=invMassFit->GetParameter(1)*invMassFit->GetParameter(2)/(pow(x[i]-invMassFit->GetParameter(0),2)+pow(0.5*invMassFit->GetParameter(1),2));
		y1[i] = re_backFit->GetParameter(1) + re_backFit->GetParameter(0)*x1[i];// + backFit->GetParameter(0)*pow(x[i],2);
		if(x1[i]>=80&&x1[i]<=100){
			background_err1 += pow(x1[i]*re_m_err,2) + pow(re_c_err,2);
		}
	}
	background_err1 = sqrt(background_err1);

	TGraph *g1 = new TGraph(200,x1,y1);
	g1->SetLineColor(kRed);
	g1->SetLineWidth(2);
	g1->Draw("same");
	legend->AddEntry(g1,"Background","l");
	//legend->Draw();
	legend->Draw("same");
	Double_t re_backIntegral = re_backFit->Integral(80/0.8,100/0.8);
	
/*
	////////////////////////////
	//	Signal fitting    //
	
	TH1D *signalHist = new TH1D("signalHist", "Signal", 200, 0, 160);
	for(Int_t i=0; i<= totalHist->GetNbinsX(); i++){	
		signalHist->SetBinContent(i, totalHist->GetBinContent(i) - y[i]);
	}
	signalHist->SetLineColor(kRed);
	
	//signalHist->Draw("hist");
	TF1 *sigFit = new TF1("sigFit", Lorentz,80,103,3 );
	sigFit->SetParNames("s#mu", "s#gamma", "sA");
	sigFit->SetParameters(90,2,10);
	sigFit->SetParLimits(0, 85,95);
	sigFit->SetParLimits(1, 0,5);
	sigFit->SetParLimits(2, 0,200);
	signalHist->SetTitle(";M_{inv}/GeV;counts/0.8GeV");
	signalHist->Fit("sigFit", "+RN");


	//OVERALL FITTING::
	Double_t x2[200], y2[200];
	for(Int_t i=0; i<200; i++){
		x2[i]=160*i/200;
		//y2[i]=invMassFit->Eval(x2[i]);
		y2[i] = invMassFit->GetParameter(order+3)+invMassFit->GetParameter(order+2)*x2[i];//+invMassFit->GetParameter(order+1)*pow(x2[i],2) + invMassFit->GetParameter(order)*pow(x2[i],3);
	}
	TGraph *g2 = new TGraph(200,x2,y2);
	g2->SetLineColor(kOrange);
	//g2->Draw("same");
	
	Double_t x3[200], y3[200];
	for(Int_t i=0; i<200; i++){
		x3[i]=160*i/200;
		y3[i]=sigFit->GetParameter(1)*sigFit->GetParameter(2)/(pow(x3[i] - invMassFit->GetParameter(0),2) + pow(sigFit->GetParameter(1)/2,2));
	}
	TGraph *g3 = new TGraph(200,x3,y3);
	g3->SetLineColor(kBlue);
	g3->SetLineWidth(2);
	//totalHist->Draw("hist");
	//g3->Draw("same");
	//g->Draw("same");
	//legend->AddEntry(g3,"Lorentzian","l");
	//legend->Draw();
	
	//sigFit->SetParLimits(0, 85,95);
	//sigFit->SetParLimits(1, 0,5);
	sigFit->SetParLimits(1, 0,200);
	signalHist->Fit("sigFit", "+RN");



	Double_t x2[200], y2[200];
	for(Int_t i=0; i<200; i++){
		x2[i]=160*i/200;
		//y2[i]=invMassFit->Eval(x2[i]);
		y2[i] = invMassFit->GetParameter(order+3)+invMassFit->GetParameter(order+2)*x2[i];//+invMassFit->GetParameter(order+1)*pow(x2[i],2) + invMassFit->GetParameter(order)*pow(x2[i],3);
	}
	TGraph *g2 = new TGraph(200,x2,y2);
	g2->SetLineColor(kOrange);
	//g2->Draw("same");
	*/
	/*Double_t x3[200], y3[200];
	for(Int_t i=0; i<200; i++){
		x3[i]=160*i/200;
		y3[i]=sigFit->GetParameter(1)*sigFit->GetParameter(2)/(pow(x3[i] - invMassFit->GetParameter(0),2) + pow(sigFit->GetParameter(1)/2,2));
	}
	TGraph *g3 = new TGraph(200,x3,y3);
	g3->SetLineColor(kBlue);
	g3->SetLineWidth(2);
	//legend->AddEntry(g3, "Signal", "l");
	//g3->Draw("same");
	//legend->Draw();
	//}
	
	Double_t efficiency = mc_Eff;   //TODO: Make sure to change this for the correct file
	//efficiency = 4.5e-5;
	std::cout << "Background integral: " << backIntegral << " +- " << background_err << std::endl;
	std::cout << "Efficiency used: " << efficiency << std::endl;
	


	//Subtracting MC signal file background fit off the real data:
	


	Double_t err;
	Double_t I = re_totalHist->IntegralAndError(80/0.8,100/0.8,err,"");
	Double_t err_tot;
	Double_t I_tot = re_totalHist->IntegralAndError(0,200,err_tot,"");
	Double_t N_sig = (2*I-I_tot)/2;
	
	N_sig = N_sig - backIntegral; 		//subtract MonteCarlo signal background

	std::cout << "Signal events : " << N_sig << std::endl;
	Double_t sigma = (N_sig-re_backIntegral)/(efficiency*L_int);
	Double_t sigma_sigma = sigma*sqrt((pow(err,2)+pow(err_tot,2)/4+pow(background_err,2))/pow(N_sig-backIntegral,2));
	std::cout<<"sigma = "<<sigma/1e3<<" +- "<<sigma_sigma/1e3<<" pb"<<std::endl;
	std::cout<<"eff="<<efficiency<<std::endl;*/
}


int plotterlep(){
	plot("2lep","invMass2l");
	return 0;
}
