//TODO: make an if statement to check whether dataSets.json contains shortFileName
#define main_cxx
#include "mainMC.h" //change this for mc or real data
#include "converter.h" //for usage of infofile.py here
#include "plotter.cxx"
#include "//athena-21.2/PhysicsAnalysis/TauID/DiTauMassTools/Root/MissingMassCalculatorV2.cxx"
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

	MissingMassCalculator* tool = new MissingMassCalculator;

	TFile output(("rootOutput/"+outputName+"output_28-01.root").c_str(),"RECREATE");
	TDirectory *TDir = output.mkdir("1lep1tau");
	std::map<string,TH1*> histograms;
	histograms["visMass"] = new TH1D("visMass","Z->l#tau_{jet}",160,0,160);
	histograms["invMass"] = new TH1D("invMass","Z->#tau#tau",160,0,160);
	histograms["invMassCutMissingEt1.5"] = new TH1D("invMassCutMissingEt1.5","Z->#tau#tau",160,0,160);
	histograms["invMassCutMissingEt1"] = new TH1D("invMassCutMissingEt1","Z->#tau#tau",160,0,160);
	histograms["invMassCutMissingEt0.5"] = new TH1D("invMassCutMissingEt0.5","Z->#tau#tau",160,0,160);
	histograms["missingEt"] = new TH1D("missingEt","missing Et",100,-M_PI,M_PI);
	Double_t nIntervals=50;
	histograms["etContainedFrac"] = new TH1D("etContainedFrac","Fraction of missing transverse momentum as a function of opening angle",nIntervals,0,M_PI);
	
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
	
	vector<Double_t> openingAngle;
	for(Double_t i=0; i<nIntervals; i++){
		openingAngle.push_back(i/nIntervals*M_PI);
	}
	vector<Double_t> fractionContained(nIntervals,0);
	vector<Double_t> openingAngleCounter(nIntervals,0);

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
		Double_t invM1, invM2; //1 for vis, 2 for full
		if(Cut(0,1,1)||Cut(1,0,1)){
			
			Int_t totalQ = (*lep_charge)[0];
			if(totalQ+(*tau_charge)[0]!=0) continue;

			Double_t t_pt = (*tau_pt)[0];
			Double_t t_phi = (*tau_phi)[0];
			Double_t t_eta = (*tau_eta)[0];
			Double_t t_theta = 2*atan(exp(-t_eta));
			Double_t l_pt = (*lep_pt)[0];
			Double_t l_phi = (*lep_phi)[0];
			Double_t l_eta = (*lep_eta)[0];
			Double_t l_theta = 2*atan(exp(-l_eta));
			
			Double_t x1, x2;
				
			invM1 = sqrt(2*l_pt*t_pt*(cosh(l_eta-t_eta)-cos(l_phi-t_phi)))/1000; //visible
			histograms["visMass"]->Fill(invM1,eventWeight);
				
			Double_t nu_T_lep = met_et*(sin(met_phi)-sin(t_phi))/(sin(l_phi)-sin(t_phi));
			Double_t nu_T_had = met_et*(sin(met_phi)-sin(l_phi))/(sin(t_phi)-sin(l_phi));
			
			x1 = l_pt/(l_pt+nu_T_lep);
			x2 = t_pt/(t_pt+nu_T_had);

			Double_t halfAng = getOpeningAngle(t_phi,l_phi)/2;
			Double_t rotationAngle;
			if(t_phi<l_phi){
				rotationAngle = -t_phi;
			}else{
				rotationAngle = -l_phi;
			}
			t_phi += rotationAngle;
			l_phi += rotationAngle;
			met_phi += rotationAngle;
			
			if(t_phi>M_PI) t_phi-=2*M_PI;
			if(l_phi>M_PI) l_phi-=2*M_PI;
			if(t_phi<0||l_phi<0){
				t_phi += halfAng;
				l_phi += halfAng;
				met_phi += halfAng;
			}else{
				t_phi -= halfAng;
				l_phi -= halfAng;
				met_phi -= halfAng;
			}

			if(met_phi>M_PI) met_phi-=2*M_PI;
			else if(met_phi<-M_PI) met_phi+=2*M_PI;
			
			Bool_t found = false;
			Int_t index = 0;
			for(vector<Double_t>::iterator it=openingAngle.begin(); it!=openingAngle.end(); it++){
				if(2*halfAng<*it && !found){//use this or previous instance of *it
					found = true;
					if(abs(*it-2*halfAng)<abs(*(it-1)-2*halfAng) || index==0){//use index
						openingAngleCounter[index]++; //counts how many events have this opening angle
						if(abs(met_phi)<=abs(halfAng)) fractionContained[index]++; //counts how many events havethis opening angle AND met_phi contained
					}else{//use (index-1)
						openingAngleCounter[index-1]++;
						if(abs(met_phi)<=abs(halfAng)) fractionContained[index-1]++;
					}
				}
				index++;
			}

			Double_t phi_rel = met_phi*M_PI/(2*halfAng);
			//+-1
			if(t_phi>l_phi){
				histograms["missingEt"]->Fill(phi_rel,eventWeight);
			}else{
				histograms["missingEt"]->Fill(-1*phi_rel,eventWeight);
			}

			invM2 = invM1/sqrt(x1*x2);
			//if(getOpeningAngle(t_phi,l_phi)<0.5/*2&&getOpeningAngle(t_phi,l_phi)>0.5*/) histograms["invMass"]->Fill(invM2);
			histograms["invMass"]->Fill(invM2,eventWeight);
			if(phi_rel>-7*M_PI/10&&phi_rel<7*M_PI/10){
				if(getOpeningAngle(t_phi,l_phi)<1.5) histograms["invMassCutMissingEt1.5"]->Fill(invM2,eventWeight);
				if(getOpeningAngle(t_phi,l_phi)<1) histograms["invMassCutMissingEt1"]->Fill(invM2,eventWeight);
				if(getOpeningAngle(t_phi,l_phi)<0.5) histograms["invMassCutMissingEt0.5"]->Fill(invM2,eventWeight);
			}
			
			//Sem1 final cuts;
			//if(invM3<80 && 2*halfAng<=2.5 && 2*halfAng>=0.5 && phi_rel<=3*M_PI/5 && phi_rel>=-7*M_PI/10){
			//if((MC)&&invM1<100&&invM1>80&&invM4<120&&invM4>40){
			//	if(sumw!=0) Efficiency+=(eventWeight/lumFactor)/sumw;
			//	}
			//}
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

	Int_t index = 0;
	for(vector<Double_t>::iterator it=fractionContained.begin(); it!=fractionContained.end(); it++){
		index++;
		histograms["etContainedFrac"]->SetBinContent(index,*it/openingAngleCounter[index-1]*100);
	}
			
	TDir->cd();
	gDirectory->cd("etContainedFrac");
	histograms["etContainedFrac"]->SetTitle(";#Delta / rad;\% events with #phi_{missing} within #Delta");
	histograms["etContainedFrac"]->Write((products+"_etContainedFrac_"+shortFileName).c_str(),TObject::kWriteDelete);
	
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
	
	v.Write("efficiency");
	output.Close(); //Close the output file

} 



// so we don't need to keep typing it in the terminal
Int_t main(){
	mini a;
	a.Run();
	plotter();
	
	return 0;
}
