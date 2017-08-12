// Plot summed event data for SuperSim hits, for ZIP #6 (G115)
//
// Usage: root -b event_sum.C("filename", "variable", "mczip6")
//
// 20170725  Michael Kelsey
// 20170811  Extend support to other trees (mczip4 is G124)

#include "TFile.h"
#include "TString.h"

void exitmsg(const TString& msg, Int_t exitcode) {
  cout << "ERROR: " << msg << endl;
  ::exit(exitcode);
}


int event_sum(TString fname, TString var, TString tree="mczip6") {
  TFile file(fname);
  if (!file.IsOpen()) exitmsg("Unable to open file "+fname, 2);

  TTree* mczip = 0;
  file.GetObject("/G4SimDir/"+tree, mczip);
  if (!mczip) exitmsg("Can't find "+tree+" TTree", 1);

  Double_t evnum;	// Reusable buffers for processing events
  Double_t thevar;
  mczip->SetBranchAddress("EventNum", &evnum);
  mczip->SetBranchAddress(var, &thevar);

  // Get variable range for plotting
  Double_t varmin = mczip->GetMinimum(var);
  varmin *= (varmin<0.) ? 1.1 : 0.9;

  Double_t varmax = mczip->GetMaximum(var);
  varmax *= (varmax<0.) ? 0.9 : 1.1;

  TH1F varplot(var.Data(),(var+" : Summed over Event").Data(),100,varmin,varmax);

  Double_t lastev = -1;
  Double_t varsum = 0.;

  // Fill histogram with variable summed over each event
  Int_t nhits = mczip->GetEntries();
  for (Int_t i=0; i<nhits; i++) {
    mczip->GetEntry(i);
    if (evnum != lastev) {
      if (i>0) varplot.Fill(varsum, 1.);	// Previous event finished
      varsum = 0.;
    }
    varsum += thevar;
    lastev = evnum;
  }

  if (nhits > 0) varplot.Fill(varsum, 1.);	// Get final event sum

  // Make a nice plot for display
  gStyle->SetLabelSize(0.15,"xyz");

  TCanvas canv;
  varplot.Draw();
  canv.Print(var+"_EventSum.eps");
}
