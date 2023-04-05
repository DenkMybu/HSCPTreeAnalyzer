#ifndef CPlots_h
#define CPlots_h

#include <TH1F.h>
#include <TH2F.h>
#include <TList.h>
#include <TFile.h>
#include <TProfile.h>
#include <map>


/**
 The class CPlots is used to create, store & fill many histograms.
 The current version contains TH1F but should be extended to TH2F and TProfile.
 It use std::map as container in order is add histo with the method AddHisto1D.
 Several labels are provide to indentify:
  - the selection
  - the region (A,B,C,D or "")
  - the Ias quantile (associated with the definition of the regions)
  - more info could be provided such as "all", "highestIas", etc ... to describe what contain the histos
  Orderd usage of methods:
   - SetLabels should be called before adding histos
   - AddHisto before any loop (in initialization)
   - FillHisto within the loop(s)
   - Write after the loop(s). Plots will be stored in a directory with the name of the selection
 **/

class CPlots: public TObject{
    public:
        CPlots(){}

        void SetLabels(std::string selection, std::string region="", std::string IasQuantile="", std::string moreinfo="");
        void AddHisto1D(std::string name, int nbins, float xmin, float xmax, std::string title = "");
        TH1F* GetHisto1D(std::string name);
        bool FillHisto1D(std::string name, float value, float weight = 1);
        bool Write(TFile* ofile);
        bool AddToList(TList* list);	

    private:
        std::string selection_;
        std::string region_;
        std::string IasQuantile_;
        std::string moreinfo_;
        std::map<std::string,TH1F*> mh1D_;

};

#endif
