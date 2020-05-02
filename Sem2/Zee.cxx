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
  //met_Electrons->clear();
  //met_Muons->clear();
  //met_Taus->clear();
  //met_container->clear();     // THINK THESE WERE CAUSING A CORE DUMP TOO
  //met_aux_container->clearDecorations();
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
  const xAOD::MuonContainer *mc_test = 0;
  CHECK(evtStore()->retrieve(mc_test, "Muons"));
  const xAOD::TauJetContainer *tjc_test = 0;
  CHECK(evtStore()->retrieve(tjc_test, "TauJets"));
  //double num_leps = ec->size() + mc_test->size() + tjc_test->size();
  //if(num_leps<3) std::cout << ec->size() + mc_test->size() + tjc_test->size() << " = leps " << std::endl;
  
  if((int)ec->size() < no_el){
    CLEAR();
    return false; //Added to just speed up the events
  }
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
  if((int)mc->size() < no_mu){
    CLEAR();
    return false;
  }
  for(auto it = mc->begin(); it != mc->end(); it++){
    const xAOD::Muon *mu = *it;
    if(mu->pt()/1000 >= 25 && abs(mu->eta()) <= 2.5){
      mu->isolation(ptc30, xAOD::Iso::ptcone30);  //TODO:: FIX THESE VARIABLES
      mu->isolation(topoetc20, xAOD::Iso::topoetcone20);
      //std::cout << ptc30/mu->pt() << " = ptc30 " << topoetc20/mu->pt() << " = topoetcone20" << std::endl;
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
  CHECK(evtStore()->retrieve(tjc, "TauJets"));
  if((int)tjc->size() < no_tau){
    CLEAR();
    return false;
  }
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
  
  Zee_hist = new TH1D("Zee_hist","",160,0,160);
  Zee_hist->SetTitle("Z#rightarrow ee Mass Distribution;M_{ee} [GeV]; N / [GeV]");
  CHECK(histSvc()->regHist("/MYSTREAM/Zee_hist",Zee_hist));
  Zee_ew_hist = new TH1D("Zee_ew_hist","",160,0,160);
  Zee_ew_hist->SetTitle("Z#rightarrow ee Mass Distribution;M_{ee} [GeV]; N / [GeV]");
  CHECK(histSvc()->regHist("/MYSTREAM/Zee_ew_hist",Zee_ew_hist));
  
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

  double lep1_pt, lep1_eta, lep1_phi;
  double lep2_pt, lep2_eta, lep2_phi;
  if(GetCandidates(2,0,0)){
    if(Electrons[0]->charge() + Electrons[1]->charge() != 0){
      CLEAR();
      return StatusCode::SUCCESS;
    }
    lep1_pt = Electrons[0]->pt() / 1000;
    lep1_eta = Electrons[0]->eta();
    lep1_phi = Electrons[0]->phi();
    lep2_pt = Electrons[1]->pt() / 1000;
    lep2_eta = Electrons[1]->eta();
    lep2_phi = Electrons[1]->phi();
    double inv_mass = sqrt(2*(lep1_pt*lep2_pt)*(cosh(lep1_eta-lep2_eta)-cos(lep1_phi-lep2_phi)));
    Zee_hist->Fill(inv_mass);
    Zee_ew_hist->Fill(inv_mass, eventWeight);
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
