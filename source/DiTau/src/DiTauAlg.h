#ifndef DITAU_DITAUALG_H
#define DITAU_DITAUALG_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "AsgTools/AnaToolHandle.h"
#include "DiTauMassTools/MissingMassTool.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTau/TauJetContainer.h"

//Example ROOT Includes
//#include "TTree.h"
//#include "TH1D.h"



class DiTauAlg: public ::AthAnalysisAlgorithm { 
 public: 
  DiTauAlg( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~DiTauAlg(); 

  ///uncomment and implement methods as required

                                        //IS EXECUTED:
  virtual StatusCode  initialize();     //once, before any input is loaded
  virtual StatusCode  beginInputFile(); //start of each input file, only metadata loaded
  //virtual StatusCode  firstExecute();   //once, after first eventdata is loaded (not per file)
  virtual StatusCode  execute();        //per event
  //virtual StatusCode  endInputFile();   //end of each input file
  //virtual StatusCode  metaDataStop();   //when outputMetaStore is populated by MetaDataTools
  virtual StatusCode  finalize();       //once, after all events processed
  double APPLY(asg::AnaToolHandle<MissingMassTool>m_mmt, const xAOD::EventInfo* ei, const xAOD::IParticle* x, const xAOD::IParticle* y, const xAOD::MissingET* met, double num);
  bool GetCandidates(const int no_el, const int no_mu, const int no_tau);


  ///Other useful methods provided by base class are:
  ///evtStore()        : ServiceHandle to main event data storegate
  ///inputMetaStore()  : ServiceHandle to input metadata storegate
  ///outputMetaStore() : ServiceHandle to output metadata storegate
  ///histSvc()         : ServiceHandle to output ROOT service (writing TObjects)
  ///currentFile()     : TFile* to the currently open input file
  ///retrieveMetadata(...): See twiki.cern.ch/twiki/bin/view/AtlasProtected/AthAnalysisBase#ReadingMetaDataInCpp


 private:
   std::vector<const xAOD::Electron*> Electrons;
   std::vector<const xAOD::Muon*> Muons;
   std::vector<const xAOD::TauJet*> TauJets;
   
   TH1D* m_myHist = 0;
   TH2D* m_my2DHist = 0;
   asg::AnaToolHandle<MissingMassTool> m_mmt;
   double pass,fail;
   double maxw_m; //reconstructed MMC mass
}; 

#endif //> !DITAU_DITAUALG_H
