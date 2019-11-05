#include <TStyle.h>
#include <TCanvas.h>
#include <TH2.h>
#include <vector>





void plotter(string chosenHist){

	
	vector<TH1D> histograms;
	vector<string> histNames;

	gROOT->SetStyle("ATLAS");
	gStyle->SetOptStat(1111111);


	TFile *f = new TFile("SER_hist_100e_X100_Y250_Z250_1.5GeV.root");
	//("rootOutput/mc_output.root");

	if(!f->IsOpen()){
		std::cout << "Couldn't open mc_output.root" << std::endl;
	}
	


	TIter next(f->GetListOfKeys());
	TKey *myKey;

	std::cout << "HERE" << std::endl;

	while((myKey = (TKey*)next())){
		
		std::cout << "HEREE" << std::endl;
		TObject *obj;
		obj = myKey->ReadObj();
		string name = obj->GetName();
		if(name == "averageWaveforms"){
			TKey *myNewKey;
			if(obj->IsA()->InheritsFrom("TDirectory")){
				TDirectory *dir;

				std::cout << "HEREEE" << std::endl;
				dir->GetObject("averageWaveforms",obj);
				TIter newnext(dir->GetListOfKeys());
				
				while((myKey = (TKey*)newnext())){
				 	
					TObject *obj2;
					obj2 = myNewKey->ReadObj();
					std::cout << "HEREEEE" << std::endl;
					if(obj2->IsA()->InheritsFrom("TH1")){
						std::cout << "HEREEEEE" << std::endl;					
						TH1D *h;
						h = (TH1D*)obj2;
						std::cout << h->GetName() << std::endl;

					}
				}

			}

		}
	}
	
	/*while((myKey=(TKey*)next())){
	
		TDirectory *myDir = new TDirectory;
		myDir = (TDirectory*)myKey->ReadObj();
		string name = myDir->GetName();
		std::cout << name << std::endl;
		
		if(name=="averageWaveforms"){
			f->GetObject(name.c_str(), myDir);
		
			TIter nextDir(myDir->GetListOfKeys());
			TKey *myDirKey;


			while((myDirKey=(TKey*)nextDir())){


				TClass *myClass = gROOT->GetClass(myDirKey->GetClassName());
				if(!myClass->InheritsFrom("TH1D")) continue;
			
				TH1D *myHist = new TH1D;
				myHist = (TH1D*)myDirKey->ReadObj();
				string histName = myHist->GetName();
				//std::cout<<histName << std::endl;
				if(histName == "channel_162_avwf"){
				//myHist->SetDirectory(0);
				//myHist->Draw();
				}
				if(histName=="channel_162_avwf"){
				std::cout<<histName<<std::endl;
				histograms.push_back(*myHist);
				histNames.push_back(histName);
				}

			}
		}
		

		TClass *myClass = gROOT->GetClass(myKey->GetClassName());
		if(!myClass->InheritsFrom("TH1D")) continue;
		
		TH1D *myHist = new TH1D;
		myHist = (TH1D*)myKey->ReadObj();
		string name = myHist->GetName();
		
		if(name == "invMassE"){
			myHist->Draw();
		}

		myHist = (TH1D*)f->Get(name.c_str());	
		histNames.push_back(name);
		histograms.push_back(*myHist);
		

	}*/


	delete f;


	//(&histograms[0])->SetDirectory(gROOT);
	//(&histograms[0])->Draw();
	

	//TCanvas *c1 = new TCanvas("mc_Canvas","MC");
	/*for(vector<TH1D>::iterator it = histograms.begin(); it != histograms.end(); ++it){
		(&it)->Draw();	
	}*/
	




}




