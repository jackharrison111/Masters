//change this
std::string me = "user.aburke.Ztautau_AOD_openinganglecuts.28-04_16-21_MYSTREAM/user.aburke.";
const int size{232};


//std::string JEDITaskID[size] = {"20718468" , "20718456"};//,"","","",...};
std::string bigJobID = "21146110";

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
	TH1D *mmc_hist = new TH1D("mmc_hist","",160,0,160);
	TH1D *vis_hist = new TH1D("vis_hist","",160,0,160);
	TH1D *col_hist = new TH1D("col_hist","",160,0,160);
	TH1D *m_phi_rel_hist = new TH1D("m_phi_rel_hist","",100,-M_PI,M_PI);
	TH1D *Zee_hist = new TH1D("Zee_hist","",160,0,160);
	
	double n_entries =0;

	int file_number = 0;
	for(int i{}; i < size; i++){
		file_number++;
		std::string intString = to_string(file_number);
		if(file_number < 10){ intString = "00" + intString;}
                else if((file_number>9)&&(file_number<100)){ intString = "0" + intString;}

		TFile *f = new TFile(( me + bigJobID + ".MYSTREAM._000" + intString + ".root").c_str());
		if(!f->IsOpen()){
			std::cout << "Couldn't open file" << std::endl;
			i--;
		}
		else{
			std::cout << "file_number = " << file_number << std::endl;
			TIter next(gDirectory->GetListOfKeys());
			TKey *aKey;
			int hist_number{0};
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
				if(hist_number == 0){
					//col_hist->Add(myHist);
				}else if(hist_number == 1){
					col_hist->Add(myHist);
				}else if(hist_number == 2){
				}else if(hist_number == 3){
				}else if(hist_number == 4){
				}else if(hist_number == 5){
				}else if(hist_number == 6){
				}else if(hist_number == 7){
					m_phi_rel_hist->Add(myHist);
				}else if(hist_number == 8){
					vis_hist->Add(myHist);
				}
				hist_number++;
			}
			f->Close();
			delete f;
		}
	}
	//legend->AddEntry(mmc_hist, "MMC", "l");
	//legend->AddEntry(col_hist, "Collinear approx.", "l");
	mmc_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
	vis_hist->SetTitle(";M_{vis} [GeV]; N / [GeV]");
	Zee_hist->SetTitle(";M_{ee} [GeV]; N / [GeV]");
	//col_hist->SetLineColor(2);
	col_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
	col_hist->Draw("histsame");
	//mmc_hist->Draw("histsame");
 	//Zee_hist->Draw("histsame");
 	//vis_hist->Draw("histsame");
	//legend->Draw();
	/*TAxis *a = m_phi_rel_hist->GetXaxis();
	a->SetNdivisions(-504);
	a->ChangeLabel(1,-1,-1,-1,-1,-1,"-#pi");
	a->ChangeLabel(-1,-1,-1,-1,-1,-1,"#pi");
	a->ChangeLabel(2,-1,-1,-1,-1,-1,"-#frac{#pi}{2} (l)");
	a->ChangeLabel(4,-1,-1,-1,-1,-1,"#frac{#pi}{2} (#tau)");
	a->SetLabelOffset(0.015);
	a->SetTitleOffset(1.2);
	m_phi_rel_hist->SetTitle(";#phi_{m} [radians];N / [2#pi/100 radians]"); 
	m_phi_rel_hist->Draw("hist");
	double gx[2] = {-M_PI/2,-M_PI/2};
	double hx[2] = {M_PI/2,M_PI/2};
	double y[2] = {0,m_phi_rel_hist->GetMaximum()};
	TGraph *g = new TGraph(2,gx,y);
	TGraph *h = new TGraph(2,hx,y);
	g->SetLineColor(kRed);
	h->SetLineColor(kRed);
	g->Draw("same");
	h->Draw("same");*/
}

int plotterZtautau(){
	plot();
	return 0;
}
