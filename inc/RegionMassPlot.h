#ifndef RegionMassPlot_h
#define RegionMassPlot_h


#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TMath.h>
#include <iostream>
//#include <fstream>

#include "PlotTools.h"
#include "MassTools.h"

//use
//crossHistosEtaBinning
//scale

/**
 The class RegionMassPlot is used for the background prediciton of the mass shape analysis.
 It fills many plots used for prediction and for control in the 4 regions: A,B,C,D.

 To be done:
  - remove many hard-coded value to be read from a config file
  - cleanin of useless variables & useless plots
  - move data members form public to private if possible
  - remove useless reference to float
**/

class RegionMassPlot{

    public:
        //Constructors & destructor
        //RegionMassPlot();
        RegionMassPlot(std::string suffix,int etabins,int ihbins,int pbins,int massbins);
        //RegionMassPlot(std::string suffix,int nbins, float* xbins, std::vector<double> v_pbins, std::vector<float> vect);
        ~RegionMassPlot();
       
        //Methods
	void initHisto();
        void initHisto(int& etabins,int& ihbins,int& pbins,int& massbins);
        void fill(float eta, float nhits, float p, float pt, float pterr, float ih, float ias, float is, float m, float tof, float npv, float w, float reW);
        void OneOverPreweighting(TH2F* ih_p_1, TH1F* p2);
        void fillMassFrom1DTemplatesEtaBinning(float weight_);
        void plotMass();
        void cross1D();
        void write();
        void addToList(TList* list);	
	//Data members

	std::string suffix_;
       


	//Plots binning
	//
	int nbins;
        float* xbins;
        int np;
        double* xp;
        float plow;
        float pup;
        int npt;
        float ptlow;
        float ptup;
        int nih;
        float ihlow;
        float ihup;
        int nias;
        float iaslow;
        float iasup;
        int neta;
        float etalow;
        float etaup;
        int nmass;
        float masslow;
        float massup;
        //std::vector<float> vect;
        std::vector<double> VectOfBins_P_;
        
        //List of all histos 
	TCanvas* c;
        TH2F* ih_pt;
        TH2F* ias_pt;
        TH2F* ih_ias;
        TH2F* ih_nhits;
        TH2F* ias_nhits;
        TH2F* eta_pt;
        TH2F* eta_p;
        TH2F* nhits_pt;
        TH2F* eta_nhits;
        TH2F* ih_eta;
        TH2F* ih_p;
        //TH3F* ih_p_eta;
        TH2F* ias_p;
        TH2F* pt_pterroverpt;
        TH2F* ias_eta;
        TH2F* mass_eta;
        TH2F* eta_npv;
        TH2F* p_npv;
        TH2F* ih_npv;
        TH2F* is_ias;
        TH2F* is_ih;
        TH2F* mass_p;
        TH2F* mass_ih;
        
	TH1F* mass;
        TH1F* massFrom1DTemplates;
        TH1F* massFrom1DTemplatesEtaBinning;
        TH1F* pred_mass;
        TH2F* eta_p_rebinned;


        //vector histogram
        TH1F* errMass;
        TH2F* Mass_errMass;
        TH2F* cross1Dtemplates;
        TH1F* ih_used;
        TH2F* mapM800;

        TH1F* hTOF;

        TH1F* momentumDistribM1000;
        TH1F* dedxDistribM1000;

};

#endif
