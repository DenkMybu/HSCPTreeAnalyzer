#ifndef PlotTools_h
#define PlotTools_h


#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TMath.h>

//#include <iostream>
//#include <fstream>


// Function returning an 1D-histogram which contains the ratio between two histograms   
TH1F* ratioHist(TH1F* h1, TH1F* h2);

// Scale the 1D-histogram given to the unit 
void scale(TH1F* h);

// Inverse the scaling ; scale the 1D-histogram to its number of entries
void invScale(TH1F* h);

// Function returning the ratio of right integer (from x to infty) for two 1D-histograms
// This function is used in the Hscp data-driven background estimate to test the mass shape prediction
// The argument to use this type of ratio is that we're in case of cut & count experiment 
TH1F* ratioIntegral(TH1F* h1, TH1F* h2);

void overflowLastBin(TH1F* h);

void overflowLastBin(TH1F* h, const float &x);

TH1F* rebinHisto(TH1F* h);

// Function returning a canvas divide in two windows
// The first one contains the two 1D-histograms, given as arguments, superposed.
// There also is a legend associated to this window where the names are defined as arguments.
// The second window contains the ratio of these 1D-histograms or the ratio of right integers of them. 
// We define which kind of ratio we want with tha 'ratioSimple' boolean.
// The 'name' given corresponds to the name of the canvas 
TCanvas* plotting(TH1F* h1, TH1F* h2, bool ratioSimple=true, std::string name="", std::string leg1="", std::string leg2="", bool rebin=false);

// Function returning a 1D-histogram with variable binning.
// To determine it, we set a minimum value 'min' corresponding to the number of entries wanted in each bins of the distribution.
// If a given bin has a number of entries below than 'min' then this bin is merged with the next one and then there is another test, etc. 
// Finally, as a reference argument, the function gives a vector composed by the binning previously determined 
TH1F* rebinning(TH1F* h1, float min, std::vector<double>& vect);

TH1F* rebinningGraph(TH1F* h1, float min, std::vector<float>& vect1, std::vector<double>& vect2);

// Function doing the eta reweighing between two 2D-histograms as done in the Hscp background estimate method,
// because of the correlation between variables (momentum & transverse momentum). 
// The first given 2D-histogram is weighted in respect to the second one. 
void etaReweighingP(TH2F* eta_p_1, TH2F* eta_p_2);

// Function returning a 2D-histogram with the relative difference of two 1D-histograms 
// given as arguments. 
void mapOfDifferences(TH2F* res, TH2F* h1, TH2F* h2);

#endif
