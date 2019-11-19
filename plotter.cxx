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
	}else{
		return 0;
	}
	return B;
}

Int_t order = 2; //global??
//combine
Double_t Fit(Double_t *x, Double_t *par){
	return Lorentz(x, par) + Background(x,&par[3],order);
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

	TH1D *totalHist = new TH1D("totalHist", "Totals", 500, 0, 3e5);
	
	
	TFile *f = new TFile("rootOutput/mc_output.root");	//("rootOutput/mc_output.root");
	if(!f->IsOpen()){
		std::cout << "Couldn't open mc_output.root" << std::endl;
	}
	
	string Zlep = "invMass2l"

	for(vector<string>::iterator it = productNames.begin(); it != productNames.end(); it++){


		f->cd((*it +"/" +Zlep).c_str());
		gDirectory->pwd();
		TIter next(gDirectory->GetListOfKeys());
		TKey *aKey;
		//legend->SetHeader("Cuts", "C");
		



		//TH1D *chosenHist = new TH1D;
		
		/*
		vector<string> names;
		names.push_back("76-106 GeV");
		names.push_back("81-101 GeV");
		names.push_back("76-106 & 86-96 GeV");
		names.push_back("at least one 86-96 GeV");
		*/
		string signalFile = "mc15_13TeV.361063.Sh_CT10_llll.2lep_raw.root"; 
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
			//std::cout<<histName << std::endl;
			//if(histName == "channel_162_avwf"){
			//std::cout << histName << std::endl;
			
			/*string printChoice = product + "_" + histType + "_" + signalFile;
			if(counter==0){//histName == printChoice){
				myHist->SetDirectory(0);
				//myHist->SetLineColor(kRed);
				//myHist->Draw("hist");
				chosenHist = myHist;	
			}
			myHist->SetLineColor(4 - counter);
			myHist->SetDirectory(0);
			myHist->Draw("histsame");
			
			myHist->SetTitle(";M_{inv} /GeV; Counts");
			legend->AddEntry(myHist, names[counter].c_str());
			*/
			myHist->SetDirectory(0);


			for(Int_t i = 1; i <= myHist->GetNbinsX(); i++){		
				Double_t content = myHist->GetBinContent(i);
				totalHist->SetBinContent(i,(totalHist->GetBinContent(i) + content));

			}
		
		//legend->Draw();
		counter++;
		}
	}
	/*totalHist->SetLineColor(kBlue);	
	totalHist->SetLineColor(kBlue);	
	totalHist->SetDirectory(0);	
	totalHist->Draw("hist");
	
	chosenHist->SetLineColor(kRed);
	chosenHist->Draw("histsame");
	std::cout << chosenHist->GetMinimum() << std::endl;
	*/
	delete f;

	
	TH1D *re_totalHist = new TH1D("re_totalHist", "Real Totals", 500, 0, 3e5);

	TFile *f2 = new TFile("rootOutput/re_output.root");
	
	if(!f2.is_open()){
		std::cout << "Couldn't open re_output.root" << std::endl;
	}

	for(vector<string>::iterator at = productNames.begin(); at != productNames.end(); at++){
		
		f2->cd((*at + "/" + Zlep).c_str());
		gDirectory->pwd();
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
			//std::cout << name << std::endl;
			
			/*if(counter==0){//histName == printChoice){
				myHist->SetDirectory(0);
				//myHist->SetLineColor(kRed);
				//myHist->Draw("hist");
				chosenHist = myHist;	
			}*/
			reHist->SetDirectory(0);
					
			for(Int_t i = 1; i <= reHist->GetNbinsX(); i++){		
				Double_t re_content = reHist->GetBinContent(i);
				re_totalHist->SetBinContent(i,(re_totalHist->GetBinContent(i) + re_content));

			}

		}
	}
	delete f2;
	//ADD LEGEND AND TITLES TO NEW HISTOGRAM
	totalHist->SetDirectory(0);
	re_totalHist->SetDirectory(0);

	c->SetTitle(";;M_{inv} (GeV); Counts");
	legend->SetHeader("Data source", "C");
	legend->AddEntry(re_totalHist, ("Real").c_str());
	legend->AddEntry(totalHist, ("MC").c_str());
	totalHist->SetLineColor(kRed);
	totalHist->Draw("hist");
	re_totalHist->Draw("histsame");
	legend->Draw();

	Int_t upperFit{120};
	Int_t lowerFit{40};


	invMassFit = new TF1("invMassFit",Fit,lowerFit,upperFit,order+4); //hardcoded
	invMassFit->SetParNames("#mu","#gamma","A","a","b","c");
	invMassFit->SetParameters(90,5,1,1,1,1);
	invMassFit->SetParLimits(0,86,96);
	invMassFit->SetParLimits(1,0,50);
	invMassFit->SetLineColor(kRed);
	re_totalHist->Fit("invMassFit","+R");
	Double_t integral=0;
	Double_t background=0;
	Double_t Nrec=0;
	for(Int_t i=lowerFit;i<upperFit;i++){
		integral+=invMassFit->Eval(i);
		background+=invMassFit->GetParameter(5)+invMassFit->GetParameter(4)*i+invMassFit->GetParameter(3)*pow(i,2);
		Nrec+=invMassFit->GetParameter(1)*invMassFit->GetParameter(2)/(pow(i-invMassFit->GetParameter(0),2)+pow(invMassFit->GetParameter(1)/2,2));
	}
	std::cout<<"integral-background: "<<integral-background<<std::endl;
	std::cout<<"Nrec: "<<Nrec<<std::endl;
		



}


int plotter(){
	
	plot("2lep","invMass2l");

	return 0;
}
