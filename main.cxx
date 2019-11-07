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

//gaussian fit for resonance
Double_t mini::Gaussian(Double_t *x, Double_t *par){
	Double_t g = par[2]*exp(-pow(((x[0]-par[0])/par[1]),2)/2);
	return g;
}

//lorentzian fit for resonance
Double_t mini::Lorentz(Double_t *x, Double_t *par){
	Double_t L = par[1]*par[2]/(pow(x[0]-par[0],2) + pow(0.5*par[1],2));
	return L;
}

//fit to resonance background (quadratic/cubic)
Double_t mini::Background(Double_t *x, Double_t *par, Int_t order){
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

Int_t mini::order = 0; //global??
//combine
Double_t mini::Fit(Double_t *x, Double_t *par){
	return Lorentz(x, par) + Background(x,&par[3],order);
}









void mini::Run(){
	
	gROOT->SetStyle("ATLAS");
	gStyle->SetOptStat(1111111);

	if (fChain == 0) return;

	Long64_t n = fChain->GetEntries/*Fast*/();
	Long64_t nbytes = 0, nb = 0;
	
	//Save the output file to the correct place based on data type
	string outputName;
	if(MC){
		outputName="mc_";
	}else{
		outputName="re_";
	}

	

	TFile output((outputName+"output.root").c_str(),"RECREATE");
	TDirectory *TDir1 = output.mkdir("1fatjet1lep");
	TDirectory *TDir2 = output.mkdir("1lep");
	TDirectory *TDir3 = output.mkdir("1lep1tau");
	TDirectory *TDir4 = output.mkdir("1tau");
	TDirectory *TDir5 = output.mkdir("2lep");
	TDirectory *TDir6 = output.mkdir("2tau");
	TDirectory *TDir7 = output.mkdir("GamGam");
	TH1D *invMassZee = new TH1D("invMassZee","Z->ee",500,0,3e5);
	TH1D *invMassE = new TH1D("invMassE","Z->ee",500,0,3e5);
	TH1D *invMassMu = new TH1D("invMassMu","Z->#mu#mu",500,0,3e5);
	TH1D *invMassTot = new TH1D("invMassTot","Z->ee||#mu#mu",500,0,3e5);
	TH1D *invMass4l = new TH1D("invMass4l","Z->llll",500,0,3e5);
	TH2D *invMass2D_EMu = new TH2D("invMass2D_EMu","ZZ->ee&&#mu#mu",100,0,3e5,100,0,3e5);
	TH2D *invMass2D_EE = new TH2D("invMass2D_EE","ZZ->ee&&ee",100,0,3e5,100,0,3e5);
	TH2D *invMass2D_MuMu = new TH2D("invMass2D_MuMu","ZZ->#mu#mu&&#mu#mu",100,0,3e5,100,0,3e5);
	std::map<string,TH1*> histograms;
	histograms["invMassZee"]=invMassZee;
	histograms["invMassE"]=invMassE;
	histograms["invMassMu"]=invMassMu;
	histograms["invMassTot"]=invMassTot;
	histograms["invMass4l"]=invMass4l;
	histograms["invMass2D_EMu"]=invMass2D_EMu;
	histograms["invMass2D_EE"]=invMass2D_EE;
	histograms["invMass2D_MuMu"]=invMass2D_MuMu;

	TH1D *test = new TH1D("test","test",500,0,3e2);


	Int_t counter{0};
	clock_t startTime = clock();
	
	string fileName;
	string oldFileName; //to calculate lumFactor only once for each MC file
	string shortFileName;
	string products;
	Double_t lumFactor;
	//Loop over all events
	for (Long64_t i=0; i<n; i++){
		Long64_t ientry = LoadTree(i);
		if(ientry < 0) break;
		nb = fChain->GetEntry(i);   nbytes += nb;
		
		fileName = (chain->GetFile())->GetName();

		Double_t eventWeight = 1;
		if(MC){
			if(fileName!=oldFileName){ //dont want to calculate lumFactor repeatedly, only once per file/per event type
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
				convert i;
				i.makeMap();
				lumFactor=1000*totRealLum*i.infos[shortFileName]["xsec"]/(i.infos[shortFileName]["sumw"]*i.infos[shortFileName]["red_eff"]);
				
				gDirectory->cd(products.c_str());
				std::map<string,TH1*>::iterator it = histograms.begin();
				while(it!=histograms.end()){
					(it->second)->SetName((products+"_"+it->first+"_"+shortFileName).c_str());
					it++;
				}
			}
			eventWeight = mcWeight*scaleFactor_PILEUP*scaleFactor_ELE*scaleFactor_MUON*scaleFactor_PHOTON*scaleFactor_TAU*scaleFactor_BTAG*scaleFactor_LepTRIGGER*scaleFactor_PhotonTRIGGER*scaleFactor_TauTRIGGER*scaleFactor_DiTauTRIGGER*lumFactor;
		}
		
		////2 ELECTRON EVENTS////
		Double_t invMee;
		if(Cut(2,0)){
			invMee = sqrt(2*(*lep_pt)[0]*(*lep_pt)[1]*(cosh((*lep_eta)[0]-(*lep_eta)[1])-cos((*lep_phi)[0]-(*lep_phi)[1])));
			histograms["invMassZee"]->Fill(invMee,eventWeight);
		}
		/////////////////////////



		////4 LEPTON EVENTS////
		Double_t invMsqrtE, invMsqrtMu;
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
				invMsqrtE = sqrt(2*(*lep_pt)[0]*(*lep_pt)[which]*(cosh((*lep_eta)[0]-(*lep_eta)[which])-cos((*lep_phi)[0]-(*lep_phi)[which])));
				invMsqrtMu = sqrt(2*(*lep_pt)[others[0]]*(*lep_pt)[others[1]]*(cosh((*lep_eta)[others[0]]-(*lep_eta)[others[1]])-cos((*lep_phi)[others[0]]-(*lep_phi)[others[1]])));
			}else{
				invMsqrtMu = sqrt(2*(*lep_pt)[0]*(*lep_pt)[which]*(cosh((*lep_eta)[0]-(*lep_eta)[which])-cos((*lep_phi)[0]-(*lep_phi)[which])));
				invMsqrtE = sqrt(2*(*lep_pt)[others[0]]*(*lep_pt)[others[1]]*(cosh((*lep_eta)[others[0]]-(*lep_eta)[others[1]])-cos((*lep_phi)[others[0]]-(*lep_phi)[others[1]])));
			}

			//fill histograms based off the 2,2 events
			histograms["invMassE"]->Fill(invMsqrtE,eventWeight);
			histograms["invMassMu"]->Fill(invMsqrtMu,eventWeight);
			histograms["invMass2D_EMu"]->Fill(invMsqrtE,invMsqrtMu/*,eventWeight*/);

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
			Double_t invMsqrt1 = sqrt(2*(*lep_pt)[pos.first]*(*lep_pt)[neg.first]*(cosh((*lep_eta)[pos.first]-(*lep_eta)[neg.first])-cos((*lep_phi)[pos.first]-(*lep_phi)[neg.first])))/1000;
			Double_t invMsqrt2 = sqrt(2*(*lep_pt)[pos.second]*(*lep_pt)[neg.second]*(cosh((*lep_eta)[pos.second]-(*lep_eta)[neg.second])-cos((*lep_phi)[pos.second]-(*lep_phi)[neg.second])))/1000;
			//first with second, second with first
			Double_t invMsqrt3 = sqrt(2*(*lep_pt)[pos.first]*(*lep_pt)[neg.second]*(cosh((*lep_eta)[pos.first]-(*lep_eta)[neg.second])-cos((*lep_phi)[pos.first]-(*lep_phi)[neg.second])))/1000;
			Double_t invMsqrt4 = sqrt(2*(*lep_pt)[pos.second]*(*lep_pt)[neg.first]*(cosh((*lep_eta)[pos.second]-(*lep_eta)[neg.first])-cos((*lep_phi)[pos.second]-(*lep_phi)[neg.first])))/1000;

			if(invMsqrt1<100&&invMsqrt1>80 && invMsqrt2<100&&invMsqrt2>80){ //hardcoded
				//std::cout<<"second pairing:  1: "<<invMsqrt3<<" , 2: "<<invMsqrt4<<std::endl<<std::endl;
				test->Fill(invMsqrt3);
				test->Fill(invMsqrt4);
			}else if(invMsqrt3<100&&invMsqrt3>80 && invMsqrt4<100&&invMsqrt4>80){ //hardcoded
				//std::cout<<"first pairing:  1: "<<invMsqrt1<<" , 2: "<<invMsqrt2<<std::endl<<std::endl;
				test->Fill(invMsqrt1);
				test->Fill(invMsqrt2);
			}


			/*pair <Int_t,Int_t> pair1, pair2;   //Pairs of ints to store the correct pairing indices
			Bool_t firstOneSet=false;
			Bool_t set=false;

			for(Int_t j=1; j<4; j++){
				if((*lep_charge)[j]!=(*lep_charge)[0] && set==false){
					pair1.first=0;		//Set the first and second pairs 
					pair1.second=j;
					set=true;
				}else if(firstOneSet==false){   //If you haven't already set the first same charged lepton
					pair2.first=j;
					firstOneSet=true;
				}else{
					pair2.second=j;         //Otherwise set the last lepton in pair2
				}
			}
			//Uncomment to check that pairs are correct
			//std::cout<<"("<<pos.first<<","<<pos.second<<") , ("<<neg.first<<","<<neg.second<<")"<<std::endl;
			//std::cout<<(*lep_charge)[0]<<","<<(*lep_charge)[1]<<","<<(*lep_charge)[2]<<","<<(*lep_charge)[3]<<std::endl<<std::endl;
			
			
			//Find InvMass for pair 1
			invMsqrtE = sqrt(2*(*lep_pt)[pair1.first]*(*lep_pt)[pair1.second]*(cosh((*lep_eta)[pair1.first]-(*lep_eta)[pair1.second])-cos((*lep_phi)[pair1.first]-(*lep_phi)[pair1.second])));
			
			//Find InvMass for pair 2 (ignore Mu label as just re-using previous variable)
			invMsqrtMu = sqrt(2*(*lep_pt)[pair2.first]*(*lep_pt)[pair2.second]*(cosh((*lep_eta)[pair2.first]-(*lep_eta)[pair2.second])-cos((*lep_phi)[pair2.first]-(*lep_phi)[pair2.second])));*/

			//Fill the histograms the correct way round
			if((*lep_type)[0]==11){
				histograms["invMassE"]->Fill(invMsqrtE,eventWeight);
				histograms["invMassE"]->Fill(invMsqrtMu,eventWeight);
				histograms["invMass2D_EE"]->Fill(invMsqrtE,invMsqrtMu/*,eventWeight*/);
			}else{
				histograms["invMassMu"]->Fill(invMsqrtE,eventWeight);
				histograms["invMassMu"]->Fill(invMsqrtMu,eventWeight);
				histograms["invMass2D_MuMu"]->Fill(invMsqrtE,invMsqrtMu/*,eventWeight*/);
			}
			
		}

		//Finding invariant mass of whole 4 lepton event using Equation [3] in lab book
		if(lep_n==4){
			histograms["invMass4l"]->Fill(sqrt(pow((*lep_pt)[0]*cosh((*lep_eta)[0])+(*lep_pt)[1]*cosh((*lep_eta)[1])+(*lep_pt)[2]*cosh((*lep_eta)[2])+(*lep_pt)[3]*cosh((*lep_eta)[3]),2)-pow((*lep_pt)[0]*cos((*lep_phi)[0])+(*lep_pt)[1]*cos((*lep_phi)[1])+(*lep_pt)[2]*cos((*lep_phi)[2])+(*lep_pt)[3]*cos((*lep_phi)[3]),2)-pow((*lep_pt)[0]*sin((*lep_phi)[0])+(*lep_pt)[1]*sin((*lep_phi)[1])+(*lep_pt)[2]*sin((*lep_phi)[2])+(*lep_pt)[3]*sin((*lep_phi)[3]),2)-pow((*lep_pt)[0]*sinh((*lep_eta)[0])+(*lep_pt)[1]*sinh((*lep_eta)[1])+(*lep_pt)[2]*sinh((*lep_eta)[2])+(*lep_pt)[3]*sinh((*lep_eta)[3]),2)),eventWeight);
		}
			
		//Fill an invariant mass histogram of both e and mu 2 events
		for(Int_t j=1; j<=histograms["invMassE"]->GetNbinsX(); j++){
			histograms["invMassTot"]->SetBinContent(j,histograms["invMassE"]->GetBinContent(j)+histograms["invMassMu"]->GetBinContent(j));
		}
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
	test->Write();
	output.Close(); //Close the output file

} 



// so we don't need to keep typing it in the terminal
Int_t main(){
	mini a;
	a.Run();

	return 0;
}
