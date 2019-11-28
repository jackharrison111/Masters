//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Sep 26 16:49:37 2019 by ROOT version 6.14/04
// from TTree mini/mini
// found on file: mc15_13TeV.363490.Sh_221_NNPDF30NNLO_llll.1lep1tau_raw.root
//////////////////////////////////////////////////////////

#ifndef main_h
#define main_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <math.h>
#include <TStyle.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class mini {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   TChain         *chain; 
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           runNumber;
   Int_t           eventNumber;
   Int_t           channelNumber;
   Float_t         mcWeight;
   Float_t         scaleFactor_PILEUP;
   Float_t         scaleFactor_ELE;
   Float_t         scaleFactor_MUON;
   Float_t         scaleFactor_PHOTON;
   Float_t         scaleFactor_TAU;
   Float_t         scaleFactor_BTAG;
   Float_t         scaleFactor_LepTRIGGER;
   Float_t         scaleFactor_PhotonTRIGGER;
   Float_t         scaleFactor_TauTRIGGER;
   Float_t         scaleFactor_DiTauTRIGGER;
   Bool_t          trigE;
   Bool_t          trigM;
   Bool_t          trigP;
   Bool_t          trigT;
   Bool_t          trigDT;
   UInt_t          lep_n;
   std::vector<bool>    *lep_truthMatched;
   std::vector<bool>    *lep_trigMatched;
   std::vector<float>   *lep_pt;
   std::vector<float>   *lep_eta;
   std::vector<float>   *lep_phi;
   std::vector<float>   *lep_E;
   std::vector<float>   *lep_z0;
   std::vector<int>     *lep_charge;
   std::vector<unsigned int> *lep_type;
   std::vector<bool>    *lep_isTightID;
   std::vector<float>   *lep_ptcone30;
   std::vector<float>   *lep_etcone20;
   std::vector<float>   *lep_trackd0pvunbiased;
   std::vector<float>   *lep_tracksigd0pvunbiased;
   Float_t         met_et;
   Float_t         met_phi;
   UInt_t          jet_n;
   std::vector<float>   *jet_pt;
   std::vector<float>   *jet_eta;
   std::vector<float>   *jet_phi;
   std::vector<float>   *jet_E;
   std::vector<float>   *jet_jvt;
   std::vector<int>     *jet_trueflav;
   std::vector<bool>    *jet_truthMatched;
   std::vector<float>   *jet_MV2c10;
   UInt_t          photon_n;
   std::vector<bool>    *photon_truthMatched;
   std::vector<bool>    *photon_trigMatched;
   std::vector<float>   *photon_pt;
   std::vector<float>   *photon_eta;
   std::vector<float>   *photon_phi;
   std::vector<float>   *photon_E;
   std::vector<bool>    *photon_isTightID;
   std::vector<float>   *photon_ptcone30;
   std::vector<float>   *photon_etcone20;
   std::vector<int>     *photon_convType;
   UInt_t          fatjet_n;
   std::vector<float>   *fatjet_pt;
   std::vector<float>   *fatjet_eta;
   std::vector<float>   *fatjet_phi;
   std::vector<float>   *fatjet_E;
   std::vector<float>   *fatjet_m;
   std::vector<int>     *fatjet_truthMatched;
   std::vector<float>   *fatjet_D2;
   std::vector<float>   *fatjet_tau32;
   UInt_t          tau_n;
   std::vector<float>   *tau_pt;
   std::vector<float>   *tau_eta;
   std::vector<float>   *tau_phi;
   std::vector<float>   *tau_E;
   std::vector<int>     *tau_charge;
   std::vector<bool>    *tau_isTightID;
   std::vector<bool>    *tau_truthMatched;
   std::vector<bool>    *tau_trigMatched;
   std::vector<int>     *tau_nTracks;
   std::vector<float>   *tau_BDTid;
   Float_t         ditau_m;
   std::vector<float>   *truth_pt;
   std::vector<float>   *truth_eta;
   std::vector<float>   *truth_phi;
   std::vector<float>   *truth_E;
   std::vector<int>     *truth_pdgid;
   std::vector<float>   *lep_pt_syst;
   Float_t         met_et_syst;
   std::vector<float>   *jet_pt_syst;
   std::vector<float>   *photon_pt_syst;
   std::vector<float>   *fatjet_pt_syst;
   std::vector<float>   *tau_pt_syst;

   // List of branches
   TBranch        *b_runNumber;   //!
   TBranch        *b_eventNumber;   //!
   TBranch        *b_channelNumber;   //!
   TBranch        *b_mcWeight;   //!
   TBranch        *b_scaleFactor_PILEUP;   //!
   TBranch        *b_scaleFactor_ELE;   //!
   TBranch        *b_scaleFactor_MUON;   //!
   TBranch        *b_scaleFactor_PHOTON;   //!
   TBranch        *b_scaleFactor_TAU;   //!
   TBranch        *b_scaleFactor_BTAG;   //!
   TBranch        *b_scaleFactor_LepTRIGGER;   //!
   TBranch        *b_scaleFactor_PhotonTRIGGER;   //!
   TBranch        *b_scaleFactor_TauTRIGGER;   //!
   TBranch        *b_scaleFactor_DiTauTRIGGER;   //!
   TBranch        *b_trigE;   //!
   TBranch        *b_trigM;   //!
   TBranch        *b_trigP;   //!
   TBranch        *b_trigT;   //!
   TBranch        *b_trigDT;   //!
   TBranch        *b_lep_n;   //!
   TBranch        *b_lep_truthMatched;   //!
   TBranch        *b_lep_trigMatched;   //!
   TBranch        *b_lep_pt;   //!
   TBranch        *b_lep_eta;   //!
   TBranch        *b_lep_phi;   //!
   TBranch        *b_lep_E;   //!
   TBranch        *b_lep_z0;   //!
   TBranch        *b_lep_charge;   //!
   TBranch        *b_lep_type;   //!
   TBranch        *b_lep_isTightID;   //!
   TBranch        *b_lep_ptcone30;   //!
   TBranch        *b_lep_etcone20;   //!
   TBranch        *b_lep_trackd0pvunbiased;   //!
   TBranch        *b_lep_tracksigd0pvunbiased;   //!
   TBranch        *b_met_et;   //!
   TBranch        *b_met_phi;   //!
   TBranch        *b_jet_n;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_E;   //!
   TBranch        *b_jet_jvt;   //!
   TBranch        *b_jet_trueflav;   //!
   TBranch        *b_jet_truthMatched;   //!
   TBranch        *b_jet_MV2c10;   //!
   TBranch        *b_photon_n;   //!
   TBranch        *b_photon_truthMatched;   //!
   TBranch        *b_photon_trigMatched;   //!
   TBranch        *b_photon_pt;   //!
   TBranch        *b_photon_eta;   //!
   TBranch        *b_photon_phi;   //!
   TBranch        *b_photon_E;   //!
   TBranch        *b_photon_isTightID;   //!
   TBranch        *b_photon_ptcone30;   //!
   TBranch        *b_photon_etcone20;   //!
   TBranch        *b_photon_convType;   //!
   TBranch        *b_fatjet_n;   //!
   TBranch        *b_fatjet_pt;   //!
   TBranch        *b_fatjet_eta;   //!
   TBranch        *b_fatjet_phi;   //!
   TBranch        *b_fatjet_E;   //!
   TBranch        *b_fatjet_m;   //!
   TBranch        *b_fatjet_truthMatched;   //!
   TBranch        *b_fatjet_D2;   //!
   TBranch        *b_fatjet_tau32;   //!
   TBranch        *b_tau_n;   //!
   TBranch        *b_tau_pt;   //!
   TBranch        *b_tau_eta;   //!
   TBranch        *b_tau_phi;   //!
   TBranch        *b_tau_E;   //!
   TBranch        *b_tau_charge;   //!
   TBranch        *b_tau_isTightID;   //!
   TBranch        *b_tau_truthMatched;   //!
   TBranch        *b_tau_trigMatched;   //!
   TBranch        *b_tau_nTracks;   //!
   TBranch        *b_tau_BDTid;   //!
   TBranch        *b_ditau_m;   //!
   TBranch        *b_truth_pt;   //!
   TBranch        *b_truth_eta;   //!
   TBranch        *b_truth_phi;   //!
   TBranch        *b_truth_E;   //!
   TBranch        *b_truth_pdgid;   //!
   TBranch        *b_lep_pt_syst;   //!
   TBranch        *b_met_et_syst;   //!
   TBranch        *b_jet_pt_syst;   //!
   TBranch        *b_photon_pt_syst;   //!
   TBranch        *b_fatjet_pt_syst;   //!
   TBranch        *b_tau_pt_syst;   //!

   mini(TTree *tree=0);
   virtual ~mini();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Run();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   Bool_t           Cut(Int_t e, Int_t mu, Int_t tau);
   Int_t            numberOfType(Int_t type);
   static Double_t  Gaussian(Double_t *x, Double_t *par);
   static Double_t  Lorentz(Double_t *x, Double_t *par);
   static Int_t     order;
   static Double_t  Background(Double_t *x, Double_t *par, Int_t order);
   static Double_t  Fit(Double_t *x, Double_t *par);
   Bool_t           MC{true};
};

#endif

#ifdef main_cxx
mini::mini(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      /*TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename.c_str());
      if (!f || !f->IsOpen()) {
         f = new TFile(("/data/ATLAS/2lep/MC/"+filename).c_str());
      }
      f->GetObject("mini",tree);*/
      chain = new TChain("mini");
      //chain->Add("/data/ATLAS/1fatjet1lep/MC/*");
      //chain->Add("/data/ATLAS/1lep/MC/*");
      //chain->Add("/data/ATLAS/1lep1tau/MC/*");
      //chain->Add("/data/ATLAS/1tau/MC/*");
      //chain->Add("/data/ATLAS/2lep/MC/*");
      //chain->Add("/data/ATLAS/2tau/MC/*");
      //chain->Add("/data/ATLAS/GamGam/MC/*");
      chain->Add("/data/ATLAS/2lep/MC/mc15_13TeV.363490.Sh_221_NNPDF30NNLO_llll.2lep_raw.root");
      //chain->Add("/data/ATLAS/2lep/MC/mc15_13TeV.361106.PwPy8EG_AZNLOCTEQ6L1_Zee.2lep_raw.root");
   }
   //Init(tree);
   Init(chain);
}

mini::~mini()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t mini::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t mini::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void mini::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   lep_truthMatched = 0;
   lep_trigMatched = 0;
   lep_pt = 0;
   lep_eta = 0;
   lep_phi = 0;
   lep_E = 0;
   lep_z0 = 0;
   lep_charge = 0;
   lep_type = 0;
   lep_isTightID = 0;
   lep_ptcone30 = 0;
   lep_etcone20 = 0;
   lep_trackd0pvunbiased = 0;
   lep_tracksigd0pvunbiased = 0;
   jet_pt = 0;
   jet_eta = 0;
   jet_phi = 0;
   jet_E = 0;
   jet_jvt = 0;
   jet_trueflav = 0;
   jet_truthMatched = 0;
   jet_MV2c10 = 0;
   photon_truthMatched = 0;
   photon_trigMatched = 0;
   photon_pt = 0;
   photon_eta = 0;
   photon_phi = 0;
   photon_E = 0;
   photon_isTightID = 0;
   photon_ptcone30 = 0;
   photon_etcone20 = 0;
   photon_convType = 0;
   fatjet_pt = 0;
   fatjet_eta = 0;
   fatjet_phi = 0;
   fatjet_E = 0;
   fatjet_m = 0;
   fatjet_truthMatched = 0;
   fatjet_D2 = 0;
   fatjet_tau32 = 0;
   tau_pt = 0;
   tau_eta = 0;
   tau_phi = 0;
   tau_E = 0;
   tau_charge = 0;
   tau_isTightID = 0;
   tau_truthMatched = 0;
   tau_trigMatched = 0;
   tau_nTracks = 0;
   tau_BDTid = 0;
   truth_pt = 0;
   truth_eta = 0;
   truth_phi = 0;
   truth_E = 0;
   truth_pdgid = 0;
   lep_pt_syst = 0;
   jet_pt_syst = 0;
   photon_pt_syst = 0;
   fatjet_pt_syst = 0;
   tau_pt_syst = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
   fChain->SetBranchAddress("channelNumber", &channelNumber, &b_channelNumber);
   fChain->SetBranchAddress("mcWeight", &mcWeight, &b_mcWeight);
   fChain->SetBranchAddress("scaleFactor_PILEUP", &scaleFactor_PILEUP, &b_scaleFactor_PILEUP);
   fChain->SetBranchAddress("scaleFactor_ELE", &scaleFactor_ELE, &b_scaleFactor_ELE);
   fChain->SetBranchAddress("scaleFactor_MUON", &scaleFactor_MUON, &b_scaleFactor_MUON);
   fChain->SetBranchAddress("scaleFactor_PHOTON", &scaleFactor_PHOTON, &b_scaleFactor_PHOTON);
   fChain->SetBranchAddress("scaleFactor_TAU", &scaleFactor_TAU, &b_scaleFactor_TAU);
   fChain->SetBranchAddress("scaleFactor_BTAG", &scaleFactor_BTAG, &b_scaleFactor_BTAG);
   fChain->SetBranchAddress("scaleFactor_LepTRIGGER", &scaleFactor_LepTRIGGER, &b_scaleFactor_LepTRIGGER);
   fChain->SetBranchAddress("scaleFactor_PhotonTRIGGER", &scaleFactor_PhotonTRIGGER, &b_scaleFactor_PhotonTRIGGER);
   fChain->SetBranchAddress("scaleFactor_TauTRIGGER", &scaleFactor_TauTRIGGER, &b_scaleFactor_TauTRIGGER);
   fChain->SetBranchAddress("scaleFactor_DiTauTRIGGER", &scaleFactor_DiTauTRIGGER, &b_scaleFactor_DiTauTRIGGER);
   fChain->SetBranchAddress("trigE", &trigE, &b_trigE);
   fChain->SetBranchAddress("trigM", &trigM, &b_trigM);
   fChain->SetBranchAddress("trigP", &trigP, &b_trigP);
   fChain->SetBranchAddress("trigT", &trigT, &b_trigT);
   fChain->SetBranchAddress("trigDT", &trigDT, &b_trigDT);
   fChain->SetBranchAddress("lep_n", &lep_n, &b_lep_n);
   fChain->SetBranchAddress("lep_truthMatched", &lep_truthMatched, &b_lep_truthMatched);
   fChain->SetBranchAddress("lep_trigMatched", &lep_trigMatched, &b_lep_trigMatched);
   fChain->SetBranchAddress("lep_pt", &lep_pt, &b_lep_pt);
   fChain->SetBranchAddress("lep_eta", &lep_eta, &b_lep_eta);
   fChain->SetBranchAddress("lep_phi", &lep_phi, &b_lep_phi);
   fChain->SetBranchAddress("lep_E", &lep_E, &b_lep_E);
   fChain->SetBranchAddress("lep_z0", &lep_z0, &b_lep_z0);
   fChain->SetBranchAddress("lep_charge", &lep_charge, &b_lep_charge);
   fChain->SetBranchAddress("lep_type", &lep_type, &b_lep_type);
   fChain->SetBranchAddress("lep_isTightID", &lep_isTightID, &b_lep_isTightID);
   fChain->SetBranchAddress("lep_ptcone30", &lep_ptcone30, &b_lep_ptcone30);
   fChain->SetBranchAddress("lep_etcone20", &lep_etcone20, &b_lep_etcone20);
   fChain->SetBranchAddress("lep_trackd0pvunbiased", &lep_trackd0pvunbiased, &b_lep_trackd0pvunbiased);
   fChain->SetBranchAddress("lep_tracksigd0pvunbiased", &lep_tracksigd0pvunbiased, &b_lep_tracksigd0pvunbiased);
   fChain->SetBranchAddress("met_et", &met_et, &b_met_et);
   fChain->SetBranchAddress("met_phi", &met_phi, &b_met_phi);
   fChain->SetBranchAddress("jet_n", &jet_n, &b_jet_n);
   fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_E", &jet_E, &b_jet_E);
   fChain->SetBranchAddress("jet_jvt", &jet_jvt, &b_jet_jvt);
   fChain->SetBranchAddress("jet_trueflav", &jet_trueflav, &b_jet_trueflav);
   fChain->SetBranchAddress("jet_truthMatched", &jet_truthMatched, &b_jet_truthMatched);
   fChain->SetBranchAddress("jet_MV2c10", &jet_MV2c10, &b_jet_MV2c10);
   fChain->SetBranchAddress("photon_n", &photon_n, &b_photon_n);
   fChain->SetBranchAddress("photon_truthMatched", &photon_truthMatched, &b_photon_truthMatched);
   fChain->SetBranchAddress("photon_trigMatched", &photon_trigMatched, &b_photon_trigMatched);
   fChain->SetBranchAddress("photon_pt", &photon_pt, &b_photon_pt);
   fChain->SetBranchAddress("photon_eta", &photon_eta, &b_photon_eta);
   fChain->SetBranchAddress("photon_phi", &photon_phi, &b_photon_phi);
   fChain->SetBranchAddress("photon_E", &photon_E, &b_photon_E);
   fChain->SetBranchAddress("photon_isTightID", &photon_isTightID, &b_photon_isTightID);
   fChain->SetBranchAddress("photon_ptcone30", &photon_ptcone30, &b_photon_ptcone30);
   fChain->SetBranchAddress("photon_etcone20", &photon_etcone20, &b_photon_etcone20);
   fChain->SetBranchAddress("photon_convType", &photon_convType, &b_photon_convType);
   fChain->SetBranchAddress("fatjet_n", &fatjet_n, &b_fatjet_n);
   fChain->SetBranchAddress("fatjet_pt", &fatjet_pt, &b_fatjet_pt);
   fChain->SetBranchAddress("fatjet_eta", &fatjet_eta, &b_fatjet_eta);
   fChain->SetBranchAddress("fatjet_phi", &fatjet_phi, &b_fatjet_phi);
   fChain->SetBranchAddress("fatjet_E", &fatjet_E, &b_fatjet_E);
   fChain->SetBranchAddress("fatjet_m", &fatjet_m, &b_fatjet_m);
   fChain->SetBranchAddress("fatjet_truthMatched", &fatjet_truthMatched, &b_fatjet_truthMatched);
   fChain->SetBranchAddress("fatjet_D2", &fatjet_D2, &b_fatjet_D2);
   fChain->SetBranchAddress("fatjet_tau32", &fatjet_tau32, &b_fatjet_tau32);
   fChain->SetBranchAddress("tau_n", &tau_n, &b_tau_n);
   fChain->SetBranchAddress("tau_pt", &tau_pt, &b_tau_pt);
   fChain->SetBranchAddress("tau_eta", &tau_eta, &b_tau_eta);
   fChain->SetBranchAddress("tau_phi", &tau_phi, &b_tau_phi);
   fChain->SetBranchAddress("tau_E", &tau_E, &b_tau_E);
   if(fChain->GetBranch("tau_charge")){
   fChain->SetBranchAddress("tau_charge", &tau_charge, &b_tau_charge);
   }
   fChain->SetBranchAddress("tau_isTightID", &tau_isTightID, &b_tau_isTightID);
   fChain->SetBranchAddress("tau_truthMatched", &tau_truthMatched, &b_tau_truthMatched);
   fChain->SetBranchAddress("tau_trigMatched", &tau_trigMatched, &b_tau_trigMatched);
   fChain->SetBranchAddress("tau_nTracks", &tau_nTracks, &b_tau_nTracks);
   fChain->SetBranchAddress("tau_BDTid", &tau_BDTid, &b_tau_BDTid);
   fChain->SetBranchAddress("ditau_m", &ditau_m, &b_ditau_m);
   fChain->SetBranchAddress("truth_pt", &truth_pt, &b_truth_pt);
   fChain->SetBranchAddress("truth_eta", &truth_eta, &b_truth_eta);
   fChain->SetBranchAddress("truth_phi", &truth_phi, &b_truth_phi);
   fChain->SetBranchAddress("truth_E", &truth_E, &b_truth_E);
   fChain->SetBranchAddress("truth_pdgid", &truth_pdgid, &b_truth_pdgid);
   fChain->SetBranchAddress("lep_pt_syst", &lep_pt_syst, &b_lep_pt_syst);
   fChain->SetBranchAddress("met_et_syst", &met_et_syst, &b_met_et_syst);
   fChain->SetBranchAddress("jet_pt_syst", &jet_pt_syst, &b_jet_pt_syst);
   fChain->SetBranchAddress("photon_pt_syst", &photon_pt_syst, &b_photon_pt_syst);
   fChain->SetBranchAddress("fatjet_pt_syst", &fatjet_pt_syst, &b_fatjet_pt_syst);
   fChain->SetBranchAddress("tau_pt_syst", &tau_pt_syst, &b_tau_pt_syst);
   Notify();
}

Bool_t mini::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void mini::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

#endif // #ifdef main_cxx
