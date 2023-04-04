#include "../inc/CPlots.h"


void CPlots::SetLabels(std::string selection, std::string region, std::string IasQuantile, std::string moreinfo){
    selection_ = selection;
    region_ = region;
    IasQuantile_ = IasQuantile;
    moreinfo_ = moreinfo;
}

void CPlots::AddHisto1D(std::string name, int nbins, float xmin, float xmax, std::string title){
   mh1D_[name] = new TH1F(name.c_str(),title.c_str(),nbins,xmin,xmax);
}

TH1F* CPlots::GetHisto1D(std::string name){
    if(mh1D_.find(name)!=mh1D_.end()) return mh1D_[name];
    return nullptr;
}

bool CPlots::Write(TFile* ofile){
   if(!ofile ||  !ofile->IsOpen()) return false;
   //store plots in the directory labelled with the selection 
   ofile->mkdir(selection_.c_str());
   ofile->cd(selection_.c_str());
   for(auto h: mh1D_) h.second->Write();
   return true;
}
