#include "CPlots.h"
#include <iostream>
#include <iomanip>
#include <sstream>

/*
CPlots::CPlots(const CPlots &c){
    std::cout <<"hi" <<std::endl;
}
*/


CPlots::~CPlots(){
    /*    
    for(std::map<std::string,TH1F*>::iterator itr = mh1D_.begin(); itr != mh1D_.end(); itr++)
    {
        delete (itr->second);
        //mh1D_.erase(itr);
    }
    */
}

void CPlots::SetLabels(std::string selection, std::string region, std::string IasQuantile, std::string moreinfo){
    selection_ = selection;
    region_ = region;
    IasQuantile_ = IasQuantile;
    moreinfo_ = moreinfo;
}

void CPlots::AddHisto1D(std::string name, int nbins, float xmin, float xmax, std::string title){
   //std::cout<<"add a plots"<<std::endl;
   size_t pos1 = name.find("_h");
   std::string xTitle = "idk";
   if (pos1 != std::string::npos) {
       size_t pos2 = name.find("_", pos1 + 2);
       xTitle = name.substr(pos1 + 2, pos2 - pos1 - 2);              
   }
   mh1D_[name] = new TH1F(name.c_str(),title.c_str(),nbins,xmin,xmax);
   mh1D_[name]->Sumw2();
   double bin_size = (xmax - xmin) / nbins;
   std::stringstream label;
   if (bin_size < 0.01 || bin_size > 1000) {
       label << std::scientific << std::setprecision(2) << bin_size;
   }else{
       label << std::fixed << std::setprecision(2) << bin_size;
   }
   TString y_label = "Normalized tracks / ";
   y_label += label.str();

   mh1D_[name]->GetXaxis()->SetTitle(xTitle.c_str()); 
   mh1D_[name]->GetYaxis()->SetTitle(y_label); 
   //std::cout << "Defined mh1D_["<<name<< "] = TH1F" << std::endl; 
   //mh1D_[name] = new TH1F(name.c_str(),title.c_str(),nbins,xmin,xmax);
   //std::cout<<"done !"<<std::endl;
}


void CPlots::AddHisto2D(std::string name, int nbinsx, float xmin, float xmax,int nbinsy, float ymin, float ymax, std::string title){
   size_t pos1 = name.find("_h");
   if (pos1 != std::string::npos) {
       size_t pos2 = name.find("_", pos1 + 2);
       std::string xTitle = name.substr(pos1 + 2, pos2 - pos1 - 2);
       size_t pos3 = name.rfind("_");
       std::string yTitle = name.substr(pos3 + 1);
       
       mh2D_[name] = new TH2F(name.c_str(),title.c_str(),nbinsx,xmin,xmax,nbinsy,ymin,ymax);
       mh2D_[name]->Sumw2();
       mh2D_[name]->GetXaxis()->SetTitle(xTitle.c_str());
       mh2D_[name]->GetYaxis()->SetTitle(yTitle.c_str());
   } else {
       std::cerr << "Error: Title does not match the expected pattern." << std::endl;
   }
   //std::cout << "Defined mh2D_["<<name<< "] = TH2F" << std::endl; 
}


/*
TH1F CPlots::GetHisto1D(std::string name){
    if(mh1D_.find(name)!=mh1D_.end()) return mh1D_[name];
    
    return nullptr;
}
*/

/*
TH1F* CPlots::GetHisto1D(std::string name){
    if(mh1D_.find(name)!=mh1D_.end()) return &mh1D_[name];
    return nullptr;
}
*/

       
bool CPlots::FillHisto1D(std::string name, float value, float weight){
    if(mh1D_.find(name)!=mh1D_.end()){
       //mh1D_[name].Fill(value,weight);
       mh1D_[name]->Fill(value,weight);
       return true;
    }
    return false;
}

bool CPlots::FillHisto2D(std::string name, float xvalue,float yvalue, float weight){
    if(mh2D_.find(name)!=mh2D_.end()){
       //mh1D_[name].Fill(value,weight);
       mh2D_[name]->Fill(xvalue,yvalue,weight);
       return true;
    }
    return false;
}


bool CPlots::Write(TFile* ofile){
   if(!ofile ||  !ofile->IsOpen()) return false;
   //store plots in the directory labelled with the selection 
   ofile->mkdir(selection_.c_str());
   ofile->cd(selection_.c_str());
   //for(auto h: mh1D_) &h.second.Write();
   for(auto h: mh1D_) h.second->Write();
   for(auto h: mh2D_) h.second->Write();
   return true;
}

bool CPlots::AddToList(TList* list){
   if (!list) return false;
   for(auto h: mh1D_) list->Add(h.second);
   for(auto h: mh2D_) list->Add(h.second);
   //for(auto h: mh1D_) list->Add(&h.second);
   return true;
}

