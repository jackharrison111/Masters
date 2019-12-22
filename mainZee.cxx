//TODO: make an if statement to check whether dataSets.json contains shortFileName
#define main_cxx
#include "mainMC.h" //change this for mc or real data
#include "converter.h" //for usage of infofile.py here
#include "plotter.cxx"
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
Bool_t mini::Cut(Int_t e, Int_t mu, Int_t tau){ //electron and muons only so far, tau=0 in mainlep.cxx
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

Double_t mini::GetOpenAngle(Double_t ang1, Double_t ang2){
	Double_t openAngle = abs(ang1-ang2);
	if(openAngle>M_PI){
		openAngle = 2*M_PI-openAngle;//TODO: *-1 ?
	}
	return openAngle;
}

void mini::Run(){

	//gROOT->SetStyle("ATLAS");
	gStyle->SetOptStat(0);

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

	

/*	TFile output(("rootOutput/" + outputName+"output_Zee_14-12.root").c_str(),"RECREATE");
	TDirectory *TDir = output.mkdir("2lep");
	std::map<string,TH1*> histograms;
	histograms["invMassZee"]=new TH1D("invMassZee","Z->ee",160,0,160);
	
	Int_t counter{0};
	clock_t startTime = clock();
	
	string fileName;
	string oldFileName; //to calculate lumFactor only once for each MC file
	string shortFileName;
	string products;
	Double_t lumFactor;
	
	Double_t Efficiency{0};
	Int_t sumwRec{0};
	Double_t sumw;

	Double_t lowerBound{86};
	Double_t upperBound{96};
	Double_t N_sing{0};
	Double_t eventCounts{0};
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
					std::cout<<"ERROR! lum factor is zero"<<std::endl;
				}
				sumw = i.infos[shortFileName]["sumw"];
			}
			
			gDirectory->cd(products.c_str());
			std::map<string,TH1*>::iterator it = histograms.begin();
			while(it!=histograms.end()){
				(it->second)->SetName((products+"_"+it->first+"_"+shortFileName).c_str());
				it++;
			}
		}
		if(sumw==0) continue;
		if(MC){
			eventWeight = mcWeight*scaleFactor_PILEUP*scaleFactor_ELE*scaleFactor_MUON*scaleFactor_PHOTON*scaleFactor_TAU*scaleFactor_BTAG*scaleFactor_LepTRIGGER*scaleFactor_PhotonTRIGGER*scaleFactor_TauTRIGGER*scaleFactor_DiTauTRIGGER*lumFactor;
		}
		////2 ELECTRON EVENTS////
		Double_t invM;
		if(Cut(2,0,0)||Cut(0,2,0)){
			//if(MC) Efficiency += eventWeight/sumw;
			invM = sqrt(2*(*lep_pt)[0]*(*lep_pt)[1]*(cosh((*lep_eta)[0]-(*lep_eta)[1])-cos((*lep_phi)[0]-(*lep_phi)[1])))/1000;
			if(Cut(2,0,0)){
				eventCounts++;
				
				histograms["invMassZee"]->Fill(invM,eventWeight);
				if(invM>=81&&invM<=101){
					//std::cout << eventWeight << " , " << sumw << "  :  " << eventWeight/sumw << " , " << Efficiency << std::endl;
					//if(MC) Efficiency += (eventWeight/lumFactor)/sumw;
					//else Efficiency++;

				}
			}
		}
		/////////////////////////
		
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
				//(it->second)->Reset();
				it++;
			}
		}

	}
	//Print the time taken to run the loop (relies on startTime at beginning of loop)
	clock_t endTime = clock();
	std::cout<<"Run time: "<<(endTime-startTime)/CLOCKS_PER_SEC<<" s"<<std::endl<<std::endl;
	std::cout<<"\n\nPERCENTAGE OF EVENTS ADDED TO HIST: "<<100*eventCounts/n<<"\n\n\n"<<std::endl;

	TF1 *fitt = new TF1("fitt",BackFit,110,160,2);
	fitt->SetParameters(1,1);
	histograms["invMassZee"]->SetDirectory(0);
	histograms["invMassZee"]->SetTitle(";M_{inv}/GeV;counts/1GeV");
	histograms["invMassZee"]->Fit("fitt","+R");
	Double_t m = fitt->GetParameter(0);
	Double_t cons = fitt->GetParameter(1);
	std::cout<<"m="<<m<<", c="<<cons<<std::endl;

	Double_t x[160], y[160];
	for(Int_t i{0}; i<160; i++){
		x[i]=1*i;
		y[i]=m*x[i]+cons;
	}

	Double_t I = 0;
	Double_t B = 0;
	Double_t lowerRange = 91-8;
	Double_t upperRange = 91+8;
	for(Int_t i{0}; i<160; i++){
		if(i>=lowerRange&&i<=upperRange){
			I+=histograms["invMassZee"]->GetBinContent(i);
			B+=y[i];
		}
	}
	Double_t N = I-B;













	for (Long64_t i=0; i<n; i++){
		Long64_t ientry = LoadTree(i);
		if(ientry < 0) break;
		nb = fChain->GetEntry(i);   nbytes += nb;
		
		fileName = (chain->GetFile())->GetName();
		
		Double_t eventWeight = 1;
		if(fileName!=oldFileName){ //dont want to calculate lumFactor repeatedly, only once per file/per event type
			std::cout<<(chain->GetFile())->GetSize()/1e6<<" MB : File "<<fileCounter<<" out of "<<((chain->GetListOfFiles())->GetLast()+1)<<", "<<fileName<<std::endl;
			fileCounter++;
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
					std::cout<<"ERROR! lum factor is zero"<<std::endl;
				}
				sumw = i.infos[shortFileName]["sumw"];
			}
		}
		if(sumw==0) continue;
		if(MC){
			eventWeight = mcWeight*scaleFactor_PILEUP*scaleFactor_ELE*scaleFactor_MUON*scaleFactor_PHOTON*scaleFactor_TAU*scaleFactor_BTAG*scaleFactor_LepTRIGGER*scaleFactor_PhotonTRIGGER*scaleFactor_TauTRIGGER*scaleFactor_DiTauTRIGGER*lumFactor;
		}
		
		Double_t invM;
		if(Cut(2,0,0)){
			invM = sqrt(2*(*lep_pt)[0]*(*lep_pt)[1]*(cosh((*lep_eta)[0]-(*lep_eta)[1])-cos((*lep_phi)[0]-(*lep_phi)[1])))/1000;
			if(invM>=lowerRange&&invM<=upperRange){
				if(MC) Efficiency += (1-(m*(int)invM+cons)/I)*(eventWeight/lumFactor)/sumw;
				else Efficiency++;
			}
		}
	}
	Double_t sigma = N /(Efficiency*L_int);//fb
	sigma/=1e6;//nb
	std::cout<<"I="<<I<<", B="<<B<<", N="<<N<<", eff="<<Efficiency<<std::endl;
	std::cout<<"sigma="<<sigma<<" nb"<<std::endl;

	if(MC){
		std::cout<<"Efficiency = "<<Efficiency<<std::endl;
	}else{
		std::cout<<"Efficiency = "<<Efficiency/n<<std::endl;
	}

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
	std::cout << "Eventcounts = " <<eventCounts << " , n = " << n << std::endl;
	v.Write("Efficiency");

	output.cd();
	output.Close(); //Close the output file
*/
} 



// so we don't need to keep typing it in the terminal
Int_t mainZee(){
	mini a;
	a.Run();
	//plotterZee();

	return 0;
}
