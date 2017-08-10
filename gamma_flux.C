// Plot individual hit data for SuperSim hits, for ZIP #6 (G115)
//
// Usage: root -b gamma_flux.C("filename", Ecut)
//
// 20170725  Michael Kelsey

#include "TFile.h"
#include "TString.h"

void exitmsg(const TString& msg, Int_t exitcode) {
  cout << "ERROR: " << msg << endl;
  ::exit(exitcode);
}


int gamma_flux(TString fname, Double_t Ecut=100e3) {
  TFile file(fname);
  if (!file.IsOpen()) exitmsg("Unable to open file "+fname, 2);

  TString var = "KE";

  TTree* flux = 0;
  file.GetObject("/G4SimDir/mcFluxCounter", flux);
  if (!flux) exitmsg("Can't find mcFluxCounter TTree", 1);

  Double_t evnum;	// Reusable buffers for processing events
  Double_t thevar;
  flux->SetBranchAddress("EventNum", &evnum);
  flux->SetBranchAddress(var, &thevar);

  // Get variable range for plotting
  Double_t varmin = flux->GetMinimum(var);
  varmin *= (varmin<0.) ? 1.1 : 0.9;

  Double_t varmax = Ecut; //*** flux->GetMaximum(var);
  varmax *= (varmax<0.) ? 0.9 : 1.1;

  TH1F varplot(var.Data(),(var+" : Gammas at G115").Data(),100,varmin,varmax);

  // Fill histogram with variable 
  Int_t nhits = flux->GetEntries();
  for (Int_t i=0; i<nhits; i++) {
    flux->GetEntry(i);
    if (thevar <= Ecut) varplot.Fill(thevar, 1.);
  }

  // Make a nice plot for display
  gStyle->SetLabelSize(0.15,"xyz");

  TCanvas canv;
  varplot.Draw();
  canv.Print(var+"_Gammas.eps");
}
