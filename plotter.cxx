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



void plot(string product, string histType){
	vector<string> productNames;
	productNames.push_back("1fatjet1lep");	
	productNames.push_back("1lep");
	productNames.push_back("1lep1tau");
	productNames.push_back("1tau");
	productNames.push_back("2lep");
	productNames.push_back("2tau");
	productNames.push_back("GamGam");

	gROOT->SetStyle("ATLAS");
	gStyle->SetOptStat(0);

	TCanvas *c = new TCanvas("c", "c");	
	TLegend *legend = new TLegend(1,0.5);
	TH1D *totalHist = new TH1D("totalHist", "Totals", 200, 0, 160);
	
	
	TFile *f = new TFile("rootOutput/mc_output_28-11_ZZllll.root");	//("rootOutput/mc_output.root");
	if(!f->IsOpen()){
		std::cout << "Couldn't open mc_output.root" << std::endl;
	}
	
	string Zlep = "invMassZmumu";
	//for(vector<string>::iterator it = productNames.begin(); it != productNames.end(); it++){
		f->cd((product +"/" + histType).c_str());
		//gDirectory->pwd();
		TIter next(gDirectory->GetListOfKeys());
		TKey *aKey;

		//TH1D *chosenHist = new TH1D;
		
		string signalFile = "mc15_13TeV.363490.Sh_221_NNPDF30NNLO_llll.2lep_raw.root"; 
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
			/*if(histName == printChoice){
				myHist->SetDirectory(0);
				myHist->SetTitle(";M_{inv} /GeV; Counts");
				myHist->SetLineColor(kRed);
				legend->AddEntry(myHist,"Signal channel","l");
				//myHist->SetCanExtend(TH1::kYaxis);
				myHist->Draw("histsame");
				//myHist->Draw("hist");
				//chosenHist = myHist;	
			}*/
			//myHist->SetLineColor(4 - counter);
			//myHist->SetDirectory(0);
			
			myHist->SetDirectory(0);
			//std::cout << histName << std::endl;
			for(Int_t i = 1; i <= myHist->GetNbinsX(); i++){		
				Double_t content = myHist->GetBinContent(i);
				if(histName != (product + "_" + histType + "_" + "mc15_13TeV.307431.MGPy8EG_A14NNPDF23LO_RS_G_ZZ_llll_c10_m0200.2lep_raw.root")){
				totalHist->SetBinContent(i,(totalHist->GetBinContent(i) + content));
				}

			}
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

	
	TH1D *re_totalHist = new TH1D("re_totalHist", "Real Totals", 200, 0, 160);
	TFile *f2 = new TFile("rootOutput/re_output_21-11_Zmumu.root");
	if(!f2->IsOpen()){
		std::cout << "Couldn't open re_output.root" << std::endl;
	}

	//for(vector<string>::iterator at = productNames.begin(); at != productNames.end(); at++){
		f2->cd((product + "/" + histType).c_str());
		//gDirectory->pwd();
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
					
			for(Int_t i = 1; i <= reHist->GetNbinsX(); i++){		
				Double_t re_content = reHist->GetBinContent(i);
				re_totalHist->SetBinContent(i,(re_totalHist->GetBinContent(i) + re_content));
			}
		}
	//}
	delete f2;

	//ADD LEGEND AND TITLES TO NEW HISTOGRAM
	legend->SetHeader("ZZllll", "C");
	legend->AddEntry(re_totalHist, "Real", "l"/*).c_str()*/);
	legend->AddEntry(totalHist, "MC", "l"/*).c_str()*/);

	totalHist->SetLineColor(kRed);
	totalHist->SetDirectory(0);
	totalHist->SetTitle(";M_{inv} /GeV; Counts /0.8GeV");
	totalHist->Draw("hist");
	re_totalHist->SetDirectory(0);
	re_totalHist->SetLineColor(kBlack);
	re_totalHist->SetTitle(";M_{inv} /GeV; Counts/0.8GeV");
	re_totalHist->Draw("histsame");
	legend->Draw();
	
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
	totalHist->Fit("invMassFit","+RN");

	
	TF1 *backFit = new TF1("backFit",BackFit,25,70,order+1); //hardcoded
	backFit->SetParNames("m","c");
	backFit->SetParameters(1,1);
	backFit->SetParLimits(0,-2,-0.01);
	totalHist->Fit("backFit", "+RN");

	
	Double_t x[200], y[200];
	for(Int_t i=0; i<200; i++){
		x[i]=160*i/200;
		//y[i]=invMassFit->GetParameter(1)*invMassFit->GetParameter(2)/(pow(x[i]-invMassFit->GetParameter(0),2)+pow(0.5*invMassFit->GetParameter(1),2));
		y[i] = backFit->GetParameter(1) + backFit->GetParameter(0)*x[i];
	}
	TGraph *g = new TGraph(200,x,y);
	g->SetLineColor(7);
	g->SetLineWidth(2);
	//legend->AddEntry(g, "Background", "l");
	//g->Draw("same");
	
	//legend->AddEntry(g, "Background", "l");
	//legend->Draw();	
	
	//TCanvas *c2 = new TCanvas("c2" , "c2");	
	TH1D *signalHist = new TH1D("signalHist", "Signal", 200, 0, 160);
	for(Int_t i=0; i<= totalHist->GetNbinsX(); i++){	
		signalHist->SetBinContent(i, totalHist->GetBinContent(i) - y[i]);
	}
	signalHist->SetLineColor(kBlack);
	
	//signalHist->Draw("hist");
	TF1 *sigFit = new TF1("sigFit", Lorentz,80,100,3 );
	sigFit->SetParNames("s#mu", "s#gamma", "sA");
	sigFit->SetParameters(91,10,8);
	
	//sigFit->SetParLimits(0, 85,95);
	//sigFit->SetParLimits(1, 0,5);
	sigFit->SetParLimits(1, 0,200);
	signalHist->Fit("sigFit", "+RN");



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
	Double_t x3[200], y3[200];
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
	Double_t integral=0;
	Double_t background=0;
	Double_t Nrec=0;
	Double_t backFromBackFit{0};
	for(Int_t i=0;i<200;i++){
		if(160*i/200>=80&&160*i/200<=100){
			Double_t ii=160*i/200;
			integral+=invMassFit->Eval(ii);
			background+=invMassFit->GetParameter(order+3)+invMassFit->GetParameter(order+2)*ii;// + invMassFit->GetParameter(order+1)*pow(i,2) + invMassFit->GetParameter(order)*pow(i,3);
			Nrec+=invMassFit->GetParameter(1)*invMassFit->GetParameter(2)/(pow(ii-invMassFit->GetParameter(0),2)+pow(invMassFit->GetParameter(1)/2,2));

			backFromBackFit += backFit->GetParameter(1) + backFit->GetParameter(0)*ii;
		}
	}
	Double_t error;
	Double_t inbuiltIntegral = totalHist->IntegralAndError(80,100,error, "");
	std::cout<<"integral-background: "<<integral-background<<std::endl;
	std::cout << "BackFit events: " << backFromBackFit << std::endl;
	std::cout << "integral - backFit: " << integral - backFromBackFit << std::endl;
	
	std::cout << "Events: " << Nrec << std::endl;
	std::cout<<"Xsec (including Branching) ="<<(Nrec*4)/(10.064*7.6585e-5)<<std::endl;
	std::cout<<"Xsec (other way) ="<<4*(integral-backFromBackFit-229.185)/(2*10.064*1.68517e-5)<<std::endl;
}


int plotter(){
	plot("2lep","invMass2l");
	return 0;
}
