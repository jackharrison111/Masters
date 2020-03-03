#include <iostream>
#include <math.h>
#include <vector>

// DiTau includes
#include "DiTauAlg.h"
#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingETContainer.h"

// core EDM includes
#include "AthContainers/AuxElement.h"
#include "AthContainers/DataVector.h"
#include "PATInterfaces/CorrectionCode.h"
#include "xAODEventInfo/EventInfo.h"

//#include "EventLoop/Worker.h"  //TODO::FIX SKIPPING EVENTS

int no_1lep1tau_events{0};

DiTauAlg::DiTauAlg( const std::string& name, ISvcLocator* pSvcLocator ) : AthAnalysisAlgorithm( name, pSvcLocator ){

  //declareProperty( "Property", m_nProperty = 0, "My Example Integer Property" ); //example property declaration

}


DiTauAlg::~DiTauAlg() {}


double DiTauAlg::APPLY(asg::AnaToolHandle<MissingMassTool>m_mmt, const xAOD::EventInfo* ei, const xAOD::IParticle* x, const xAOD::IParticle* y, const xAOD::MissingET* met, double num){	

    double mass = -1;
    CP::CorrectionCode c = m_mmt->apply(*ei, x, y, met, num);
    if((c != CP::CorrectionCode::Ok)||(m_mmt->GetFittedMass(MMCFitMethod::MAXW)==0)){
      fail++;        
    }else{
      pass++;
      mass = m_mmt->GetFittedMass(MMCFitMethod::MAXW);
    }
    return mass;
}


bool DiTauAlg::GetCandidates(const int no_el, const int no_mu, const int no_tau){
  
  //ELECTRONS
  const xAOD::ElectronContainer *ec = 0;
  CHECK(evtStore()->retrieve(ec, "Electrons"));
  for(auto it = ec->begin(); it != ec->end(); it++){
    const xAOD::Electron *e = *it;
    if(e->pt()/1000 >= 25 && abs(e->eta()) <= 2.5){
      Electrons.push_back(e);
    }
  }
  
  //MUONS
  const xAOD::MuonContainer *mc = 0;
  CHECK(evtStore()->retrieve(mc, "Muons"));
  for(auto it = mc->begin(); it != mc->end(); it++){
    const xAOD::Muon *mu = *it;
    if(mu->pt()/1000 >= 25 && abs(mu->eta()) <= 2.5){
      Muons.push_back(mu);
    }
  }
  
  //TAUS
  const xAOD::TauJetContainer *tjc = 0;
  CHECK(evtStore()->retrieve(tjc, "TauJets"));
  for(auto it = tjc->begin(); it != tjc->end(); it++){
    const xAOD::TauJet *tj = *it;
    if(tj->pt()/1000 >= 20 && abs(tj->eta()) <= 2.5){
      // https://arxiv.org/pdf/1607.05979.pdf p7 :
      if(abs(tj->eta()) > 1.37 && abs(tj->eta()) < 1.52){
        TauJets.push_back(tj);
      }
    }
  }

  if(no_el == (int)Electrons.size() && no_mu == (int)Muons.size() && no_tau == (int)TauJets.size()){
    return true;
  } else {
    // incase (1,0,1) doesn't pass but (0,1,1) will :
    Electrons.clear(); Muons.clear(); TauJets.clear();
    return false;
  }
}


StatusCode DiTauAlg::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");
  
  m_myHist = new TH1D("invMass","Invariant Mass Distribution",160,0,160);
  collinear_Hist = new TH1D("col_invMass","Invariant Mass Distribution",160,0,160);


  //m_my2DHist = new TH2D("invMassvsRMS","Invariant Mass against RMS/m.p.v",160,0,160,160,0,1);
  m_myHist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  m_myHist->SetStats(0);
  //m_my2DHist->SetTitle(";M_{#tau#tau} [GeV]; RMS/m.p.v");
  //m_my2DHist->SetStats(0);

  CHECK( histSvc()->regHist("/MYSTREAM/invMass", m_myHist) ); //registers histogram to output stream
  //CHECK( histSvc()->regHist("/MYSTREAM/2DHist", m_my2DHist) ); //registers histogram to output stream
  CHECK( histSvc()->regHist("/MYSTREAM/col_Hist", collinear_Hist) );


  //INITIALISE THE MISSING MASS TOOL
  m_mmt.setTypeAndName("MissingMassTool/MissingMassTool");
  CHECK(m_mmt.initialize());
  CHECK(m_mmt->setProperty("UseTauProbability", 1));
  CHECK(m_mmt->setProperty("CalibSet", "2016MC15C"));
  CHECK(m_mmt->setProperty("NiterFit2", 30));
  CHECK(m_mmt->setProperty("NiterFit3", 10));
  CHECK(m_mmt->setProperty("NsigmaMET", 4));
  CHECK(m_mmt->setProperty("alg_version", 3));
  //CHECK(m_mmt->setProperty("UseMETDphiLL", 1)); only for leplep
  CHECK(m_mmt->setProperty("UseEfficiencyRecovery", 1));

   orFlags.boostedLeptons = true;
   orFlags.doElectrons = true;
   orFlags.doMuons = true;
   orFlags.doJets = true;
   orFlags.doTaus = true;
   orFlags.doPhotons = true;//false;

   CHECK( ORUtils::recommendedTools(orFlags, toolBox) );
   CHECK( toolBox.initialize() );

  pass = 0;
  fail = 0;
  maxw_m = 0;

  return StatusCode::SUCCESS;
}


StatusCode DiTauAlg::execute() {  
  ATH_MSG_DEBUG ("Executing " << name() << "...");
  setFilterPassed(false); //optional: start with algorithm not passed


  //EVENT INFO:
  const xAOD::EventInfo* ei = 0;
  CHECK( evtStore()->retrieve( ei , "EventInfo" ) );


  //ELECTRONS:
  /*const xAOD::ElectronContainer *ec = 0;
  CHECK(evtStore()->retrieve(ec, "Electrons"));
  std::vector<const xAOD::Electron*> candidate_els; 
  int no_el = 0;
  for(xAOD::ElectronContainer::const_iterator it=ec->begin(); it!=ec->end(); it++){
    const xAOD::Electron *e = *it;
    if(e->pt()/1000 >= 25 && abs(e->eta()) <= 2.5){ 
      no_el++;
      candidate_els.push_back(*it);
    }
  }
//  if(no_el > 1) wk()->skipEvent();
  
  //MUONS:
  const xAOD::MuonContainer *mc = 0;
  CHECK(evtStore()->retrieve(mc, "Muons"));
  int no_mu = 0; 
  std::vector<const xAOD::Muon*> candidate_mus; 
  for(xAOD::MuonContainer::const_iterator it=mc->begin(); it!=mc->end(); it++){
    const xAOD::Muon *m = *it;
    if(m->pt()/1000 >= 25 && abs(m->eta()) <= 2.5){ 
      no_mu++;
      candidate_mus.push_back(*it);	
    }
  }
//  if(no_mu > 1) wk()->skipEvent();
*/
  //JETS:
  const xAOD::JetContainer *jc = 0;
  double no_25Jets = 0;
  CHECK(evtStore()->retrieve(jc, "AntiKt4LCTopoJets"));
  for(xAOD::JetContainer::const_iterator it=jc->begin(); it!=jc->end(); it++){
    const xAOD::Jet *j = *it;
    if((j->pt()>25e3)&&( abs(j->eta())<2.5)){
      no_25Jets++;
      }
    }
  

  //TAU JETS:

  //double tauJetTotal = 0;
  //double tauJetPass = 0;
/*
  const xAOD::TauJetContainer *tjc = 0;
  CHECK(evtStore()->retrieve(tjc, "TauJets"));
  std::vector<const xAOD::TauJet*> candidate_tjs;
  for(xAOD::TauJetContainer::const_iterator it=tjc->begin(); it!=tjc->end(); it++){
    const xAOD::TauJet *tj = *it;
    if((tj->pt()>20e3)&&( abs(tj->eta())<=2.5)){
      if((abs(tj->eta())>1.37)&&(abs(tj->eta())<1.52)){}	//cut introduced due to transition region - found in https://arxiv.org/pdf/1607.05979.pdf p7
      else{
        candidate_tjs.push_back(*it);
      }
    }
  }
  */
  //MISSING ENERGY:
  const xAOD::MissingETContainer *metc = 0;
  CHECK(evtStore()->retrieve(metc, "MET_Calo"));
  //Get the last one
  //std::cout << metc->size() << " = size of metc" << std::endl;
  const xAOD::MissingET *met1 = 0;
  met1 = metc->at(metc->size() - 1);
  //met1 = metc->at(7);

  /*
  //Iterate over all (Don't think needed)
  std::vector<xAOD::MissingET> met_vector;
  double total_met = 0;
  double total_sumet = 0;
  int number = 1;
  for(xAOD::MissingETContainer::const_iterator it=metc->begin(); it!=metc->end(); it++){
    met1 = *it;
    met_vector.push_back(*met1);
    std::cout<< met1->met() << " - met," << met1->sumet() << " - sumet" << std::endl;
    if(number <= 7){
      total_met += met1->met();
      total_sumet += met1->sumet();
    }
    number++;
  }
  std::cout << "Total met (first 7) = " << total_met << ", total sumet (first 7) = " << total_sumet << std::endl << std::endl;
  */


  /*
  if(no_el == 2){
    if(candidate_els[0]->charge() == -candidate_els[1]->charge()){
      maxw_m = APPLY(m_mmt, ei, candidate_els[0], candidate_els[1], met1, no_25Jets);
      m_myHist->Fill(maxw_m);
    }   
  }else if(no_mu == 2){
    if(candidate_mus[0]->charge() == -candidate_mus[1]->charge()){
      maxw_m = APPLY(m_mmt, ei, candidate_mus[0], candidate_mus[1], met1, no_25Jets);
      m_myHist->Fill(maxw_m);   
    }
  }
  else if((no_mu == 1)&&(no_el == 1)){
    if(candidate_els[0]->charge() == -candidate_mus[0]->charge()){
      maxw_m = APPLY(m_mmt, ei, candidate_els[0], candidate_mus[0], met1, no_25Jets);
      m_myHist->Fill(maxw_m);   
    }
  }else{
    //fail++;
    //wk()->skipEvent();
  }
  
  
  if(met1->met() > 20e3){
  if(candidate_tjs.size() == 1){
    if((no_el == 1)&&(no_mu == 0)){
      if(candidate_els[0]->charge() == -candidate_tjs[0]->charge()){
        //maxw_m = APPLY(m_mmt, ei, candidate_tjs[0], candidate_els[0], met1, no_25Jets);
        //std::cout << m_mmt->get()->GetFitSignificance(1) << " - signif. " << std::endl;
        //double rms2Mpv = m_mmt->get()->GetRms2Mpv();
        //m_myHist->Fill(maxw_m);   
      }
    }else if((no_el == 0)&&(no_mu == 1)){
      if(candidate_mus[0]->charge() == -candidate_tjs[0]->charge()){
        //maxw_m = APPLY(m_mmt, ei, candidate_tjs[0], candidate_mus[0], met1, no_25Jets);
        //std::cout << m_mmt->get()->GetFitSignificance(1) << " - signif. " << std::endl;
        //m_myHist->Fill(maxw_m);   
      }
    }
  }
  }
  //if(maxw_m != 0) std::cout << maxw_m << " = mass " << std::endl;
  */



  double lep_pt, lep_phi, lep_eta, tau_pt, tau_phi, tau_eta, nu_T_lep, nu_T_had, met_et, met_phi, x1, x2;

  double invM1, invM2;
  if(GetCandidates(1,0,1) || GetCandidates(0,1,1)){
    double total_charge = TauJets[0]->charge();
    if(Electrons.size() == 1){
      total_charge += Electrons[0]->charge();
    } else {
      total_charge += Muons[0]->charge();
    }
    if(total_charge == 0){ //wk->skipEvent();?????
      if(met1->met() > 20e3){
      if(Electrons.size() == 1){
        lep_pt = Electrons[0]->pt();
        lep_phi = Electrons[0]->phi();
        lep_eta = Electrons[0]->eta();
        maxw_m = APPLY(m_mmt, ei, TauJets[0], Electrons[0], met1, no_25Jets);
      }else{
        lep_pt = Muons[0]->pt();
        lep_phi = Muons[0]->phi();
        lep_eta = Muons[0]->eta();
        maxw_m = APPLY(m_mmt, ei, TauJets[0], Muons[0], met1, no_25Jets);
      }
      tau_pt = TauJets[0]->pt();
      tau_eta = TauJets[0]->eta();
      tau_phi = TauJets[0]->phi();
      met_et = met1->met();
      met_phi = met1->phi();

      nu_T_lep = met_et*(sin(met_phi)-sin(tau_phi))/(sin(lep_phi)-sin(tau_phi));
      nu_T_had = met_et*(sin(met_phi)-sin(lep_phi))/(sin(tau_phi)-sin(lep_phi));

      invM1 = sqrt(2*lep_pt*tau_pt*(cosh(lep_eta-tau_eta)-cos(lep_phi-tau_phi)))/1000;
      x1 = lep_pt/(lep_pt+nu_T_lep);
      x2 = tau_pt/(tau_pt+nu_T_had);
      invM2 = invM1/sqrt(x1*x2);
 
      collinear_Hist->Fill(invM2); 
      m_myHist->Fill(maxw_m);
    }
    }
  else{
    //std::cout << "Skipping event" << std::endl;
    //wk()->SkipEvent();
  }
    // reset vectors after event
    Electrons.clear(); Muons.clear(); TauJets.clear();
  }





  setFilterPassed(true); //if got here, assume that means algorithm passed
  return StatusCode::SUCCESS;
}


StatusCode DiTauAlg::finalize() {
  ATH_MSG_INFO ("Finalizing " << name() << "...");
 
  std::cout << "Passed : " << pass << " , Failed : " << fail << std::endl;
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
