#Skeleton joboption for a simple analysis job

#---- Minimal job options -----

jps.AthenaCommonFlags.AccessMode = "ClassAccess"              #Choose from TreeAccess,BranchAccess,ClassAccess,AthenaAccess,POOLAccess
#jps.AthenaCommonFlags.TreeName = "MyTree"                    #when using TreeAccess, must specify the input tree name

jps.AthenaCommonFlags.HistOutputs = ["MYSTREAM:output.root"]  #register output files like this. MYSTREAM is used in the code

athAlgSeq += CfgMgr.DiTauAlg()                               #adds an instance of your alg to the main alg sequence


#---- Options you could specify on command line -----
jps.AthenaCommonFlags.EvtMax=-1                          #set on command-line with: --evtMax=-1
#jps.AthenaCommonFlags.SkipEvents=0                       #set on command-line with: --skipEvents=0
jps.AthenaCommonFlags.FilesInput = ["/afs/cern.ch/user/a/aburke/public/mc15_13TeV.361108.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Ztautau.recon.AOD.e3601_s2576_s2132_r6630_tid05358802_00/AOD.05358802._004299.pool.root.1"]        #set on command-line with: --filesInput=...


include("AthAnalysisBaseComps/SuppressLogging.py")              #Optional include to suppress as much athena output as possible. Keep at bottom of joboptions so that it doesn't suppress the logging of the things you have configured above

