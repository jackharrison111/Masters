//TODO: make an if statement to check whether dataSets.json contains shortFileName
#define main_cxx
#include "main.h" //change this for mc or real data
#include "converter.h" //for usage of infofile.py here
//#include "plotterlep.cxx"
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
	//std::cout << n << std::endl;
	Long64_t nbytes = 0, nb = 0;
	
	//Save the output file to the correct place based on data type
	string outputName;
	if(MC){
		outputName="mc_";
	}else{
		outputName="re_";
	}

	

	TFile output(("rootOutput/" + outputName+"output_final2lep_22-12.root").c_str(),"RECREATE");
	TDirectory *TDir1 = output.mkdir("1lep1tau");
	TDirectory *TDir2 = output.mkdir("2lep");
	std::map<string,TH1*> histograms;
	histograms["invMassZee"]=new TH1D("invMassZee","Z->ee",160,0,160);
	histograms["invMassZmumu"]=new TH1D("invMassZmumu","Z->#mu#mu",160,0,160);
	histograms["invMass2l"]=new TH1D("invMass2l","Z->ll",160,0,160);
	histograms["invMass2lB"]=new TH1D("invMass2lB","",160,0,160);
	histograms["invMass2lR"]=new TH1D("invMass2lR","",160,0,160);
	histograms["invMass2lG"]=new TH1D("invMass2lG","",160,0,160);
	histograms["invMass4l"]=new TH1D("invMass4l","Z'->llll",300,0,300);
	histograms["invMass2D_EMu"]=new TH2D("invMass2D_EMu","ZZ->ee&&#mu#mu",160,0,160,160,0,160);
	histograms["invMass2D_EE"]=new TH2D("invMass2D_EE","ZZ->ee&&ee",100,0,160,100,0,160);
	histograms["invMass2D_MuMu"]=new TH2D("invMass2D_MuMu","ZZ->#mu#mu&&#mu#mu",100,0,160,100,0,160);
	histograms["opAngDist"] = new TH1D("opAngDist", "Opening angle distribution",100,0,M_PI);

	TH1D *tot = new TH1D("tot","",160,0,160);
	tot->SetTitle(";M_{ll} [GeV];N / [GeV]");

	Int_t counter{0};
	clock_t startTime = clock();
	
	string fileName;
	string oldFileName; //to calculate lumFactor only once for each MC file
	string shortFileName;
	string products;
	Double_t lumFactor;
	
	Int_t sumwRec{0};
	Double_t sumw;

	Double_t lowerBound{86};
	Double_t upperBound{96};
	Double_t N_sing{0};

	string ZZllll = "mc15_13TeV.361603.PwPy8EG_CT10nloME_AZNLOCTEQ6L1_ZZllll_mll4.2lep_raw.root";
	string ZllZll = "mc15_13TeV.363490.Sh_221_NNPDF30NNLO_llll.2lep_raw.root";

	Int_t fileCounter{1};
	Bool_t newFile{true};

	double N=0;
	double Efficiency = 0;

	for (Long64_t i=0; i<n; i++){
		Long64_t ientry = LoadTree(i);
		if(ientry < 0) break;
		nb = fChain->GetEntry(i);   nbytes += nb;
		
		fileName = (chain->GetFile())->GetName();
		
		//Discard the ZZllll file
		if(fileName == ("/data/ATLAS/2lep/MC/"+ZZllll).c_str()) continue;

		//ZllZll filename : "/data/ATLAS/2lep/MC/mc15_13TeV.363490.Sh_221_NNPDF30NNLO_llll.2lep_raw.root"

		Double_t eventWeight = 1;
		if(fileName!=oldFileName){ //dont want to calculate lumFactor repeatedly, only once per file/per event type
			std::cout<<(chain->GetFile())->GetSize()/1e6<<" MB : File "<<fileCounter<<" out of "<<((chain->GetListOfFiles())->GetLast()+1)<<", "<<fileName<<std::endl;
			fileCounter++;
			if(i!=0){
				tot->Add(histograms["invMass2l"]);
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
			if(lumFactor==0) continue; //breaks the current iteration of for loop, continues with next event?
		}


		/////////////////////
		////4 LEPTON EVENTS////
		Double_t invM1, invM2, invM3, invM4;
		Int_t lower = 86;
		Int_t higher = 96;
		//Check for 2e 2mu events
		if(Cut(2,2,0)){
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
			
			histograms["invMass2D_EMu"]->Fill(invM1,invM2);
//		if(abs(zMass-invM1)<abs(zMass-invM2)){
			histograms["invMass2l"]->Fill(invM1,eventWeight);
//		}else{
			histograms["invMass2l"]->Fill(invM2,eventWeight);
//		}

			if((invM1<higher&&invM1>lower)||(invM2<higher&&invM2>lower)){ //hardcoded
				histograms["invMass2lR"]->Fill(invM1);
				histograms["invMass2lR"]->Fill(invM2);
				histograms["invMass2lB"]->Fill(invM1);
				histograms["invMass2lB"]->Fill(invM2);
				histograms["invMass2lG"]->Fill(invM1);
				histograms["invMass2lG"]->Fill(invM2);
				
				histograms["opAngDist"]->Fill(GetOpenAngle((*lep_phi)[0],(*lep_phi)[which]));
				histograms["opAngDist"]->Fill(GetOpenAngle((*lep_phi)[others[0]],(*lep_phi)[others[1]]));
			}
			if((invM1<102&&invM1>80)&&(invM2<102&&invM2>80)){
				Efficiency+=(eventWeight/lumFactor)/sumw;
				if(MC&&sumw!=0) N+=eventWeight;
				else if(!MC) N+=1;
			}

		}else if(Cut(4,0,0)||Cut(0,4,0)){    //include 4 lepton events of all the same type
			pair<Int_t,Int_t> pos, neg; //pai positive leptons and negative leptons
			Bool_t posSet=false;
			Bool_t negSet=false; //check that the first of the pair hsa been assigned
	Int_t fileCounter{1};
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
	
			
			if((invM1<higher&&invM1>lower)||(invM2<higher&&invM2>lower)){ //hardcoded
				histograms["invMass2lR"]->Fill(invM1);
				histograms["invMass2lR"]->Fill(invM2);
			}else if((invM3<higher&&invM3>lower)||(invM4<higher&&invM4>lower)){
				histograms["invMass2lR"]->Fill(invM3);
				histograms["invMass2lR"]->Fill(invM4);
			}

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
			//Track the pairing numbers used for each invMass
			pair<Int_t, Int_t> pair_num1 (pos.first, neg.first);
			pair<Int_t, Int_t> pair_num2 (pos.second, neg.second);
			pair<Int_t, Int_t> pair_num3 (pos.first, neg.second);
			pair<Int_t, Int_t> pair_num4 (pos.second, neg.first);
			//Store the minimum pairings
			pair<Int_t, Int_t> chosen_pair1;
			pair<Int_t, Int_t> chosen_pair2;
			

			for(vector<Double_t>::iterator it = masses.begin(); it != masses.end(); it++){
				Double_t diff = abs(*it - zMass);
				if(diff == deltas[0]){
					if((*it == pair1.first)||(*it == pair1.second)){
						chosen_pair1 = pair_num1;
						chosen_pair2 = pair_num2;
						
					}else{
						chosen_pair1 = pair_num3;
						chosen_pair2 = pair_num4;
						
						//Reuse M1 and M2 for bants
						invM1 = invM3;
						invM2 = invM4;
					}
				}
			}

			histograms["invMass2lB"]->Fill(invM1);
			histograms["invMass2lB"]->Fill(invM2);
//		if(abs(zMass-invM1)<abs(zMass-invM2)){
			histograms["invMass2l"]->Fill(invM1,eventWeight);
//		}else{
			histograms["invMass2l"]->Fill(invM2,eventWeight);
//		}

			if((*lep_type)[0]==11){
				histograms["invMass2D_EE"]->Fill(invM1,invM2);
			}else{
				histograms["invMass2D_MuMu"]->Fill(invM1,invM2);
			}
			
			if((invM1<higher&&invM1>lower)||(invM2<higher&&invM2>lower)){ //hardcoded
				//Add the opening angles:
				histograms["opAngDist"]->Fill(GetOpenAngle((*lep_phi)[chosen_pair1.first],(*lep_phi)[chosen_pair1.second]));
				histograms["opAngDist"]->Fill(GetOpenAngle((*lep_phi)[chosen_pair2.first],(*lep_phi)[chosen_pair2.second]));
			}
			if((invM1<102&&invM1>80)&&(invM2<102&&invM2>80)){
				Efficiency+=(eventWeight/lumFactor)/sumw;
				if(MC&&sumw!=0) N+=eventWeight;
				else if(!MC) N+=1;
			}
				
		  	//else if((invM3<higher&&invM3>lower)||(invM4<higher&&invM4>lower)){ //hardcoded
			//	if(MC&&sumw!=0&&shortFileName==ZllZll){
			//		Efficiency+=eventWeight/sumw;
			//}else if(!MC){
			//		Efficiency++;//=1/n;
			//	}
			//	histograms["invMass2l"]->Fill(invM3,eventWeight);
			//	histograms["invMass2l"]->Fill(invM4,eventWeight);	
			//	if((*lep_type)[0]==11){
			//		histograms["invMass2D_EE"]->Fill(invM3,invM4);
			//	}else{
			//		histograms["invMass2D_MuMu"]->Fill(invM3,invM4);
			//	}
			//}/

		}

		//Finding invariant mass of whole 4 lepton event using Equation [3] in lab book
		if(lep_n==4){
			histograms["invMass4l"]->Fill(sqrt(pow((*lep_pt)[0]*cosh((*lep_eta)[0])+(*lep_pt)[1]*cosh((*lep_eta)[1])+(*lep_pt)[2]*cosh((*lep_eta)[2])+(*lep_pt)[3]*cosh((*lep_eta)[3]),2)-pow((*lep_pt)[0]*cos((*lep_phi)[0])+(*lep_pt)[1]*cos((*lep_phi)[1])+(*lep_pt)[2]*cos((*lep_phi)[2])+(*lep_pt)[3]*cos((*lep_phi)[3]),2)-pow((*lep_pt)[0]*sin((*lep_phi)[0])+(*lep_pt)[1]*sin((*lep_phi)[1])+(*lep_pt)[2]*sin((*lep_phi)[2])+(*lep_pt)[3]*sin((*lep_phi)[3]),2)-pow((*lep_pt)[0]*sinh((*lep_eta)[0])+(*lep_pt)[1]*sinh((*lep_eta)[1])+(*lep_pt)[2]*sinh((*lep_eta)[2])+(*lep_pt)[3]*sinh((*lep_eta)[3]),2))/1000);//,eventWeight);
		}
		
		/////////////////////
		
		//to write the last files histograms (loop ends after last event in last file,
		//but writing normally occurs at start of next loop)
		if(i==(n-1)){
			tot->Add(histograms["invMass2l"]);
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


	Double_t lowerMass=110;//40;
	Double_t higherMass=125;//70;
	TF1 *fit;
	fit = new TF1("fit",BackFit,lowerMass,higherMass,2);//3);
	fit->SetParameters(1,1);//,1);
	tot->SetDirectory(0);
	tot->SetTitle(";M_{ll} [GeV];N / [GeV]");
	tot->Fit("fit","+RN");

	Double_t a = fit->GetParameter(0);
	Double_t err_a = fit->GetParError(0);
	Double_t m = fit->GetParameter(1);
	Double_t err_m = fit->GetParError(1);
	Double_t cons = fit->GetParameter(2);
	Double_t err_cons = fit->GetParError(2);
	std::cout<<"a="<<a<<"+-"<<fit->GetParError(0)<<", m="<<m<<"+-"<<fit->GetParError(1)<<", c="<<cons<<"+-"<<fit->GetParError(2)<<std::endl;
	Double_t x[160], y[160];
	Double_t B = 0;
	Double_t err_B = 0;
	Double_t lowerRange = 91-11;//80
	Double_t upperRange = 91+11;//102
	for(Int_t i{0}; i<160; i++){
		x[i]=i;
		y[i]=a*i+m;//pow(x[i],2)+m*x[i]+cons;
		if(i>=lowerRange&&i<=upperRange){
			B+=y[i];
			err_B+=pow(err_a*i,2)+pow(err_m,2);
		}
	}
	err_B=sqrt(err_B);
	TGraph *g = new TGraph(160,x,y);
	g->SetLineColor(kRed);
	g->SetLineWidth(2);
	tot->Draw("hist");
	g->Draw("same");
	
	//stat
	Efficiency=0.0105559;//from MC
	Double_t err_N=sqrt(N);
	Double_t err_Eff=Efficiency*err_N/N;
	std::cout<<"eff="<<Efficiency<<"+-"<<err_Eff<<std::endl;
	Double_t stat_sigma = sqrt(pow(err_N/N,2)+pow(err_Eff/Efficiency,2));
	
	//sys
	Double_t err_I;
	Double_t I=tot->IntegralAndError(lowerRange+1,upperRange+1,err_I,"");
	Double_t R=(I-B)/I;
	Double_t err_R=R*sqrt(pow(err_B/B,2)+pow(err_I/I,2));
	Double_t sys_sigma = err_R/R;
	
	//lumi
	Double_t err_Lint=1.7/100*L_int;
	Double_t lumi_sigma=err_Lint/L_int; //ie error is 1.7% of luminosity

	N*=R;
	Efficiency*=R;
	Double_t sigma = N /(Efficiency*L_int);//fb
	sigma/=1e3;//pb*

	stat_sigma*=sigma;
	sys_sigma*=sigma;
	lumi_sigma*=sigma;

	std::cout<<"I="<<I<<", B="<<B<<", N="<<N<<", eff="<<Efficiency<<std::endl;
	std::cout<<"sigma="<<sigma<<" += "<<stat_sigma<<" (stat) +- "<<sys_sigma<<" (sys) +- "<<lumi_sigma<<" (lumi) pb"<<std::endl;


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

	output.cd();
	output.Close(); //Close the output file

} 



// so we don't need to keep typing it in the terminal
Int_t mainlep(){
	mini a;
	a.Run();
	//plotter();

	return 0;
}
