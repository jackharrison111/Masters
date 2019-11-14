
#define CLoop_cxx

#include <../Analysis.C"

void CLoop::Loop(double lumFactor, bool fastMode)
{
//    In a ROOT session, you can do:
//        root> .L CLoop.C
//        root> CLoop t
//        root> t.GetEntry(12); // Fill t data members with entry number 12
//        root> t.Show();         // Show values of entry 12
//        root> t.Show(16);      // Read and show values of entry 16
//        root> t.Loop();         // Loop on all entries
//

//      This is the loop skeleton where:
//     jentry is the global entry number in the chain
//     ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//     jentry for TChain::GetEntry
//     ientry for TTree::GetEntry and TBranch::GetEntry
//
//         To read only selected branches, Insert statements like:
// METHOD1:
//     fChain->SetBranchStatus("*",0);  // disable all branches
//     fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//     fChain->GetEntry(jentry);         //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

    clock_t startTime = clock(); // get start time

    if (fChain == 0) return;


    // book histograms
    Book();
    // end booking

    Long64_t nentries = fChain->GetEntriesFast();

    // if in fast mode only loop over 1% of the entries
    Long64_t nLoop = nentries;
    if (fastMode) nLoop = nentries * 0.01;

    Long64_t nbytes = 0, nb = 0;

    // loop over number of entries
    for (Long64_t jentry=0; jentry<nLoop;jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);    nbytes += nb;
        // if (Cut(ientry) < 0) continue;
        
        // calculate event weight
        double eventWeight = 1;

        // check if event is from real data
        if (mcWeight != 0) {
            // take product of all scale factors
            eventWeight = mcWeight*scaleFactor_PILEUP*scaleFactor_ELE*scaleFactor_MUON*
                scaleFactor_PHOTON*scaleFactor_TAU*scaleFactor_BTAG*scaleFactor_LepTRIGGER*
                scaleFactor_PhotonTRIGGER*scaleFactor_TauTRIGGER*scaleFactor_DiTauTRIGGER*lumFactor;
        }

        // fill histograms
        Fill(eventWeight);
        // end filling
    }
    
    // set style of histograms and write to output file
    // open output file
    TFile outfile("outfile.root","recreate");
    Style();
    // end style and writing
    //
    outfile.Close();
    
    clock_t endTime = clock(); // get end time
    // calculate time taken and print it
    double time_spent = (endTime - startTime) / CLOCKS_PER_SEC;
    cout << time_spent << std::endl;
}
