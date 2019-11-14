# script which the user launches to ask which data sets to analyse, handle weighting and launch the
# analysis

import sys
sys.path.insert(0, "backend") # allow code to be imported from subdirectory

import ROOT as r
import os
from DrawC import DrawC
from getInput import getInput
from infofile import infos
from dataSets import dataSets, totRealLum, realList, dataCombos

def fastStr(fMode):
    """
    Return "_fast" if fMode is true, ""  otherwise
    """
    if fMode:
        return "_fast"
    else:
        return ""

def runAnalysis(key, fast):
    """
    Function to run the analysis for a given decay chain labelled 'key'
    """
    # get filename
    filename = dataSets[key]

    # get luminosity weight if data is MC
    if key in realList:
        lumStr = "1"
    else:
        # calculate luminosity weight
        # if it does not work try again without "_1lep" or "_2lep" suffix for key
        try:
            lumWeight = totRealLum * 1000 * infos[key]["xsec"] / (infos[key]["sumw"] *
                infos[key]["red_eff"])
        except KeyError:
            shortKey = key[:-5]
            lumWeight = (totRealLum * 1000 * infos[shortKey]["xsec"] /
                (infos[shortKey]["sumw"] * infos[shortKey]["red_eff"]))

        lumStr = "%.5E" % (lumWeight)

    # launch the analysis script for the given data set
    DrawC(filename,lumStr,fast)

    # move the output to a different directory
    os.system("mv outfile.root out/" + key + fastStr(fast) + ".root")

def combine(files, fast):
    """
    function to get a list of .root files containing histograms and
    add all the histograms together
    """

    # store histograms from the first section of data in a list
    totHist = []
    sec0 = r.TFile("out/"+files[0]+fastStr(fast)+".root") # first file
    key0 = sec0.GetListOfKeys() # first list of keys
    for j in range(len(key0)):
        obj0 = sec0.Get(key0[j].GetName()) # get first object
        if obj0.InheritsFrom("TH1"): # if object is a histogram add it to the list
            totHist.append(obj0)

    # loop over other output files
    for i in range(1,len(files)):
        # read in output file for this section of data
        secFile = r.TFile("out/" + files[i] + fastStr(fast) + ".root")
        
        # get histogram keys
        keys = secFile.GetListOfKeys()
        
        # loop over histograms in the file and add them to the total histograms
        for j in range(len(keys)):
            obj = secFile.Get(keys[j].GetName()) # get object
            if obj.InheritsFrom("TH1"): # if object is a histogram add it on
                totHist[j].Add(obj)

    # save the combined histograms to a file
    name = "_".join(files) # name of output file
    totFile = r.TFile("out/"+ name + fastStr(fast) + ".root","RECREATE")
    for hist in totHist:
        hist.Write()
    totFile.Close()


# get input from user
# keep asking until answered
chainsValid = False
while (not chainsValid):
    print("Please enter a comma-seperated list of decay chains.")
    print("Use '+' to add data sets together.")
    print("Write 'text' if you would prefer to read a list from 'input.txt':")
    chains, chainsValid = getInput()
    print()

# if all decay chains are valid loop over series 
# detect whether the user wants to run in 'fast' mode for only 1% of data
answered = False
while (not answered):
    print("Would you like to run in fast mode to only analyse 1% of data? (yes/no)")
    useFast = input().lower()
    if useFast in "yes":
        answered = True
        fastMode = True
    elif useFast in "no":
        answered = True
        fastMode = False

# iterate over sums of chains from user input
for i in range(len(chains)):
    # loop over chains in the series and run the analysis
    for j in range(len(chains[i])):
        chain = chains[i][j]

        # print a message so the user knows what's up
        print("Analysing "+chain+"...")

        # if the decay chain corresponds to data in more than one file then
        # analyse all the files and add them
        if (chain in dataCombos.keys()):
            for subChain in dataCombos[chain]:
                print(subChain)
                runAnalysis(subChain, fastMode)
            combine(dataCombos[chain], fastMode)

            # rename the outputted file to use the input key
            oldName = "_".join(dataCombos[chain])+fastStr(fastMode)+".root"
            os.system("mv out/"+oldName+" out/"+chain+
                    fastStr(fastMode)+".root")
            
        # otherwise run the analysis for the single file
        else:
            runAnalysis(chain,fastMode)

    # combine chains in the series if it contains more than one chain
    if (len(chains[i])>1):
        combine(chains[i], fastMode)
