//TODO: make an if statement to check whether dataSets.json contains shortFileName
#define main_cxx
#include "mainMC.h" //change this for mc or real data
#include "converter.h" //for usage of infofile.py here
#include "plottertau.cxx"
#include <TH2.h>
//#include <TROOT.h>
//#include <TRint.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <math.h>
#include <map>

//MeV:
Double_t eMass = 0.511;
Double_t pMass = 938.28;
Double_t sqrtS = 13e6;
//Double_t zMass = 91.2;
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
				return true;
			}
		}
	}
	return false;
}

Double_t mini::GetOpenAngle(Double_t ang1, Double_t ang2){
	Double_t openAngle = abs(ang1-ang2);
	if(openAngle>M_PI){
		openAngle = 2*M_PI-openAngle;//TODO: *-1 ?
	}
	return openAngle;
}

Double_t getOpeningAngle(Double_t tauPhi, Double_t lepPhi){

	Double_t angle;
	angle = tauPhi - lepPhi;
	angle = abs(angle);
	if(angle > M_PI){
		angle = 2*M_PI - angle;
	}

	return angle;
}

Double_t cot(Double_t angle){
	return cos(angle) / sin(angle);
}

Double_t acot(Double_t angle){
	return atan(1/angle);
}


void mini::Run(){
	gStyle->SetOptStat(0);

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


	TFile output(("rootOutput/"+outputName+"output_tau_23-12.root").c_str(),"RECREATE");

	TDirectory *TDir = output.mkdir("1lep1tau");
	std::map<string,TH1*> histograms;

	histograms["invMass"] = new TH1D("invMass3lep1tau","Z->lll#tau",160,0,160);
	


	
	clock_t startTime = clock();
	
	string fileName;
	string oldFileName; //to calculate lumFactor only once for each MC file
	string shortFileName;
	string products;
	Double_t lumFactor;

	Double_t Efficiency{0};
	Double_t sumw;

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
				sumw = i.infos[shortFileName]["sumw"];
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




			
		//Cuts for 2tau	
		Double_t invM1, invM2; //1&2 for leplep, 3 for vis, 4 for leptau
		if(Cut(0,1,1)||Cut(1,0,1)){
			
			Int_t totalQ = (*lep_charge)[0];
			if(totalQ+(*tau_charge)[0]!=0) continue;

			Int_t tauPartner;
			Int_t oddLep;
			
			
			Double_t t = (*tau_phi)[0];
			Double_t l;
			Double_t pt_t = (*tau_pt)[0];
			Double_t pt_l;
			Double_t theta_t = 2*atan(exp(-(*tau_eta)[0]));
			Double_t theta_l;
			l = (*lep_phi)[tauPartner];
			Double_t x1, x2;
			
			pt_l = (*lep_pt)[tauPartner];
			theta_l = 2*atan(exp(-(*lep_eta)[tauPartner]));
			
			x1 = (*lep_pt)[tauPartner]/((*lep_pt)[tauPartner]+nu_T_lep);
			x2 = (*tau_pt)[0]/((*tau_pt)[0]+nu_T_had);
				
			invM1 = sqrt(2*(*lep_pt)[oddLep]*(*lep_pt)[sameLeps[0]]*(cosh((*lep_eta)[oddLep]-(*lep_eta)[sameLeps[0]])-cos((*lep_phi)[oddLep]-(*lep_phi)[sameLeps[0]])))/1000;
				
				
			Double_t nu_T_lep = met_et*(sin(met_phi)-sin((*tau_phi)[0]))/(sin((*lep_phi)[tauPartner])-sin((*tau_phi)[0]));
			Double_t nu_T_had = met_et*(sin(met_phi)-sin((*lep_phi)[tauPartner]))/(sin((*tau_phi)[0])-sin((*lep_phi)[tauPartner]));

			
			//Sem1 final cuts;
			//if(invM3<80 && 2*halfAng<=2.5 && 2*halfAng>=0.5 && phi_rel<=3*M_PI/5 && phi_rel>=-7*M_PI/10){
			if((MC)&&invM1<100&&invM1>80&&invM4<120&&invM4>40){
				if(sumw!=0) Efficiency+=(eventWeight/lumFactor)/sumw;
				}
			}
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
	
	gDirectory->cd(products.c_str());
	gDirectory->mkdir("Efficiency");
	gDirectory->cd("Efficiency");
	TVectorD v(1);
	if(MC){
		v[0]=Efficiency;
	}else{
		v[0]=Efficiency/n;
	}
	
	std::cout<<"efficiency from vector = "<<v[0]<<std::endl;
	v.Write("efficiency");
	output.Close(); //Close the output file

} 



// so we don't need to keep typing it in the terminal
Int_t maintau(){
	mini a;
	a.Run();
	//plottertau();

	
	return 0;
}
