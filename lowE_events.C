// Plot summed event data for events below 100 keV, for ZIP #6 (G115)
//
// Usage: root -b lowE_events.C("filename", Ecut)
//
// 20170725  Michael Kelsey

#include "TFile.h"
#include "TString.h"

void exitmsg(const TString& msg, Int_t exitcode) {
  cout << "ERROR: " << msg << endl;
  ::exit(exitcode);
}


int lowE_events(TString fname, Double_t Ecut=100e3) {
  TFile file(fname);
  if (!file.IsOpen()) exitmsg("Unable to open file "+fname, 2);

  TTree* mczip6 = 0;
  file.GetObject("/G4SimDir/mczip6", mczip6);
  if (!mczip6) exitmsg("Can't find mczip6 TTree", 1);

  Double_t evnum;	// Reusable buffers for processing events
  Double_t ehit;
  mczip6->SetBranchAddress("EventNum", &evnum);
  mczip6->SetBranchAddress("KE", &ehit);

  // Get variable range for plotting
  Double_t emin = mczip6->GetMinimum("KE");
  emin *= (emin<0.) ? 1.1 : 0.9;

  Double_t emax = mczip6->GetMaximum("KE");
  emax *= (emax<0.) ? 0.9 : 1.1;

  emax = Ecut;

  TH1F eplot("KE","Energy : Summed over Event",100,emin,emax);

  Double_t lastev = -1;
  Double_t esum = 0.;

  // Fill histogram with variable summed over each event
  Int_t nhits = mczip6->GetEntries();
  for (Int_t i=0; i<nhits; i++) {
    mczip6->GetEntry(i);
    if (evnum != lastev) {
      if (i>0 && esum<=Ecut) eplot.Fill(esum, 1.);	// Previous event finished
      esum = 0.;
    }
    esum += ehit;
    lastev = evnum;
  }

  if (nhits > 0) eplot.Fill(esum, 1.);	// Get final event sum

  // Make a nice plot for display
  gStyle->SetLabelSize(0.15,"xyz");

  TCanvas canv;
  eplot.Draw();
  canv.Print("Energy_EventSum.eps");
}
