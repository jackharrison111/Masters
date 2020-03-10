#include <iostream>
#include <math.h>
#include <vector>

// DiTau includes
#include "DiTauAlg.h"
#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "METInterface/IMETMaker.h"

// core EDM includes
#include "AthContainers/AuxElement.h"
#include "AthContainers/DataVector.h"
#include "PATInterfaces/CorrectionCode.h"
#include "xAODEventInfo/EventInfo.h"


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

void DiTauAlg::CLEAR(){
  Electrons.clear(); Muons.clear(); TauJets.clear();
}

double DiTauAlg::GetOpenAngle(double ang1, double ang2){
  double openAngle = abs(ang1 - ang2);
  if(openAngle > M_PI){
    openAngle = 2*M_PI - openAngle;
  }
  return openAngle;
}


bool DiTauAlg::GetCandidates(const int no_el, const int no_mu, const int no_tau){
  
  //ELECTRONS
  const xAOD::ElectronContainer *ec = 0;
  CHECK(evtStore()->retrieve(ec, "Electrons"));
  for(auto it = ec->begin(); it != ec->end(); it++){
    const xAOD::Electron *e = *it;
    if(e->pt()/1000 >= 7 && abs(e->eta()) <= 2.5){
      Electrons.push_back(e);
    }
  }
  if((int)Electrons.size() != no_el){CLEAR(); return false;} 
 
  //MUONS
  const xAOD::MuonContainer *mc = 0;
  CHECK(evtStore()->retrieve(mc, "Muons"));
  for(auto it = mc->begin(); it != mc->end(); it++){
    const xAOD::Muon *mu = *it;
    if(mu->pt()/1000 >= 7 && abs(mu->eta()) <= 2.5){
      Muons.push_back(mu);
    }
  }
  if((int)Muons.size() != no_mu){CLEAR(); return false;} 
  
  //TAUS
  const xAOD::TauJetContainer *tjc = 0;
  CHECK(evtStore()->retrieve(tjc, "TauJets"));
  for(auto it = tjc->begin(); it != tjc->end(); it++){
    const xAOD::TauJet *tj = *it;
    if(tau_selection_t->accept(*tj)){	//pt>20 , eta0-1.37 1.52-2.5, EOR, |tauCharge|=1,( https://arxiv.org/pdf/1607.05979.pdf p7 )
      TauJets.push_back(tj);
    }
  }
  if((int)TauJets.size() != no_tau){CLEAR(); return false;} 
  return true;
}


StatusCode DiTauAlg::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");
 
  vis_hist = new TH1D("visMass","Visible Mass Distribution",160,0,160);
  mmc_hist = new TH1D("invMass","Invariant Mass Distribution",160,0,160);
  collinear_hist = new TH1D("met7_invMass","Invariant Mass Distribution",160,0,160);
  phi_rel_hist = new TH1D("phi_rel_hist","Missing Energy Distribution",100,-M_PI,M_PI);


  //m_my2DHist = new TH2D("invMassvsRMS","Invariant Mass against RMS/m.p.v",160,0,160,160,0,1);
  vis_hist->SetTitle("Visible Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
  mmc_hist->SetTitle("Invariant Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
  mmc_hist->SetStats(0);
  //m_my2DHist->SetTitle(";M_{#tau#tau} [GeV]; RMS/m.p.v");
  //m_my2DHist->SetStats(0);

  CHECK( histSvc()->regHist("/MYSTREAM/visMass", vis_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/invMass", mmc_hist) ); //registers histogram to output stream
  //CHECK( histSvc()->regHist("/MYSTREAM/2DHist", m_my2DHist) ); //registers histogram to output stream
  CHECK( histSvc()->regHist("/MYSTREAM/col_Hist", collinear_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/phi_rel_hist", phi_rel_hist) );


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

  const auto TauSelectionToolName = "TauAnalysisTools::TauSelectionTool/TauSelectionTool";
  tau_selection_t.setTypeAndName(TauSelectionToolName);
  CHECK(tau_selection_t.initialize());

   /*orFlags.boostedLeptons = true;
   orFlags.doElectrons = true;
   orFlags.doMuons = true;
   orFlags.doJets = true;
   orFlags.doTaus = true;
   orFlags.doPhotons = true;//false;*/

   //CHECK( ORUtils::recommendedTools(orFlags, toolBox) );
   //CHECK( toolBox.initialize() );
   /*const auto masterToolName = "ORUtils::OverlapRemovalTool/ORTool3";
   masterHandle.setType(masterToolName);
   overlapHandle.setType("");

   const auto overlapToolName = "ORUtils::DeltaROverlapTool/ORTool3.DrORT3";
   const auto key = "EleJetORT";

   overlapHandle.setTypeAndName(overlapToolName);
   CHECK( masterHandle.setProperty(key, overlapHandle) );
   CHECK( masterHandle.initialize() );
   CHECK( masterHandle.get() != nullptr );*/

  pass = 0;
  fail = 0;
  maxw_m = 0;

  return StatusCode::SUCCESS;
}


StatusCode DiTauAlg::execute() {  
  ATH_MSG_DEBUG ("Executing " << name() << "...");
  setFilterPassed(false); //optional: start with algorithm not passed

  /*if(no_1lep1tau_events > 163208){ //163208 is no. events in mc15 Ntuples
    setFilterPassed(true);
    return StatusCode::SUCCESS;
  }*/

  //EVENT INFO:
  const xAOD::EventInfo* ei = 0;
  CHECK( evtStore()->retrieve( ei , "EventInfo" ) );

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
  
  //MISSING ENERGY:
  const xAOD::MissingETContainer *metc = 0;
  CHECK(evtStore()->retrieve(metc, "MET_Calo"));
  //Get the last one
  //std::cout << metc->size() << " = size of metc" << std::endl;
  const xAOD::MissingET *met1 = 0;
  const xAOD::MissingET *met2 = 0;
  met1 = metc->at(7);

  
  const xAOD::ElectronContainer *ec = 0;
  CHECK(evtStore()->retrieve(ec, "Electrons"));
  const xAOD::MuonContainer *mc = 0;
  CHECK(evtStore()->retrieve(mc, "Muons"));
  const xAOD::TauJetContainer *tjc = 0;
  CHECK(evtStore()->retrieve(tjc, "TauJets"));
  if(ec->size()>6){
    //std::cout << "size before = " << ec->size() << std::endl;
    //masterHandle->removeOverlaps(ec, mc, jc);
    //std::cout << "size after = " << ec->size() << std::endl;
  }


  double lep_pt, lep_phi, lep_eta, tau_pt, tau_phi, tau_eta, nu_T_lep, nu_T_had, met_et, met_phi, x1, x2;
  double mass2 = -1;
  
  double invM1, invM2;
  if(GetCandidates(1,0,1) || GetCandidates(0,1,1)){
    no_1lep1tau_events++;
    double total_charge = TauJets[0]->charge();
    if(Electrons.size() == 1){
      total_charge += Electrons[0]->charge();
    }else {
      total_charge += Muons[0]->charge();
    }
    if(total_charge == 0){ 
      if(Electrons.size() == 1){
        if(GetOpenAngle(TauJets[0]->phi(), Electrons[0]->phi()) < 2){
          lep_pt = Electrons[0]->pt();
          lep_phi = Electrons[0]->phi();
          lep_eta = Electrons[0]->eta();
          maxw_m = APPLY(m_mmt, ei, TauJets[0], Electrons[0], met1, no_25Jets);
        }else{
          CLEAR();
          setFilterPassed(true); //if got here, assume that means algorithm passed
          return StatusCode::SUCCESS;
        }
      }else{
        if(GetOpenAngle(TauJets[0]->phi(), Muons[0]->phi()) < 2){
          lep_pt = Muons[0]->pt();
          lep_phi = Muons[0]->phi();
          lep_eta = Muons[0]->eta();
          maxw_m = APPLY(m_mmt, ei, TauJets[0], Muons[0], met1, no_25Jets);
        }else{
          CLEAR();
          setFilterPassed(true); //if got here, assume that means algorithm passed
          return StatusCode::SUCCESS;
        }
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
        
      Double_t halfAng = GetOpenAngle(tau_phi,lep_phi)/2;
      Double_t rotationAngle;
      if(tau_phi<lep_phi){
        rotationAngle = -tau_phi;
      }else{
        rotationAngle = -lep_phi;
      }
      tau_phi += rotationAngle;
      lep_phi += rotationAngle;
      met_phi += rotationAngle;
      if(tau_phi > M_PI) tau_phi -= 2 * M_PI;
      if(lep_phi > M_PI) lep_phi -= 2 * M_PI;
      if(tau_phi < 0 || lep_phi < 0){
        tau_phi += halfAng;
        lep_phi += halfAng;
        met_phi += halfAng;
      }else{
        tau_phi -= halfAng;
	lep_phi -= halfAng;
	met_phi -= halfAng;
      }
      if(met_phi > M_PI) met_phi -= 2 * M_PI;
      else if(met_phi < -M_PI) met_phi += 2 * M_PI;
      Double_t phi_rel = met_phi * M_PI / (2 * halfAng);
      if(tau_phi > lep_phi){
	phi_rel_hist->Fill(phi_rel);
      }else{
	phi_rel_hist->Fill(-1 * phi_rel);
      }
	
      vis_hist->Fill(invM1);
      //if(invM1<80)
      collinear_hist->Fill(invM2); 
      mmc_hist->Fill(maxw_m);
      }
  }


  CLEAR();
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
