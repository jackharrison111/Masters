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
Bool_t mini::Cut(Int_t e, Int_t mu){ //electron and muons only so far
	if(lep_n==e+mu){ //select events with number of leptons equal to function inputs
		if(numberOfType(11)==e && numberOfType(13)==mu){ //further selection
			Int_t charge{0};
			for(Int_t i=0; i<e+mu; i++){
				charge+=(*lep_charge)[i];
			}
			if(charge==0){ //ie number of leptons equals number of antileptons
				//if(stuff to do with pt,ptcone etc..
				//pt>25e3,ptcone30/pt<0.05..
				return true;
			}
		}
	}
	return false;
}




void mini::Run(){
	
	gROOT->SetStyle("ATLAS");
	gStyle->SetOptStat(1111111);

	if (fChain == 0) return;

	Long64_t n = fChain->GetEntries();//Fast();
	//if(n == 9223372036854775807){
	//	n = fChain->GetEntries();
	//}
	std::cout << n << std::endl;
	Long64_t nbytes = 0, nb = 0;
	
	//Save the output file to the correct place based on data type
	string outputName;
	if(MC){
		outputName="mc_";
	}else{
		outputName="re_";
	}

	

	TFile output((outputName+"output_helpme.root").c_str(),"RECREATE");
	TDirectory *TDir1 = output.mkdir("1fatjet1lep");
	TDirectory *TDir2 = output.mkdir("1lep");
	TDirectory *TDir3 = output.mkdir("1lep1tau");
	TDirectory *TDir4 = output.mkdir("1tau");
	TDirectory *TDir5 = output.mkdir("2lep");
	TDirectory *TDir6 = output.mkdir("2tau");
	TDirectory *TDir7 = output.mkdir("GamGam");
	std::map<string,TH1*> histograms;
	histograms["invMassZee"]=new TH1D("invMassZee","Z->ee",200,0,160);
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
			std::cout<<(chain->GetFile())->GetSize()/1e6<<" MB : File "<<fileCounter<<" out of "<<((chain->GetListOfFiles())->GetLast()+1)<<std::endl;
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
				fileCounter++;
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
		
		if(shortFileName == "mc15_13TeV.307431.MGPy8EG_A14NNPDF23LO_RS_G_ZZ_llll_c10_m0200.2lep_raw.root"){
			//std::cout << eventWeight << std::endl;
		}


		////2 ELECTRON EVENTS////
		Double_t invMee;
		if(Cut(2,0)){
			invMee = sqrt(2*(*lep_pt)[0]*(*lep_pt)[1]*(cosh((*lep_eta)[0]-(*lep_eta)[1])-cos((*lep_phi)[0]-(*lep_phi)[1])))/1000;
			histograms["invMassZee"]->Fill(invMee,eventWeight);
		}
		/////////////////////////
		////4 LEPTON EVENTS////
		Double_t invM1, invM2, invM3, invM4;
		//Check for 2e 2mu events
		if(Cut(2,2)){
			//pair up the same type leptons
			counter++;
			Int_t k{0};
			Int_t which;
			Int_t others[2];
			for(Int_t j=1; j<4; j++){
				if((*lep_type)[j]==(*lep_type)[0]){
					which = j;
				}else{
					others[k]=j;
					k++;
				}
			}

			//Find the electron/muon invariant mass from the correct pairings
			if((*lep_type)[0]==11){
				invM1 = sqrt(2*(*lep_pt)[0]*(*lep_pt)[which]*(cosh((*lep_eta)[0]-(*lep_eta)[which])-cos((*lep_phi)[0]-(*lep_phi)[which])))/1000;
				invM2 = sqrt(2*(*lep_pt)[others[0]]*(*lep_pt)[others[1]]*(cosh((*lep_eta)[others[0]]-(*lep_eta)[others[1]])-cos((*lep_phi)[others[0]]-(*lep_phi)[others[1]])))/1000;
			}else{
				invM2 = sqrt(2*(*lep_pt)[0]*(*lep_pt)[which]*(cosh((*lep_eta)[0]-(*lep_eta)[which])-cos((*lep_phi)[0]-(*lep_phi)[which])))/1000;
				invM1 = sqrt(2*(*lep_pt)[others[0]]*(*lep_pt)[others[1]]*(cosh((*lep_eta)[others[0]]-(*lep_eta)[others[1]])-cos((*lep_phi)[others[0]]-(*lep_phi)[others[1]])))/1000;
			}
			//fill histograms based off the 2,2 events
			histograms["invMass2l"]->Fill(invM1,eventWeight);
			histograms["invMass2l"]->Fill(invM2,eventWeight);
			histograms["invMass2D_EMu"]->Fill(invM1,invM2/*,eventWeight*/);

		}else if(Cut(4,0)||Cut(0,4)){    //include 4 lepton events of all the same type
			pair<Int_t,Int_t> pos, neg; //pai positive leptons and negative leptons
			Bool_t posSet=false;
			Bool_t negSet=false; //check that the first of the pair hsa been assigned
			for(Int_t j=0; j<4; j++){
				if((*lep_charge)[j]==1){
					if(posSet==false){
						pos.first=j;
						posSet=true;
					}else{
						pos.second=j;
					}
				}else if((*lep_charge)[j]==-1){
					if(negSet==false){
						neg.first=j;
						negSet=true;
					}else{
						neg.second=j;
					}
				}
			}
			//first with first, second with second
			invM1 = sqrt(2*(*lep_pt)[pos.first]*(*lep_pt)[neg.first]*(cosh((*lep_eta)[pos.first]-(*lep_eta)[neg.first])-cos((*lep_phi)[pos.first]-(*lep_phi)[neg.first])))/1000;
			invM2 = sqrt(2*(*lep_pt)[pos.second]*(*lep_pt)[neg.second]*(cosh((*lep_eta)[pos.second]-(*lep_eta)[neg.second])-cos((*lep_phi)[pos.second]-(*lep_phi)[neg.second])))/1000;
			//first with second, second with first
			invM3 = sqrt(2*(*lep_pt)[pos.first]*(*lep_pt)[neg.second]*(cosh((*lep_eta)[pos.first]-(*lep_eta)[neg.second])-cos((*lep_phi)[pos.first]-(*lep_phi)[neg.second])))/1000;
			invM4 = sqrt(2*(*lep_pt)[pos.second]*(*lep_pt)[neg.first]*(cosh((*lep_eta)[pos.second]-(*lep_eta)[neg.first])-cos((*lep_phi)[pos.second]-(*lep_phi)[neg.first])))/1000;
	
			/*
			// this finds closest reconstruction to M_Z
			vector<Double_t> deltas;
			deltas.push_back(abs(invM1-zMass));
			deltas.push_back(abs(invM2-zMass));
			deltas.push_back(abs(invM3-zMass));
			deltas.push_back(abs(invM4-zMass));
			std::sort(deltas.begin(), deltas.end());
			vector<Double_t> masses;
			masses.push_back(invM1);
			masses.push_back(invM2);
			masses.push_back(invM3);
			masses.push_back(invM4);
			pair<Double_t, Double_t> pair1(invM1, invM2);
			pair<Double_t, Double_t> pair2(invM3, invM4);
			for(vector<Double_t>::iterator it = masses.begin(); it != masses.end(); it++){
				Double_t diff = abs(*it - zMass);
				if(diff == deltas[0]){
					if((*it == pair1.first)||(*it == pair1.second)){
						test->Fill(pair1.first);
						test->Fill(pair1.second);
					}else{
						test->Fill(pair2.first);
						test->Fill(pair2.second);
					}

				}

			}
			*/
			
			Int_t lower = 86;
			Int_t higher = 96;
			if((invM1<higher&&invM1>lower)||(invM2<higher&&invM2>lower)){ //hardcoded
				histograms["invMass2l"]->Fill(invM1,eventWeight);
				histograms["invMass2l"]->Fill(invM2,eventWeight);
				if((*lep_type)[0]==11){
					histograms["invMass2D_EE"]->Fill(invM1,invM2);
				}else{
					histograms["invMass2D_MuMu"]->Fill(invM1,invM2);
				}
			}
		  	else if((invM3<higher&&invM3>lower)||(invM4<higher&&invM4>lower)){ //hardcoded
				histograms["invMass2l"]->Fill(invM3,eventWeight);
				histograms["invMass2l"]->Fill(invM4,eventWeight);
				if((*lep_type)[0]==11){
					histograms["invMass2D_EE"]->Fill(invM3,invM4);
				}else{
					histograms["invMass2D_MuMu"]->Fill(invM3,invM4);
				}
			}
		}

		//Finding invariant mass of whole 4 lepton event using Equation [3] in lab book
		if(lep_n==4){
			histograms["invMass4l"]->Fill(sqrt(pow((*lep_pt)[0]*cosh((*lep_eta)[0])+(*lep_pt)[1]*cosh((*lep_eta)[1])+(*lep_pt)[2]*cosh((*lep_eta)[2])+(*lep_pt)[3]*cosh((*lep_eta)[3]),2)-pow((*lep_pt)[0]*cos((*lep_phi)[0])+(*lep_pt)[1]*cos((*lep_phi)[1])+(*lep_pt)[2]*cos((*lep_phi)[2])+(*lep_pt)[3]*cos((*lep_phi)[3]),2)-pow((*lep_pt)[0]*sin((*lep_phi)[0])+(*lep_pt)[1]*sin((*lep_phi)[1])+(*lep_pt)[2]*sin((*lep_phi)[2])+(*lep_pt)[3]*sin((*lep_phi)[3]),2)-pow((*lep_pt)[0]*sinh((*lep_eta)[0])+(*lep_pt)[1]*sinh((*lep_eta)[1])+(*lep_pt)[2]*sinh((*lep_eta)[2])+(*lep_pt)[3]*sinh((*lep_eta)[3]),2))/1000,eventWeight);
		}
		
		/*
		//Fill an invariant mass histogram of both e and mu 2 events
		for(Int_t j=1; j<=histograms["invMassE"]->GetNbinsX(); j++){
			histograms["invMassTot"]->SetBinContent(j,histograms["invMassE"]->GetBinContent(j)+histograms["invMassMu"]->GetBinContent(j));
		}
		*/
		/////////////////////
		
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

} 



// so we don't need to keep typing it in the terminal
Int_t main(){
	mini a;
	a.Run();

	return 0;
}
