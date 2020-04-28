//change this
std::string user = "user.jaharris.";
std::string me = "user.jaharris.ZZdataset2_real_900f_18-04_13-18_MYSTREAM/user.jaharris.";

std::string file3 = "local_outputs/ZZ_MCSTDM3_12files_NSIGMET4_22-04_14:37.root";
std::string file4 = "local_outputs/ZZ_MCSTDM3_12files_NSIGMET6_22-04_15:08.root";
std::string file2 = "local_outputs/ZZ_MCSTDM3_12files_NITER52_22-04_19:25.root";
 
std::string file1 = "local_outputs/ZZ_MCSTDM3_12files_NITER3010_22-04_17:59.root";
std::string file5 = "local_outputs/ZZ_MCSTDM3_12files_NITER5030_22-04_18:40.root";

//"user.jaharris.ZZdataset2_real_100files_17-04/user.jaharris.";

const int size{1};
//std::string JEDITaskID[size] = {"20718468" , "20718456"};//,"","","",...};
std::string bigJobID1 = "21069446";
std::string bigJobID2 = "";
//"21065134";
//"20814417";
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
	TH1D *mmc_hist = new TH1D("mmc_hist","",160,0,160);
	TH1D *leplep_hist = new TH1D("leplep_hist","",160,0,160);
	TH1D *mmc_hist_m7 = new TH1D("mmc_hist_m7","",160,0,160);
	TH1D *vis_hist = new TH1D("vis_hist","",160,0,160);
	TH1D *col_hist = new TH1D("col_hist","",160,0,160);
	TH1D *col_hist_m7 = new TH1D("col_hist_m7","",160,0,160);
	TH1D *m_phi_rel_hist = new TH1D("m_phi_rel_hist","",100,-M_PI,M_PI);
	TH1D *met_ang_diffs_hist = new TH1D("met_ang_diffs_hist", "", 100, 0, M_PI);
	TH1D *mmc_hist_metref8 = new TH1D("mmc_hist_metref8","",160,0,160);
	//TH2D *invMass2D = new TH2D("invMass2D", 160,0,160,160,0,160);	  //Not working??
	
       
	TH1D *mmc_hist_f2 = new TH1D("mmc_hist_f2","",160,0,160);
	TH1D *leplep_hist_f2 = new TH1D("leplep_hist_f2","",160,0,160);
	TH1D *mmc_hist_m7_f2 = new TH1D("mmc_hist_m7_f2","",160,0,160);
	TH1D *vis_hist_f2 = new TH1D("vis_hist_f2","",160,0,160);
	TH1D *col_hist_f2 = new TH1D("col_hist_f2","",160,0,160);
	TH1D *col_hist_m7_f2 = new TH1D("col_hist_m7_f2","",160,0,160);
	TH1D *m_phi_rel_hist_f2 = new TH1D("m_phi_rel_hist_f2","",100,-M_PI,M_PI);
	TH1D *met_ang_diffs_hist_f2 = new TH1D("met_ang_diffs_hist_f2", "", 100, 0, M_PI);
	TH1D *mmc_hist_metref8_f2 = new TH1D("mmc_hist_metref8_f2","",160,0,160);
	
        double n_entries =0;

	int file_number = 0;
	for(int i{}; i < size; i++){
		file_number++;
		std::string intString = to_string(file_number);
		if(file_number < 10){ intString = "00" + intString;}
                else if((file_number>9)&&(file_number<100)){ intString = "0" + intString;}

		TFile *f = new TFile(( file1 /*+ bigJobID1 + ".MYSTREAM._000" + intString + ".root"*/).c_str());
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
				/*if(!myClass->InheritsFrom("TH1D")){
					std::cout << "Skipping..." << std::endl;
					continue;
				}*/
				TH1D *myHist = new TH1D;
				myHist = (TH1D*)aKey->ReadObj();
				string histName = myHist->GetName();
				myHist->SetDirectory(0);
				myHist->SetLineWidth(1);
				if(hist_number == 0){
					vis_hist->Add(myHist);
				}else if(hist_number == 1){
					leplep_hist->Add(myHist);
				}else if(hist_number == 2){
					col_hist->Add(myHist);
				}else if(hist_number == 3){
					col_hist_m7->Add(myHist);
				}else if(hist_number == 4){
					mmc_hist->Add(myHist);
				}else if(hist_number == 5){
					mmc_hist_m7->Add(myHist);
				}else if(hist_number == 6){
					mmc_hist_metref8->Add(myHist);
				}else if(hist_number == 7){
					m_phi_rel_hist->Add(myHist);	
				}else if(hist_number == 8){
					met_ang_diffs_hist->Add(myHist);
				}else if(hist_number == 9){
					//invMass2D->Add(myHist);
				}
				hist_number++;
			}
			f->Close();
			delete f;
		}
	}
        


	for(int i{}; i < size; i++){
		file_number++;
		std::string intString = to_string(file_number);
		if(file_number < 10){ intString = "00" + intString;}
                else if((file_number>9)&&(file_number<100)){ intString = "0" + intString;}

		TFile *f = new TFile(( file2 /*+ bigJobID2 + ".MYSTREAM._000" + intString + ".root"*/).c_str());
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
				/*if(!myClass->InheritsFrom("TH1D")){
					std::cout << "Skipping..." << std::endl;
					continue;
				}*/
				TH1D *myHist = new TH1D;
				myHist = (TH1D*)aKey->ReadObj();
				string histName = myHist->GetName();
				myHist->SetDirectory(0);
				myHist->SetLineWidth(1);
				if(hist_number == 0){
					vis_hist_f2->Add(myHist);
				}else if(hist_number == 1){
					leplep_hist_f2->Add(myHist);
				}else if(hist_number == 2){
					col_hist_f2->Add(myHist);
				}else if(hist_number == 3){
					col_hist_m7_f2->Add(myHist);
				}else if(hist_number == 4){
					mmc_hist_f2->Add(myHist);
				}else if(hist_number == 5){
					mmc_hist_m7_f2->Add(myHist);
				}else if(hist_number == 6){
					mmc_hist_metref8_f2->Add(myHist);
				}else if(hist_number == 7){
					m_phi_rel_hist_f2->Add(myHist);	
				}else if(hist_number == 8){
					met_ang_diffs_hist_f2->Add(myHist);
				}else if(hist_number == 9){
					//invMass2D->Add(myHist);
				}
				hist_number++;
			}
			f->Close();
			delete f;
		}
	}





 	//Add entries
	//legend->AddEntry(mmc_hist, "MMC", "l");
	//legend->AddEntry(mmc_hist_m7, "MMC MetTrack", "l");
	//legend->AddEntry(mmc_hist_m7_f2, "Sigma = 6", "l");
	legend->AddEntry(col_hist, "Collinear approx.", "l");
	//legend->AddEntry(col_hist_m7, "Collinear approx.", "l");
	legend->AddEntry(mmc_hist_metref8, "MMC RefMet", "l");
	//legend->AddEntry(mmc_hist_metref8_f2, "Reduced iter.", "l");

	//Set titles
        mmc_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
        mmc_hist_m7->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
        mmc_hist_metref8->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
	col_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
	col_hist_m7->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
        leplep_hist->SetTitle(";M_{ll} [GeV]; N / [GeV]");
	vis_hist->SetTitle(";M_{vis} [GeV]; N / [GeV]");

        mmc_hist_f2->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
        mmc_hist_m7_f2->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
        mmc_hist_metref8_f2->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
	col_hist_f2->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
	col_hist_m7_f2->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
        leplep_hist_f2->SetTitle(";M_{ll} [GeV]; N / [GeV]");
	vis_hist_f2->SetTitle(";M_{vis} [GeV]; N / [GeV]");
	

	col_hist->SetLineColor(2);
	mmc_hist_m7->SetLineColor(1);
	//mmc_hist_m7_f2->SetLineColor(2);
	//mmc_hist_metref8_f2->SetLineColor(2);
	
	//Draws
	col_hist->Draw("histsame");
	//col_hist_m7->Draw("histsame");
	//mmc_hist->Draw("histsame");
	//mmc_hist_m7->Draw("histsame");
	//mmc_hist_m7_f2->Draw("histsame");
	mmc_hist_metref8->Draw("histsame");
	//mmc_hist_metref8_f2->Draw("histsame");
	
	//vis_hist->Draw("histsame");
	
	legend->Draw();
	
	//For drawing met_ang_diffs:	 TODO::FIX THIS AXIS
	/*
	TAxis *ax = met_ang_diffs_hist->GetXaxis();
	ax->SetNdivisions(502);
	ax->ChangeLabel(1,-1,-1,-1,-1,-1,"0");
	ax->ChangeLabel(-1,-1,-1,-1,-1,-1,"#pi");
	ax->ChangeLabel(2,-1,-1,-1,-1,-1,"#frac{#pi}{2}");
	ax->SetLabelOffset(0.015);
	ax->SetTitleOffset(1.2);
	met_ang_diffs_hist->SetTitle(";#phi_{d} [radians];N / [#pi/50 radians]"); 
	met_ang_diffs_hist->Draw("hist");
	*/

	//For drawing M_PHI_REL:
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

int plotter_diff_files(){
	plot();
	return 0;
}
