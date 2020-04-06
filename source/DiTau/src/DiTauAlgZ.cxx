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
#include "xAODMissingET/MissingETAuxContainer.h"
#include "xAODMissingET/versions/MissingETBase.h"

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
    if(e->pt()/1000 >= 25 && abs(e->eta()) <= 2.5){
      Electrons.push_back(e);
    }
  }
  if((int)Electrons.size() != no_el){CLEAR(); return false;} 
 
  //MUONS
  const xAOD::MuonContainer *mc = 0;
  CHECK(evtStore()->retrieve(mc, "Muons"));
  for(auto it = mc->begin(); it != mc->end(); it++){
    const xAOD::Muon *mu = *it;
    if(mu->pt()/1000 >= 25 && abs(mu->eta()) <= 2.5){
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
 
  vis_hist = new TH1D("vis_hist","Visible Mass Distribution",160,0,160);
  col_hist = new TH1D("col_hist","Collinear Mass Distribution",160,0,160);
  mmc_hist = new TH1D("mmc_hist","MMC Mass Distribution",160,0,160);
  m_phi_rel_hist = new TH1D("m_phi_rel_hist","Missing Energy Distribution",100,-M_PI,M_PI);

  vis_hist->SetTitle("Visible Mass Distribution;M_{l#tau} [GeV]; N / [GeV]");
  vis_hist->SetTitle("Collinear Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
  mmc_hist->SetTitle("MMC Mass Distribution;M_{l#tau} [GeV]; N / [GeV]");
  m_phi_rel_hist->SetTitle("Missing Energy Distribution;#phi_{rel} [rad];N / [#pi/50]");
  //mmc_hist->SetStats(0);

  CHECK( histSvc()->regHist("/MYSTREAM/vis_hist", vis_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/col_hist", col_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/mmc_hist", mmc_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/m_phi_rel_hist", m_phi_rel_hist) );

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

  met_tool.setTypeAndName("met::METMaker/METMaker");
  CHECK(met_tool.initialize());

  pass = 0;
  fail = 0;
  maxw_m = 0;

  warning_message = false;

  return StatusCode::SUCCESS;
}




StatusCode DiTauAlg::execute() {  
  ATH_MSG_DEBUG ("Executing " << name() << "...");
  setFilterPassed(false); //optional: start with algorithm not passed

  if(no_1lep1tau_events > 16395){ //163208 is no. events in mc15 Ntuples, 16395 is no. entries in col_mass from Sem1
    if(!warning_message){
      std::cout << "16395 events reached!!" << std::endl;
      warning_message = true;
    }
    setFilterPassed(true);
    return StatusCode::SUCCESS;
  }

  //EVENT INFO:
  const xAOD::EventInfo* ei = 0;
  CHECK( evtStore()->retrieve( ei , "EventInfo" ) );
  double eventWeight = ei->mcEventWeight();
 
  //std::cout << ew->getWeight() << " = weight " << std::endl;
  

  //const McEventWeight* ew; 
  //std::cout << ew->getWeight() << " = weight " << std::endl;
/*  ATTEMPTS AT USING METMAKER::  
  //xAOD::MissingETContainer newMETContainer;   
  //xAOD::MissingETAuxContainer newMetAuxContainer;
  //newMETContainer.setStore(newMetAuxContainer);
  const xAOD::MissingETContainer* coreMet  = nullptr;
  CHECK(evtStore()->retrieve(coreMet, "MET_Core_" + chosenJetType));

  const xAOD::MissingETAssociationMap* metMap = nullptr; 
  CHECK(evtStore()->retrieve(metMap, "METAssoc_" + chosenJetType));

  const std::string arg1 = "RefJetTrk";   //change name
  const std::string arg2 = "PVSoftTrk";   //change name

  
  const xAOD::ElectronContainer *ec = 0;
  CHECK(evtStore()->retrieve(ec, "Electrons"));
  for(auto it = ec->begin(); it != ec->end(); it++){
    const xAOD::Electron *e = *it;
    if(e->pt()/1000 >= 25 && abs(e->eta()) <= 2.5){
      Electrons.push_back(e);
    }
  }
  MissingETBase::UsageHandler::Policy objScale = MissingETBase::UsageHandler::PhysicsObject;
  const std::string rebuildKey = "RefEle";
  //met_tool->rebuildMET(rebuildKey, xAOD::Type::Electron, newMETContainer, ec, metMap, objScale);
   */



  double lep_pt{}, lep_eta, lep_phi;
  if(GetCandidates(1,0,1)){
    if(Electrons[0]->charge() == -TauJets[0]->charge() && (Electrons[0]->charge()==1||Electrons[0]->charge()==-1) ){
      lep_pt = Electrons[0]->pt();
      lep_eta = Electrons[0]->eta();
      lep_phi = Electrons[0]->phi();
    }
  }
  else if(GetCandidates(0,1,1)){
    if(Muons[0]->charge() == -TauJets[0]->charge() && (Muons[0]->charge()==1||Muons[0]->charge()==-1) ){
      lep_pt = Muons[0]->pt();
      lep_eta = Muons[0]->eta();
      lep_phi = Muons[0]->phi();
    }
  }

  if(lep_pt != 0){
      // IMPORTANT - now GeV
      lep_pt /= 1000;
      double tau_pt = TauJets[0]->pt() / 1000;
      double tau_eta = TauJets[0]->eta();
      double tau_phi = TauJets[0]->phi();

      double vis_mass = sqrt( 2 * lep_pt * tau_pt * ( cosh(lep_eta - tau_eta) - cos(lep_phi - tau_phi) ) );
      vis_hist->Fill(vis_mass);
      
      if(vis_mass > 5){
        // MET
        const xAOD::MissingETContainer *metc = 0;
        CHECK(evtStore()->retrieve(metc, "MET_Calo"));
        const xAOD::MissingET* met = 0;
        met = metc->at(7);
        double m_et = met->met() / 1000;
        double m_phi = met->phi();
  
        // JETS
	const xAOD::JetContainer *jc = 0;
        double no_25Jets = 0;
        CHECK(evtStore()->retrieve(jc, "AntiKt4LCTopoJets"));
        for(xAOD::JetContainer::const_iterator it=jc->begin(); it!=jc->end(); it++){
          const xAOD::Jet *j = *it;
          if((j->pt()>25e3)&&( abs(j->eta())<2.5)){
            no_25Jets++;
          }
        }
        
        // COLLINEAR
        double nu_lep_pt = m_et * (sin(m_phi) - sin(tau_phi)) / (sin(lep_phi) - sin(tau_phi));
        double nu_tau_pt = m_et * (sin(m_phi) - sin(lep_phi)) / (sin(tau_phi) - sin(lep_phi));
        double x1 = lep_pt / (lep_pt + nu_lep_pt);
        double x2 = tau_pt / (tau_pt + nu_tau_pt);
        double col_mass = vis_mass / sqrt(x1 * x2);

        // ANGULAR
	double half_angle = GetOpenAngle(lep_phi, tau_phi) / 2;
	double rotation_angle;
	if(tau_phi < lep_phi){
	  rotation_angle = -tau_phi;
	}
	else{
	  rotation_angle = -lep_phi;
	}
	tau_phi += rotation_angle;
	lep_phi += rotation_angle;
	m_phi += rotation_angle;
	if(tau_phi > M_PI ) tau_phi -= 2 * M_PI;
	if(lep_phi > M_PI ) lep_phi -= 2 * M_PI;
        if(tau_phi < 0 || lep_phi < 0){
	  tau_phi += half_angle;
	  lep_phi += half_angle;
	  m_phi += half_angle;
	}
	else{
	  tau_phi -= half_angle;
	  lep_phi -= half_angle;
	  m_phi -= half_angle;
	}
	if(m_phi > M_PI) m_phi -= 2 * M_PI;
	else if(m_phi < -M_PI) m_phi += 2 * M_PI;
	double m_phi_rel = (m_phi * M_PI) / (2 * half_angle);
	if(tau_phi > lep_phi) m_phi_rel_hist->Fill(m_phi_rel);
	else m_phi_rel_hist->Fill(-m_phi_rel);

        // SAME IF STATEMENT AS LAST SEMESTER (only there was a <80GeV cut last semester which isn't needed here)
        if(2*half_angle <= 2.5 && 2*half_angle >= 0.5 && m_phi_rel <= 3*M_PI/5 && m_phi_rel >= -7*M_PI/10){
	  no_1lep1tau_events++;
          col_hist->Fill(col_mass, eventWeight);
          
	  // MMC
          if(Electrons.size() == 1){
            maxw_m = APPLY(m_mmt, ei, TauJets[0], Electrons[0], met, no_25Jets);
	  }
	  else{
	    maxw_m = APPLY(m_mmt, ei, TauJets[0], Muons[0], met, no_25Jets);
	  }
          mmc_hist->Fill(maxw_m, eventWeight);
	}
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
