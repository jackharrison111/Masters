//change this
std::string me = "jaharris.1stAOD_MMC_met7Comparison_05-03_MYSTREAM/user.jaharris.";
const int size{17};
//std::string JEDITaskID[size] = {"20718468" , "20718456"};//,"","","",...};
std::string bigJobID = "20730738";
//user.jaharris.20719143.MYSTREAM._000001.root  example big job file

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
	
	double n_entries =0;

	for(int i{}; i < size; i++){
		i++;
		std::string intString = to_string(i);
		if(i < 10){ intString = "00" + intString;}
                else if((i>9)&&(i<100)){ intString = "0" + intString;}

		TFile *f = new TFile(("user." + me + bigJobID + ".MYSTREAM._000" + intString + ".root").c_str());
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
				double myHist_entries = myHist->GetEntries();
				//Find the biggest file in the histograms::
				/*if(myHist_entries > n_entries){ 
					n_entries = myHist_entries;
					std::cout << i << std::endl;
				}
				if(i==46){totalHist_col->Add(myHist);}*/
			}else{
				totalHist_col->Add(myHist);
			}
			filenumber++;
		}
		f->Close();
		delete f;
	}
	legend->AddEntry(totalHist, "met(-1)", "l");
	legend->AddEntry(totalHist_col, "met(7)", "l");
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
