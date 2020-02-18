#include <iostream>
#include <math.h>
#include <vector>

// DiTau includes
#include "DiTauAlg.h"

#include "xAODEgamma/ElectronContainer.h"

#include "xAODMuon/MuonContainer.h"

#include "xAODJet/JetContainer.h"

#include "xAODTau/TauJetContainer.h"

#include "xAODMissingET/MissingETContainer.h"

// core EDM includes
#include "AthContainers/AuxElement.h"
#include "AthContainers/DataVector.h"
//#include "PATInterfaces/CorrectionCode.h"
#include "xAODEventInfo/EventInfo.h"




DiTauAlg::DiTauAlg( const std::string& name, ISvcLocator* pSvcLocator ) : AthAnalysisAlgorithm( name, pSvcLocator ){

  //declareProperty( "Property", m_nProperty = 0, "My Example Integer Property" ); //example property declaration

}


DiTauAlg::~DiTauAlg() {}

double DiTauAlg::APPLY(asg::AnaToolHandle<MissingMassTool>m_mmt, const xAOD::EventInfo* ei, const xAOD::IParticle* x, const xAOD::IParticle* y, const xAOD::MissingET* met, double num){	

    double mass = -1;
    CP::CorrectionCode c = m_mmt->apply(*ei, x, y, met, num);
    if (c != CP::CorrectionCode::Ok){
      fail++;        
    }else{
      pass++;
      mass = m_mmt->GetFittedMass(MMCFitMethod::MAXW);
    }
   return mass;
}



StatusCode DiTauAlg::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");
  //
  //This is called once, before the start of the event loop
  //Retrieves of tools you have configured in the joboptions go here
  //

  //HERE IS AN EXAMPLE
  //We will create a histogram and a ttree and register them to the histsvc
  //Remember to configure the histsvc stream in the joboptions
  //
  //m_myHist = new TH1D("myHist","myHist",100,0,100);
  //CHECK( histSvc()->regHist("/MYSTREAM/myHist", m_myHist) ); //registers histogram to output stream
  //m_myTree = new TTree("myTree","myTree");
  //CHECK( histSvc()->regTree("/MYSTREAM/SubDirectory/myTree", m_myTree) ); //registers tree to output stream inside a sub-directory

  //INITIALISE THE MISSING MASS TOOL
  m_mmt.setTypeAndName("MissingMassTool/MissingMassTool");
  CHECK(m_mmt->setProperty("UseTauProbability", 1));
  CHECK(m_mmt.initialize());


  pass = 0;
  fail = 0;
  maxw_m = 0;

  return StatusCode::SUCCESS;
}

StatusCode DiTauAlg::finalize() {
  ATH_MSG_INFO ("Finalizing " << name() << "...");
  //
  //Things that happen once at the end of the event loop go here
  //
  
  std::cout << "Passed : " << pass << " , Failed : " << fail << std::endl;
  return StatusCode::SUCCESS;
}

StatusCode DiTauAlg::execute() {  
  ATH_MSG_DEBUG ("Executing " << name() << "...");
  setFilterPassed(false); //optional: start with algorithm not passed



  //
  //Your main analysis code goes here
  //If you will use this algorithm to perform event skimming, you
  //should ensure the setFilterPassed method is called
  //If never called, the algorithm is assumed to have 'passed' by default
  //

  //CUTS NEEDED:
  //Net charge 0
  //2 electrons with pt > 7GEV and eta < 2.5

  //EVENT INFO:
  const xAOD::EventInfo* ei = 0;
  CHECK( evtStore()->retrieve( ei , "EventInfo" ) );
  //m_myHist->Fill( ei->averageInteractionsPerCrossing() ); //fill mu into histogram


  //ELECTRONS:
  const xAOD::ElectronContainer *ec = 0;
  CHECK(evtStore()->retrieve(ec, "Electrons"));
  std::vector<const xAOD::Electron*> candidate_els; 
  int no_el = 0;
  for(xAOD::ElectronContainer::const_iterator it=ec->begin(); it!=ec->end(); it++){
    const xAOD::Electron *e = *it;
    if(e->pt()/1000 >= 7 && abs(e->eta()) <= 2.5){ 
      no_el++;
      candidate_els.push_back(*it);
    }
  }
  
  //MUONS:
  const xAOD::MuonContainer *mc = 0;
  CHECK(evtStore()->retrieve(mc, "Muons"));
  int no_mu = 0; 
  std::vector<const xAOD::Muon*> candidate_mus; 
  for(xAOD::MuonContainer::const_iterator it=mc->begin(); it!=mc->end(); it++){
    const xAOD::Muon *m = *it;
    if(m->pt()/1000 >= 7 && abs(m->eta()) <= 2.5){ 
      no_mu++;
      candidate_mus.push_back(*it);	
    }
  }


  //JETS:
  std::vector<TLorentzVector> JetsVector;
  const xAOD::JetContainer *jc = 0;
  double no_25Jets = 0;
  CHECK(evtStore()->retrieve(jc, "AntiKt4LCTopoJets"));
  for(xAOD::JetContainer::const_iterator it=jc->begin(); it!=jc->end(); it++){
    const xAOD::Jet *j = *it;
    if(j->pt()>25e3){
      no_25Jets++;
      if( abs(j->eta())<4.5){
      TLorentzVector *temp = new TLorentzVector;
      double theta = 2*atan(exp(-j->eta()));
      temp->SetPxPyPzE(j->pt()*cos(j->phi()), j->pt()*sin(j->phi()), j->pt()/tan(theta), j->e());
      JetsVector.push_back(*temp);
      delete temp;
      }
    }
  }


  //TAU JETS:
  /*const xAOD::TauJetContainer *tjc = 0;
  CHECK(evtStore()->retrieve(tjc, "TauJets"));
  for(xAOD::TauJetContainer::const_iterator it=tjc->begin(); it!=tjc->end(); it++){
    const xAOD::TauJet *tj = *it;
    ATH_MSG_INFO("tau jet pt="<<tj->pt());
  }*/


  //MISSING ENERGY:
  const xAOD::MissingETContainer *metc = 0;
  CHECK(evtStore()->retrieve(metc, "MET_Calo"));
  const xAOD::MissingET *met1 = 0;
  for(xAOD::MissingETContainer::const_iterator it=metc->begin(); it!=metc->end(); it++){
    met1 = *it;
  }

  //Need to set calibrations: MMC.SetCalibrationSet(MMCCalibrationSet::MMC2015);
 
 
  if(no_el == 2){
    maxw_m = APPLY(m_mmt, ei, candidate_els[0], candidate_els[1], met1, no_25Jets);
  }else if(no_mu == 2){
    maxw_m = APPLY(m_mmt, ei, candidate_mus[0], candidate_mus[1], met1, no_25Jets);
  }
  else if((no_mu == 1)&&(no_el == 1)){
    maxw_m = APPLY(m_mmt, ei, candidate_els[0], candidate_mus[0], met1, no_25Jets);
  }else{
    //fail++;
  }

  setFilterPassed(true); //if got here, assume that means algorithm passed
  return StatusCode::SUCCESS;
}

StatusCode DiTauAlg::beginInputFile() { 
  //
  //This method is called at the start of each input file, even if
  //the input file contains no events. Accumulate metadata information here
  //

  //example of retrieval of CutBookkeepers: (remember you will need to include the necessary header files and use statements in requirements file)
  // const xAOD::CutBookkeeperContainer* bks = 0;
  // CHECK( inputMetaStore()->retrieve(bks, "CutBookkeepers") );

  //example of IOVMetaData retrieval (see https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/AthAnalysisBase#How_to_access_file_metadata_in_C)
  //float beamEnergy(0); CHECK( retrieveMetadata("/TagInfo","beam_energy",beamEnergy) );
  //std::vector<float> bunchPattern; CHECK( retrieveMetadata("/Digitiation/Parameters","BeamIntensityPattern",bunchPattern) );



  return StatusCode::SUCCESS;
}


