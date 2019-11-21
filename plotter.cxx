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
	TH1D *totalHist = new TH1D("totalHist", "Totals", 200, 0, 160);
	
	
	TFile *f = new TFile("mc_output_21-11.root");	//("rootOutput/mc_output.root");
	if(!f->IsOpen()){
		std::cout << "Couldn't open mc_output.root" << std::endl;
	}
	
	string Zlep = "invMass2l";
	//for(vector<string>::iterator it = productNames.begin(); it != productNames.end(); it++){
		f->cd((product +"/" + histType).c_str());
		gDirectory->pwd();
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
			std::cout << histName << std::endl;
			for(Int_t i = 1; i <= myHist->GetNbinsX(); i++){		
				Double_t content = myHist->GetBinContent(i);
				std::cout << content << std::endl;
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
	TFile *f2 = new TFile("re_output_21-11.root");
	if(!f2->IsOpen()){
		std::cout << "Couldn't open re_output.root" << std::endl;
	}

	//for(vector<string>::iterator at = productNames.begin(); at != productNames.end(); at++){
		f2->cd((product + "/" + histType).c_str());
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
			reHist->SetDirectory(0);
					
			for(Int_t i = 1; i <= reHist->GetNbinsX(); i++){		
				Double_t re_content = reHist->GetBinContent(i);
				re_totalHist->SetBinContent(i,(re_totalHist->GetBinContent(i) + re_content));
			}
		}
	//}
	delete f2;

	//ADD LEGEND AND TITLES TO NEW HISTOGRAM
	legend->SetHeader("Data source", "C");
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
	
	Int_t upperFit{120};
	Int_t lowerFit{40};
	TF1 *invMassFit = new TF1("invMassFit",Fit,lowerFit,upperFit,order+4); //hardcoded
	invMassFit->SetParNames("#mu","#gamma","A","a","b","c");
	invMassFit->SetParameters(90,5,1,1,1,1);
	invMassFit->SetParLimits(0,86,96);
	invMassFit->SetParLimits(1,0,50);
	invMassFit->SetLineColor(kRed);
	re_totalHist->Fit("invMassFit","+RN");
	
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
