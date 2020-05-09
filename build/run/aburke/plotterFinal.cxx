#include <math.h>

//change this
std::string user = "user.jaharris.";
std::string me = "user.jaharris.ZZdataset2_real_900f_18-04_13-18_MYSTREAM/user.jaharris.";

std::string file1 = "local_outputs/ZZ_MCSTDM3_12files_NSIGMET4_22-04_14:37.root";
std::string file2 = "local_outputs/ZZ_MCSTDM3_12files_NSIGMET6_22-04_15:08.root";


std::string real_ds = "user.aburke.final_EW.04-05_23-19_MYSTREAM/user.aburke.";
//"user.jaharris.ZZ_MC_derivSTDM3_SUSY_02-05_22-27_MYSTREAM/user.jaharris.";
//"user.jaharris.ZZ_RE_derivSTDM3_SUSY_04-05_18-38_MYSTREAM/user.jaharris.";
//"user.jaharris.ZZ_MC_derivSTDM3_SUSY_02-05_22-27_MYSTREAM/user.jaharris.";
//"user.jaharris.ZZ_REDAOD_realDatatxt.derivSTDM3_25-04_14-49_MYSTREAM/user.jaharris.";
//"user.jaharris.ZZ_MCDAOD_All364250.derivSTDM3_24-04_21-03_MYSTREAM/user.jaharris.";
//"user.jaharris.ZZ_MCDAOD_4datasets_23-04_18-36_MYSTREAM/user.jaharris.";
//"user.jaharris.ZZ_REDAOD_20datasets_23-04_22-21_MYSTREAM/user.jaharris.";
//user.jaharris.21112268.MYSTREAM._000001.root
//"user.jaharris.ZZdataset2_real_100files_17-04/user.jaharris.";


const int size{24};
//std::string JEDITaskID[size] = {"20718468" , "20718456"};//,"","","",...};
std::string bigJobID1 = "21199461";
//"21186352";  ZZ_MC_derivSTDM3_SUSY_02-05_22-27_MYSTREAM 13
//"21196822";  ZZ_RE_derivSTDM3_SUSY_04-05_18-38_MYSTREAM 27
//"21186352";  ZZ_MC_derivSTDM3_SUSY_02-05_22-27_MYSTREAM 13
//"21126935";  ZZ_REDAOD_realDatatxt.derivSTDM3_25-04_14-49_MYSTREAM
//"21123455";  ZZ_MCDAOD_All364250.derivSTDM3_24-04_21-03_MYSTREAM
//"21110495";  ZZ_MCDAOD_4datasets_23-04_18-36_MYSTREAM
//"21112268";  ZZ_REDAOD_20datasets_23-04_22-21_MYSTREAM
//"21069446";
std::string bigJobID2 = "";
//"21065134";
//"20814417";
//user.jaharris.20719143.MYSTREAM._000001.root  example big job file


double getMEAN(TH1D *myHist, int start_value, int stop_value, int binwidth){
	double mean{};
	double total_counts{};
	for(int i=start_value/binwidth;i<stop_value/binwidth;i+=binwidth){
		mean += myHist->GetBinContent(i)*i*binwidth;
		total_counts+= myHist->GetBinContent(i);
	}
	mean/=total_counts; 	//(stop_value-start_value);
	return mean;
}


double getRMS(TH1D *myHist, int start_value, int stop_value, int binwidth){
	double meanSquare{};
	double total_counts{};
	for(int i=start_value/binwidth;i<stop_value/binwidth;i+=1){
                meanSquare += (myHist->GetBinContent(i))*pow((i*binwidth),2);
                total_counts+= myHist->GetBinContent(i);
        }	
	meanSquare/=total_counts;
	return meanSquare;
}


void plot(){
	//gROOT->SetStyle("ATLAS");
	gROOT->pwd();
	gStyle->SetOptStat(0);
	//gStyle->SetOptStat(1111111);
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


TH1D *vis_hist = new TH1D("vis_hist","Visible Mass Distribution",300,0,300);
TH1D *leplep_hist = new TH1D("leplep_hist", "Direct Z#rightarrow ll Invariant Mass Distribution", 300,0,300);
TH1D *phi_rel_hist = new TH1D("phi_rel_hist", "",100,-2*M_PI,2*M_PI);

TH1D *col_hist = new TH1D("col_hist","Collinear Mass Distribution",300,0,300);
TH1D *col_hist_our = new TH1D("col_hist_our","Collinear Mass Distribution",300,0,300);
TH1D *col_hist_susy = new TH1D("col_hist_susy","Collinear Mass Distribution",300,0,300);

TH1D *mmc_hist = new TH1D("mmc_hist","MMC Mass Distribution",300,0,300);
TH1D *mmc_hist_our = new TH1D("mmc_hist_our","MMC Mass Distribution",300,0,300);
TH1D *mmc_hist_susy = new TH1D("mmc_hist_susy","MMC Mass Distribution",300,0,300);

TH2D *mmc_leps_2D = new TH2D("mmc_leps_2D", "", 300, 0, 300, 300, 0, 300);
TH2D *mmc_leps_2D_our = new TH2D("mmc_leps_2D_our", "", 300, 0, 300, 300, 0, 300);
TH2D *mmc_leps_2D_susy = new TH2D("mmc_leps_2D_susy","",300,0,300,300,0,300);

TH1D *met_hist = new TH1D("met_hist","",400,0,400);
TH1D *met_hist_our = new TH1D("met_hist_our","",400,0,400);
TH1D *met_hist_susy = new TH1D("met_hist_susy","",400,0,400);

TH1D *met_hist_4l = new TH1D("met_hist_4l","",400,0,400);
TH1D *met_hist_4l_our = new TH1D("met_hist_4l_our","",400,0,400);
TH1D *met_hist_4l_susy = new TH1D("met_hist_4l_susy","",400,0,400);

TH1D *col_3piover4_hist = new TH1D("col_3piover4_hist", "",300,0,300);
TH1D *col_3piover4_hist_our = new TH1D("col_3piover4_hist_our", "",300,0,300);
TH1D *col_3piover4_hist_susy = new TH1D("col_3piover4_hist_susy", "",300,0,300);


TH1D *col_piover2_hist = new TH1D("col_piover2_hist", "",300,0,300);
TH1D *col_piover2_hist_our = new TH1D("col_piover2_hist_our", "",300,0,300);
TH1D *col_piover2_hist_susy = new TH1D("col_piover2_hist_susy", "",300,0,300);

TH1D *col_piover4_hist = new TH1D("col_piover4_hist", "",300,0,300);
TH1D *col_piover4_hist_our = new TH1D("col_piover4_hist_our", "",300,0,300);
TH1D *col_piover4_hist_susy = new TH1D("col_piover4_hist_susy", "",300,0,300);

TH1D *col_piover8_hist = new TH1D("col_piover8_hist", "",300,0,300);
TH1D *col_piover8_hist_our = new TH1D("col_piover8_hist_our", "",300,0,300);
TH1D *col_piover8_hist_susy = new TH1D("col_piover8_hist_susy", "",300,0,300);

TH1D *col_piover16_hist = new TH1D("col_piover16_hist", "",300,0,300);
TH1D *col_piover16_hist_our = new TH1D("col_piover16_hist_our", "",300,0,300);
TH1D *col_piover16_hist_susy = new TH1D("col_piover16_hist_susy", "",300,0,300);

TH1D *col_piover32_hist = new TH1D("col_piover32_hist", "",300,0,300);
TH1D *col_piover32_hist_our = new TH1D("col_piover32_hist_our", "",300,0,300);
TH1D *col_piover32_hist_susy = new TH1D("col_piover32_hist_susy", "",300,0,300);


int bin_width{4};
int tail_start{100};
TH1D *mmc_hist_susy_bigBin = new TH1D("mmc_hist_susy_bigBin","MMC Mass Distribution",300/bin_width,0,300);
TH1D *col_hist_susy_bigBin = new TH1D("col_hist_susy_bigBin","MMC Mass Distribution",300/bin_width,0,300);

TH1D *mmc_hist_susy_bigBin_tails = new TH1D("mmc_hist_susy_bigBin_tails","MMC Mass Distribution",(300-tail_start)/bin_width,tail_start,300);
TH1D *col_hist_susy_bigBin_tails = new TH1D("col_hist_susy_bigBin_tails","MMC Mass Distribution",(300-tail_start)/bin_width,tail_start,300);


TH1D *col_3piover4_hist_susy_big = new TH1D("col_3piover4_hist_susy_big", "",300/bin_width,0,300);
TH1D *col_piover2_hist_susy_big = new TH1D("col_piover2_hist_susy_big", "",300/bin_width,0,300);
TH1D *col_piover4_hist_susy_big = new TH1D("col_piover4_hist_susy_big", "",300/bin_width,0,300);


	
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
					col_3piover4_hist_our->Add(myHist);
				}else if(hist_number == 2){
					col_3piover4_hist_susy->Add(myHist);
				}else if(hist_number == 3){
					col_hist->Add(myHist);
				}else if(hist_number == 4){
					col_hist_our->Add(myHist);
				}else if(hist_number == 5){
					col_hist_susy->Add(myHist);
				}else if(hist_number == 6){
					leplep_hist->Add(myHist);
				}else if(hist_number == 7){
					met_hist->Add(myHist);
				}else if(hist_number == 8){
					met_hist_4l->Add(myHist);
				}else if(hist_number == 9){
					met_hist_4l_our->Add(myHist);
				}else if(hist_number == 10){
					met_hist_4l_susy->Add(myHist);
				}else if(hist_number == 11){
					met_hist_our->Add(myHist);
				}else if(hist_number == 12){
					met_hist_susy->Add(myHist);
				}else if(hist_number == 13){
					mmc_hist->Add(myHist);
				}else if(hist_number == 14){
					mmc_leps_2D->Add(myHist);
				}else if(hist_number == 15){
					mmc_leps_2D_our->Add(myHist);
				}else if(hist_number == 16){
					mmc_leps_2D_susy->Add(myHist);
				}else if(hist_number == 17){
					mmc_hist_our->Add(myHist);
				}else if(hist_number == 18){
					mmc_hist_susy->Add(myHist);
				}else if(hist_number == 19){
					phi_rel_hist->Add(myHist);
				}else if(hist_number == 20){
					col_piover16_hist->Add(myHist);
				}else if(hist_number == 21){
					col_piover16_hist_our->Add(myHist);
				}else if(hist_number == 22){
					col_piover16_hist_susy->Add(myHist);
				}else if(hist_number == 23){
					col_piover2_hist->Add(myHist);
				}else if(hist_number == 24){
					col_piover2_hist_our->Add(myHist);
				}else if(hist_number == 25){
					col_piover2_hist_susy->Add(myHist);
				}else if(hist_number == 26){
					col_piover32_hist->Add(myHist);
				}else if(hist_number == 27){
					col_piover32_hist_our->Add(myHist);
				}else if(hist_number == 28){
					col_piover32_hist_susy->Add(myHist);
				}else if(hist_number == 29){
					col_piover4_hist->Add(myHist);
				}else if(hist_number == 30){
					col_piover4_hist_our->Add(myHist);
				}else if(hist_number == 31){
					col_piover4_hist_susy->Add(myHist);
				}else if(hist_number == 32){
					col_piover8_hist->Add(myHist);
				}else if(hist_number == 33){
					col_piover8_hist_our->Add(myHist);
				}else if(hist_number == 34){
					col_piover8_hist_susy->Add(myHist);
				}else if(hist_number == 35){
					vis_hist->Add(myHist);
				}       
				hist_number++;
			}
			f->Close();
			delete f;
		}
	}
        

	for(int i=0; i <300; i++){
		double mmc_total_bin{};
		double col_total_bin{};
		double pi34_total_bin{};
		double pi2_total_bin{};
		double pi4_total_bin{};
		for(int j =0;j<bin_width;j++){
			double bin1 = mmc_hist_susy->GetBinContent(i+j);
			double col_bin1 = col_hist_susy->GetBinContent(i+j);
			mmc_total_bin+=bin1;
			col_total_bin+=col_bin1;
			bin1 = col_3piover4_hist->GetBinContent(i+j);
			pi34_total_bin+=bin1;
			bin1 = col_piover2_hist->GetBinContent(i+j);
			pi2_total_bin+=bin1;
			bin1 = col_piover4_hist->GetBinContent(i+j);
			pi4_total_bin+=bin1;
		}
		mmc_hist_susy_bigBin->SetBinContent(i/bin_width,mmc_total_bin);
		col_hist_susy_bigBin->SetBinContent(i/bin_width,col_total_bin);
		if(i>=tail_start){
		mmc_hist_susy_bigBin_tails->SetBinContent((i-tail_start)/bin_width,mmc_total_bin);
		col_hist_susy_bigBin_tails->SetBinContent((i-tail_start)/bin_width,col_total_bin);
		}


  		col_3piover4_hist_susy_big->SetBinContent(i/bin_width,pi34_total_bin);
		col_piover2_hist_susy_big->SetBinContent(i/bin_width,pi2_total_bin);
		col_piover4_hist_susy_big->SetBinContent(i/bin_width,pi4_total_bin);
		/*double bin1 = mmc_hist_susy->GetBinContent(i);
		double bin2 = mmc_hist_susy->GetBinContent(i+1);	
		double bin3 = mmc_hist_susy->GetBinContent(i+2);	
		double bin4 = 0;//mmc_hist_susy->GetBinContent(i+3);	
		mmc_hist_susy_bigBin->SetBinContent(i/bin_width,bin1 + bin2 + bin3 + bin4);
		double col_bin1 = col_hist_susy->GetBinContent(i);
		double col_bin2 = col_hist_susy->GetBinContent(i+1);	
		double col_bin3 = col_hist_susy->GetBinContent(i+2);	
		double col_bin4 = 0;//col_hist_susy->GetBinContent(i+3);	
		col_hist_susy_bigBin->SetBinContent(i/bin_width,col_bin1 + col_bin2 + col_bin3 + col_bin4);*/
		i+=(bin_width-1);
	}


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
  std::string title = ";M_{#tau#tau} [GeV]; N / ["+std::to_string(bin_width)+"GeV]";
  col_hist_susy_bigBin->SetTitle(title.c_str());
  col_hist_susy_bigBin_tails->SetTitle(title.c_str());
  mmc_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  mmc_hist_susy->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  mmc_hist_susy_bigBin->SetTitle(title.c_str());
  mmc_hist_susy_bigBin_tails->SetTitle(title.c_str());
  mmc_leps_2D->SetTitle(";M_{#tau#tau} [GeV];M_{ll} [GeV]");
  mmc_leps_2D_susy->SetTitle(";M_{#tau#tau} [GeV];M_{ll} [GeV]");
  met_hist->SetTitle(";MET [GeV];N / [GeV]");
  met_hist_susy->SetTitle(";MET [GeV];N / [GeV]");
  phi_rel_hist->SetTitle(";#phi_{rel} [rad]; N / [2#pi/100 rad]");

  col_3piover4_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  col_3piover4_hist_susy_big->SetTitle(title.c_str());
  col_piover2_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover2_hist_susy_big->SetTitle(title.c_str());
  col_piover4_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover4_hist_susy_big->SetTitle(title.c_str());
  col_piover8_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover16_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover32_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
	
 	col_hist_susy->SetLineColor(2);
 	mmc_hist->SetLineColor(2);
 	mmc_hist_susy_bigBin->SetLineColor(2);
 	mmc_hist_susy_bigBin_tails->SetLineColor(2);
	met_hist->SetLineColor(2);
	col_piover2_hist_susy_big->SetLineColor(2);
	col_piover4_hist_susy_big->SetLineColor(3);
	
	//Draws
	//vis_hist->Draw("hist");
	//leplep_hist->Draw("hist");
	//col_hist->Draw("hist");
	//col_hist_susy->Draw("histsame");
	mmc_hist_susy_bigBin->Draw("hist");
	//col_hist_susy_bigBin->Draw("histsame");
	//mmc_hist_susy_bigBin_tails->Draw("hist");
	//col_hist_susy_bigBin_tails->Draw("histsame");
	//mmc_hist_susy->Draw("histsame");
	//mmc_hist_susy->Draw("histsame");
	//mmc_hist->Draw("histsame");
	//mmc_hist_m7->Draw("histsame");
	//met_hist_susy->Draw("histsame");
	//met_hist->Draw("histsame");
	//invMass2D->Draw("col");
	

  	//col_3piover4_hist_susy_big->Draw("histsame");
	//col_piover2_hist_susy_big->Draw("histsame");
	//col_piover4_hist_susy_big->Draw("histsame");
	//legend->Draw();
	
	int peak_start{60};
	int peak_end{120};

	double mmc_mean = getMEAN(mmc_hist_susy_bigBin, peak_start, peak_end, bin_width);
	double mmc_rms = sqrt(getRMS(mmc_hist_susy_bigBin, peak_start, peak_end, bin_width));
	double mmc_sd = sqrt(getRMS(mmc_hist_susy_bigBin, peak_start, peak_end, bin_width)-pow(mmc_mean,2));




	double col_mean = getMEAN(col_hist_susy_bigBin, peak_start, peak_end, bin_width);
	double col_rms = sqrt(getRMS(col_hist_susy_bigBin, peak_start, peak_end, bin_width));
	double col_sd = sqrt(getRMS(col_hist_susy_bigBin, peak_start, peak_end, bin_width)-pow(col_mean,2));


	std::cout << mmc_mean << "GeV  =  mmc mean" << std::endl;
	std::cout << mmc_rms << "GeV  =  mmc rms" << std::endl;
	std::cout << mmc_sd << "GeV  =  mmc s.d." << std::endl;
	std::cout << col_mean << "GeV  =  col mean" << std::endl;
	std::cout << col_rms << "GeV  =  col rms" << std::endl;
	std::cout << col_sd << "GeV = col s.d." << std::endl;
//	std::cout << getMEAN(mmc_hist_susy, 60, 120, 1) << "GeV  =  mmc 1bin mean" << std::endl;
//	std::cout << getMEAN(col_hist_susy, 60, 120, 1) << "GeV  =  col 1bin mean" << std::endl;


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

int plotterFinal(){
	plot();
	return 0;
}
