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
				return true;
			}
		}
	}
	return false;
}

Double_t mini::GetOpenAngle(Double_t lepAng, Double_t tauAng){
	Double_t openAngle = abs(lepAng-tauAng);
	if(openAngle>pi){
		openAngle = 2*pi-openAngle;
	}
	return openAngle;
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

	

	TFile output(("rootOutput/"+outputName+"output_tau_piMinus_5-12.root").c_str(),"RECREATE");
	TDirectory *TDir = output.mkdir("1lep1tau");
	std::map<string,TH1*> histograms;

	histograms["invMassVis"] = new TH1D("invMassVis", "Z#rightarrowllVis",200,0,160);
	histograms["invMassleptau"] = new TH1D("invMassleptau","Z->l#tau",200,0,160);
	histograms["invMass3lep1tau"] = new TH1D("invMass3lep1tau","Z->lll#tau",200,0,160);
	histograms["missEtDist"] = new TH1D("missEtDist","Distribution of missing transverse momentum",200,-1*pi,pi);


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




		

		Double_t invM1, invM2, invM3, invM4;
		if(Cut(2,1,1)||Cut(1,2,1)||Cut(3,0,1)||Cut(0,3,1)){
			Int_t totalQ = (*lep_charge)[0]+(*lep_charge)[1]+(*lep_charge)[2];
			if(totalQ+(*tau_charge)[0]!=0) continue;
			Int_t tauPartner;
			Int_t oddLep;
			

			// all 3 leps same type
			if((*lep_type)[0]==(*lep_type)[1]&&(*lep_type)[0]==(*lep_type)[2]){
				vector<Int_t> sameLeps; //stores the indices of the leptons which have the same charge eg 2 electrons
				for(Int_t j=0; j<3; j++){
					if((*lep_charge)[j]==-totalQ){
					//eg if totalQ is -1 then there are 2 electrons and 1 positron
					//then the odd lepton is the positron
					//ie the odd lepton has opposite charge to totalQ
						oddLep=j;
					}
				}
				for(Int_t j=0; j<3; j++){
					if(j!=oddLep){
						sameLeps.push_back(j);
					}
				}
				// now compare lepton - lepton pairings and oddlepton - tau pairings
				// ......
				invM1 = sqrt(2*(*lep_pt)[oddLep]*(*lep_pt)[sameLeps[0]]*(cosh((*lep_eta)[oddLep]-(*lep_eta)[sameLeps[0]])-cos((*lep_phi)[oddLep]-(*lep_phi)[sameLeps[0]])))/1000;
				invM2 = sqrt(2*(*lep_pt)[oddLep]*(*lep_pt)[sameLeps[1]]*(cosh((*lep_eta)[oddLep]-(*lep_eta)[sameLeps[1]])-cos((*lep_phi)[oddLep]-(*lep_phi)[sameLeps[1]])))/1000;
				
				if(abs(invM1 - zMass) < abs(invM2 - zMass)){
					histograms["invMassleptau"]->Fill(invM1);
					tauPartner = sameLeps[1];
					//Efficiency++;
				}else{
					histograms["invMassleptau"]->Fill(invM2);
					tauPartner = sameLeps[0];
					//Efficiency++
				}
				
				Double_t nu_T_lep = met_et*(sin(met_phi)-sin((*tau_phi)[0]))/(sin((*lep_phi)[tauPartner])-sin((*tau_phi)[0]));
				Double_t nu_T_had = met_et*(sin(met_phi)-sin((*lep_phi)[tauPartner]))/(sin((*tau_phi)[0])-sin((*lep_phi)[tauPartner]));
				Double_t A = nu_T_lep*cosh((*lep_eta)[tauPartner])+nu_T_had*cosh((*tau_eta)[0])
				            +(*lep_pt)[tauPartner]*cosh((*lep_eta)[tauPartner])+(*tau_pt)[0]*cosh((*tau_eta)[0]);
				Double_t B = nu_T_lep*cos((*lep_phi)[tauPartner])+nu_T_had*cos((*tau_phi)[0])
				            +(*lep_pt)[tauPartner]*cos((*lep_phi)[tauPartner])+(*tau_pt)[0]*cos((*tau_phi)[0]);
				Double_t C = nu_T_lep*sin((*lep_phi)[tauPartner])+nu_T_had*sin((*tau_phi)[0])
				            +(*lep_pt)[tauPartner]*sin((*lep_phi)[tauPartner])+(*tau_pt)[0]*sin((*tau_phi)[0]);
				Double_t D = nu_T_lep*sinh((*lep_eta)[tauPartner])+nu_T_had*sinh((*tau_eta)[0])
				            +(*lep_pt)[tauPartner]*sinh((*lep_eta)[tauPartner])+(*tau_pt)[0]*sinh((*tau_eta)[0]);
				invM3 = sqrt(pow(A,2)-pow(B,2)-pow(C,2)-pow(D,2))/1000;
				histograms["invMassleptau"]->Fill(invM3);

				Double_t t = (*tau_phi)[0];
				Double_t l = (*lep_phi)[tauPartner];
				if(t>l){
					histograms["missEtDist"]->Fill((met_phi/*-pi/2*/-(*lep_phi)[tauPartner])*pi/(t-l));//GetOpenAngle(t,l));
				}else{
					histograms["missEtDist"]->Fill(-1*(met_phi/*-pi/2*/-(*tau_phi)[0])*pi/(l-t));//GetOpenAngle(t,l));
				}

				invM4 = sqrt(2*(*lep_pt)[tauPartner]*(*tau_pt)[0]*(cosh((*lep_eta)[tauPartner]-(*tau_eta)[0])-cos((*lep_phi)[tauPartner]-(*tau_phi)[0])))/1000;
				histograms["invMassVis"]->Fill(invM4);
			
			}

			// 2 leps same type, other not
			else{
				pair<Int_t, Int_t> leps;
				if((*lep_type)[0]==(*lep_type)[1]){ //pair 0&1
					leps.first=0;
					leps.second=1;
				}else if((*lep_type)[1]==(*lep_type)[2]){ //pair 1&2
					leps.first=1;
					leps.second=2;		
				}else{ //pair 0&2
					leps.first=0;
					leps.second=2;
				}
				for(Int_t j=0; j<3; j++){
					if(j==leps.first||j==leps.second) continue;
					//the odd lepton in this case is the one which is not the same type as the other two
					oddLep=j;
				}
				// now compare lepton - lepton pairings and oddlepton - tau pairings
				// ......
				invM1 = sqrt(2*(*lep_pt)[leps.first]*(*lep_pt)[leps.second]*(cosh((*lep_eta)[leps.first]-(*lep_eta)[leps.second])-cos((*lep_phi)[leps.first]-(*lep_phi)[leps.second])))/1000;
				histograms["invMassleptau"]->Fill(invM1);

				Double_t nu_T_lep = met_et*(sin(met_phi)-sin((*tau_phi)[0]))/(sin((*lep_phi)[oddLep])-sin((*tau_phi)[0]));
				Double_t nu_T_had = met_et*(sin(met_phi)-sin((*lep_phi)[oddLep]))/(sin((*tau_phi)[0])-sin((*lep_phi)[oddLep]));
				Double_t A = nu_T_lep*cosh((*lep_eta)[oddLep])+nu_T_had*cosh((*tau_eta)[0])
				            +(*lep_pt)[oddLep]*cosh((*lep_eta)[oddLep])+(*tau_pt)[0]*cosh((*tau_eta)[0]);
				Double_t B = nu_T_lep*cos((*lep_phi)[oddLep])+nu_T_had*cos((*tau_phi)[0])
				            +(*lep_pt)[oddLep]*cos((*lep_phi)[oddLep])+(*tau_pt)[0]*cos((*tau_phi)[0]);
				Double_t C = nu_T_lep*sin((*lep_phi)[oddLep])+nu_T_had*sin((*tau_phi)[0])
				            +(*lep_pt)[oddLep]*sin((*lep_phi)[oddLep])+(*tau_pt)[0]*sin((*tau_phi)[0]);
				Double_t D = nu_T_lep*sinh((*lep_eta)[oddLep])+nu_T_had*sinh((*tau_eta)[0])
				            +(*lep_pt)[oddLep]*sinh((*lep_eta)[oddLep])+(*tau_pt)[0]*sinh((*tau_eta)[0]);
				invM2 = sqrt(pow(A,2)-pow(B,2)-pow(C,2)-pow(D,2))/1000;
				histograms["invMassleptau"]->Fill(invM2);

				Double_t t = (*tau_phi)[0];
				Double_t l = (*lep_phi)[oddLep];
				if(t>l){
					histograms["missEtDist"]->Fill((met_phi/*-pi/2*/-(*lep_phi)[oddLep])*pi/(t-l));//GetOpenAngle(t,l));
				}else{
					histograms["missEtDist"]->Fill(-1*(met_phi/*-pi/2*/-(*tau_phi)[0])*pi/(l-t));//GetOpenAngle(t,l));
				}

				histograms["missEtDist"]->Fill(pi*met_phi/((*tau_phi)[0]-(*lep_phi)[oddLep]));
			
				invM4 = sqrt(2*(*lep_pt)[oddLep]*(*tau_pt)[0]*(cosh((*lep_eta)[oddLep]-(*tau_eta)[0])-cos((*lep_phi)[oddLep]-(*tau_phi)[0])))/1000;
				histograms["invMassVis"]->Fill(invM4);
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
	output.Close(); //Close the output file
} 



// so we don't need to keep typing it in the terminal
Int_t maintau(){
	mini a;
	a.Run();

	return 0;
}
