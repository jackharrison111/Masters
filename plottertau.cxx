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
	c->SetTickx();
	c->SetTicky();
	c->SetGridx();
	c->SetGridy();
	TLegend *legend = new TLegend(1,0.5);
	legend->SetHeader("need to set header","c");
	legend->SetBorderSize(4);
	legend->SetShadowColor(1);
	legend->SetDrawOption("br");
	TH1D *totalHist = new TH1D("totalHist","",160,0,160);
	TH1D *etDist = new TH1D("missEtDist","",100,0,M_PI);
	


	TFile *f = new TFile("rootOutput/mc_output_tau_16-12.root");	//("rootOutput/mc_output.root");
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
		//f->cd((product +"/" + histType).c_str());
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
			//TODO: dont delete - how to choose a specific hist
			//std::cout<<histName << std::endl;
			
			string printChoice = product + "_" + histType + "_" + signalFile;
			//if(histName == printChoice){
				myHist->SetDirectory(0);
				//myHist->SetTitle(";M_{inv}/GeV; Counts/0.8GeV");
				//myHist->SetLineColor(kBlue-4);
				myHist->SetLineWidth(1);
				//legend->AddEntry(myHist,"Signal channel","l");
				//myHist->SetCanExtend(TH1::kYaxis);
				//myHist->Draw("hist");
				etDist->Add(myHist);
				//chosenHist = myHist;	
			//}
				//myHist->SetLineColor(4 - counter);
				//myHist->SetDirectory(0);
			
			//std::cout << histName << std::endl;
			//etDist->Add(myHist);
			//totalHist->Add(myHist);
				//if(histName != (product + "_" + histType + "_" + "mc15_13TeV.307431.MGPy8EG_A14NNPDF23LO_RS_G_ZZ_llll_c10_m0200.2lep_raw.root")){
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
	
	etDist->SetTitle(";#Delta [radians];% of events with #phi_{m} contained within #Delta");
	etDist->Draw();

	/*TAxis* a = etDist->GetXaxis();
	a->SetNdivisions(-504);
	a->ChangeLabel(1,-1,-1,-1,-1,-1,"-#pi");
	
	a->ChangeLabel(-1,-1,-1,-1,-1,-1,"#pi");
	a->ChangeLabel(2,-1,-1,-1,-1,-1,"-#frac{#pi}{2} (l)");
	a->ChangeLabel(4,-1,-1,-1,-1,-1,"#frac{#pi}{2} (#tau)");
	a->SetLabelOffset(0.015);
	a->SetTitleOffset(1.2);
	
	etDist->SetTitle(";#phi_{m} [radians];N / [2#pi/100 radians]");
	etDist->Draw("hist");
	Double_t gx[2] = {-M_PI/2,-M_PI/2};
	Double_t hx[2] = {M_PI/2,M_PI/2};
	Double_t y[2] = {0,etDist->GetMaximum()};
	TGraph *g = new TGraph(2,gx,y);
	TGraph *h = new TGraph(2,hx,y);
	h->SetLineColor(kRed);
	g->Draw("same");
	h->Draw("same");
	
	Double_t lowerMass=40;
	Double_t higherMass=140;
	TF1 *fit;

	fit = new TF1("fit",Gaussian,lowerMass,higherMass,6);
	fit->SetParameters(85,10,100,91,5,100);
	totalHist->SetTitle(";M_{inv}/GeV;counts/0.8GeV");
	totalHist->Fit("fit","+R");

	*//*if(order==1){
		fit = new TF1("fit",Fit,lowerMass,higherMass,8);
		fit->SetParameters(91,5,1,80,5,1,1,1);
	}else if(order==2){
		fit = new TF1("fit",Fit,lowerMass,higherMass,9);
		fit->SetParameters(91,5,1,80,5,1,1,1,1);
	}
	totalHist->SetTitle(";M_{inv}/GeV;counts/0.8GeV");
	totalHist->Fit("fit","+R");
	
	Int_t nBins=200;
	Double_t x[nBins], y1[nBins], y2[nBins], y3[nBins];
	for(Int_t i=0; i<200; i++){
		x[i]=0.8*(i+1);
		y1[i]=fit->GetParameter(2)*exp(-pow((x[i]-fit->GetParameter(0))/fit->GetParameter(1),2)/2);
		y2[i]=fit->GetParameter(5)*exp(-pow((x[i]-fit->GetParameter(3))/fit->GetParameter(4),2)/2);
		if(order==1){
			y3[i]=fit->GetParameter(6)*x[i]+fit->GetParameter(7);
		}else if(order==2){
			y3[i]=fit->GetParameter(6)*pow(x[i],2)+fit->GetParameter(7)*x[i]+fit->GetParameter(8);
		}
	}
	TGraph *g1 = new TGraph(nBins,x,y1);
	g1->SetLineColor(kBlue);
	g1->SetLineWidth(2);
	g1->Draw("same");
	TGraph *g2 = new TGraph(nBins,x,y2);
	g2->SetLineColor(kGreen);
	g2->SetLineWidth(2);
	g2->Draw("same");
	TGraph *g3 = new TGraph(nBins,x,y3);
	g3->SetLineColor(7);
	g3->SetLineWidth(2);
	g3->Draw("same");

	Double_t backIntegral=0;
	Double_t background_err=0;
	for(Int_t i=80/0.8; i<=100/0.8; i++){
		Double_t xx = 0.8*i;
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
			backIntegral += A*exp(-pow((xx-mu)/ep,2)/2) + m*xx + c;
			background_err += pow(-A/(2*ep)*pow((xx-mu)/ep,3)*exp(-pow((xx-mu)/ep,2)/2)*Emu,2) + pow(-A/(2*ep)*pow((xx-mu)/ep,4)*exp(-pow((xx-mu)/ep,2)/2)*Eep,2) + pow(exp(-pow((xx-mu)/ep,2)/2)*EA,2) + pow(xx*Em,2) + pow(Ec,2); //propagating errors
		}else std::cout<<"order==2"<<std::endl;
	}
	background_err=sqrt(background_err);
	
	Double_t efficiency = mc_Eff;

	TH1D *signalHist = new TH1D("signalHist","signalHist",200,0,160);
	std::cout<<"check dark blue is the signal gaussian and not green"<<std::endl;
	for(Int_t i=1; i<=200; i++){
		signalHist->SetBinContent(i,totalHist->GetBinContent(i)-y2[i]-y3[i]);
		if(signalHist->GetBinContent(i)<0) signalHist->SetBinContent(i,0);
	}
	
	Double_t efficiency = mc_Eff;

	Double_t err;
	//Double_t I = totalHist->IntegralAndError(80/0.8,100/0.8,err,"");
	Double_t I = signalHist->IntegralAndError(80/0.8,100/0.8,err,"");
	Double_t err_tot;
	//Double_t I_tot = totalHist->IntegralAndError(0,200,err_tot,"");
	Double_t I_tot = signalHist->IntegralAndError(0,200,err_tot,"");
	Double_t N_sig = (2*I-I_tot)/2;
	//Double_t sigma = pow(2*Br_lep,2)*(N_sig-backIntegral)/(efficiency*L_int);
	Double_t sigma = pow(Br_lep,1)*N_sig/(efficiency*L_int);
	//Double_t sigma_sigma = sigma*sqrt((pow(err,2)+pow(err_tot,2)/4+pow(background_err,2))/pow(N_sig-backIntegral,2));
	Double_t sigma_sigma = sigma*sqrt((pow(err,2)+pow(err_tot,2)/4+pow(background_err,2))/pow(N_sig,2));
	std::cout<<"eff="<<efficiency<<", Br_lep="<<Br_lep<<", N_sig="<<N_sig<<", bkgnd="<<backIntegral<<", L_int="<<L_int<<std::endl;
		
	//totalHist->SetTitle(";M_{inv}/GeV; counts/0.8GeV");
	//totalHist->Draw("hist");

	
		
		
		
		
	/*
		TH1D *re_totalHist = new TH1D("re_totalHist", "", 200, 0, 160);
		re_totalHist->SetTitle(";M_{inv}/GeV;counts/0.8GeV");
		TFile *f2 = new TFile("rootOutput/re_output_4-12_tau.root");
		if(!f2->IsOpen()){
			std::cout << "Couldn't open re_output.root" << std::endl;
		}
			
		TVectorD *Re_Eff_Vector= (TVectorD*)f2->Get((product+"/Efficiency/efficiency").c_str());
		Double_t re_Eff;
		Double_t double_counts;
		if(Re_Eff_Vector != NULL){	
			re_Eff = (*Re_Eff_Vector)[0];
			double_counts = (*Re_Eff_Vector)[1];
		}

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
		}

		delete f2;

		//ADD LEGEND AND TITLES TO NEW HISTOGRAM
		legend->SetHeader("ZZ#rightarrow4l", "C");
		//legend->AddEntry(totalHist, "MC", "l");


		totalHist->SetLineColor(kRed);
		//legend->AddEntry(re_totalHist, "Real", "l");
		totalHist->SetDirectory(0);
		totalHist->SetTitle(";M_{inv} /GeV; Counts /0.8GeV");
		//totalHist->Draw("hist");
		re_totalHist->SetDirectory(0);
		//re_totalHist->SetZTitle("Counts/(1.6GeV)^{2}");
		//re_totalHist->Draw("hist");
		//legend->Draw();
		//etDist->SetDirectory(0);	
		//etDist->Draw("hist");
		Int_t upperFit{140};
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
		
		
		Double_t lower_range{15/0.8};
		Double_t upper_range{60/0.8};
		
		
		TF1 *backFit = new TF1("backFit",BackFit,lower_range,upper_range,order+1); //hardcoded
		backFit->SetParNames("a","b");
		//backFit->SetParameters(2,20000);
		backFit->SetParameters(1,1);
		backFit->SetParameters(0,-1);
		re_totalHist->Fit("backFit", "+RN");

		
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
		legend->AddEntry(g,"Background","l");
		
		
		
		
		
		
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

		*/
		//OVERALL FITTING::
		/*Double_t x2[200], y2[200];
		for(Int_t i=0; i<200; i++){
			x2[i]=160*i/200;
			//y2[i]=invMassFit->Eval(x2[i]);
			y2[i] = invMassFit->GetParameter(order+3)+invMassFit->GetParameter(order+2)*x2[i];//+invMassFit->GetParameter(order+1)*pow(x2[i],2) + invMassFit->GetParameter(order)*pow(x2[i],3);
		}
		TGraph *g2 = new TGraph(200,x2,y2);
		g2->SetLineColor(kOrange);
		//g2->Draw("same");
		*//*
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


		*/
		/*Double_t x2[200], y2[200];
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
		//legend->Draw();*/
		//}
		
		/*Double_t backIntegral = backFit->Integral(80/0.8,100/0.8);
		Double_t efficiency = mc_Eff;   //TODO: Make sure to change this for the correct file
		std::cout << "Background integral: " << backIntegral << " +- " << background_err << std::endl;
		std::cout << "Efficiency used: " << efficiency << std::endl;
		

		Double_t err;
		Double_t I = re_totalHist->IntegralAndError(80/0.8,100/0.8,err,"");
		Double_t err_tot;
		Double_t I_tot = re_totalHist->IntegralAndError(0,200,err_tot,"");
		Double_t N_sig = (2*I-I_tot)/2;
		Double_t sigma = pow(2*Br_lep,2)*(N_sig-backIntegral)/(efficiency*L_int);
		Double_t sigma_sigma = sigma*sqrt((pow(err,2)+pow(err_tot,2)/4+pow(background_err,2))/pow(N_sig-backIntegral,2));
		std::cout<<"sigma = "<<sigma/1e3<<" +- "<<sigma_sigma/1e3<<" pb"<<std::endl;*/
		
	}


	int plottertau(){
		plot("1lep1tau","missEtDist");
		return 0;
	}
