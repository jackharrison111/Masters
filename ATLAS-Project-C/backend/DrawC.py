import ROOT as r
from os import listdir
from dataSets import dirs

def DrawC(filename,lumStr,fast):
    """
    Function to load in the C++ code and run it for a given data set
    """

    # search through several directories to find where the input file is located
    for path in dirs:
        if filename in listdir(path):
            correctPath = path
            break

    # reset environment and get path to file
    r.gROOT.Reset()
    fullPath = correctPath + filename 

    # load in CLoop.C
    r.gROOT.ProcessLine(".L backend/CLoop.C")

    # load in tree from file
    r.gROOT.ProcessLine("TFile* f = new TFile(\""+fullPath+"\")")
    r.gROOT.ProcessLine("TTree * minTree = new TTree")
    r.gROOT.ProcessLine("f->GetObject(\"mini\",minTree)")

    # create new instance of CLoop and loop over events
    r.gROOT.ProcessLine("CLoop* t = new CLoop(minTree)")
    r.gROOT.ProcessLine("t->Loop("+lumStr+","+str(fast).lower()+")")
