// Analysis.py
// Skeleton code in python provided for you
// In place of this comment you should write [your name] -- [the date] and update it as you go!
// Make sure to make backups and comment as you go along :)

// Header guard to ensure file is imported properly
#ifndef Analysis
#define Analysis

// Include the file that lets the program know about the data
#include <backend/CLoop.h"

void CLoop::Book() {
    // This function is where you "book" your histograms
    // It is called once per data set
    // You will need to do this for each histogram you want to plot
    // The syntax is: histogram = new TH1F("histogram_name", "Title", number_of_bins, x_min, x_max);
    
    // For example, booking a histogram to plot number of leptons per event:
    h_lep_n = new TH1F("lep_n","Number of leptons",10,-0.5,9.5);

    // A couple of other basic histograms
    h_lep_type = new TH1F("lep_type","Type of leptons",10,10.5,20.5);
    h_lep_pt = new TH1F("lep_pt","Transverse momentum of electrons",200,0,14e4);
}

void CLoop::Fill(double weight) {
    // This function is where you select events and fill your histograms
    // It is called once PER EVENT.
    
    // To fill a histogram you should write:
    // histogram->Fill(quantity,weight)

    // For example, filling a histogram with the number of leptons in each event
    h_lep_n -> Fill(lep_n, weight);

    // loop over leptons in the event
    for (size_t ilep=0; ilep<lep_type->size(); ilep++) {
        // This is where you will want to fill histograms with properties of individual leptons
        // You will need to use quantity->at(ilep) to refer to properties of the lepton

        // For example, filling a histogram with the type of each lepton
        // electrons have type = 11, muons have type = 13
        h_lep_type -> Fill(lep_type->at(ilep),weight);

        // Filling the transverse momentum of each lepton, but only if it is an electron
        if (lep_type->at(ilep) == 11) {
            h_lep_pt -> Fill(lep_pt->at(ilep),weight);
        }

    } // End of loop over leptons

}

void CLoop::Style() {
    // This function is where you can control the style elements of your histograms and write them to a file
    // It is called once per data set

    // For example, set some properties of the lep_n histogram
    h_lep_n->GetXaxis()->SetTitle("Number of leptons per event"); // label x axis
    h_lep_n->GetYaxis()->SetTitle("Number of entries/bin"); // label y axis
    h_lep_n->SetTitle("Number of leptons  per event");
    h_lep_n->SetLineColor(kRed); // set the line colour to red
    // For more information see https://root.cern.ch/root/htmldoc/guides/users-guide/Histograms.html

    // Write histograms to a file
    // This needs to be done for each histogram
    h_lep_n->Write();
    h_lep_type->Write();
    h_lep_pt->Write();
}

#endif // End header guard
