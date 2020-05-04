//change this
std::string user = "user.jaharris.";
std::string me = "user.jaharris.ZZdataset2_real_900f_18-04_13-18_MYSTREAM/user.jaharris.";

std::string file1 = "local_outputs/ZZ_MCSTDM3_12files_NSIGMET4_22-04_14:37.root";
std::string file2 = "local_outputs/ZZ_MCSTDM3_12files_NSIGMET6_22-04_15:08.root";

std::string real_ds = "user.jaharris.ZZ_MC_derivSTDM3_SUSY_02-05_22-27_MYSTREAM/user.jaharris.";
//"user.jaharris.ZZ_REDAOD_realDatatxt.derivSTDM3_25-04_14-49_MYSTREAM/user.jaharris.";
//"user.jaharris.ZZ_MCDAOD_All364250.derivSTDM3_24-04_21-03_MYSTREAM/user.jaharris.";
//"user.jaharris.ZZ_MCDAOD_4datasets_23-04_18-36_MYSTREAM/user.jaharris.";
//"user.jaharris.ZZ_REDAOD_20datasets_23-04_22-21_MYSTREAM/user.jaharris.";
//user.jaharris.21112268.MYSTREAM._000001.root
//"user.jaharris.ZZdataset2_real_100files_17-04/user.jaharris.";


const int size{12};
//std::string JEDITaskID[size] = {"20718468" , "20718456"};//,"","","",...};
std::string bigJobID1 = "21186352";
//"21126935";  ZZ_REDAOD_realDatatxt.derivSTDM3_25-04_14-49_MYSTREAM
//"21123455";  ZZ_MCDAOD_All364250.derivSTDM3_24-04_21-03_MYSTREAM
//"21110495";  ZZ_MCDAOD_4datasets_23-04_18-36_MYSTREAM
//"21112268";  ZZ_REDAOD_20datasets_23-04_22-21_MYSTREAM
//"21069446";
std::string bigJobID2 = "";
//"21065134";
//"20814417";
//user.jaharris.20719143.MYSTREAM._000001.root  example big job file


void plot(){
	//gROOT->SetStyle("ATLAS");
	gROOT->pwd();
	gStyle->SetOptStat(1111111);
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


TH1D *vis_hist = new TH1D("vis_hist","Visible Mass Distribution",160,0,160);
TH1D *leplep_hist = new TH1D("leplep_hist", "Direct Z#rightarrow ll Invariant Mass Distribution", 160,0,160);
TH1D *col_hist = new TH1D("col_hist","Collinear Mass Distribution",160,0,160);
TH1D *col_hist_susy = new TH1D("col_hist_susy","Collinear Mass Distribution",160,0,160);
TH1D *mmc_hist = new TH1D("mmc_hist","MMC Mass Distribution",160,0,160);
TH1D *mmc_hist_susy = new TH1D("mmc_hist_susy","MMC Mass Distribution",160,0,160);
TH2D *mmc_leps_2D = new TH2D("mmc_leps_2D", "", 160, 0, 160, 160, 0, 160);
TH2D *mmc_leps_2D_susy = new TH2D("mmc_leps_2D_susy","",160,0,160,160,0,160);
TH1D *met_hist = new TH1D("met_hist","",400,0,400);
TH1D *met_hist_susy = new TH1D("met_hist_susy","",400,0,400);
TH1D *phi_rel_hist = new TH1D("phi_rel_hist", "",50,-M_PI,M_PI);

TH1D *col_3piover4_hist = new TH1D("col_3piover4_hist", "",160,0,160);
TH1D *col_piover2_hist = new TH1D("col_piover2_hist", "",160,0,160);
TH1D *col_piover4_hist = new TH1D("col_piover4_hist", "",160,0,160);
TH1D *col_piover8_hist = new TH1D("col_piover8_hist", "",160,0,160);
TH1D *col_piover16_hist = new TH1D("col_piover16_hist", "",160,0,160);
TH1D *col_piover32_hist = new TH1D("col_piover32_hist", "",160,0,160);


TH1D *mmc_hist_susy_bigBin = new TH1D("mmc_hist_susy_bigBin","MMC Mass Distribution",80,0,160);
TH1D *col_hist_susy_bigBin = new TH1D("col_hist_susy_bigBin","MMC Mass Distribution",80,0,160);





	
       /*
	TH1D *mmc_hist_f2 = new TH1D("mmc_hist_f2","",160,0,160);
	TH1D *leplep_hist_f2 = new TH1D("leplep_hist_f2","",160,0,160);
	TH1D *mmc_hist_f2_m7 = new TH1D("mmc_hist_f2_m7","",160,0,160);
	TH1D *mmc_hist_f2_metref8 = new TH1D("mmc_hist_f2_metref8","",160,0,160);
	TH1D *vis_hist_f2 = new TH1D("vis_hist_f2","",160,0,160);
	TH1D *col_hist_f2 = new TH1D("col_hist_f2","",160,0,160);
	TH1D *col_hist_f2_m7 = new TH1D("col_hist_f2_m7","",160,0,160);
	TH1D *m_phi_rel_hist_f2 = new TH1D("m_phi_rel_hist_f2","",100,-M_PI,M_PI);
	TH1D *met_ang_diffs_hist_f2 = new TH1D("met_ang_diffs_hist_f2", "", 100, 0, M_PI);

	TH2D *invMass2D_f2 = new TH2D("invMass2D_f2", "invMass2D_f2", 160,0,160,160,0,160);
	TH2D *invMass2D_m7_f2 = new TH2D("invMass2D_m7_f2", "invMass2D_m7_f2", 160,0,160,160,0,160);
	*/
        double n_entries =0;

	int file_number = 0;
	for(int i{}; i < size; i++){
		file_number++;
		std::string intString = to_string(file_number);
		if(file_number < 10){ intString = "00" + intString;}
                else if((file_number>9)&&(file_number<100)){ intString = "0" + intString;}

		TFile *f = new TFile(( real_ds + bigJobID1 + ".MYSTREAM._000" + intString + ".root").c_str());
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
					col_3piover4_hist->Add(myHist);
				}else if(hist_number == 1){
					col_hist->Add(myHist);
				}else if(hist_number == 2){
					col_hist_susy->Add(myHist);
				}else if(hist_number == 3){
					col_piover16_hist->Add(myHist);
				}else if(hist_number == 4){
					col_piover2_hist->Add(myHist);
				}else if(hist_number == 5){
					col_piover32_hist->Add(myHist);	
				}else if(hist_number == 6){
					col_piover4_hist->Add(myHist);
				}else if(hist_number == 7){
					col_piover8_hist->Add(myHist);
				}else if(hist_number == 8){
					leplep_hist->Add(myHist);
				}else if(hist_number == 9){
					met_hist->Add(myHist);
				}else if(hist_number == 10){
					met_hist_susy->Add(myHist);
				}else if(hist_number == 11){
					mmc_hist->Add(myHist);
				}else if(hist_number == 12){
					mmc_hist_susy->Add(myHist);
				}else if(hist_number == 13){
					mmc_leps_2D->Add(myHist);
				}else if(hist_number == 14){
					mmc_leps_2D_susy->Add(myHist);
				}else if(hist_number == 15){
					phi_rel_hist->Add(myHist);
				}else if(hist_number == 16){
					vis_hist->Add(myHist);
				}       
				hist_number++;
			}
			f->Close();
			delete f;
		}
	}
        

	for(int i=0; i <160; i++){
		double bin1 = mmc_hist_susy->GetBinContent(i);
		double bin2 = mmc_hist_susy->GetBinContent(i+1);	
		mmc_hist_susy_bigBin->SetBinContent(i/2,bin1 + bin2);
		double col_bin1 = col_hist_susy->GetBinContent(i);
		double col_bin2 = col_hist_susy->GetBinContent(i+1);	
		col_hist_susy_bigBin->SetBinContent(i/2,col_bin1 + col_bin2);
		i++;
	}

/*


	file_number = 0;
	for(int i{}; i < size; i++){
		file_number++;
		std::string intString = to_string(file_number);
		if(file_number < 10){ intString = "00" + intString;}
                else if((file_number>9)&&(file_number<100)){ intString = "0" + intString;}
*/
//		TFile *f = new TFile(( file2 /*+ bigJobID2 + ".MYSTREAM._000" + intString + ".root"*/).c_str());
/*		if(!f->IsOpen()){
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
*/				/*if(!myClass->InheritsFrom("TH1D")){
					std::cout << "Skipping..." << std::endl;
					continue;
				}*/
/*				TH1D *myHist = new TH1D;
				myHist = (TH1D*)aKey->ReadObj();
				string histName = myHist->GetName();
				myHist->SetDirectory(0);
				myHist->SetLineWidth(1);
				if(hist_number == 0){
					invMass2D_f2->Add(myHist);
				}else if(hist_number == 1){
					invMass2D_m7_f2->Add(myHist);
				}else if(hist_number == 2){
					col_hist_f2->Add(myHist);
				}else if(hist_number == 3){
					col_hist_f2_m7->Add(myHist);
				}else if(hist_number == 4){
					leplep_hist_f2->Add(myHist);
				}else if(hist_number == 5){
					m_phi_rel_hist_f2->Add(myHist);	
				}else if(hist_number == 6){
					met_ang_diffs_hist_f2->Add(myHist);
				}else if(hist_number == 7){
					mmc_hist_f2->Add(myHist);
				}else if(hist_number == 8){
					mmc_hist_f2_m7->Add(myHist);
				}else if(hist_number == 9){
					mmc_hist_f2_metref8->Add(myHist);
				}else if(hist_number == 10){
					vis_hist_f2->Add(myHist);
				}
				hist_number++;
			}
			f->Close();
			delete f;
		}
	}


*/


 	//Add entries
	//legend->AddEntry(mmc_hist, "MET Tool", "l");
	legend->AddEntry(mmc_hist_susy_bigBin, "MMC", "l");
	//legend->AddEntry(mmc_hist_susy, "1 GeV", "l");
	//legend->AddEntry(col_hist, "Collinear approx.", "l");
	//legend->AddEntry(col_hist_susy, "Collinear approx.", "l");
	legend->AddEntry(col_hist_susy_bigBin, "Collinear approx.", "l");
	//legend->AddEntry(met_hist, "MetTool", "l");
	//legend->AddEntry(met_hist_susy, "SUSYTool", "l");
	
	//Set titles

  vis_hist->SetTitle(";M_{l#tau} [GeV]; N / [GeV]");
  leplep_hist->SetTitle(";M_{ll} [GeV]; N / [GeV]");
  col_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  col_hist_susy->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  col_hist_susy_bigBin->SetTitle(";M_{#tau#tau} [GeV]; N / [2GeV]");
  mmc_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  mmc_hist_susy->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  mmc_hist_susy_bigBin->SetTitle(";M_{#tau#tau} [GeV]; N / [2GeV]");
  mmc_leps_2D->SetTitle(";M_{#tau#tau} [GeV];M_{ll} [GeV]");
  mmc_leps_2D_susy->SetTitle(";M_{#tau#tau} [GeV];M_{ll} [GeV]");
  met_hist->SetTitle(";MET [GeV];N / [GeV]");
  met_hist_susy->SetTitle(";MET [GeV];N / [GeV]");
  phi_rel_hist->SetTitle(";#phi_{rel} [rad]; N / [2#pi/100 rad]");

  col_3piover4_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover2_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover4_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover8_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover16_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover32_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
	
 	col_hist_susy->SetLineColor(2);
 	mmc_hist->SetLineColor(2);
 	mmc_hist_susy_bigBin->SetLineColor(2);
	met_hist->SetLineColor(2);
	
	//Draws
	//vis_hist->Draw("hist");
	//leplep_hist->Draw("hist");
	//col_hist->Draw("hist");
	//col_hist_susy->Draw("histsame");
	mmc_hist_susy_bigBin->Draw("hist");
	col_hist_susy_bigBin->Draw("histsame");
	//mmc_hist_susy->Draw("histsame");
	//mmc_hist_susy->Draw("histsame");
	//mmc_hist->Draw("histsame");
	//mmc_hist_m7->Draw("histsame");
	//met_hist_susy->Draw("histsame");
	//met_hist->Draw("histsame");
	//invMass2D->Draw("col");
	legend->Draw();
	
	/*TCanvas *d = new TCanvas("d", "d");
	d->SetTickx();
	d->SetTicky();
	mmc_hist_metref8->Draw("hist");
	*/
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
	h->Draw("same");
	*/
}

int plotterzz(){
	plot();
	return 0;
}
