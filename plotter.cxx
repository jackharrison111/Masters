#include <TStyle.h>
#include <TCanvas.h>
#include <TH2.h>
#include <vector>

void plot(string product, string histType){

	

	gROOT->SetStyle("ATLAS");
	gStyle->SetOptStat(0);


	TFile *f = new TFile("mc_output.root");
	//("rootOutput/mc_output.root");

	if(!f->IsOpen()){
		std::cout << "Couldn't open mc_output.root" << std::endl;
	}
	//
	//
	//f->cd(product + "/" + histType);
	
	f->cd((product +"/" +histType).c_str());
	TIter next(gDirectory->GetListOfKeys());
	TKey *aKey;
	TCanvas *c = new TCanvas("c", "c");
	
	TLegend *legend = new TLegend(1,0.5);
	legend->SetHeader("Cuts", "C");
	


	TH1D *totalHist = new TH1D("totalHist", "Totals", 500, 0, 3e5);
	TH1D *chosenHist = new TH1D;
	
	TList *histList = new TList;



	vector<string> names;
	names.push_back("76-106 GeV");
	names.push_back("81-101 GeV");
	names.push_back("76-106 & 86-96 GeV");
	names.push_back("at least one 86-96 GeV");

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
		std::cout << histName << std::endl;
		
		string printChoice = product + "_" + histType + "_" + signalFile;
		/*if(counter==0){//histName == printChoice){
			myHist->SetDirectory(0);
			//myHist->SetLineColor(kRed);
			//myHist->Draw("hist");
			chosenHist = myHist;	
		}*/
		myHist->SetLineColor(4 - counter);
		myHist->SetDirectory(0);
		myHist->Draw("histsame");
		
		myHist->SetTitle(";M_{inv} /GeV; Counts");
		legend->AddEntry(myHist, names[counter].c_str());
		//myHist->GetName
		myHist->SetDirectory(0);
		//myHist->Draw("same");

		
		for(Int_t i = 1; i <= myHist->GetNbinsX(); i++){		
			Double_t content = myHist->GetBinContent(i);
			totalHist->SetBinContent(i,(totalHist->GetBinContent(i) + content));

		}
	
	legend->Draw();
	c->SetTitle(";;M_{inv} (GeV); Counts");
	counter++;
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
}



int plotter(){
	
	plot("foJack", "foJackSq");

	return 0;
}
