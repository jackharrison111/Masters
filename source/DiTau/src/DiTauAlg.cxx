#include <iostream>
#include <math.h>
#include <vector>

// DiTau includes
#include "DiTauAlg.h"
#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODCore/ShallowAuxContainer.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODBase/IParticleHelpers.h"

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
  Electrons.clear();
  Muons.clear();
  TauJets.clear();
  met_Electrons->clear();
  met_Muons->clear();
  met_Taus->clear();
  met_container->clear();     // THINK THESE WERE CAUSING A CORE DUMP TOO
  met_aux_container->clearDecorations();
  //std::cout << "met_aux_container->size() = " << met_aux_container->size() << std::endl;
}




double DiTauAlg::GetOpenAngle(double ang1, double ang2){
  double openAngle = abs(ang1 - ang2);
  if(openAngle > M_PI){
    openAngle = 2*M_PI - openAngle;
  }
  return openAngle;
}




bool DiTauAlg::GetCandidates(const int no_el, const int no_mu, const int no_tau){
  
  float ptc30 = -11.0f;
  float topoetc20 = -11.0f;  //Using this because apparently etc20 is not used anymore?  - https://gitlab.cern.ch/ATauLeptonAnalysiS/xTauFramework/-/blob/master/source/xVariables/Root/xVarGroups.cxx#L1890



  //ELECTRONS
  const xAOD::ElectronContainer *ec = 0;
  CHECK(evtStore()->retrieve(ec, "Electrons"));
  for(auto it = ec->begin(); it != ec->end(); it++){
    const xAOD::Electron *e = *it;
    if(e->pt()/1000 >= 25 && abs(e->eta()) <= 2.47){	//look at http://opendata.atlas.cern/release/2020/documentation/datasets/objects.html for examples of cuts
    
    e->isolation(ptc30, xAOD::Iso::ptcone30);  //TODO:: FIX THESE VARIABLES
    e->isolation(topoetc20, xAOD::Iso::topoetcone20);
    double pt_frac = ptc30/e->pt();
    double et_frac = topoetc20/e->pt();
    if((pt_frac < 0.15)&&(et_frac < 0.15)){ 
      //isolation cuts - not sure if using the right ones https://gitlab.cern.ch/ATauLeptonAnalysiS/xTauFramework/-/blob/master/source/xVariables/Root/xVarGroups.cxx#L1890
      Electrons.push_back(e);
      met_Electrons->push_back(*it);
      }
    }
    ptc30 = -11.0f;
    topoetc20 = -11.0f;
  }
  if((int)Electrons.size() != no_el){
    CLEAR();
    return false;
  }
 
  //MUONS
  const xAOD::MuonContainer *mc = 0;
  CHECK(evtStore()->retrieve(mc, "Muons"));
  for(auto it = mc->begin(); it != mc->end(); it++){
    const xAOD::Muon *mu = *it;
    if(mu->pt()/1000 >= 25 && abs(mu->eta()) <= 2.5){
      mu->isolation(ptc30, xAOD::Iso::ptcone30);  //TODO:: FIX THESE VARIABLES
      mu->isolation(topoetc20, xAOD::Iso::topoetcone20);
      std::cout << ptc30/mu->pt() << " = ptc30 " << topoetc20/mu->pt() << " = topoetcone20" << std::endl;
      double pt_frac_m = ptc30/mu->pt();
      double et_frac_m = topoetc20/mu->pt();
      if((pt_frac_m < 0.15)&&(et_frac_m < 0.15)){ 
        //isolation cuts - not sure if using the right ones https://gitlab.cern.ch/ATauLeptonAnalysiS/xTauFramework/-/blob/master/source/xVariables/Root/xVarGroups.cxx#L1890
        Muons.push_back(mu);
        met_Muons->push_back(*it);
      }
    }
    ptc30 = -11.0f;
    topoetc20 = -11.0f;
  }
  if((int)Muons.size() != no_mu){
    CLEAR();
    return false;
  }
  
  //TAUS
  const xAOD::TauJetContainer *tjc = 0;
  //NEEDS TO BE DATAVECTOR?
  //DataVector<xAOD::TauJet> *tjs = 0; 
  CHECK(evtStore()->retrieve(tjc, "TauJets"));
  for(auto it = tjc->begin(); it != tjc->end(); it++){
    const xAOD::TauJet *tj = *it;
    if(tau_selection_t->accept(*tj)){	//pt>20 , eta0-1.37 1.52-2.5, EOR, |tauCharge|=1,( https://arxiv.org/pdf/1607.05979.pdf p7 )
      TauJets.push_back(tj);
      met_Taus->push_back(*it);
    }
  }
  if((int)TauJets.size() != no_tau){
    CLEAR();
    return false;
  }
  
  return true;
}




StatusCode DiTauAlg::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");
 
  start_time = clock();
  vis_hist = new TH1D("vis_hist","Visible Mass Distribution",160,0,160);
  leplep_hist = new TH1D("leplep_hist", "Direct Z#rightarrow ll Invariant Mass Distribution", 160,0,160);
  col_hist = new TH1D("col_hist","Collinear Mass Distribution",160,0,160);
  col_hist_met7 = new TH1D("col_hist_met7","Collinear Mass Distribution",160,0,160);
  mmc_hist = new TH1D("mmc_hist","MMC Mass Distribution",160,0,160);
  mmc_hist_met7 = new TH1D("mmc_hist_met7","MMC Mass Distribution",160,0,160);
  mmc_hist_metref8 = new TH1D("mmc_hist_metref8","MMC Mass Distribution",160,0,160);
  m_phi_rel_hist = new TH1D("m_phi_rel_hist","Missing Energy Distribution",100,-M_PI,M_PI);
  met_ang_diffs_hist = new TH1D("met_ang_diffs_hist","Separation between reconstructed and initial MET",100,0,M_PI);
  m_my2DHist = new TH2D("2DinvMass" , "", 160, 0, 160, 160, 0, 160);
  m_my2DHist_met7 = new TH2D("2DinvMass_met7" , "", 160, 0, 160, 160, 0, 160);
  metrefmmc_col_2D = new TH2D("metrefmmc_col_2D", "", 160, 0, 160, 160, 0, 160);


  vis_hist->SetTitle("Visible Mass Distribution;M_{l#tau} [GeV]; N / [GeV]");
  leplep_hist->SetTitle("Direct Z#rightarrow ll Mass Distribution;M_{ll} [GeV]; N / [GeV]");
  col_hist->SetTitle("Collinear Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
  col_hist_met7->SetTitle("Collinear Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
  mmc_hist->SetTitle("MMC Mass Distribution;M_{l#tau} [GeV]; N / [GeV]");
  mmc_hist_met7->SetTitle("MMC Mass Distribution;M_{l#tau} [GeV]; N / [GeV]");
  m_phi_rel_hist->SetTitle("Missing Energy Distribution;#phi_{rel} [rad];N / [#pi/50]");
  met_ang_diffs_hist->SetTitle("Separation between initial and rebuilt MET vectors;#phi_{d} [rad];N / [#pi/50]");
  m_my2DHist->SetTitle("Candidate Z Boson Invariant Mass Distributions; M_{#tau#tau} [GeV]; M_{ll} [GeV]");
  m_my2DHist_met7->SetTitle("Candidate Z Boson Invariant Mass Distributions; M_{#tau#tau} [GeV]; M_{ll} [GeV]");
  //mmc_hist->SetStats(0);

  CHECK( histSvc()->regHist("/MYSTREAM/vis_hist", vis_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/leplep_hist", leplep_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/col_hist", col_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/col_hist_met7", col_hist_met7) );
  CHECK( histSvc()->regHist("/MYSTREAM/mmc_hist", mmc_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/mmc_hist_met7", mmc_hist_met7) );
  CHECK( histSvc()->regHist("/MYSTREAM/mmc_hist_metref8", mmc_hist_metref8) );
  CHECK( histSvc()->regHist("/MYSTREAM/m_phi_rel_hist", m_phi_rel_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/met_ang_diffs_hist", met_ang_diffs_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/m_my2DHist", m_my2DHist) );
  CHECK( histSvc()->regHist("/MYSTREAM/m_my2DHist_met7", m_my2DHist_met7) );
  CHECK( histSvc()->regHist("/MYSTREAM/metrefmmc_col_2D", metrefmmc_col_2D) );

  //INITIALISE THE MISSING MASS TOOL
  m_mmt.setTypeAndName("MissingMassTool/MissingMassTool");
  CHECK(m_mmt.initialize());
  CHECK(m_mmt->setProperty("UseTauProbability", 1));
  CHECK(m_mmt->setProperty("CalibSet", "2016MC15C"));
  CHECK(m_mmt->setProperty("NiterFit2", 40));
  CHECK(m_mmt->setProperty("NiterFit3", 20));
  CHECK(m_mmt->setProperty("NsigmaMET", 6));
  CHECK(m_mmt->setProperty("alg_version", 3));
  //CHECK(m_mmt->setProperty("UseMETDphiLL", 1)); only for leplep
  CHECK(m_mmt->setProperty("UseEfficiencyRecovery", 1));
  CHECK(m_mmt->setProperty("UseTailCleanup", 1));

  //INITIALISE TAU SELECTION TOOL
  const auto TauSelectionToolName = "TauAnalysisTools::TauSelectionTool/TauSelectionTool";
  tau_selection_t.setTypeAndName(TauSelectionToolName);
  CHECK(tau_selection_t.initialize());

  //INITIALISE MET RECONSTRUCTION TOOL
  met_tool.setTypeAndName("met::METMaker/METMaker");
  CHECK(met_tool.initialize());
  obj_scale = MissingETBase::UsageHandler::PhysicsObject;
  met_container = new xAOD::MissingETContainer();
  met_aux_container = new xAOD::MissingETAuxContainer();
  met_container->setStore(met_aux_container);
  met_Electrons = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
  met_Muons = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
  met_Taus = new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS);

  //INITIALISE JET CALIBRATION TOOL
  CHECK(ASG_MAKE_ANA_TOOL(jet_calib_tool, JetCalibrationTool));
  jet_calib_tool.setName("jetCalibTool");
  CHECK(jet_calib_tool.setProperty("JetCollection", "AntiKt4EMTopo")); // jet_type
  CHECK(jet_calib_tool.setProperty("ConfigFile", "JES_data2017_2016_2015_Recommendation_Feb2018_rel21.config"));
  CHECK(jet_calib_tool.setProperty("CalibSequence", "JetArea_Residual_EtaJES_GSC"));
  CHECK(jet_calib_tool.setProperty("IsData", false));
  CHECK(jet_calib_tool.retrieve());
  
  pass = 0;
  fail = 0;
  maxw_m = 0;

  warning_message = false;

  MC = false;
  if(!MC) std::cout << "USING REAL DATA" << std::endl;
  else std::cout << "USING MC DATA" << std::endl;
  
  return StatusCode::SUCCESS;
}




StatusCode DiTauAlg::execute() {  
  ATH_MSG_DEBUG ("Executing " << name() << "...");
  setFilterPassed(false); //optional: start with algorithm not passed

  /*if(no_1lep1tau_events > 16395){ //163208 is no. events in mc15 Ntuples, 16395 is no. entries in col_mass from Sem1
    if(!warning_message){
      std::cout << "16395 events reached!!" << std::endl;
      warning_message = true;
    }
    setFilterPassed(true);
    return StatusCode::SUCCESS;
  }*/


  //EVENT INFO:
  const xAOD::EventInfo* ei = 0;
  CHECK( evtStore()->retrieve( ei , "EventInfo" ) );
  

  double eventWeight;
  try{ 
    eventWeight = ei->mcEventWeight();
  }
  catch(...){
    eventWeight = 1;
  }

  //Recalibrate the jets:  
  const std::string jet_type = "AntiKt4LCTopo";  //removed 'Anti'
  
  
  const xAOD::JetContainer* uncalibJets = nullptr;
  CHECK( evtStore()->retrieve(uncalibJets, jet_type+"Jets"));//this retrieves and applies the correction
  std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > calibJetsPair = xAOD::shallowCopyContainer( *uncalibJets );//make a shallow copy to calibrate
  xAOD::JetContainer *& calibJets = calibJetsPair.first;//create a reference to the first element of the pair (i.e. the JetContainer)     
  
  //TODO::need to figure out this bit: can't find the necessary file to include to use the function
  //met::addGhostMuonsToJets(*muons, *calibJets);     
  
  for ( const auto& jet : *calibJets ) { 	//Shallow copy is needed 	
    if(!jet_calib_tool->applyCalibration(*jet)){ //apply the calibration   
      //std::cout << "Calibration failed." << std::endl;  
      CLEAR();
      return StatusCode::SUCCESS;
    }
  }   
  if(!xAOD::setOriginalObjectLink(*uncalibJets, *calibJets)){//tell calib container what old container it matches       
    std::cout << "Failed to set the original object links" << std::endl;
  }     
  CHECK( evtStore()->record(calibJets, "CalibJets") );
  
  double lep1_pt, lep1_eta, lep1_phi;
  double lep2_pt, lep2_eta, lep2_phi;
  double tau_partner_pt{}, tau_partner_eta, tau_partner_phi;//, tau_partner_int;
  const xAOD::IParticle* tau_partner; 
  
  double Z_mass = 91.2; 
  if(GetCandidates(3,0,1)){
    double totalQ = Electrons[0]->charge() + Electrons[1]->charge() + Electrons[2]->charge();
    if(totalQ + TauJets[0]->charge() != 0){
      CLEAR();
      return StatusCode::SUCCESS;
    }

    std::vector<int> same_leps;
    double odd_lep{};
    for(int j=0;j<3;j++){
      if(Electrons[j]->charge() == -totalQ){
	odd_lep=j;
      }
      else{
        same_leps.push_back(j);
      }
    }

    double invM1_leps, invM1_taus, invM2_leps, invM2_taus;
    invM1_leps = sqrt(2*(Electrons[odd_lep]->pt()*Electrons[same_leps[0]]->pt())*(cosh(Electrons[odd_lep]->eta()-Electrons[same_leps[0]]->eta())-cos(Electrons[odd_lep]->phi()-Electrons[same_leps[0]]->phi())))/1000;
    invM1_taus = sqrt(2*(TauJets[0]->pt()*Electrons[same_leps[1]]->pt())*(cosh(TauJets[0]->eta()-Electrons[same_leps[1]]->eta())-cos(TauJets[0]->phi()-Electrons[same_leps[1]]->phi())))/1000;

    invM2_leps = sqrt(2*(Electrons[odd_lep]->pt()*Electrons[same_leps[1]]->pt())*(cosh(Electrons[odd_lep]->eta()-Electrons[same_leps[1]]->eta())-cos(Electrons[odd_lep]->phi()-Electrons[same_leps[1]]->phi())))/1000;
    invM2_taus = sqrt(2*(TauJets[0]->pt()*Electrons[same_leps[0]]->pt())*(cosh(TauJets[0]->eta()-Electrons[same_leps[0]]->eta())-cos(TauJets[0]->phi()-Electrons[same_leps[0]]->phi())))/1000;

    lep1_pt = Electrons[odd_lep]->pt();
    lep1_eta = Electrons[odd_lep]->eta();
    lep1_phi = Electrons[odd_lep]->phi();
    if((abs(invM2_leps - Z_mass) + abs(invM2_taus - Z_mass)) > (abs(invM1_leps - Z_mass) + abs(invM1_taus - Z_mass))){
      //set pairings 1 to be the right ones
      lep2_pt = Electrons[same_leps[0]]->pt();
      lep2_eta = Electrons[same_leps[0]]->eta();
      lep2_phi = Electrons[same_leps[0]]->phi();
      tau_partner_pt = Electrons[same_leps[1]]->pt();
      tau_partner_eta = Electrons[same_leps[1]]->eta();
      tau_partner_phi = Electrons[same_leps[1]]->phi();
      //tau_partner_int = same_leps[1];
      tau_partner = Electrons[same_leps[1]];
    }
    else{
      //set pairings 1 to be the right ones 
      lep2_pt = Electrons[same_leps[1]]->pt();
      lep2_eta = Electrons[same_leps[1]]->eta();
      lep2_phi = Electrons[same_leps[1]]->phi();
      tau_partner_pt = Electrons[same_leps[0]]->pt();
      tau_partner_eta = Electrons[same_leps[0]]->eta();
      tau_partner_phi = Electrons[same_leps[0]]->phi();
      //tau_partner_int = same_leps[0];
      tau_partner = Electrons[same_leps[0]];
    }

  }
  else if(GetCandidates(1,2,1)){
    double totalQ = Electrons[0]->charge() + Muons[0]->charge() + Muons[1]->charge();
    if(totalQ + TauJets[0]->charge() != 0){
      CLEAR();
      return StatusCode::SUCCESS;
    }
    lep1_pt = Muons[0]->pt();
    lep1_eta = Muons[0]->eta();
    lep1_phi = Muons[0]->phi();
    lep2_pt = Muons[1]->pt();
    lep2_eta = Muons[1]->eta();
    lep2_phi = Muons[1]->phi();
    tau_partner_pt = Electrons[0]->pt();
    tau_partner_eta = Electrons[0]->eta();
    tau_partner_phi = Electrons[0]->phi();
    tau_partner = Electrons[0];
  }

  else if(GetCandidates(2,1,1)){
    double totalQ = Electrons[0]->charge() + Electrons[1]->charge() + Muons[0]->charge();
    if(totalQ + TauJets[0]->charge() != 0){
      CLEAR();
      return StatusCode::SUCCESS;
    }
    lep1_pt = Electrons[0]->pt();
    lep1_eta = Electrons[0]->eta();
    lep1_phi = Electrons[0]->phi();
    lep2_pt = Electrons[1]->pt();
    lep2_eta = Electrons[1]->eta();
    lep2_phi = Electrons[1]->phi();
    tau_partner_pt = Muons[0]->pt();
    tau_partner_eta = Muons[0]->eta();
    tau_partner_phi = Muons[0]->phi();
    tau_partner = Muons[0];
  }

  else if(GetCandidates(0,3,1)){	//repeat for muons
    double totalQ = Muons[0]->charge() + Muons[1]->charge() + Muons[2]->charge();
    if(totalQ + TauJets[0]->charge() != 0){
      CLEAR();
      return StatusCode::SUCCESS;
    }
    std::vector<int> same_leps;
    double odd_lep{};
    for(int j=0;j<3;j++){
      if(Muons[j]->charge() == -totalQ){
	odd_lep=j;
      }
      else{
        same_leps.push_back(j);
      }
    }

    double invM1_leps, invM1_taus, invM2_leps, invM2_taus;
    invM1_leps = sqrt(2*(Muons[odd_lep]->pt()*Muons[same_leps[0]]->pt())*(cosh(Muons[odd_lep]->eta()-Muons[same_leps[0]]->eta())-cos(Muons[odd_lep]->phi()-Muons[same_leps[0]]->phi())))/1000;
    invM1_taus = sqrt(2*(TauJets[0]->pt()*Muons[same_leps[1]]->pt())*(cosh(TauJets[0]->eta()-Muons[same_leps[1]]->eta())-cos(TauJets[0]->phi()-Muons[same_leps[1]]->phi())))/1000;

    invM2_leps = sqrt(2*(Muons[odd_lep]->pt()*Muons[same_leps[1]]->pt())*(cosh(Muons[odd_lep]->eta()-Muons[same_leps[1]]->eta())-cos(Muons[odd_lep]->phi()-Muons[same_leps[1]]->phi())))/1000;
    invM2_taus = sqrt(2*(TauJets[0]->pt()*Muons[same_leps[0]]->pt())*(cosh(TauJets[0]->eta()-Muons[same_leps[0]]->eta())-cos(TauJets[0]->phi()-Muons[same_leps[0]]->phi())))/1000;
    
    lep1_pt = Muons[odd_lep]->pt();
    lep1_eta = Muons[odd_lep]->eta();
    lep1_phi = Muons[odd_lep]->phi();
    if((abs(invM2_leps - Z_mass) + abs(invM2_taus - Z_mass)) > (abs(invM1_leps - Z_mass) + abs(invM1_taus - Z_mass))){
      //set pairings 1 to be the right ones
      lep2_pt = Muons[same_leps[0]]->pt();
      lep2_eta = Muons[same_leps[0]]->eta();
      lep2_phi = Muons[same_leps[0]]->phi();
      tau_partner_pt = Muons[same_leps[1]]->pt();
      tau_partner_eta = Muons[same_leps[1]]->eta();
      tau_partner_phi = Muons[same_leps[1]]->phi();
      //tau_partner_int = same_leps[1];
      tau_partner = Muons[same_leps[1]];
    }
    else{
      //set pairings 1 to be the right ones 
      lep2_pt = Muons[same_leps[1]]->pt();
      lep2_eta = Muons[same_leps[1]]->eta();
      lep2_phi = Muons[same_leps[1]]->phi();
      tau_partner_pt = Muons[same_leps[0]]->pt();
      tau_partner_eta = Muons[same_leps[0]]->eta();
      tau_partner_phi = Muons[same_leps[0]]->phi();
      //tau_partner_int = same_leps[0];
      tau_partner = Muons[same_leps[0]];
    } 
  }


  if(tau_partner_pt != 0){
    // IMPORTANT - now GeV
    lep1_pt /= 1000;
    lep2_pt /= 1000;
    tau_partner_pt /= 1000;
    double tau_pt = TauJets[0]->pt() / 1000;
    double tau_eta = TauJets[0]->eta();
    double tau_phi = TauJets[0]->phi();

    double vis_mass = sqrt( 2 * tau_partner_pt * tau_pt * ( cosh(tau_partner_eta - tau_eta) - cos(tau_partner_phi - tau_phi) ) );
    vis_hist->Fill(vis_mass);
     


    if(vis_mass > 5){
     
      double no_25Jets = 0;
      ConstDataVector<xAOD::JetContainer> met_Jets(SG::VIEW_ELEMENTS);
      const xAOD::JetContainer *jc = nullptr;
      CHECK( evtStore()->retrieve(jc, /*jet_type + "Jets"*/"CalibJets") );
      for(auto it = jc->begin(); it != jc->end(); it++){
        //if((*it)->pt()/1000>20){ // TODO: have commented this since met tool needs all jets?
          met_Jets.push_back(*it);
          if(((*it)->pt()/1000>25)&&(abs((*it)->eta())<2.5)){
            no_25Jets++;
          }
        //}
      }

      const xAOD::MissingETAssociationMap* metMap = nullptr; 
      CHECK(evtStore()->retrieve(metMap, "METAssoc_" + jet_type));

       
      //REBUILD MET::
      //order has to be electrons,photons,taus,muons,jets
      
      metMap->resetObjSelectionFlags();
      met_tool->rebuildMET("RefEle" , xAOD::Type::Electron, met_container, met_Electrons->asDataVector(), metMap, obj_scale); 
  
      ConstDataVector<xAOD::PhotonContainer> met_Photons(SG::VIEW_ELEMENTS);
      const xAOD::PhotonContainer *pc = nullptr;
      CHECK( evtStore()->retrieve(pc, "Photons") );
      for(auto it = pc->begin(); it != pc->end(); it++){
        const xAOD::Photon *p = *it;
        if((p->pt()/1000 > 25) && (abs(p->eta())<2.37)){ //Cuts taken from http://opendata.atlas.cern/release/2020/documentation/datasets/objects.html
          met_Photons.push_back(*it);
        }
      }
      //Rebuild photons
      metMap->resetObjSelectionFlags();
      met_tool->rebuildMET("RefPhoton", xAOD::Type::Photon, met_container, met_Photons.asDataVector(), metMap, obj_scale);

      //const std::string tauName = "RefTau";
      metMap->resetObjSelectionFlags();
      met_tool->rebuildMET("RefTau", xAOD::Type::Tau, met_container, met_Taus->asDataVector(), metMap, obj_scale);
      

      //const std::string muName = "RefMu";     
      metMap->resetObjSelectionFlags();
      met_tool->rebuildMET("RefMu" , xAOD::Type::Muon, met_container, met_Muons->asDataVector(), metMap, obj_scale);

      const xAOD::MissingETContainer* met_core = nullptr;
      CHECK( evtStore()->retrieve(met_core, "MET_Core_" + jet_type) );
  
      metMap->resetObjSelectionFlags();
      met_tool->rebuildJetMET("RefJet", "SoftClus", "PVSoftTrk", met_container, /*calibJets*/ met_Jets.asDataVector(), met_core, metMap, true);
      met_tool->buildMETSum("FinalTrk", met_container, MissingETBase::Source::Track);
  
      const xAOD::MissingETContainer* met_calo = nullptr;
      CHECK( evtStore()->retrieve(met_calo, "MET_Calo") );

      const xAOD::MissingET* met7 = met_calo->at(7);
      double met7_pt = met7->met() /1000;
      double met7_phi = met7->phi();


      // MET
      double met_pt = (*met_container)["FinalTrk"]->met() / 1000;
      std::cout << "rebuilt MET, met_pt = " << met_pt << " GeV" << std::endl << std::endl;
      double m_phi = (*met_container)["FinalTrk"]->phi();
      
      // MET REFERENCE?
      int k{};
      std::cout << "new event:" << std::endl;
      const xAOD::MissingETContainer* met_ref = nullptr;
      CHECK( evtStore()->retrieve(met_ref, "MET_Reference_" + jet_type) );
      for(auto it = met_ref->begin(); it != met_ref->end(); it++){
        const xAOD::MissingET* met = *it;
	k++;
        std::cout << "MissingETContainer reference entry " << k << ": met->name() = " << met->name() << ", met->met() = " << met->met() << " MeV " << std::endl;
      }
      const xAOD::MissingET* met8 = met_ref->at(8);
      met_pt = met8->met() / 1000;
      m_phi = met8->phi();

  
      double met_diffs = GetOpenAngle(met7_phi, m_phi);  
      double invMass_leps = sqrt(2*(lep1_pt*lep2_pt)*(cosh(lep1_eta-lep2_eta)-cos(lep1_phi - lep2_phi)));
  
      // COLLINEAR
      double nu_lep_pt = met_pt * (sin(m_phi) - sin(tau_phi)) / (sin(tau_partner_phi) - sin(tau_phi));
      double nu_tau_pt = met_pt * (sin(m_phi) - sin(tau_partner_phi)) / (sin(tau_phi) - sin(tau_partner_phi));
      double x1 = tau_partner_pt / (tau_partner_pt + nu_lep_pt);
      double x2 = tau_pt / (tau_pt + nu_tau_pt);
      double col_mass = vis_mass / sqrt(x1 * x2);


      double nu_lep_pt_m7 = met7_pt * (sin(met7_phi) - sin(tau_phi)) / (sin(tau_partner_phi) - sin(tau_phi));
      double nu_tau_pt_m7 = met7_pt * (sin(met7_phi) - sin(tau_partner_phi)) / (sin(tau_phi) - sin(tau_partner_phi));
      double x1_7 = tau_partner_pt / (tau_partner_pt + nu_lep_pt_m7);
      double x2_7 = tau_pt / (tau_pt + nu_tau_pt_m7);
      double col_mass_m7 = vis_mass / sqrt(x1_7 * x2_7);

      // ANGULAR
      double half_angle = GetOpenAngle(tau_partner_phi, tau_phi) / 2;
      double rotation_angle;
      if(tau_phi < tau_partner_phi){
	rotation_angle = -tau_phi;
      }
      else{
        rotation_angle = -tau_partner_phi;
      }
      tau_phi += rotation_angle;
      tau_partner_phi += rotation_angle;
      m_phi += rotation_angle;
      if(tau_phi > M_PI ) tau_phi -= 2 * M_PI;
      if(tau_partner_phi > M_PI ) tau_partner_phi -= 2 * M_PI;
      if(tau_phi < 0 || tau_partner_phi < 0){
        tau_phi += half_angle;
	tau_partner_phi += half_angle;
	m_phi += half_angle;
      }
      else{
	tau_phi -= half_angle;
	tau_partner_phi -= half_angle;
	m_phi -= half_angle;
      }
      if(m_phi > M_PI) m_phi -= 2 * M_PI;
      else if(m_phi < -M_PI) m_phi += 2 * M_PI;
      double m_phi_rel = (m_phi * M_PI) / (2 * half_angle);
      if(tau_phi > tau_partner_phi) m_phi_rel_hist->Fill(m_phi_rel);
      else m_phi_rel_hist->Fill(-m_phi_rel);

      // SAME IF STATEMENT AS LAST SEMESTER (only there was a <80GeV cut last semester which isn't needed here)
      if(2*half_angle <= 2.5 && 2*half_angle >= 0.5 && m_phi_rel <= 3*M_PI/5 && m_phi_rel >= -7*M_PI/10){
	no_1lep1tau_events++;
        col_hist->Fill(col_mass, eventWeight);
        col_hist_met7->Fill(col_mass_m7,eventWeight);
        leplep_hist->Fill(invMass_leps, eventWeight);  
        met_ang_diffs_hist->Fill(met_diffs);
        // MMC 
	//maxw_m = APPLY(m_mmt, ei, TauJets[0], tau_partner, met, no_25Jets);
	maxw_m = APPLY(m_mmt, ei, TauJets[0], tau_partner, (*met_container)["FinalTrk"], no_25Jets);
	double maxw_m_met7 = APPLY(m_mmt, ei, TauJets[0], tau_partner, met7, no_25Jets);
	double maxw_m_met8 = APPLY(m_mmt, ei, TauJets[0], tau_partner, met8, no_25Jets);
        mmc_hist->Fill(maxw_m, eventWeight);
        mmc_hist_met7->Fill(maxw_m_met7, eventWeight);
	mmc_hist_metref8->Fill(maxw_m_met8, eventWeight);
	m_my2DHist->Fill(maxw_m, invMass_leps);
	m_my2DHist_met7->Fill(maxw_m_met7, invMass_leps);
	metrefmmc_col_2D->Fill(maxw_m_met8, invMass_leps, eventWeight);
      }
    } // vis_mass > 5
  } // tau_partner != 0
    

  CLEAR();
  setFilterPassed(true); //if got here, assume that means algorithm passed
  return StatusCode::SUCCESS;
}




StatusCode DiTauAlg::finalize() {
  ATH_MSG_INFO ("Finalizing " << name() << "...");
 
  end_time = clock();
  std::cout<<"Run time: "<<(end_time-start_time)/CLOCKS_PER_SEC<<" s"<<std::endl<<std::endl;

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
