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
int no_1leptau_events{0};

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




bool DiTauAlg::GetCandidates(const int no_el, const int no_mu, const int no_tau){
  
  float ptc30 = -11.0f;
  float topoetc20 = -11.0f;  //Using this because apparently etc20 is not used anymore?  - https://gitlab.cern.ch/ATauLeptonAnalysiS/xTauFramework/-/blob/master/source/xVariables/Root/xVarGroups.cxx#L1890

  const xAOD::ElectronContainer *ec = 0;
  CHECK(evtStore()->retrieve(ec, "Electrons"));
  const xAOD::MuonContainer *mc_test = 0;
  CHECK(evtStore()->retrieve(mc_test, "Muons"));
  const xAOD::TauJetContainer *tjc_test = 0;
  CHECK(evtStore()->retrieve(tjc_test, "TauJets"));
  
  //ELECTRONS
  if((int)ec->size() < no_el){
    CLEAR();
    return false; //Added to just speed up the events
  }
  for(auto it = ec->begin(); it != ec->end(); it++){
    const xAOD::Electron *e = *it;
    if(e->pt()/1000 >= 25 && abs(e->eta()) <= 2.47){	//look at http://opendata.atlas.cern/release/2020/documentation/datasets/objects.html for examples of cuts
    double pt_frac = ptc30/e->pt();
    double et_frac = topoetc20/e->pt();
    if((pt_frac < 0.15)&&(et_frac < 0.15)){ 
      //isolation cuts - not sure if using the right ones https://gitlab.cern.ch/ATauLeptonAnalysiS/xTauFramework/-/blob/master/source/xVariables/Root/xVarGroups.cxx#L1890
      Electrons.push_back(e);
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
  if((int)mc->size() < no_mu){
    CLEAR();
    return false;
  }
  for(auto it = mc->begin(); it != mc->end(); it++){
    const xAOD::Muon *mu = *it;
    if(mu->pt()/1000 >= 25 && abs(mu->eta()) <= 2.5){
      double pt_frac_m = ptc30/mu->pt();
      double et_frac_m = topoetc20/mu->pt();
      if((pt_frac_m < 0.15)&&(et_frac_m < 0.15)){ 
        //isolation cuts - not sure if using the right ones https://gitlab.cern.ch/ATauLeptonAnalysiS/xTauFramework/-/blob/master/source/xVariables/Root/xVarGroups.cxx#L1890
        Muons.push_back(mu);
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
  CHECK(evtStore()->retrieve(tjc, "TauJets"));
  if((int)tjc->size() < no_tau){
    CLEAR();
    return false;
  }
  for(auto it = tjc->begin(); it != tjc->end(); it++){
    const xAOD::TauJet *tj = *it;
    if(tau_selection_t->accept(*tj)){	//pt>20 , eta0-1.37 1.52-2.5, EOR, |tauCharge|=1,( https://arxiv.org/pdf/1607.05979.pdf p7 )
      TauJets.push_back(tj);
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

  vis_hist = new TH1D("vis_hist","",160,0,160);
  vis_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  CHECK(histSvc()->regHist("/MYSTREAM/vis_hist",vis_hist));
  
  col_hist = new TH1D("col_hist","",160,0,160);
  col_hist->SetTitle(";M_{#tau#tau} [GeV]; N / [GeV]");
  CHECK(histSvc()->regHist("/MYSTREAM/col_hist",col_hist));

  phi_rel_hist = new TH1D("phi_rel_hist","",50,-M_PI,M_PI);
  phi_rel_hist->SetTitle(";#phi_{rel} [rad]; N / [2#pi/50]");
  CHECK(histSvc()->regHist("/MYSTREAM/phi_rel_hist",phi_rel_hist));
  
  col_3piover4_hist = new TH1D("col_3piover4_hist","",160,0,160);
  col_3piover4_hist->SetTitle(";M_{#tau#tau} [GeV];N / [GeV]");
  CHECK(histSvc()->regHist("/MYSTREAM/col_3piover4_hist",col_3piover4_hist));
  
  col_piover2_hist = new TH1D("col_piover2_hist","",160,0,160);
  col_piover2_hist->SetTitle(";M_{#tau#tau} [GeV];N / [GeV]");
  CHECK(histSvc()->regHist("/MYSTREAM/col_piover2_hist",col_piover2_hist));
  
  col_piover4_hist = new TH1D("col_piover4_hist","",160,0,160);
  col_piover4_hist->SetTitle(";M_{#tau#tau} [GeV];N / [GeV]");
  CHECK(histSvc()->regHist("/MYSTREAM/col_piover4_hist",col_piover4_hist));
  
  col_piover8_hist = new TH1D("col_piover8_hist","",160,0,160);
  col_piover8_hist->SetTitle(";M_{#tau#tau} [GeV];N / [GeV]");
  CHECK(histSvc()->regHist("/MYSTREAM/col_piover8_hist",col_piover8_hist));
  
  col_piover16_hist = new TH1D("col_piover16_hist","",160,0,160);
  col_piover16_hist->SetTitle(";M_{#tau#tau} [GeV];N / [GeV]");
  CHECK(histSvc()->regHist("/MYSTREAM/col_piover16_hist",col_piover16_hist));
  
  col_piover32_hist = new TH1D("col_piover32_hist","",160,0,160);
  col_piover32_hist->SetTitle(";M_{#tau#tau} [GeV];N / [GeV]");
  CHECK(histSvc()->regHist("/MYSTREAM/col_piover32_hist",col_piover32_hist));
  
  //INITIALISE TAU SELECTION TOOL
  const auto TauSelectionToolName = "TauAnalysisTools::TauSelectionTool/TauSelectionTool";
  tau_selection_t.setTypeAndName(TauSelectionToolName);
  CHECK(tau_selection_t.initialize());

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

  /*if(no_1leptau_events > 16395){ //163208 is no. events in mc15 Ntuples, 16395 is no. entries in col_mass from Sem1
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

  double lep_pt, lep_eta, lep_phi;
  double tau_pt, tau_eta, tau_phi;
  if(GetCandidates(1,0,1)){
    if(Electrons[0]->charge() + TauJets[0]->charge() != 0){
      CLEAR();
      return StatusCode::SUCCESS;
    }
    lep_pt = Electrons[0]->pt() / 1000;
    lep_eta = Electrons[0]->eta();
    lep_phi = Electrons[0]->phi();
  }else if(GetCandidates(0,1,1)){
    if(Muons[0]->charge() + TauJets[0]->charge() != 0){
      CLEAR();
      return StatusCode::SUCCESS;
    }
    lep_pt = Muons[0]->pt() / 1000;
    lep_eta = Muons[0]->eta();
    lep_phi = Muons[0]->phi();
  }else{
    CLEAR();
    return StatusCode::SUCCESS;
  }
  tau_pt = TauJets[0]->pt() / 1000;
  tau_eta = TauJets[0]->eta();
  tau_phi = TauJets[0]->phi();
  
  const xAOD::MissingETContainer* metc = nullptr;
  CHECK(evtStore()->retrieve(metc, "MET_Calo"));
  const xAOD::MissingET* met = metc->at(7); // AllCells
  double met_pt = met->met() / 1000;
  double met_phi = met->phi();
  
  double vis_mass = sqrt(2 * (lep_pt * tau_pt) * (cosh(lep_eta - tau_eta) - cos(lep_phi - tau_phi)));
  vis_hist->Fill(vis_mass);
  
  double nu_lep_pt = met_pt * (sin(met_phi) - sin(tau_phi)) / (sin(lep_phi) - sin(tau_phi));
  double nu_tau_pt = met_pt * (sin(met_phi) - sin(lep_phi)) / (sin(tau_phi) - sin(lep_phi));
  double x1 = lep_pt / (lep_pt + nu_lep_pt);
  double x2 = tau_pt / (tau_pt + nu_tau_pt);
  double col_mass = vis_mass / sqrt(x1 * x2);
  col_hist->Fill(col_mass);

  double opening_angle = GetOpenAngle(lep_phi, tau_phi);
  double rotation_angle;
  if(tau_phi < lep_phi) rotation_angle = -tau_phi;
  else rotation_angle = -lep_phi;
  tau_phi += rotation_angle;
  lep_phi += rotation_angle;
  met_phi += rotation_angle;
  if(tau_phi > M_PI) tau_phi -= 2 * M_PI;
  if(lep_phi > M_PI) lep_phi -= 2 * M_PI;
  if(tau_phi < 0 || lep_phi < 0){
    tau_phi += opening_angle / 2;
    lep_phi += opening_angle / 2;
    met_phi += opening_angle / 2;
  }else{
    tau_phi -= opening_angle / 2;
    lep_phi -= opening_angle / 2;
    met_phi -= opening_angle / 2;
  }
  if(met_phi > M_PI) met_phi -= 2 * M_PI;
  else if(met_phi < M_PI) met_phi += 2 * M_PI;
  
  double met_phi_rel = (met_phi * M_PI) / (opening_angle);
  if(tau_phi > lep_phi) phi_rel_hist->Fill(met_phi_rel);
  else phi_rel_hist->Fill(-met_phi_rel);

  if(vis_mass > 5){
    if(abs(met_phi) < abs(opening_angle / 2)){ // ie met contained within visible products
      
      if(opening_angle < (3 * M_PI / 4)){
        col_3piover4_hist->Fill(col_mass);
	if(opening_angle < (M_PI / 2)){
          col_piover2_hist->Fill(col_mass);
	  if(opening_angle < (M_PI / 4)){
            col_piover4_hist->Fill(col_mass);
	    if(opening_angle < (M_PI / 8)){
              col_piover8_hist->Fill(col_mass);
	      if(opening_angle < (M_PI / 16)){
                col_piover16_hist->Fill(col_mass);
	        if(opening_angle < (M_PI / 32)){
                  col_piover32_hist->Fill(col_mass);
		}
	      }
	    }
	  }
	}
      }

    }
  }

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
