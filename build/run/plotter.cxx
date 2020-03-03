//change this
std::string me = "jaharris/user.jaharris.";
const int size{2};
std::string JEDITaskID[size] = {"20718468" , "20718456"};//,"","","",...};

void plot(){
	//gROOT->SetStyle("ATLAS");
	gROOT->pwd();
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
	TH1D *totalHist_col = new TH1D("totalHist","",160,0,160);
	for(int i{}; i < size; i++){
		TFile *f = new TFile(("user." + me + JEDITaskID[i] + ".MYSTREAM._000001.root").c_str());
		if(!f->IsOpen()){
			std::cout << "Couldn't open file" << std::endl;
		}
		TIter next(gDirectory->GetListOfKeys());
		TKey *aKey;
		int filenumber{0};
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
			if(filenumber!=0){
				totalHist->Add(myHist);
			}else{
				totalHist_col->Add(myHist);
			}
			filenumber++;
		}
		f->Close();
		delete f;
	}
	legend->AddEntry(totalHist, "MMC", "l");
	legend->AddEntry(totalHist_col, "Collinear approx.", "l");
	totalHist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
	totalHist->Draw("hist");
	totalHist_col->SetLineColor(2);
	totalHist_col->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
	totalHist_col->Draw("histsame");
	legend->Draw();
}

int plotter(){
	plot();
	return 0;
}
