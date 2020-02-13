#include <iostream>
#include <math.h>

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

#include "xAODEventInfo/EventInfo.h"




DiTauAlg::DiTauAlg( const std::string& name, ISvcLocator* pSvcLocator ) : AthAnalysisAlgorithm( name, pSvcLocator ){

  //declareProperty( "Property", m_nProperty = 0, "My Example Integer Property" ); //example property declaration

}


DiTauAlg::~DiTauAlg() {}


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

  m_mmt.setTypeAndName("MissingMassTool/MissingMassTool");
  CHECK(m_mmt.initialize());

  no_events = 0;
  no_events_more_than_2_electrons = 0;

  return StatusCode::SUCCESS;
}

StatusCode DiTauAlg::finalize() {
  ATH_MSG_INFO ("Finalizing " << name() << "...");
  //
  //Things that happen once at the end of the event loop go here
  //
  
  // PERSONAL OUTPUTS
  std::cout << "\n\n\nPERSONAL OUTPUTS:\n";
  std::cout << "no. events with more than 2 electrons = " << no_events_more_than_2_electrons << " out of a total of " << no_events << std::endl;
  std::cout << "\n\n\n";

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


  //HERE IS AN EXAMPLE
  //const xAOD::EventInfo* ei = 0;
  //CHECK( evtStore()->retrieve( ei , "EventInfo" ) );
  //ATH_MSG_INFO("eventNumber=" << ei->eventNumber() );
  //m_myHist->Fill( ei->averageInteractionsPerCrossing() ); //fill mu into histogram

  no_events++;

  const xAOD::ElectronContainer *ec = 0;
  CHECK(evtStore()->retrieve(ec, "Electrons"));
  int no_el = 0;
  for(xAOD::ElectronContainer::const_iterator it=ec->begin(); it!=ec->end(); it++){
    const xAOD::Electron *e = *it;
    if(e->pt()/1000 >= 7 && abs(e->eta()) <= 2.5) no_el++;
  }
  if(no_el>2) no_events_more_than_2_electrons++;
  //ATH_MSG_INFO("no. of electrons in this event with pt>7GeV, eta<2.5 = " << no_el);


  const xAOD::MuonContainer *mc = 0;
  CHECK(evtStore()->retrieve(mc, "Muons"));
  for(xAOD::MuonContainer::const_iterator it=mc->begin(); it!=mc->end(); it++){
    //const xAOD::Muon *m = *it;
    //ATH_MSG_INFO("muon invariant mass="<<m->m());
  }


  std::vector<TLorentzVector> JetsVector;
  const xAOD::JetContainer *jc = 0;
  CHECK(evtStore()->retrieve(jc, "AntiKt4LCTopoJets"));
  for(xAOD::JetContainer::const_iterator it=jc->begin(); it!=jc->end(); it++){
    const xAOD::Jet *j = *it;
    //ATH_MSG_INFO("jet pt="<<j->pt());
    if(j->pt()>25e3 && abs(j->eta())<4.5){
      TLorentzVector *temp = new TLorentzVector;
      double theta = 2*atan(exp(-j->eta()));
      temp->SetPxPyPzE(j->pt()*cos(j->phi()), j->pt()*sin(j->phi()), j->pt()/tan(theta), j->e());
      JetsVector.push_back(*temp);
      delete temp;
    }
  }


  /*const xAOD::TauJetContainer *tjc = 0;
  CHECK(evtStore()->retrieve(tjc, "TauJets"));
  for(xAOD::TauJetContainer::const_iterator it=tjc->begin(); it!=tjc->end(); it++){
    const xAOD::TauJet *tj = *it;
    ATH_MSG_INFO("tau jet pt="<<tj->pt());
  }*/


  TVector2 *METVector = new TVector2;
  const xAOD::MissingETContainer *metc = 0;
  CHECK(evtStore()->retrieve(metc, "MET_Calo"));
  for(xAOD::MissingETContainer::const_iterator it=metc->begin(); it!=metc->end(); it++){
    const xAOD::MissingET *met = *it;
    METVector->Set(met->mpx(),met->mpy());
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


