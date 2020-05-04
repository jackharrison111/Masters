#ifndef DITAU_DITAUALG_H
#define DITAU_DITAUALG_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "AsgTools/AnaToolHandle.h"
#include "DiTauMassTools/MissingMassTool.h"
#include "AssociationUtils/OverlapRemovalInit.h"
#include "AssociationUtils/IOverlapTool.h"
#include "AssociationUtils/IOverlapRemovalTool.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "JetCalibTools/JetCalibrationTool.h"
#include "TauAnalysisTools/TauSelectionTool.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODMissingET/MissingETAuxContainer.h"
#include "METUtilities/METMaker.h"
#include "ReweightUtils/McEventWeight.h"
#include "SUSYTools/ISUSYObjDef_xAODTool.h"
#include "TauAnalysisTools/ITauTruthMatchingTool.h"
#include "TauAnalysisTools/TauTruthMatchingTool.h"

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
  double GetOpenAngle(double ang1, double ang2);
  void CLEAR();
  xAOD::MissingET& InsertMETTerm(xAOD::MissingETContainer* metCont, std::string name, MissingETBase::Types::bitmask_t source);
  double no_25Jets;
  const xAOD::MissingET *finalMET;

  ///Other useful methods provided by base class are:
  ///evtStore()        : ServiceHandle to main event data storegate
  ///inputMetaStore()  : ServiceHandle to input metadata storegate
  ///outputMetaStore() : ServiceHandle to output metadata storegate
  ///histSvc()         : ServiceHandle to output ROOT service (writing TObjects)
  ///currentFile()     : TFile* to the currently open input file
  ///retrieveMetadata(...): See twiki.cern.ch/twiki/bin/view/AtlasProtected/AthAnalysisBase#ReadingMetaDataInCpp


 private:
   TH1D* vis_hist;
   TH1D* leplep_hist;
   TH1D* phi_rel_hist;

   TH1D* col; // met7
   TH1D* col_our; // our rebuild of met
   TH1D* col_susy; // susy_tool->GetMET
   
   TH1D* mmc;
   TH1D* mmc_our;
   TH1D* mmc_susy;

   TH2D* mmc_leps_2D;
   TH2D* mmc_leps_2D_our;
   TH2D* mmc_leps_2D_susy;

   TH1D* met_hist;
   TH1D* met_hist_our;
   TH1D* met_hist_susy;

   TH1D* met_hist_4l;
   TH1D* met_hist_4l_our;
   TH1D* met_hist_4l_susy;

   TH1D* _3po4; //3*pi/4
   TH1D* po2;
   TH1D* po4;
   TH1D* po8;
   TH1D* po16;
   TH1D* po32;

   TH1D* _3po4_our;
   TH1D* po2_our;
   TH1D* po4_our;
   TH1D* po8_our;
   TH1D* po16_our;
   TH1D* po32_our;

   TH1D* _3po4_susy;
   TH1D* po2_susy;
   TH1D* po4_susy;
   TH1D* po8_susy;
   TH1D* po16_susy;
   TH1D* po32_susy;

   std::vector<const xAOD::Electron*> Electrons;
   std::vector<const xAOD::Muon*> Muons;
   std::vector<const xAOD::TauJet*> TauJets;

   MissingETBase::UsageHandler::Policy obj_scale;

   clock_t start_time;
   clock_t end_time;

   
   asg::AnaToolHandle<MissingMassTool> m_mmt;
   McEventWeight* ew = new McEventWeight("test");
   asg::AnaToolHandle<TauAnalysisTools::TauSelectionTool> tau_selection_t;
   asg::AnaToolHandle<TauAnalysisTools::TauTruthMatchingTool> tau_truthmatching_t;
   asg::AnaToolHandle<met::METMaker> met_tool;
   asg::AnaToolHandle<ST::ISUSYObjDef_xAODTool> susy_tool;

   double pass,fail;
   bool warning_message;
   double maxw_m; //reconstructed MMC mass
   bool MC;
}; 

#endif //> !DITAU_DITAUALG_H
