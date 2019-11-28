//TODO: make an if statement to check whether dataSets.json contains shortFileName
#define main_cxx
#include "mainMC.h" //change this for mc or real data
#include "converter.h" //for usage of infofile.py here
#include <TH2.h>
//#include <TROOT.h>
//#include <TRint.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <math.h>
#include <map>
const Double_t pi = M_PI;

//MeV:
Double_t eMass = 0.511;
Double_t pMass = 938.28;
Double_t sqrtS = 13e6;
Double_t zMass = 91.2;
Double_t totRealLum = 10.064; //inv fb - open data set

//function to return number of chosen lepton type in an event
Int_t mini::numberOfType(Int_t type){
	Int_t num=0;
	for(Int_t i=0; i<lep_n; i++){
		if((*lep_type)[i] == type){
			num++;
		}
	}
	return num;
}

//add cut decisions to this function
Bool_t mini::Cut(Int_t e, Int_t mu, Int_t tau){ //electron and muons only so far
	if(lep_n==e+mu && tau_n==tau){ //select events with number of leptons equal to function inputs
		if(numberOfType(11)==e && numberOfType(13)==mu){ //further selection
			Int_t charge{0};
			for(Int_t i=0; i<e+mu; i++){
				charge+=(*lep_charge)[i];
			}
			if(charge==1 || charge==-1){ //ie 2 leptons 1 antilepton or vice versa
				std::cout<<"lepton no.: "<<lep_n<<std::endl;
				std::cout<<"no. electrons: "<<e<<", no. muons: "<<mu<<std::endl;
				std::cout<<"total lepton charge: "<<charge<<std::endl;
				std::cout<<"no. jets: "<<jet_n<<std::endl;
				std::cout<<"no. taus: "<<tau_n<<std::endl;
				std::cout<<"tau charges: "<<(*tau_charge)[0]<<", "<<(*tau_charge)[1]<<std::endl<<std::endl;
				return true;
			}
		}
	}
	return false;
}




void mini::Run(){
	Int_t tauCounter{0};
	
	gROOT->SetStyle("ATLAS");
	gStyle->SetOptStat(1111111);

	if (fChain == 0) return;

	Long64_t n = fChain->GetEntries();//Fast();
	//if(n == 9223372036854775807){
	//	n = fChain->GetEntries();
	//}
	//std::cout << n << std::endl;
	Long64_t nbytes = 0, nb = 0;
	
	//Save the output file to the correct place based on data type
	string outputName;
	if(MC){
		outputName="mc_";
	}else{
		outputName="re_";
	}

	

	TFile output((outputName+"output_26-11_tau.root").c_str(),"RECREATE");
	TDirectory *TDir3 = output.mkdir("1lep1tau");
	std::map<string,TH1*> histograms;
	histograms["invMassZee"]=new TH1D("invMassZee","Z->ee",200,0,160);
	histograms["invMassZmumu"]=new TH1D("invMassZmumu","Z->#mu#mu",200,0,160);
	//histograms["invMassE"]=new TH1D("invMassE","Z->ee",200,0,160);
	//histograms["invMassMu"]=new TH1D("invMassMu","Z->#mu#mu",200,0,160);
	//histograms["invMassTot"]=new TH1D("invMassTot","Z->ee||#mu#mu",200,0,160);
	histograms["invMass2l"]=new TH1D("invMass2l","Z->ll",200,0,160);
	histograms["invMass4l"]=new TH1D("invMass4l","Z'->llll",200,0,160);
	histograms["invMass2D_EMu"]=new TH2D("invMass2D_EMu","ZZ->ee&&#mu#mu",100,0,160,100,0,160);
	histograms["invMass2D_EE"]=new TH2D("invMass2D_EE","ZZ->ee&&ee",100,0,160,100,0,160);
	histograms["invMass2D_MuMu"]=new TH2D("invMass2D_MuMu","ZZ->#mu#mu&&#mu#mu",100,0,160,100,0,160);

	Int_t counter{0};
	clock_t startTime = clock();
	
	string fileName;
	string oldFileName; //to calculate lumFactor only once for each MC file
	string shortFileName;
	string products;
	Double_t lumFactor;

	Int_t fileCounter{1};
	Bool_t newFile{true};
	for (Long64_t i=0; i<n; i++){
		Long64_t ientry = LoadTree(i);
		if(ientry < 0) break;
		nb = fChain->GetEntry(i);   nbytes += nb;
		
		fileName = (chain->GetFile())->GetName();

		Double_t eventWeight = 1;
		if(fileName!=oldFileName){ //dont want to calculate lumFactor repeatedly, only once per file/per event type
			std::cout<<(chain->GetFile())->GetSize()/1e6<<" MB : File "<<fileCounter<<" out of "<<((chain->GetListOfFiles())->GetLast()+1)<<", "<<fileName<<std::endl;
			fileCounter++;
			if(i!=0){
				std::map<string,TH1*>::iterator it=histograms.begin();
				while(it!=histograms.end()){
					if(gDirectory->cd(it->first.c_str())){
						(it->second)->Write((products+"_"+it->first+"_"+shortFileName).c_str(),TObject::kWriteDelete);
					}
					else{
						gDirectory->mkdir(it->first.c_str());
						gDirectory->cd(it->first.c_str());
						(it->second)->Write((products+"_"+it->first+"_"+shortFileName).c_str(),TObject::kWriteDelete);
					}
					gDirectory->cd("..");
					(it->second)->Reset();
					it++;
				}
				output.cd();
			}
			oldFileName=fileName;
			products=oldFileName.substr(12,oldFileName.find('/',12)-12); //12 is the position after "/data/ATLAS/"
			shortFileName=oldFileName.substr(oldFileName.find_last_of('/')+1,oldFileName.length()-oldFileName.find_last_of('/')); //to get rid of "/data/ATLAS/2lep/MC/" from the TChain file strings
			if(MC){
				convert i;
				i.makeMap();
				
				lumFactor=1000*totRealLum*i.infos[shortFileName]["xsec"]/(i.infos[shortFileName]["sumw"]*i.infos[shortFileName]["red_eff"]);
				//TODO: fix this:
				if(i.infos[shortFileName]["sumw"]==0){
					lumFactor=0;
				}
				//std::cout << i.infos[shortFileName]["xsec"] << " / (" << i.infos[shortFileName]["sumw"] << " * " << i.infos[shortFileName]["red_eff"] << std::endl; 
			}
			
			gDirectory->cd(products.c_str());
			std::map<string,TH1*>::iterator it = histograms.begin();
			while(it!=histograms.end()){
				(it->second)->SetName((products+"_"+it->first+"_"+shortFileName).c_str());
				it++;
			}
		}
		if(MC){
			eventWeight = mcWeight*scaleFactor_PILEUP*scaleFactor_ELE*scaleFactor_MUON*scaleFactor_PHOTON*scaleFactor_TAU*scaleFactor_BTAG*scaleFactor_LepTRIGGER*scaleFactor_PhotonTRIGGER*scaleFactor_TauTRIGGER*scaleFactor_DiTauTRIGGER*lumFactor;
		}
		if(lumFactor==0) continue; //breaks the current iteration of for loop, continues with next event?

//		if(Cut(2,1,1)||Cut(1,2,1)){
//		}
		if(tau_n==1 && lep_n==3){
			/*tauCounter++;
			std::cout<<"lep_n="<<lep_n<<std::endl<<"lep_charges: ";
			for(Int_t j=0; j<lep_n; j++){
				std::cout<<(*lep_charge)[j]<<" ";
			}
			std::cout<<"\ntau_charges="<<(*tau_charge)[0]<<","<<(*tau_charge)[1]<<std::endl<<std::endl;*/
			std::cout<<(*lep_charge)[0]+(*lep_charge)[1]+(*lep_charge)[2]<<std::endl;
		}
		
		
		//to write the last files histograms (loop ends after last event in last file,
		//but writing normally occurs at start of next loop)
		if(i==(n-1)){
			std::map<string,TH1*>::iterator it=histograms.begin();
			while(it!=histograms.end()){
				if(gDirectory->cd(it->first.c_str())){
					(it->second)->Write((products+"_"+it->first+"_"+shortFileName).c_str(),TObject::kWriteDelete);
				}
				else{
					gDirectory->mkdir(it->first.c_str());
					gDirectory->cd(it->first.c_str());
					(it->second)->Write((products+"_"+it->first+"_"+shortFileName).c_str(),TObject::kWriteDelete);
				}
				gDirectory->cd("..");
				(it->second)->Reset();
				it++;
			}
		}
	}
	//Print the time taken to run the loop (relies on startTime at beginning of loop)
	clock_t endTime = clock();
	std::cout<<"Run time: "<<(endTime-startTime)/CLOCKS_PER_SEC<<" s"<<std::endl<<std::endl;

	output.cd();
	output.Close(); //Close the output file
	
	std::cout<<tauCounter<<std::endl;
} 



// so we don't need to keep typing it in the terminal
Int_t maintau(){
	mini a;
	a.Run();

	return 0;
}