#ifndef MassTools_h
#define MassTools_h


#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TRandom3.h>
#include <TMath.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TCanvas.h>

//#include <fstream>



/**
 To do:
  - values of K & C are hard-coded --> should go in config file
 **/

//extern float K,C,K_data2018,C_data2018,K_data2017,C_data2017;


// Function
TH2F* BetheBlochForMass(float mass);

// Function returning the MassErr as function momentum, dEdx, and errors on momentum and dEdx
// Not take into account any erros coming from K&C factors because this function is used to see the impact of binning in p and dedx on mass error
double GetMassErr (double P, double PErr, double dEdx, double dEdxErr, double M, double dEdxK, double dEdxC);

// Return the mass as a function of momentum, dEdx, K and C. 
// It corresponds to the Bethe-Bloch parametrisation used in the Hscp analysis
float GetMass(float p, float ih, float k, float c);


void crossHistos(TH2F* res, TH1F* h1, TH1F* h2);

// Function doing the crossing between 1D-histograms of dEdx and momentum and returning a 2D-histogram (p,ih),
// and respecting the eta binning as in the mass distribution calculation 
void crossHistosEtaBinning(TH2F* res, TH2F* eta_p, TH2F* ih_eta);

#endif
