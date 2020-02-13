#ifndef DITAU_DITAUALG_H
#define DITAU_DITAUALG_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "AsgTools/AnaToolHandle.h"
#include "DiTauMassTools/MissingMassTool.h"

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
  

  ///Other useful methods provided by base class are:
  ///evtStore()        : ServiceHandle to main event data storegate
  ///inputMetaStore()  : ServiceHandle to input metadata storegate
  ///outputMetaStore() : ServiceHandle to output metadata storegate
  ///histSvc()         : ServiceHandle to output ROOT service (writing TObjects)
  ///currentFile()     : TFile* to the currently open input file
  ///retrieveMetadata(...): See twiki.cern.ch/twiki/bin/view/AtlasProtected/AthAnalysisBase#ReadingMetaDataInCpp


 private: 

   //Example algorithm property, see constructor for declaration:
   //int m_nProperty = 0;

   //Example histogram, see initialize method for registration to output histSvc
   //TH1D* m_myHist = 0;
   //TTree* m_myTree = 0;
   asg::AnaToolHandle<MissingMassTool> m_mmt;
   int no_events;
   int no_events_more_than_2_electrons;

}; 

#endif //> !DITAU_DITAUALG_H
