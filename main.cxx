#define main_cxx
#include "mainMC.h" //change this for mc or real data
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <math.h>

//1D *uncutInvMass = new TH1D("uncut","Z->ll",500,0,3e5);
TH1D *invMassE = new TH1D("invMassE","Z->ee",500,0,3e5);
TH1D *invMassMu = new TH1D("invMassMu","Z->#mu#mu",500,0,3e5);
TH1D *invMassTot = new TH1D("invMassTot","Z->ee||#mu#mu",500,0,3e5);
TH1D *invMass4l = new TH1D("invMass4l","Z->llll",500,0,3e5);
//TH1D *pt = new TH1D("pt","Z->ll",1e4,0,1e6);
//TH1D *etCone = new TH1D("etCone20","Z->ll",1e4,0,1e5);
//TH1D *ptCone = new TH1D("ptCone30","Z->ll",1e4,0,1e5);
TH2D *invMass2D_EMu = new TH2D("invMass2D_EMu","ZZ->ee&&#mu#mu",100,0,3e5,100,0,3e5);
TH2D *invMass2D_EE = new TH2D("invMass2D_EE","ZZ->ee&&ee",100,0,3e5,100,0,3e5);
TH2D *invMass2D_MuMu = new TH2D("invMass2D_MuMu","ZZ->#mu#mu&&#mu#mu",100,0,3e5,100,0,3e5);


const Double_t pi = M_PI;
//MeV:
Double_t eMass = 0.511;
Double_t pMass = 938.28;
Double_t sqrtS = 13e6;
Double_t c = 299792458;

//function to return number of chosen lepton type in an event
Int_t mini::numberOfType(Int_t type){
	Int_t num=0;
	for(int i=0; i<lep_n; i++){
		if((*lep_type)[i] == type){
			num++;
		}
	}
	return num;
}

//add cut decisions to this function
Bool_t mini::Cut(Int_t e, Int_t mu){
	if(lep_n==e+mu){
		if(numberOfType(11)==e && numberOfType(13)==mu){
			Int_t charge{0};
			for(Int_t i=0; i<e+mu; i++){
				charge+=(*lep_charge)[i];
			}
			if(charge==0){
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

Int_t mini::order = 0; //mmm global??
//combine
Double_t mini::Fit(Double_t *x, Double_t *par){
	return Lorentz(x, par) + Background(x,&par[3],order);
}









void mini::Run(){
	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0;

	Int_t counterboi{0};
	Long64_t percents{1};
	
	clock_t startTime = clock();
	
	for (Long64_t jentry=0; jentry<nentries; jentry++){
		Long64_t ientry = LoadTree(jentry);
		if(ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;


		/*pt->Fill((*lep_pt)[0]);
		pt->Fill((*lep_pt)[1]);
		ptCone->Fill((*lep_ptcone30)[0]);
		etCone->Fill((*lep_etcone20)[0]);
		*/
		

		////4 LEPTON EVENTS////
		Double_t invMsqrtE, invMsqrtMu;
		if(Cut(2,2)){
			counterboi++;
			Int_t j{0};
			Int_t which;
			Int_t others[2];
			for(Int_t i=1; i<4; i++){
				if((*lep_type)[i]==(*lep_type)[0]){
					which = i;
				}else{
					others[j]=i;
					j++;
				}
			}

			if((*lep_type)[0]==11){
				invMsqrtE = sqrt(2*(*lep_pt)[0]*(*lep_pt)[which]*(cosh((*lep_eta)[0]-(*lep_eta)[which])-cos((*lep_phi)[0]-(*lep_phi)[which])));
				invMsqrtMu = sqrt(2*(*lep_pt)[others[0]]*(*lep_pt)[others[1]]*(cosh((*lep_eta)[others[0]]-(*lep_eta)[others[1]])-cos((*lep_phi)[others[0]]-(*lep_phi)[others[1]])));
			}else{
				invMsqrtMu = sqrt(2*(*lep_pt)[0]*(*lep_pt)[which]*(cosh((*lep_eta)[0]-(*lep_eta)[which])-cos((*lep_phi)[0]-(*lep_phi)[which])));
				invMsqrtE = sqrt(2*(*lep_pt)[others[0]]*(*lep_pt)[others[1]]*(cosh((*lep_eta)[others[0]]-(*lep_eta)[others[1]])-cos((*lep_phi)[others[0]]-(*lep_phi)[others[1]])));
			}
			
			invMassE->Fill(invMsqrtE);
			invMassMu->Fill(invMsqrtMu);
			invMass2D_EMu->Fill(invMsqrtE,invMsqrtMu);
		}else if(Cut(4,0)||Cut(0,4)){
			pair <Int_t,Int_t> pair1, pair2;
			Bool_t firstOneSet=false;
			Bool_t set=false;
			for(Int_t i=1; i<4; i++){
				if((*lep_charge)[i]!=(*lep_charge)[0] && set==false){
					pair1.first=0;
					pair1.second=i;
					set=true;
				}else if(firstOneSet==false){
					pair2.first=i;
					firstOneSet=true;
				}else{
					pair2.second=i;
				}
			}
			/*//check that pairs are correct
			std::cout<<"("<<pair1.first<<","<<pair1.second<<") , ("<<pair2.first<<","<<pair2.second<<")"<<std::endl;
			std::cout<<(*lep_charge)[0]<<","<<(*lep_charge)[1]<<","<<(*lep_charge)[2]<<","<<(*lep_charge)[3]<<std::endl;
			*/
			
			invMsqrtE = sqrt(2*(*lep_pt)[pair1.first]*(*lep_pt)[pair1.second]*(cosh((*lep_eta)[pair1.first]-(*lep_eta)[pair1.second])-cos((*lep_phi)[pair1.first]-(*lep_phi)[pair1.second]))); //this is for pair 1...
			invMsqrtMu = sqrt(2*(*lep_pt)[pair2.first]*(*lep_pt)[pair2.second]*(cosh((*lep_eta)[pair2.first]-(*lep_eta)[pair2.second])-cos((*lep_phi)[pair2.first]-(*lep_phi)[pair2.second]))); //..this is for pair 2, just using these variables as theyre already defined, ignore the E/Mu label
			if((*lep_type)[0]==11){
				invMassE->Fill(invMsqrtE);
				invMassE->Fill(invMsqrtMu);
				invMass2D_EE->Fill(invMsqrtE,invMsqrtMu);
			}else{
				invMassMu->Fill(invMsqrtE);
				invMassMu->Fill(invMsqrtMu);
				invMass2D_MuMu->Fill(invMsqrtE,invMsqrtMu);
			}
			
		}

		if(lep_n==4){
			invMass4l->Fill(sqrt(pow((*lep_pt)[0]*cosh((*lep_eta)[0])+(*lep_pt)[1]*cosh((*lep_eta)[1])+(*lep_pt)[2]*cosh((*lep_eta)[2])+(*lep_pt)[3]*cosh((*lep_eta)[3]),2)-pow((*lep_pt)[0]*cos((*lep_phi)[0])+(*lep_pt)[1]*cos((*lep_phi)[1])+(*lep_pt)[2]*cos((*lep_phi)[2])+(*lep_pt)[3]*cos((*lep_phi)[3]),2)-pow((*lep_pt)[0]*sin((*lep_phi)[0])+(*lep_pt)[1]*sin((*lep_phi)[1])+(*lep_pt)[2]*sin((*lep_phi)[2])+(*lep_pt)[3]*sin((*lep_phi)[3]),2)-pow((*lep_pt)[0]*sinh((*lep_eta)[0])+(*lep_pt)[1]*sinh((*lep_eta)[1])+(*lep_pt)[2]*sinh((*lep_eta)[2])+(*lep_pt)[3]*sinh((*lep_eta)[3]),2)));
		}
			

		for(Int_t i=1; i<=invMassE->GetNbinsX(); i++){
			invMassTot->SetBinContent(i,invMassE->GetBinContent(i)+invMassMu->GetBinContent(i));
		}
		/////////////////////
	}

	clock_t endTime = clock();

	std::cout<<"Run time: "<<(endTime-startTime)/CLOCKS_PER_SEC<<" s"<<std::endl<<std::endl;
	

	/*
	TF1 *myGaussian = new TF1("myGaus",Gaussian,70e3,120e3,3);
	myGaussian->SetParNames("#mu","#sigma", "A");
	myGaussian->SetParameters(9e4,3e3,400);
	myGaussian->SetParLimits(0,7e4,1e5);
	myGaussian->SetParLimits(1,0,1e4);
	myGaussian->SetParLimits(2,0,600);
	histogram->Fit("myGaus","R+");
	*/
	
	TF1 *myFit = new TF1("myFit",Fit,75e3,110e3,order+4);
	myFit->SetParNames("#mu","#Gamma","A");
	myFit->SetParameters(9.1e4,3.9e3,4.5e5);
	myFit->SetParLimits(0,7e4,1.1e5);
	myFit->SetParLimits(1,0,1.4e4);
	myFit->SetParLimits(2,0,1e16);
	invMassTot->Fit("myFit","R+");
	
	string outputName;
	if(MC){
		outputName="mc_";
	}else{
		outputName="re_";
	}
	TFile output((outputName+"output.root").c_str(),"RECREATE");
	
	invMassE->SetTitle("Z->ee;M_inv/MeV;counts");
	invMassE->Write();
	invMassMu->SetTitle("Z->#mu#mu;M_inv/MeV;counts");
	invMassMu->Write();
	invMassTot->SetTitle("Z->ee||#mu#mu;M_inv/MeV;counts");
	invMassTot->Write();
	invMass4l->SetTitle("Z->llll;M_inv/MeV;counts");
	invMass4l->Write();
	invMass2D_EMu->SetTitle("ZZ->ee&&#mu#mu;M_inv_ee/MeV;M_inv_#mu#mu/MeV");
	invMass2D_EMu->Write();
	invMass2D_EE->SetTitle("ZZ->ee&&ee;M_inv_ee1/MeV;M_inv_ee2/MeV");
	invMass2D_EE->Write();
	invMass2D_MuMu->SetTitle("ZZ->#mu#mu&#mu#mu;M_inv_#mu#mu1/MeV;M_inv_#mu#mu2/MeV");
	invMass2D_MuMu->Write();
	/*uncutInvMass->SetTitle("Uncut Resonance;M_inv/MeV;counts");
	uncutInvMass->Write();
	pt->SetTitle("pt;pt / MeV;counts");
	pt->Write();
	ptCone->SetTitle("ptCone;sum pt;counts");
	ptCone->Write();
	etCone->SetTitle("etCone;sum et;counts");
	etCone->Write();
	*/
	output.Close();
}
