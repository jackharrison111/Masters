#include <iostream>
#include <math.h>
#include <vector>

// DiTau includes
#include "DiTauAlg.h"
#include "xAODJet/JetContainer.h"
#include "xAODTau/TauJetAuxContainer.h"
#include "xAODMuon/MuonAuxContainer.h"
#include "xAODEgamma/ElectronAuxContainer.h"
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
#include "xAODMissingET/MissingETComposition.h"
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
}




double DiTauAlg::GetOpenAngle(double ang1, double ang2){
  double openAngle = abs(ang1 - ang2);
  if(openAngle > M_PI){
    openAngle = 2*M_PI - openAngle;
  }
  return openAngle;
}




xAOD::MissingET& DiTauAlg::InsertMETTerm(xAOD::MissingETContainer* metCont, std::string name, MissingETBase::Types::bitmask_t source){
  metCont->push_back(new xAOD::MissingET);
  xAOD::MissingET& newMet = *metCont->back();
  newMet.setName(name);
  newMet.setSource(source);
  return newMet;
}




bool DiTauAlg::GetCandidates(const int no_el, const int no_mu, const int no_tau){
  if(((int)Electrons.size() == no_el) && ((int)Muons.size() == no_mu) && ((int)TauJets.size() == no_tau)){
    //std::cout << "Passed GetCandidates" << std::endl;
    return true;
  }
  //std::cout << "Failed GetCandidates" << std::endl;
  CLEAR();
  return false;

}




StatusCode DiTauAlg::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");
  
  vis_hist = new TH1D("vis_hist","Visible Mass Distribution",160,0,160);
  leplep_hist = new TH1D("leplep_hist", "Direct Z#rightarrow ll Invariant Mass Distribution", 160,0,160);
  col_hist = new TH1D("col_hist","Collinear Mass Distribution",160,0,160);
  col_hist_susy = new TH1D("col_hist_susy","Collinear Mass Distribution",160,0,160);
  mmc_hist = new TH1D("mmc_hist","MMC Mass Distribution",160,0,160);
  mmc_hist_susy = new TH1D("mmc_hist_susy","MMC Mass Distribution",160,0,160);
  mmc_leps_2D = new TH2D("mmc_leps_2D", "", 160, 0, 160, 160, 0, 160);
  mmc_leps_2D_susy = new TH2D("mmc_leps_2D_susy","",160,0,160,160,0,160);
  met_hist = new TH1D("met_hist","",400,0,400);
  met_hist_susy = new TH1D("met_hist_susy","",400,0,400);
  phi_rel_hist = new TH1D("phi_rel_hist", "",50,-M_PI,M_PI);

  col_3piover4_hist = new TH1D("col_3piover4_hist", "",160,0,160);
  col_piover2_hist = new TH1D("col_piover2_hist", "",160,0,160);
  col_piover4_hist = new TH1D("col_piover4_hist", "",160,0,160);
  col_piover8_hist = new TH1D("col_piover8_hist", "",160,0,160);
  col_piover16_hist = new TH1D("col_piover16_hist", "",160,0,160);
  col_piover32_hist = new TH1D("col_piover32_hist", "",160,0,160);



  vis_hist->SetTitle("Visible Mass Distribution;M_{l#tau} [GeV]; N / [GeV]");
  leplep_hist->SetTitle("Direct Z#rightarrow ll Mass Distribution;M_{ll} [GeV]; N / [GeV]");
  col_hist->SetTitle("Collinear Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
  col_hist_susy->SetTitle("Collinear Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
  mmc_hist->SetTitle("MMC Mass Distribution;M_{l#tau} [GeV]; N / [GeV]");
  mmc_hist_susy->SetTitle("MMC Mass Distribution;M_{l#tau} [GeV]; N / [GeV]");
  mmc_leps_2D->SetTitle("Missing Energy Distribution;M_{#tau#tau} [GeV];M_{ll} [GeV]");
  mmc_leps_2D_susy->SetTitle("Missing Energy Distribution;M_{#tau#tau} [GeV];M_{ll} [GeV]");
  met_hist->SetTitle("Missing Energy Distribution;MET [GeV];N / [GeV]");
  met_hist_susy->SetTitle("Missing Energy Distribution;MET [GeV];N / [GeV]");
  phi_rel_hist->SetTitle("Phi rel Distribution;#phi_{rel} [rad]; N / [2#pi/100 rad]");

  col_3piover4_hist->SetTitle("Collinear Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover2_hist->SetTitle("Collinear Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover4_hist->SetTitle("Collinear Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover8_hist->SetTitle("Collinear Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover16_hist->SetTitle("Collinear Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
  col_piover32_hist->SetTitle("Collinear Mass Distribution;M_{#tau#tau} [GeV]; N / [GeV]");
 

  CHECK( histSvc()->regHist("/MYSTREAM/vis_hist", vis_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/leplep_hist", leplep_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/col_hist", col_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/col_hist_susy", col_hist_susy) );
  CHECK( histSvc()->regHist("/MYSTREAM/mmc_hist", mmc_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/mmc_hist_susy", mmc_hist_susy) );
  CHECK( histSvc()->regHist("/MYSTREAM/mmc_leps_2D", mmc_leps_2D) );
  CHECK( histSvc()->regHist("/MYSTREAM/mmc_leps_2D_susy", mmc_leps_2D_susy) );
  CHECK( histSvc()->regHist("/MYSTREAM/met_hist", met_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/met_hist_susy", met_hist_susy) );
  CHECK( histSvc()->regHist("/MYSTREAM/phi_rel_hist", phi_rel_hist) );
  
  CHECK( histSvc()->regHist("/MYSTREAM/col_3piover4_hist", col_3piover4_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/col_piover2_hist", col_piover2_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/col_piover4_hist", col_piover4_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/col_piover8_hist", col_piover8_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/col_piover16_hist", col_piover16_hist) );
  CHECK( histSvc()->regHist("/MYSTREAM/col_piover32_hist", col_piover32_hist) );
  
  obj_scale = MissingETBase::UsageHandler::PhysicsObject;

  start_time = clock();

  //INITIALISE MISSING MASS TOOL
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
  CHECK(m_mmt->setProperty("UseTailCleanup", 1));

  //INITIALISE TAU SELECTION TOOL
  tau_selection_t.setTypeAndName("TauAnalysisTools::TauSelectionTool/TauSelectionTool");
  CHECK(tau_selection_t.initialize());
  
  //INITIALISE TAU TRUTH MATCHING TOOL
  tau_truthmatching_t.setTypeAndName("TauAnalysisTools::TauTruthMatchingTool/TauTruthMatchingTool");
  CHECK(tau_truthmatching_t.setProperty("WriteTruthTaus", true)); // this line ensures that using the truthmatchtool actually rewrites the evtStore() taus so that they are truthmatched (i think)
  CHECK(tau_truthmatching_t.initialize());

  //INITIALISE SUSY TOOL
  CHECK(ASG_MAKE_ANA_TOOL(susy_tool, ST::SUSYObjDef_xAOD));
  susy_tool.setName("SUSYTools");
  CHECK(susy_tool.retrieve());
  
  pass = 0;
  fail = 0;
  maxw_m = 0;

  warning_message = false;

  MC = false;
  //if(!MC) std::cout << "USING REAL DATA" << std::endl;
  //else std::cout << "USING MC DATA" << std::endl;
  
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
  CHECK(evtStore()->retrieve( ei , "EventInfo" ));
  

  double eventWeight;
  try{ 
    eventWeight = ei->mcEventWeight();
  }
  catch(...){
    eventWeight = 1;
  }

   
  // calibration ??? :
  // https://twiki.cern.ch/twiki/bin/view/AtlasComputing/SoftwareTutorialxAODAnalysisInAthena#9_An_example_of_skimming_events
  // https://gitlab.cern.ch/atlas/athena/-/blob/master/PhysicsAnalysis/SUSYPhys/SUSYTools/SUSYTools/SUSYObjDef_xAOD.h

  float ptc30 = -11.0f;
  float topoetc20 = -11.0f;  //Using this because apparently etc20 is not used anymore?  - https://gitlab.cern.ch/ATauLeptonAnalysiS/xTauFramework/-/blob/master/source/xVariables/Root/xVarGroups.cxx#L1890

  // ELECTRONS
  xAOD::ElectronContainer* ec = nullptr;
  xAOD::ShallowAuxContainer* e_aux = nullptr;
  susy_tool->GetElectrons(ec, e_aux, true, "Electrons", ec);

  xAOD::ElectronContainer* electrons_met = new xAOD::ElectronContainer;
  xAOD::ElectronAuxContainer* electrons_met_aux = new xAOD::ElectronAuxContainer;
  electrons_met->setStore(electrons_met_aux);  

  for(auto it = ec->begin(); it != ec->end(); it++){
    xAOD::Electron *e = *it;
    xAOD::Electron *el_test = new xAOD::Electron;
    if(e->pt()/1000 >= 25 && abs(e->eta()) <= 2.47){	//look at http://opendata.atlas.cern/release/2020/documentation/datasets/objects.html for examples of cuts
      e->isolation(ptc30, xAOD::Iso::ptcone30);  //TODO:: FIX THESE VARIABLES
      e->isolation(topoetc20, xAOD::Iso::topoetcone20);
      double pt_frac = ptc30/e->pt();
      double et_frac = topoetc20/e->pt();
      if((pt_frac < 0.15)&&(et_frac < 0.15)){ 
        //isolation cuts - not sure if using the right ones https://gitlab.cern.ch/ATauLeptonAnalysiS/xTauFramework/-/blob/master/source/xVariables/Root/xVarGroups.cxx#L1890
        Electrons.push_back(e);
        electrons_met->push_back(el_test);
	*el_test = *(*it);
      }
    }
    //delete el_test;
  }
  CHECK(evtStore()->record(electrons_met, "GoodElectrons"));
  CHECK(evtStore()->record(electrons_met_aux, "GoodElectronsAux"));
  //delete electrons_met;
  //delete electrons_met_aux;
  //std::cout << "deleted electrons" << std::endl;

  if(((int)Electrons.size() != 0)&&((int)Electrons.size() != 1)&&((int)Electrons.size() != 3)){
    CLEAR();
    return StatusCode::SUCCESS;
  }
  // TAUS
  const xAOD::TauJetContainer *untruthmatched_tjc = nullptr;
  CHECK(evtStore()->retrieve(untruthmatched_tjc, "TauJets"));
  for(auto it = untruthmatched_tjc->begin(); it != untruthmatched_tjc->end(); it++){
    const xAOD::TauJet* tj = *it;
    tau_truthmatching_t->getTruth(*tj);
  }

  xAOD::TauJetContainer* tjc = nullptr;
  xAOD::TauJetContainer* taus_met = new xAOD::TauJetContainer;
  xAOD::ShallowAuxContainer* t_aux = nullptr;
  xAOD::TauJetAuxContainer* tmet_aux = new xAOD::TauJetAuxContainer;
  taus_met->setStore(tmet_aux);
  susy_tool->GetTaus(tjc, t_aux, true, "TauJets", tjc);
  for(auto it = tjc->begin(); it != tjc->end(); it++){
    xAOD::TauJet *tj = *it;
    xAOD::TauJet *test_tau = new xAOD::TauJet;
    if(tau_selection_t->accept(*tj)){	//pt>20 , eta0-1.37 1.52-2.5, EOR, |tauCharge|=1,( https://arxiv.org/pdf/1607.05979.pdf p7 )
      TauJets.push_back(tj);
      taus_met->push_back(test_tau);
      *test_tau = *(*it); 
    }
   //delete test_tau;
  }
  CHECK(evtStore()->record(taus_met, "GoodTaus"));
  CHECK(evtStore()->record(tmet_aux, "GoodTausAux"));
  //delete taus_met;
  //delete tmet_aux;
  //std::cout << "deleted taus" << std::endl;
  if((int)TauJets.size() != 1){
    CLEAR();
    return StatusCode::SUCCESS;
  }


  // MUONS
  xAOD::MuonContainer* mc = nullptr;
  xAOD::ShallowAuxContainer* m_aux = nullptr;
  susy_tool->GetMuons(mc, m_aux, true, "Muons", mc);
  
  xAOD::MuonContainer* muons_met = new xAOD::MuonContainer;
  xAOD::MuonAuxContainer* muons_met_aux = new xAOD::MuonAuxContainer;
  muons_met->setStore(muons_met_aux);  

  for(auto it = mc->begin(); it != mc->end(); it++){
    xAOD::Muon *m = *it;
    xAOD::Muon *mu = new xAOD::Muon;
    
    if(m->pt()/1000 >= 25 && abs(m->eta()) <= 2.5){
      m->isolation(ptc30, xAOD::Iso::ptcone30);  //TODO:: FIX THESE VARIABLES
      m->isolation(topoetc20, xAOD::Iso::topoetcone20);
      //std::cout << ptc30/mu->pt() << " = ptc30 " << topoetc20/mu->pt() << " = topoetcone20" << std::endl;
      double pt_frac_m = ptc30/m->pt();
      double et_frac_m = topoetc20/m->pt();
      if((pt_frac_m < 0.15)&&(et_frac_m < 0.15)){ 
        //isolation cuts - not sure if using the right ones https://gitlab.cern.ch/ATauLeptonAnalysiS/xTauFramework/-/blob/master/source/xVariables/Root/xVarGroups.cxx#L1890
        Muons.push_back(m);
	muons_met->push_back(mu);
        *mu = *(*it);
      }
    }
    //delete mu;
  }
  CHECK(evtStore()->record(muons_met, "GoodMuons"));
  CHECK(evtStore()->record(muons_met_aux, "GoodMuonsAux"));
  //delete muons_met;
  //delete muons_met_aux;

  if(((int)Muons.size() != 0)&&((int)Muons.size() != 1)&&((int)Muons.size() != 3)){
    CLEAR();
    return StatusCode::SUCCESS;
  }
  

  const std::string jet_type = "AntiKt4EMTopo";  //removed 'Anti'

  // BUILD MET AND THEN STORE IT
  const xAOD::MissingETAssociationMap* metMap = nullptr; 
  CHECK(evtStore()->retrieve(metMap, "METAssoc_" + jet_type));
  const xAOD::MissingETContainer* metCore = nullptr;
  CHECK(evtStore()->retrieve(metCore, "MET_Core_" + jet_type));
      
  metMap->resetObjSelectionFlags();

  xAOD::MissingETContainer* metCont = new xAOD::MissingETContainer;
  xAOD::MissingETAuxContainer* metContAux = new xAOD::MissingETAuxContainer;
  CHECK(evtStore()->record(metCont, "MyRebuiltMETContainer"));
  CHECK(evtStore()->record(metContAux, "MyRebuiltMETContainerAux."));
  metCont->setStore(metContAux);

  xAOD::MissingET& metEle = InsertMETTerm(metCont, "ElectronTerm", MissingETBase::Source::electron());
  xAOD::MissingET& metMu = InsertMETTerm(metCont, "MuonTerm", MissingETBase::Source::muon());
  xAOD::MissingET& metTau = InsertMETTerm(metCont, "TauTerm", MissingETBase::Source::tau());
     
  for(const auto& el : *ec){
    bool isSelected = xAOD::MissingETComposition::selectIfNoOverlaps(metMap, el, MissingETBase::UsageHandler::TrackCluster); // TODO: why not obj_scale?
    if(isSelected){
      if(el->pt()/1000 >= 25 && abs(el->eta()) <= 2.47){	//look at http://opendata.atlas.cern/release/2020/documentation/datasets/objects.html for examples of cuts
        el->isolation(ptc30, xAOD::Iso::ptcone30);  //TODO:: FIX THESE VARIABLES
        el->isolation(topoetc20, xAOD::Iso::topoetcone20);
        double pt_frac = ptc30/el->pt();
        double et_frac = topoetc20/el->pt();
        if((pt_frac < 0.15)&&(et_frac < 0.15)){ 
          metEle += el;
        }
      }
    }
  }

  for(auto const& tau : *tjc){
    bool isSelected = xAOD::MissingETComposition::selectIfNoOverlaps(metMap, tau, obj_scale);
    if(isSelected){
      if(tau_selection_t->accept(*tau)){
        metTau += tau;
      }
    }
  }

  for(const auto& mu : *mc){
    bool isSelected = xAOD::MissingETComposition::selectIfNoOverlaps(metMap, mu, MissingETBase::UsageHandler::OnlyTrack);
    if(isSelected){
      if(mu->pt()/1000 >= 25 && abs(mu->eta()) <= 2.5){
        mu->isolation(ptc30, xAOD::Iso::ptcone30);  //TODO:: FIX THESE VARIABLES
        mu->isolation(topoetc20, xAOD::Iso::topoetcone20);
        double pt_frac_m = ptc30/mu->pt();
        double et_frac_m = topoetc20/mu->pt();
        if((pt_frac_m < 0.15)&&(et_frac_m < 0.15)){ 
          metMu += mu;
        }
      }
    }
  }

  // jets and soft term simultaneuosly
  const xAOD::MissingET& metSoftClusCore = *(*metCore)["SoftClusCore"];
  const xAOD::MissingET& metSoftTrkCore = *(*metCore)["PVSoftTrkCore"];
  xAOD::MissingET& metJet = InsertMETTerm(metCont, "JetTerm", MissingETBase::Source::jet());
  xAOD::MissingET& metSoftClus = InsertMETTerm(metCont, "ClusterSoftTerm", metSoftClusCore.source());
  xAOD::MissingET& metSoftTrk = InsertMETTerm(metCont, "TrackSoftTerm", metSoftTrkCore.source());
  metSoftClus += metSoftClusCore;
  metSoftTrk += metSoftTrkCore;
  xAOD::JetContainer* jc = nullptr;
  xAOD::ShallowAuxContainer* j_aux = nullptr;
  susy_tool->GetJets(jc, j_aux, true, jet_type + "Jets", jc);
  no_25Jets = 0;
  //ConstDataVector<xAOD::JetContainer> met_Jets(SG::VIEW_ELEMENTS);
  for(const auto& jet : *jc){
    const xAOD::MissingETAssociation* assoc = xAOD::MissingETComposition::getAssociation(metMap, jet);
    xAOD::JetFourMom_t jetconstp4 = jet->jetP4(xAOD::JetConstitScaleMomentum);
    MissingETBase::Types::constvec_t jettrkvec = assoc->jetTrkVec();
    MissingETBase::Types::constvec_t overlapcalvec = assoc->overlapCalVec();
    MissingETBase::Types::constvec_t overlaptrkvec = assoc->overlapTrkVec();

    float uniqueEnergy = jetconstp4.E() - overlapcalvec.ce();
    if(uniqueEnergy > 0.5){
      float calibfactor = jet->pt() / jetconstp4.Pt();
      float opx = (jetconstp4.Px() - overlapcalvec.cpx()) * calibfactor;
      float opy = (jetconstp4.Py() - overlapcalvec.cpy()) * calibfactor;
      float opt = sqrt(opx * opx + opy * opy);
      metJet.add(opx, opy, opt);
    }else{
      float calopx = (jetconstp4.Px() - overlapcalvec.cpx());
      float calopy = (jetconstp4.Py() - overlapcalvec.cpy());
      float calopt = sqrt(calopx * calopx + calopy * calopy);
      metSoftClus.add(calopx, calopy, calopt);
      float trkopx = (jettrkvec.cpx() - overlaptrkvec.cpx());
      float trkopy = (jettrkvec.cpy() - overlaptrkvec.cpy());
      float trkopt = sqrt(trkopx * trkopx + trkopy * trkopy);
      metSoftTrk.add(trkopx, trkopy, trkopt);
    }
    //if(jet->pt() / 1000 > 20){ // TODO: have comented this since met tool needs all jets?
    //met_Jets.push_back(jet);
    if(jet->pt() / 1000 > 25 && abs(jet->eta()) < 2.5){
      no_25Jets++;
    }
	//}
  }

  // misc association
  const xAOD::MissingETAssociation* miscAssoc = metMap->getMiscAssociation();
  MissingETBase::Types::constvec_t overlapcalvec = miscAssoc->overlapCalVec();
  MissingETBase::Types::constvec_t overlaptrkvec = miscAssoc->overlapTrkVec();
  metSoftClus.add(overlapcalvec.cpx(), overlapcalvec.cpy(), overlapcalvec.sumpt());
  metSoftTrk.add(overlaptrkvec.cpx(), overlaptrkvec.cpy(), overlaptrkvec.sumpt());
  
  // total sum
  xAOD::MissingET& metFinalClus = InsertMETTerm(metCont, "TotalTermWithCST", MissingETBase::Source::total());
  xAOD::MissingET& metFinalTrk = InsertMETTerm(metCont, "TotalTermWithTST", MissingETBase::Source::total());
  for(const auto& met : *metCont){
    if(MissingETBase::Source::isTotalTerm(met->source())) continue;
      if(!MissingETBase::Source::isSoftTerm(met->source())){
	metFinalClus += *met;
	metFinalTrk += *met;
      }else{
	if(MissingETBase::Source::isTrackTerm(met->source())){
	  metFinalTrk += *met;
	}else{
	  metFinalClus += *met;
	}
      }
  }




  //finalMET = (*metCont)["TotalTermWithTST"];   //JUST CHANGE THIS IF WANT TO USE A DIFFERNET MET
  const xAOD::MissingETContainer *met7 = nullptr;
  CHECK(evtStore()->retrieve(met7, "MET_Track"));
   /*   for(const auto& met : *met7){
        std::cout << "  MET term \"" << met->name() << "\""
	          << "-- magnitude: " << met->met() / 1000
		  << " GeV, phi: " << met->phi()
		  << std::endl;
      }
  */
  finalMET = met7->at(0);//(*metCont)["TotalTermWithTST"];
  met_hist->Fill(finalMET->met() / 1000);

  xAOD::MissingETContainer *susyMET = new xAOD::MissingETContainer;
  xAOD::MissingETAuxContainer *susyMET_aux = new xAOD::MissingETAuxContainer;
  susyMET->setStore(susyMET_aux);
  const xAOD::PhotonContainer* gamma = nullptr;
  const xAOD::IParticleContainer* invis = nullptr;
  const xAOD::ElectronContainer* e_met = nullptr;
  CHECK(evtStore()->retrieve(e_met, "GoodElectrons"));
  const xAOD::MuonContainer* mu_met = nullptr;
  CHECK(evtStore()->retrieve(mu_met, "GoodMuons"));
  const xAOD::TauJetContainer* tj_met = nullptr;
  CHECK(evtStore()->retrieve(tj_met, "GoodTaus"));
  susy_tool->GetMET(*susyMET, jc, e_met, mu_met, gamma, tj_met, true, invis);/**susyMET, jc, e_met, mu_met, gamma, tj_met, true, invis); */

  xAOD::MissingET *susy_met = (*susyMET)["Final"];
  met_hist_susy->Fill(susy_met->met() / 1000);
  //std::cout << "SUSY MET = " << susy_met->met()/1000 << " GeV , FinalMET = " << finalMET->met()/1000 << " GeV" <<  std::endl; 
 
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
      //tau_partner = Muons[same_leps[1]];
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
      //tau_partner = Muons[same_leps[0]];
    } 
  }
  else{
    CLEAR();
    return StatusCode::SUCCESS;
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
     


    if(vis_mass > 5){
      // final check
      /*std::cout << "Final MET check." << std::endl;
      for(const auto& met : *susyMET){
        std::cout << "  MET term \"" << met->name() << "\""
	          << "-- magnitude: " << met->met() / 1000
		  << " GeV, phi: " << met->phi()
		  << std::endl;
      }
      std::cout << std::endl;*/
      double met_pt = finalMET->met() / 1000;
      double m_phi = finalMET->phi();

      double met_pt_susy = susy_met->met() / 1000;
      double met_phi_susy = susy_met->phi(); 

      double invMass_leps = sqrt(2 * (lep1_pt * lep2_pt) * (cosh(lep1_eta - lep2_eta) - cos(lep1_phi - lep2_phi)));
      
      // COLLINEAR
      double nu_lep_pt = met_pt * (sin(m_phi) - sin(tau_phi)) / (sin(tau_partner_phi) - sin(tau_phi));
      double nu_tau_pt = met_pt * (sin(m_phi) - sin(tau_partner_phi)) / (sin(tau_phi) - sin(tau_partner_phi));
      double x1 = tau_partner_pt / (tau_partner_pt + nu_lep_pt);
      double x2 = tau_pt / (tau_pt + nu_tau_pt);
      double col_mass = vis_mass / sqrt(x1 * x2);


      // COLLINEAR - SUSY
      double nu_lep_pt_susy = met_pt_susy * (sin(met_phi_susy) - sin(tau_phi)) / (sin(tau_partner_phi) - sin(tau_phi));
      double nu_tau_pt_susy = met_pt_susy * (sin(met_phi_susy) - sin(tau_partner_phi)) / (sin(tau_phi) - sin(tau_partner_phi));
      double x3 = tau_partner_pt / (tau_partner_pt + nu_lep_pt_susy);
      double x4 = tau_pt / (tau_pt + nu_tau_pt_susy);
      double col_mass_susy = vis_mass / sqrt(x3 * x4);


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
      if(tau_phi > tau_partner_phi) phi_rel_hist->Fill(m_phi_rel);
      else phi_rel_hist->Fill(-m_phi_rel);


      // SAME IF STATEMENT AS LAST SEMESTER (only there was a <80GeV cut last semester which isn't needed here)
      if(2*half_angle <= 2.5 && 2*half_angle >= 0.5 && m_phi_rel <= 3*M_PI/5 && m_phi_rel >= -7*M_PI/10){
	
        no_1lep1tau_events++;
        vis_hist->Fill(vis_mass);
        leplep_hist->Fill(invMass_leps, eventWeight);  
        
        //collinear
        col_hist->Fill(col_mass, eventWeight);
        col_hist_susy->Fill(col_mass_susy, eventWeight);
 
        // MMC 
	double mmc_mass = APPLY(m_mmt, ei, TauJets[0], tau_partner, finalMET, no_25Jets);	
	double mmc_mass_susy = APPLY(m_mmt, ei, TauJets[0], tau_partner, susy_met, no_25Jets);
        mmc_hist->Fill(mmc_mass, eventWeight);
        mmc_hist_susy->Fill(mmc_mass_susy, eventWeight);

        //2D hists
	mmc_leps_2D->Fill(mmc_mass, invMass_leps);
	mmc_leps_2D_susy->Fill(mmc_mass_susy, invMass_leps);
      }
     if(abs(m_phi) < abs(half_angle)){ // ie met contained within visible products
      if(2*half_angle < (3 * M_PI / 4)){
        col_3piover4_hist->Fill(col_mass_susy);
        if(2*half_angle < (M_PI / 2)){
          col_piover2_hist->Fill(col_mass_susy);
          if(2*half_angle < (M_PI / 4)){
            col_piover4_hist->Fill(col_mass_susy);
            if(2*half_angle < (M_PI / 8)){
              col_piover8_hist->Fill(col_mass_susy);
              if(2*half_angle < (M_PI / 16)){
                col_piover16_hist->Fill(col_mass_susy);
                if(2*half_angle < (M_PI / 32)){
                  col_piover32_hist->Fill(col_mass_susy);
                }
              }
            }
          }
        }
      }
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
