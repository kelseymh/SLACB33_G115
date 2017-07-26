// Plot individual hit data for SuperSim hits, for ZIP #6 (G115)
//
// Usage: root -b hit_plot.C("filename", "variable")
//
// 20170725  Michael Kelsey

#include "TFile.h"
#include "TString.h"

void exitmsg(const TString& msg, Int_t exitcode) {
  cout << "ERROR: " << msg << endl;
  ::exit(exitcode);
}


int hit_plot(TString fname, TString var) {
  TFile file(fname);
  if (!file.IsOpen()) exitmsg("Unable to open file "+fname, 2);

  TTree* mczip6 = 0;
  file.GetObject("/G4SimDir/mczip6", mczip6);
  if (!mczip6) exitmsg("Can't find mczip6 TTree", 1);

  Double_t evnum;	// Reusable buffers for processing events
  Double_t thevar;
  mczip6->SetBranchAddress("EventNum", &evnum);
  mczip6->SetBranchAddress(var, &thevar);

  // Get variable range for plotting
  Double_t varmin = mczip6->GetMinimum(var);
  varmin *= (varmin<0.) ? 1.1 : 0.9;

  Double_t varmax = mczip6->GetMaximum(var);
  varmax *= (varmax<0.) ? 0.9 : 1.1;

  TH1F varplot(var.Data(),(var+" : Single Hits").Data(),100,varmin,varmax);

  Double_t lastev = -1;
  Double_t varsum = 0.;

  // Fill histogram with variable 
  Int_t nhits = mczip6->GetEntries();
  for (Int_t i=0; i<nhits; i++) {
    mczip6->GetEntry(i);
    varplot.Fill(thevar, 1.);
  }

  // Make a nice plot for display
  gStyle->SetLabelSize(0.15,"xyz");

  TCanvas canv;
  varplot.Draw();
  canv.Print(var+"_Hits.eps");
}
