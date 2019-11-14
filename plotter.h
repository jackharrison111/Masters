#include <TStyle.h>
#include <TCanvas.h>
#include <TH2.h>
#include <vector>





void plotter(string product, string histType){

	

	gROOT->SetStyle("ATLAS");
	gStyle->SetOptStat(1111111);


	TFile *f = new TFile("mc_output.root");
	//("rootOutput/mc_output.root");

	if(!f->IsOpen()){
		std::cout << "Couldn't open mc_output.root" << std::endl;
	}
	

	//TIter next(f->GetListOfKeys());
	//TKey *myKey;


	//Doesnt work
	/*while((myKey = (TKey*)next())){

		
		TObject *obj;
		obj = myKey->ReadObj();
		string name = obj->GetName();
		if(name == "invMassE"){
			if(obj->IsA()->InheritsFrom("TDirectory")){
				TDirectory *dir = new TDirectory("dir","dir");

				dir->GetObject("invMassE",obj);
				dir->cd();
				TIter newnext(f->GetListOfKeys());
				TKey *myNewKey;

				std::cout << "HEREEE" << std::endl;
				//dir->GetObject("averageWaveforms",obj);
				//TIter newnext(dir->GetListOfKeys());
				std::cout << "HEREEEE" << std::endl;	
				//while((myNewKey = (TKey*)newnext())){

				 	
					TObject *obj2;
					obj2 = myNewKey->ReadObj();

					//if(obj2->IsA()->InheritsFrom("TH1")){
						std::cout << "HERE" << std::endl;					
						TH1D *h = new TH1D("h","h",500,0,3e5);
						//h = (TH1D*)obj2;
						//h->Draw();
						h->GetObject("
						std::cout << h->GetName() << std::endl;

				//}
					//}

			}

		}
	}

	//Loop over all the keys in the file (1st set of directories) == 1lep1tau etc	
	while((myKey=(TKey*)next())){
	
		//Make a directory of
		std::cout << myKey->GetName() << "TEST NAME " << endl;
		TDirectory *myDir = new TDirectory;
		myDir = (TDirectory*)myKey->ReadObj();
		string name = myDir->GetName();
		std::cout << name << std::endl;
		
		if(name=="averageWaveforms"){
			f->GetObject(name.c_str(), myDir);
		
			TIter nextDir(myDir->GetListOfKeys());
			TKey *myDirKey;
			
			TCanvas *c = new TCanvas("c", "c");

			while((myDirKey=(TKey*)nextDir())){


				TClass *myClass = gROOT->GetClass(myDirKey->GetClassName());
				if(!myClass->InheritsFrom("TH1D")) continue;
			
				TH1D *myHist = new TH1D;
				myHist = (TH1D*)myDirKey->ReadObj();
				string histName = myHist->GetName();
				//std::cout<<histName << std::endl;
				//if(histName == "channel_162_avwf"){
				myHist->SetDirectory(0);
				myHist->Draw("same");
				//}
				//if(histName=="channel_162_avwf"){
				//std::cout<<histName<<std::endl;
				//}

			}
		}
		

		TClass *myClass = gROOT->GetClass(myKey->GetClassName());
		if(!myClass->InheritsFrom("TH1D")) continue;
		
		TH1D *myHist = new TH1D;
		myHist = (TH1D*)myKey->ReadObj();
		string  Hname = myHist->GetName();
		
		if(Hname == "invMassE"){
			myHist->Draw();
		}

		myHist = (TH1D*)f->Get(Hname.c_str());	
		

	}*/




	//ATTEMPT 2:::
	//
	//
	//f->cd(product + "/" + histType);
	
	f->cd((product +"/" +histType).c_str());
	TIter next(gDirectory->GetListOfKeys());
	TKey *aKey;
	TCanvas *c = new TCanvas("c", "c");
	TH1D *totalHist = new TH1D("totalHist", "Totals", 500, 0, 3e5);
	TH1D *chosenHist = new TH1D;
	
	TList *histList = new TList;

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
<<<<<<< HEAD
		/*if(counter==0){//histName == printChoice){
=======
		if(counter==0){//histName == printChoice){
>>>>>>> f8951f4fb59ab5146330f7955db3279c5ceb3fee
			myHist->SetDirectory(0);
			//myHist->SetLineColor(kRed);
			//myHist->Draw("hist");
			chosenHist = myHist;	
<<<<<<< HEAD
		}*/

		myHist->SetDirectory(0);
		myHist->Draw("histsame");
=======
		}

		myHist->SetDirectory(0);
		//myHist->Draw("same");
>>>>>>> f8951f4fb59ab5146330f7955db3279c5ceb3fee

		
		for(Int_t i = 1; i <= myHist->GetNbinsX(); i++){		
			Double_t content = myHist->GetBinContent(i);
			totalHist->SetBinContent(i,(totalHist->GetBinContent(i) + content));

		}
		
	counter++;
	}
<<<<<<< HEAD
	/*totalHist->SetLineColor(kBlue);	
=======
	totalHist->SetLineColor(kBlue);	
>>>>>>> f8951f4fb59ab5146330f7955db3279c5ceb3fee
	totalHist->SetDirectory(0);	
	totalHist->Draw("hist");
	
	chosenHist->SetLineColor(kRed);
	chosenHist->Draw("histsame");
	std::cout << chosenHist->GetMinimum() << std::endl;
<<<<<<< HEAD
	*/
	delete f;
=======
	delete f;
	//TCanvas *c1 = new TCanvas("mc_Canvas","MC");
	/*for(vector<TH1D>::iterator it = histograms.begin(); it != histograms.end(); ++it){
		(&it)->Draw();	
	}*/
>>>>>>> f8951f4fb59ab5146330f7955db3279c5ceb3fee
}



int plot(){
	
<<<<<<< HEAD
	plotter("foJack", "foJackSq");
=======
	plotter("2lep", "invMassE");
>>>>>>> f8951f4fb59ab5146330f7955db3279c5ceb3fee

	return 0;
}



