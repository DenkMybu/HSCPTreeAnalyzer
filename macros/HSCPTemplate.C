#define HSCPSelector_cxx
// The class definition in HSCPSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("HSCPSelector.C")
// root> T->Process("HSCPSelector.C","some options")
// root> T->Process("HSCPSelector.C+")
//

//K and C values are set based on the config file dataset name (see below)
float K(2.54), C(3.14); 


float K_signal2018(2.49), C_signal2018(3.18); //MC signal 2018
float K_signal2017(2.48), C_signal2017(3.19); //MC signal 2017

float K_bckg2017(2.48), C_bckg2017(3.19); //MC background 2017
float K_bckg2018(2.49), C_bckg2018(3.18); //MC background 2018

float K_data2018(2.55), C_data2018(3.14); //Data 2018
float K_data2017(2.54), C_data2017(3.14); //Data 2017


#include "HSCPSelector.h"
#include <TH2.h>
#include <TStyle.h>




//ADD-SELECTION-METHODS
bool HSCPSelector::PassPreselection(int hscpIndex){
  if (hscpIndex<0 || hscpIndex>(int)Pt.GetSize()) return false;
  return passPreselection->at(hscpIndex);
}

bool HSCPSelector::PassPreselectionSept8(int hscpIndex){
  if (hscpIndex<0 || hscpIndex>(int)Pt.GetSize()) return false;
  return passPreselectionSept8[hscpIndex];
}




void HSCPSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
   TString option = GetOption();

   TObjArray *tx = option.Tokenize(",");
   std::string ext = "_Fpix.root";


  
   std::string ptname( ((TObjString *)(tx->At(0)))->String().Data());
   int ptInt = std::stoi(ptname);

   
   oFile_ = ((TObjString *)(tx->At(7)))->String().Data();
   oFile_ += "_massCut_";
   oFile_ += ((TObjString *)(tx->At(5)))->String().Data();
   oFile_ += "_pT";
   oFile_ += to_string(ptInt);
   oFile_ += "_";
   oFile_ += ((TObjString *)(tx->At(8)))->String().Data();
   oFile_ += ext;
   

   //oFile_ = "SingleMu2017B_highmass_preselDylan.root";
   //Add selections into a vector - to be updated
   //FILL-SELECTION-VECTOR
   selections_.push_back(&HSCPSelector::PassPreselection);
   selections_.push_back(&HSCPSelector::PassPreselectionSept8);
   selLabels_.push_back("PassPreselection");
   selLabels_.push_back("PassPreselectionSept8");

   //https://root-forum.cern.ch/t/proof-session-tselector-how-to-pass-object-to-slaves/25502
   std::cout << "Properly terminated function Begin " <<std::endl;
   std::cout << " Sel Label size : " << selLabels_.size() <<std::endl;
   


}



void HSCPSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).



   //CALL METHOD FOR BINNING

   TString option = GetOption();
   //GetOption() func retrieves the options sent in the macro.cc file when calling chain->Process();


   TObjArray *tx = option.Tokenize(",");
   //Tokenize splits the initial TString by the delimiter "," and returns an array of strings 
   //EXAMPLE : TString m = "14,8,3,17" | m.Tokenize(",") |  tx->Print() will display 14
   //								                     8
   //								                     3
   //								                     17

   //Printing the content of option

   //Here we read every TString and transform them into strings
   std::string ptTmp( ((TObjString *)(tx->At(0)))->String().Data());
   std::string etaTmp( ((TObjString *)(tx->At(1)))->String().Data());
   std::string ihTmp( ((TObjString *)(tx->At(2)))->String().Data());
   std::string pTmp( ((TObjString *)(tx->At(3)))->String().Data());
   std::string massTmp( ((TObjString *)(tx->At(4)))->String().Data());
   std::string massCutTmp( ((TObjString *)(tx->At(5)))->String().Data());

   std::string tofCutTmp( ((TObjString *)(tx->At(6)))->String().Data());
   std::string dataset( ((TObjString *)(tx->At(7)))->String().Data());
   std::string version( ((TObjString *)(tx->At(8)))->String().Data());



   Usefpixel=true;
   BothMethods = false;

   //Finally, transform strings into int for cuts/binning, and make them correspond to header defined variables (binning and pt cut)
   ptcut_ = std::stod(ptTmp);
   etabins_ = std::stoi(etaTmp);
   ihbins_ = std::stoi(ihTmp); 
   pbins_ = std::stoi(pTmp);
   massbins_ = std::stoi(massTmp);
   masscut_ = std::stoi(massCutTmp);
   tofbins_ = std::stoi(tofCutTmp);
   dataset_ = dataset;
   version_ = version; 
   //create all objects that will be used by the methods Process
    
   std::string firstThreeChars = dataset_.substr(0, 3);

   if(dataset_ == "highmass2017B"){
       K = K_data2017;
       C = C_data2017;
   }

   if(dataset_ == "Mu2017" || dataset_ == "Ele2017"){ 
       K = K_data2017;
       C = C_data2017;
   }
   if(dataset_ == "Mu2018"){ 
       K = K_data2018;
       C = C_data2018;
   }
   if(firstThreeChars == "Glu" || firstThreeChars == "pSt"){ 
       K = K_signal2018;
       C = C_signal2018;
   }
   if(dataset_ == "qcd170_300" || dataset_ == "wjets_0j"){
       K = K_bckg2017;
       C = C_bckg2017;
   }

   std::cout << "Dataset is : " << dataset_ << " , C = "<< C << " and K = " << K << std::endl;
   std::cout << " Starting SLAVE ---------PT  cut for regions = " << ptcut_ << " , Eta #bin : "<< etabins_ << ", Ih #bins" << ihbins_ << " , P #bins " << pbins_ << " , mass #bins " << massbins_ << " TOF #bins " << tofbins_ << ", mass cut : " << masscut_ << " , sel labels size : " << selLabels_.size() << std::endl;

   
   flagP = true;
   flagIH = true;
   reweightChoice = 0;//0 no reweighting, 1 : 1/p reweighting, 2 : IH reweighting, 3 : Eta reweighting, 4 : 2D eta/p reweighting
   

   //-------------------------------------
   //Add selections into a vector - to be updated
   //FILL-SELECTION-VECTOR
   //-------------------------------------
   selections_.push_back(&HSCPSelector::PassPreselection);
   selections_.push_back(&HSCPSelector::PassPreselectionSept8);
   selLabels_.push_back("PassPreselection");
   selLabels_.push_back("PassPreselectionSept8");
   //Change here the naming of the hists to be compatible with old way 
   
   //-------------------------------------
   //create RegionMassPlot for all selection
   //-------------------------------------
   for(unsigned int i=0;i<selLabels_.size();i++){
      //The binning has to be changed and to become configurable
      
      //std::cout << "Running over sel " <<i << " with name " << selLabels_[i] <<std::endl;
     
      if(Usefpixel){

          std::string label_FpixAll = regFpixAll + "_" + selLabels_[i]; 
    
          std::string label_FpixA_3f6 = regFpixA_3f6 + "_" + selLabels_[i]; 
          std::string label_FpixA_6f9 = regFpixA_6f9 + "_" + selLabels_[i]; 
          std::string label_FpixA_3f8 = regFpixA_3f8 + "_" + selLabels_[i]; 
          std::string label_FpixA_3f9 = regFpixA_3f9 + "_" + selLabels_[i]; 


          std::string label_FpixA_3f4 = regFpixA_3f4 + "_" + selLabels_[i]; 
          std::string label_FpixA_4f5 = regFpixA_4f5 + "_" + selLabels_[i]; 
          std::string label_FpixA_5f6 = regFpixA_5f6 + "_" + selLabels_[i]; 
          std::string label_FpixA_6f7 = regFpixA_6f7 + "_" + selLabels_[i]; 
          std::string label_FpixA_7f8 = regFpixA_7f8 + "_" + selLabels_[i]; 
          std::string label_FpixA_8f9 = regFpixA_8f9 + "_" + selLabels_[i]; 
          std::string label_FpixA_9f10 = regFpixA_9f10 + "_" + selLabels_[i];
          std::string label_FpixA_99f10 = regFpixA_99f10 + "_" + selLabels_[i];
          std::string label_FpixA_999f10 = regFpixA_999f10 + "_" + selLabels_[i];
   
 
          std::string label_FpixC_3f8 = regFpixC_3f8 + "_" + selLabels_[i]; 
          std::string label_FpixC_3f9 = regFpixC_3f9 + "_" + selLabels_[i]; 

          std::string label_FpixC_3f4 = regFpixC_3f4 + "_" + selLabels_[i]; 
          std::string label_FpixC_4f5 = regFpixC_4f5 + "_" + selLabels_[i]; 
          std::string label_FpixC_5f6 = regFpixC_5f6 + "_" + selLabels_[i]; 
          std::string label_FpixC_6f7 = regFpixC_6f7 + "_" + selLabels_[i]; 
          std::string label_FpixC_7f8 = regFpixC_7f8 + "_" + selLabels_[i]; 
          std::string label_FpixC_8f9 = regFpixC_8f9 + "_" + selLabels_[i]; 

          std::string label_FpixC_3f6 = regFpixC_3f6 + "_" + selLabels_[i]; 
          std::string label_FpixC_6f9 = regFpixC_6f9 + "_" + selLabels_[i]; 
    
          std::string label_FpixD_6f9 = regFpixD_6f9 + "_" + selLabels_[i];

          std::string label_FpixD_3f4 = regFpixD_3f4 + "_" + selLabels_[i]; 
          std::string label_FpixD_4f5 = regFpixD_4f5 + "_" + selLabels_[i]; 
          std::string label_FpixD_5f6 = regFpixD_5f6 + "_" + selLabels_[i]; 
          std::string label_FpixD_6f7 = regFpixD_6f7 + "_" + selLabels_[i]; 
          std::string label_FpixD_7f8 = regFpixD_7f8 + "_" + selLabels_[i]; 
          std::string label_FpixD_8f9 = regFpixD_8f9 + "_" + selLabels_[i]; 

          std::string label_FpixD_8f10 = regFpixD_8f10 + "_" + selLabels_[i];

          std::string label_FpixD_9f10 = regFpixD_9f10 + "_" + selLabels_[i]; 
          std::string label_FpixD_99f10 = regFpixD_99f10 + "_" + selLabels_[i]; 
          std::string label_FpixD_999f10 = regFpixD_999f10 + "_" + selLabels_[i]; 

    
          std::string label_FpixB_3f6 = regFpixB_3f6 + "_" + selLabels_[i]; 
          std::string label_FpixB_6f9 = regFpixB_6f9 + "_" + selLabels_[i]; 
          std::string label_FpixB_3f8 = regFpixB_3f8 + "_" + selLabels_[i]; 
          std::string label_FpixB_3f9 = regFpixB_3f9 + "_" + selLabels_[i]; 
          std::string label_FpixB_3f4 = regFpixB_3f4 + "_" + selLabels_[i]; 
          std::string label_FpixB_4f5 = regFpixB_4f5 + "_" + selLabels_[i]; 
          std::string label_FpixB_5f6 = regFpixB_5f6 + "_" + selLabels_[i]; 
          std::string label_FpixB_6f7 = regFpixB_6f7 + "_" + selLabels_[i]; 
          std::string label_FpixB_7f8 = regFpixB_7f8 + "_" + selLabels_[i]; 
          std::string label_FpixB_8f9 = regFpixB_8f9 + "_" + selLabels_[i]; 

          std::string label_FpixB_8f10 = regFpixB_8f10 + "_" + selLabels_[i];
          std::string label_FpixB_9f10 = regFpixB_9f10 + "_" + selLabels_[i];
          std::string label_FpixB_99f10 = regFpixB_99f10 + "_" + selLabels_[i];
          std::string label_FpixB_999f10 = regFpixB_999f10 + "_" + selLabels_[i];
    
    
          RegionMassPlot regAll(label_FpixAll.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 

          RegionMassPlot regA_3f6(label_FpixA_3f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regA_6f9(label_FpixA_6f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
          RegionMassPlot regA_3f8(label_FpixA_3f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regA_3f9(label_FpixA_3f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 


          RegionMassPlot regA_3f4(label_FpixA_3f4.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regA_4f5(label_FpixA_4f5.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regA_5f6(label_FpixA_5f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regA_6f7(label_FpixA_6f7.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regA_7f8(label_FpixA_7f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
          RegionMassPlot regA_8f9(label_FpixA_8f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regA_9f10(label_FpixA_9f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regA_99f10(label_FpixA_99f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regA_999f10(label_FpixA_999f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 

    
          RegionMassPlot regC_3f8(label_FpixC_3f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regC_3f9(label_FpixC_3f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
    
          RegionMassPlot regC_3f4(label_FpixC_3f4.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regC_4f5(label_FpixC_4f5.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regC_5f6(label_FpixC_5f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regC_6f7(label_FpixC_6f7.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regC_7f8(label_FpixC_7f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regC_8f9(label_FpixC_8f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 

          RegionMassPlot regC_3f6(label_FpixC_3f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regC_6f9(label_FpixC_6f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
    
          RegionMassPlot regD_6f9(label_FpixD_6f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 

          RegionMassPlot regD_3f4(label_FpixD_3f4.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regD_4f5(label_FpixD_4f5.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regD_5f6(label_FpixD_5f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regD_6f7(label_FpixD_6f7.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regD_7f8(label_FpixD_7f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);     
          RegionMassPlot regD_8f9(label_FpixD_8f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 

          RegionMassPlot regD_8f10(label_FpixD_8f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regD_9f10(label_FpixD_9f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regD_99f10(label_FpixD_99f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regD_999f10(label_FpixD_999f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
    
          RegionMassPlot regB_3f6(label_FpixB_3f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regB_6f9(label_FpixB_6f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regB_3f8(label_FpixB_3f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regB_3f9(label_FpixB_3f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regB_3f4(label_FpixB_3f4.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regB_4f5(label_FpixB_4f5.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regB_5f6(label_FpixB_5f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regB_6f7(label_FpixB_6f7.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regB_7f8(label_FpixB_7f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);     
          RegionMassPlot regB_8f9(label_FpixB_8f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 

          RegionMassPlot regB_8f10(label_FpixB_8f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regB_9f10(label_FpixB_9f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regB_99f10(label_FpixB_99f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
          RegionMassPlot regB_999f10(label_FpixB_999f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_); 
    
          vmrp_regionFpix_all.push_back(std::move(regAll));


          vmrp_regionA_3f6.push_back(std::move(regA_3f6));
          vmrp_regionA_6f9.push_back(std::move(regA_6f9));
          vmrp_regionA_3f8.push_back(std::move(regA_3f8));
          vmrp_regionA_3f9.push_back(std::move(regA_3f9));
    
    
          vmrp_regionA_3f4.push_back(std::move(regA_3f4));
          vmrp_regionA_4f5.push_back(std::move(regA_4f5));
          vmrp_regionA_5f6.push_back(std::move(regA_5f6));
          vmrp_regionA_6f7.push_back(std::move(regA_6f7));
          vmrp_regionA_7f8.push_back(std::move(regA_7f8));
          vmrp_regionA_8f9.push_back(std::move(regA_8f9));
          vmrp_regionA_9f10.push_back(std::move(regA_9f10));
          vmrp_regionA_99f10.push_back(std::move(regA_99f10));
          vmrp_regionA_999f10.push_back(std::move(regA_999f10));
    
          vmrp_regionB_3f6.push_back(std::move(regB_3f6));
          vmrp_regionB_6f9.push_back(std::move(regB_6f9));

          vmrp_regionB_3f8.push_back(std::move(regB_3f8));
          vmrp_regionB_3f9.push_back(std::move(regB_3f9));
          vmrp_regionB_3f4.push_back(std::move(regB_3f4));
          vmrp_regionB_4f5.push_back(std::move(regB_4f5));
          vmrp_regionB_5f6.push_back(std::move(regB_5f6));
          vmrp_regionB_6f7.push_back(std::move(regB_6f7));
          vmrp_regionB_7f8.push_back(std::move(regB_7f8));
          vmrp_regionB_8f9.push_back(std::move(regB_8f9));

          vmrp_regionB_8f10.push_back(std::move(regB_8f10));
          vmrp_regionB_9f10.push_back(std::move(regB_9f10));
          vmrp_regionB_99f10.push_back(std::move(regB_99f10));
          vmrp_regionB_999f10.push_back(std::move(regB_999f10));
    

          vmrp_regionD_6f9.push_back(std::move(regD_6f9));


          vmrp_regionD_3f4.push_back(std::move(regD_3f4));
          vmrp_regionD_4f5.push_back(std::move(regD_4f5));
          vmrp_regionD_5f6.push_back(std::move(regD_5f6));
          vmrp_regionD_6f7.push_back(std::move(regD_6f7));
          vmrp_regionD_7f8.push_back(std::move(regD_7f8));
          vmrp_regionD_8f9.push_back(std::move(regD_8f9));

          vmrp_regionD_8f10.push_back(std::move(regD_8f10));
          vmrp_regionD_9f10.push_back(std::move(regD_9f10));
          vmrp_regionD_99f10.push_back(std::move(regD_99f10));
          vmrp_regionD_999f10.push_back(std::move(regD_999f10));
    
          vmrp_regionC_3f8.push_back(std::move(regC_3f8));
          vmrp_regionC_3f9.push_back(std::move(regC_3f9));
    
          vmrp_regionC_3f4.push_back(std::move(regC_3f4));
          vmrp_regionC_4f5.push_back(std::move(regC_4f5));
          vmrp_regionC_5f6.push_back(std::move(regC_5f6));
          vmrp_regionC_6f7.push_back(std::move(regC_6f7));
          vmrp_regionC_7f8.push_back(std::move(regC_7f8));
          vmrp_regionC_8f9.push_back(std::move(regC_8f9));

          vmrp_regionC_3f6.push_back(std::move(regC_3f6));
          vmrp_regionC_6f9.push_back(std::move(regC_6f9));

      }

      /*
      std::string label_all = rAll + "_" + selLabels_[i]; 
      RegionMassPlot reg(label_all.c_str(),etabins_,ihbins_,pbins_,massbins_);
      */
    
      //Definition of regions D with all quantiles and different names
      //
      if(BothMethods){
          std::string labelD_50ias60 = rD_50 + "_" + selLabels_[i];
          RegionMassPlot regD_50ias60(labelD_50ias60.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelD_60ias70 = rD_60 + "_" + selLabels_[i];
          RegionMassPlot regD_60ias70(labelD_60ias70.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelD_70ias80 = rD_70 + "_" + selLabels_[i];
          RegionMassPlot regD_70ias80(labelD_70ias80.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelD_80ias90 = rD_80 + "_" + selLabels_[i];
          RegionMassPlot regD_80ias90(labelD_80ias90.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelD_50ias90 = rD_50_90 + "_" + selLabels_[i];
          RegionMassPlot regD_50ias90(labelD_50ias90.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelD_50ias99 = rD_50_99 + "_" + selLabels_[i];
          RegionMassPlot regD_50ias99(labelD_50ias99.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelD_50ias999 = rD_50_999 + "_" + selLabels_[i];
          RegionMassPlot regD_50ias999(labelD_50ias999.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelD_90ias100 = rD_90_100 + "_" + selLabels_[i];
          RegionMassPlot regD_90ias100(labelD_90ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelD_99ias100 = rD_99_100 + "_" + selLabels_[i];
          RegionMassPlot regD_99ias100(labelD_99ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelD_999ias100 = rD_999_100 + "_" + selLabels_[i];
          RegionMassPlot regD_999ias100(labelD_999ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          //Definition of regions A with all quantiles and different names
          std::string labelA_50ias = rA_med + "_" + selLabels_[i];
          RegionMassPlot regA_ias50(labelA_50ias.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
          std::string labelA_80ias = rA_80 + "_" + selLabels_[i];
          RegionMassPlot regA_ias80(labelA_80ias.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
          std::string labelA_90ias = rA_90 + "_" + selLabels_[i];
          RegionMassPlot regA_ias90(labelA_90ias.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
    
          //Definition of regions C with all quantiles and different names
          std::string labelC_50ias = rC_med + "_" + selLabels_[i];
          RegionMassPlot regC_ias50(labelC_50ias.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
          std::string labelC_80ias = rC_80 + "_" + selLabels_[i];
          RegionMassPlot regC_ias80(labelC_80ias.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
          std::string labelC_90ias = rC_90 + "_" + selLabels_[i];
          RegionMassPlot regC_ias90(labelC_90ias.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
    
          //Definition of regions B with all quantiles and different names
          std::string labelB_50ias60 = rB_50 + "_" + selLabels_[i];
          RegionMassPlot regB_50ias60(labelB_50ias60.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelB_60ias70 = rB_60 + "_" + selLabels_[i];
          RegionMassPlot regB_60ias70(labelB_60ias70.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelB_70ias80 = rB_70 + "_" + selLabels_[i];
          RegionMassPlot regB_70ias80(labelB_70ias80.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelB_80ias90 = rB_80 + "_" + selLabels_[i];
          RegionMassPlot regB_80ias90(labelB_80ias90.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelB_50ias90 = rB_50_90 + "_" + selLabels_[i];
          RegionMassPlot regB_50ias90(labelB_50ias90.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelB_50ias100 = rB_50_100 + "_" + selLabels_[i];
          RegionMassPlot regB_50ias100(labelB_50ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelB_50ias99 = rB_50_99 + "_" + selLabels_[i];
          RegionMassPlot regB_50ias99(labelB_50ias99.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelB_50ias999 = rB_50_999 + "_" + selLabels_[i];
          RegionMassPlot regB_50ias999(labelB_50ias999.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelB_90ias100 = rB_90_100 + "_" + selLabels_[i];
          RegionMassPlot regB_90ias100(labelB_90ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
    
          std::string labelB_99ias100 = rB_99_100 + "_" + selLabels_[i];
          RegionMassPlot regB_99ias100(labelB_99ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);
          
          std::string labelB_999ias100 = rB_999_100 + "_" + selLabels_[i];
          RegionMassPlot regB_999ias100(labelB_999ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_);


          vmrp_regionD_50ias60.push_back(std::move(regD_50ias60));
          vmrp_regionD_60ias70.push_back(std::move(regD_60ias70));
          vmrp_regionD_70ias80.push_back(std::move(regD_70ias80));
          vmrp_regionD_80ias90.push_back(std::move(regD_80ias90));
          vmrp_regionD_50ias90.push_back(std::move(regD_50ias90));
          vmrp_regionD_50ias99.push_back(std::move(regD_50ias99));
          vmrp_regionD_50ias999.push_back(std::move(regD_50ias999));
    
          vmrp_regionD_90ias100.push_back(std::move(regD_90ias100));
          vmrp_regionD_99ias100.push_back(std::move(regD_99ias100));
          vmrp_regionD_999ias100.push_back(std::move(regD_999ias100));
    
    
          vmrp_regionA_ias50.push_back(std::move(regA_ias50));
          vmrp_regionA_ias80.push_back(std::move(regA_ias80));
          vmrp_regionA_ias90.push_back(std::move(regA_ias90));
    
          vmrp_regionC_ias50.push_back(std::move(regC_ias50));
          vmrp_regionC_ias80.push_back(std::move(regC_ias80));
          vmrp_regionC_ias90.push_back(std::move(regC_ias90));
    
          vmrp_regionB_50ias60.push_back(std::move(regB_50ias60));
          vmrp_regionB_60ias70.push_back(std::move(regB_60ias70));
          vmrp_regionB_70ias80.push_back(std::move(regB_70ias80));
          vmrp_regionB_80ias90.push_back(std::move(regB_80ias90));
          vmrp_regionB_50ias90.push_back(std::move(regB_50ias90));
          vmrp_regionB_50ias100.push_back(std::move(regB_50ias100));
          vmrp_regionB_50ias99.push_back(std::move(regB_50ias99));
          vmrp_regionB_50ias999.push_back(std::move(regB_50ias999));
          
          vmrp_regionB_90ias100.push_back(std::move(regB_90ias100));
          vmrp_regionB_99ias100.push_back(std::move(regB_99ias100));
          vmrp_regionB_999ias100.push_back(std::move(regB_999ias100));

      }


      //vmrp_region_all.push_back(std::move(reg));


      /*    
      CPlots plots;
      std::cout << "Defined CPlots plots" << std::endl;
      //Need to add plots here
      //ADD-CPLOTS-HERE
      
      plots.AddHisto1D(selLabels_[i]+"_hpt",10,0,200);
      std::cout << "After plots.Addhisto1D" << std::endl;
      vcp.push_back(std::move(plots));
      //vcp.push_back(plots);
      std::cout << "Pushed plots to vcp vector" << std::endl;
      */
   }
   std::cout << "Got ouf of loop on selections_" <<std::endl;
   //fOutput->Add(fout);
}

Bool_t HSCPSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
   //std::cout << "Entering Process" << std::endl; 
   
   fReader.SetLocalEntry(entry);
   //----------------------------------
   //Loop over all HSCP candidates
   //----------------------------------
   
   //Use the size of any array such as Pt
   
   //Store the index (in iCand) of the most ionizing candidate (maxIh) 
   //passing the preselection
   //int iCand = -1; // index on the selected candidate per event


   vector<int> iCand(selLabels_.size(),-1);
   vector<float> maxIh(selLabels_.size(),-1);
   
   //std::cout << "Before loop on all HSCP, selections size =" << selections_.size() << " and sel label size : " << selLabels_.size() << std::endl; 
   



   for(unsigned int i=0;i<Pt.GetSize();i++){
      //Run over all possible  HSCP preselection
      for(int s=0;s<selections_.size();s++){
        
         //Use a pointer on methods 
         bool (HSCPSelector::*ptr)(int);
	 ptr = selections_[s];
	 if((this->*ptr)(i)){
	   //std::cout << "Passed preselection " << s << " named " << selLabels_[s] << std::endl; 
	   //Fill the histos at preselection level for all HSCP candidates
           //FILL-CPLOTS-HERE
	   //vcp[s].FillHisto1D(selLabels_[s]+"_hpt",Pt[i]);
	   
	   //vmrp[s].fill(eta[i],NOM[i],1./(Pt[i]*cosh(eta[i])),Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),1); 
	    
	    //search the most ionizing candidate
	    if(Ih_StripOnly[i]>maxIh[s]){
                maxIh[s]=Ih_StripOnly[i];
                iCand[s]=i;
            }
         }
      }    
   }
   //std::cout << "After defining moost ionizing HSCP" << std::endl; 
   //End of loop over all HSCP candidates

   // Plots for the most ionizing candidate per event passing the preselection
   for(int s=0;s<selections_.size();s++){
        int i=iCand[s];
        double Ias = Ias_StripOnly[i];
	if(i<0) continue;
	//vmrp[s].fill(eta[i],NOM[i],1./(Pt[i]*cosh(eta[i])),Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),1); 
	
        float P = 10000./(Pt[i]*cosh(eta[i]));

        //float beta = gParticleBeta[i];

        float Ih = Ih_StripOnly[i];

        float Fpix = float(1.0 - ProbQ_noL1[i]);

        float weight_p = 1.0; 
        float weight_ih = 1.0;
        float newWeight = 1.0;          
        int which_bin=-1;
        float Eta = eta[i];
        float weight_eta = 1.0;
        float weight2D = 1.0;
        bool stopLoop = false;

        float mom = p[i];
        float Ihnol1 = Ih_noL1[i];
        float IhStrip = Ih_StripOnly[i];
        float pt = Pt[i];
        float ptErr = PtErr[i];
        float sigptopt = ptErr*1.0/pt;
        float sigptopt2 = ptErr*1.0/(pt*pt);
        float eop = EoverP[i];

        float relTKiso = track_genTrackMiniIsoSumPt[i];
        float miniIso = PFMiniIso_relative[i];
        float mass = GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C);
        float massIhnoL1 = GetMass(Pt[i]*cosh(eta[i]),Ih_noL1[i],K,C);
        bool matchedmu = *matchedMuonWasFound.Get();
        bool HLTmu = *HLT_Mu50.Get();

        unsigned long long EVENT = *Event;
        unsigned int RUN = *Run;
        unsigned int PU = *PileUp;
        unsigned int LUMI = *Lumi;




        /*
        if (massIhnoL1 > 4000 && selLabels_[s] == "testIhPt"){
           
           cout << "Event with mass(Ih_noL1) = " << massIhnoL1 << " , mass(Ih_StripOnly)  : " << mass << " found with sel label : " << selLabels_[s] << endl;
           cout << "It was found in Event : " << EVENT << " from run #" << RUN << " , in LS : " << LUMI <<" with associated PU : " << PU << endl;
           cout << "Matched muon : " << matchedmu << endl;
           cout << "HLT_Mu50 : " << HLTmu << endl;
           cout << "Pt = " << pt <<endl;
           cout << "P = " << mom << endl;
           cout << "Eta = " << Eta << endl; 
           cout << "sigpt/pt = " << sigptopt << endl;
           cout << "Ih strip only = " << Ih << endl;
           cout << "Ih no L1 = " << Ihnol1 << endl;
       
           //cout << "Fixed Iso Tk : "<< fixIso << endl;
           cout << "Var code size Iso Tk : "<< relTKiso << endl;
           cout << "MiniRelIsoPF : "<< miniIso << endl;
           cout << "Fpix : "<< Fpix << endl;
           cout << "E/P : "<< eop << endl;
           cout << "sigpt/pt^2 : "<< sigptopt2 << endl;
           cout << "Ias strip only : " << Ias << endl;
        }
        */
        
        newWeight = 1.0;
        
        if(Usefpixel){           
            vmrp_regionFpix_all[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);

           if(Pt[i] <= ptcut_){
               
               if( (Fpix > fpix3) && (Fpix <= fpix4) ) vmrp_regionA_3f4[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix4) && (Fpix <= fpix5) ) vmrp_regionA_4f5[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix5) && (Fpix <= fpix6) ) vmrp_regionA_5f6[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix7) ) vmrp_regionA_6f7[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix7) && (Fpix <= fpix8) ) vmrp_regionA_7f8[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix8) && (Fpix <= fpix9) ) vmrp_regionA_8f9[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);

               if( (Fpix > fpix9) && (Fpix <= fpix10) ) vmrp_regionA_9f10[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix99) && (Fpix <= fpix10) ) vmrp_regionA_99f10[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix999) && (Fpix <= fpix10) ) vmrp_regionA_999f10[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);



               if( (Fpix > fpix3) && (Fpix <= fpix6) ) vmrp_regionA_3f6[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix9) ) vmrp_regionA_6f9[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix8) ) vmrp_regionA_3f8[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix9) ) vmrp_regionA_3f9[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);


               if( (Fpix > fpix3) && (Fpix <= fpix6) ) vmrp_regionB_3f6[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix9) ) vmrp_regionB_6f9[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix8) ) vmrp_regionB_3f8[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix9) ) vmrp_regionB_3f9[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix4) ) vmrp_regionB_3f4[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix4) && (Fpix <= fpix5) ) vmrp_regionB_4f5[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix5) && (Fpix <= fpix6) ) vmrp_regionB_5f6[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix7) ) vmrp_regionB_6f7[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix7) && (Fpix <= fpix8) ) vmrp_regionB_7f8[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix8) && (Fpix <= fpix9) ) vmrp_regionB_8f9[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);

               if( (Fpix > fpix8) && (Fpix <= fpix10) ) vmrp_regionB_8f10[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix9) && (Fpix <= fpix10) ) vmrp_regionB_9f10[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix99) && (Fpix <= fpix10) ) vmrp_regionB_99f10[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix999) && (Fpix <= fpix10) ) vmrp_regionB_999f10[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               
           }
           else{  

               if( (Fpix > fpix6) && (Fpix <= fpix9) ) vmrp_regionD_6f9[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               

               if( (Fpix > fpix3) && (Fpix <= fpix4) ) vmrp_regionD_3f4[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix4) && (Fpix <= fpix5) ) vmrp_regionD_4f5[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix5) && (Fpix <= fpix6) ) vmrp_regionD_5f6[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix7) ) vmrp_regionD_6f7[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix7) && (Fpix <= fpix8) ) vmrp_regionD_7f8[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix8) && (Fpix <= fpix9) ) vmrp_regionD_8f9[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);


               if( (Fpix > fpix8) && (Fpix <= fpix10) ) vmrp_regionD_8f10[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix9) && (Fpix <= fpix10) ) vmrp_regionD_9f10[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix99) && (Fpix <= fpix10) ) vmrp_regionD_99f10[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix999) && (Fpix <= fpix10) ) vmrp_regionD_999f10[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);

               if( (Fpix > fpix3) && (Fpix <= fpix8) ){
                   vmrp_regionC_3f8[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);

                   double massBKG = GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C);
            
                   if(massBKG >=mwGlu800[0] && massBKG <= mwGlu800[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[0] += 1;
                   }
                   if(massBKG>=mwGlu1000[0] && massBKG <= mwGlu1000[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[1]+=1;
                   }
                   if(massBKG>=mwGlu1400[0] && massBKG <= mwGlu1400[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[2]+=1;
                   }
                   if(massBKG>=mwGlu1600[0] && massBKG <= mwGlu1600[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[3]+=1;
                   }
                   if(massBKG>=mwGlu1800[0] && massBKG <= mwGlu1800[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[4]+=1;
                   }
                   if(massBKG>=mwGlu2000[0] && massBKG <= mwGlu2000[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[5]+=1;
                   }
                   if(massBKG>=mwGlu2200[0] && massBKG <= mwGlu2200[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[6]+=1;
                   }
                   if(massBKG>=mwGlu2400[0] && massBKG <= mwGlu2400[1]){
                       if(selLabels_[s] == "testIhPt")  nbSig[7]+=1;
                   }
                   if(massBKG>=mwGlu2600[0] && massBKG <= mwGlu2600[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[8]+=1;
                   }

                   if(massBKG>=mwStau308[0] && massBKG <= mwStau308[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[9]+=1;
                   }
                   if(massBKG>=mwStau432[0] && massBKG <= mwStau432[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[10]+=1;
                   }
                   if(massBKG>=mwStau557[0] && massBKG <= mwStau557[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[11]+=1;
                   }
                   if(massBKG>=mwStau651[0] && massBKG <= mwStau651[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[12]+=1;
                   }
                   if(massBKG>=mwStau745[0] && massBKG <= mwStau745[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[13]+=1;
                   }
                   if(massBKG>=mwStau871[0] && massBKG <= mwStau871[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[14]+=1;
                   }
                   if(massBKG>=mwStau1029[0] && massBKG <= mwStau1029[1]){
                       if(selLabels_[s] == "testIhPt") nbSig[15]+=1;
                   }

               }
               if( (Fpix > fpix3) && (Fpix <= fpix9) ) vmrp_regionC_3f9[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);

               if( (Fpix > fpix3) && (Fpix <= fpix4) ) vmrp_regionC_3f4[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix4) && (Fpix <= fpix5) ) vmrp_regionC_4f5[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix5) && (Fpix <= fpix6) ) vmrp_regionC_5f6[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix7) ) vmrp_regionC_6f7[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix7) && (Fpix <= fpix8) ) vmrp_regionC_7f8[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix8) && (Fpix <= fpix9) ) vmrp_regionC_8f9[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               
               if( (Fpix > fpix3) && (Fpix <= fpix6) ) vmrp_regionC_3f6[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix9) ) vmrp_regionC_6f9[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
               
           }
        }


        if(BothMethods){

            if(Pt[i] <= ptcut_){               
                if(Ias < quan50) vmrp_regionA_ias50[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
                if(Ias < quan80) vmrp_regionA_ias80[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
                if(Ias < quan90) vmrp_regionA_ias90[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
    
    
                if(Ias >= quan50 && Ias < quan60){vmrp_regionB_50ias60[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan60 && Ias < quan70){vmrp_regionB_60ias70[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan70 && Ias < quan80){vmrp_regionB_70ias80[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan80 && Ias < quan90){vmrp_regionB_80ias90[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan50 && Ias < quan90){vmrp_regionB_50ias90[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan50 && Ias < quan99){vmrp_regionB_50ias99[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                
                if(Ias >= quan50 && Ias < quan999){vmrp_regionB_50ias999[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
    
                if(Ias >= quan50){vmrp_regionB_50ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                
                if(Ias >= quan90){vmrp_regionB_90ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan99){vmrp_regionB_99ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan999){vmrp_regionB_999ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                 
            }
            else{
                
                if(Ias < quan50) vmrp_regionC_ias50[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
                if(Ias < quan80) vmrp_regionC_ias80[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
                if(Ias < quan90) vmrp_regionC_ias90[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1);
    
    
                if(Ias >= quan50 && Ias < quan60){vmrp_regionD_50ias60[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan60 && Ias < quan70){vmrp_regionD_60ias70[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan70 && Ias < quan80){vmrp_regionD_70ias80[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan80 && Ias < quan90){vmrp_regionD_80ias90[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan50 && Ias < quan90){vmrp_regionD_50ias90[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan50 && Ias < quan99){vmrp_regionD_50ias99[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan50 && Ias < quan999){vmrp_regionD_50ias999[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
            
                if(Ias >= quan90 ){vmrp_regionD_90ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan99 ){vmrp_regionD_99ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
                if(Ias >= quan999 ){vmrp_regionD_999ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,GetMass(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C),TOF[i],*nofVtx.Get(),newWeight,1); }
    
            }
            
       }  
   } 

   //*/

   return kTRUE;
}

void HSCPSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   // All plots should be added to the list fOutput to be later on merged
   //Add plots for all MassRegionPlots
   std::cout << "There was " << befPreSel << " events passing empty preselection" << std::endl;
   std::cout << "There was " << preselTestIh << " events passing testIhPt preselection" << std::endl;


   for(int k = 0; k < 16;k++){
       std::string content;
       cout << namesSig[k] << " has nb events :" << nbSig[k] <<std::endl;   
   }


   if(Usefpixel){
       std::cout<<"We use Fpix for the cut on ionization (based on pixels, uncorellated with Ih_strip)" << std::endl;

       for(auto obj: vmrp_regionFpix_all) obj.addToList(fOutput);

       for(auto obj: vmrp_regionA_3f6) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_6f9) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_3f8) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_3f9) obj.addToList(fOutput);

       for(auto obj: vmrp_regionA_3f4) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_4f5) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_5f6) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_6f7) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_7f8) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_8f9) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_9f10) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_99f10) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_999f10) obj.addToList(fOutput);


       for(auto obj: vmrp_regionB_3f6) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_6f9) obj.addToList(fOutput);

       for(auto obj: vmrp_regionB_3f8) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_3f9) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_3f4) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_4f5) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_5f6) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_6f7) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_7f8) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_8f9) obj.addToList(fOutput);


       for(auto obj: vmrp_regionB_8f10) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_9f10) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_99f10) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_999f10) obj.addToList(fOutput);


       for(auto obj: vmrp_regionC_3f6) obj.addToList(fOutput);
       for(auto obj: vmrp_regionC_6f9) obj.addToList(fOutput);

       for(auto obj: vmrp_regionC_3f8) obj.addToList(fOutput);
       for(auto obj: vmrp_regionC_3f9) obj.addToList(fOutput);
       for(auto obj: vmrp_regionC_3f4) obj.addToList(fOutput);
       for(auto obj: vmrp_regionC_4f5) obj.addToList(fOutput);
       for(auto obj: vmrp_regionC_5f6) obj.addToList(fOutput);
       for(auto obj: vmrp_regionC_6f7) obj.addToList(fOutput);
       for(auto obj: vmrp_regionC_7f8) obj.addToList(fOutput);
       for(auto obj: vmrp_regionC_8f9) obj.addToList(fOutput);


       for(auto obj: vmrp_regionD_6f9) obj.addToList(fOutput);

       for(auto obj: vmrp_regionD_3f4) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_4f5) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_5f6) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_6f7) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_7f8) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_8f9) obj.addToList(fOutput);

       for(auto obj: vmrp_regionD_8f10) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_9f10) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_99f10) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_999f10) obj.addToList(fOutput);
   }

   if(BothMethods){ 
       //for(auto obj: vmrp_region_all) obj.addToList(fOutput);
       for(auto obj: vmrp_regionC_ias50) obj.addToList(fOutput);
       for(auto obj: vmrp_regionC_ias80) obj.addToList(fOutput);
       for(auto obj: vmrp_regionC_ias90) obj.addToList(fOutput);
    
    
       for(auto obj: vmrp_regionA_ias50) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_ias80) obj.addToList(fOutput);
       for(auto obj: vmrp_regionA_ias90) obj.addToList(fOutput);
    
       for(auto obj: vmrp_regionD_50ias60) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_60ias70) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_70ias80) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_80ias90) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_50ias90) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_50ias99) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_50ias999) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_90ias100) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_99ias100) obj.addToList(fOutput);
       for(auto obj: vmrp_regionD_999ias100) obj.addToList(fOutput);
    
       for(auto obj: vmrp_regionB_50ias60) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_60ias70) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_70ias80) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_80ias90) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_50ias90) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_50ias99) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_50ias999) obj.addToList(fOutput);
    
       for(auto obj: vmrp_regionB_90ias100) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_99ias100) obj.addToList(fOutput);
       for(auto obj: vmrp_regionB_999ias100) obj.addToList(fOutput);
   }

   //for(auto obj: vmrp) obj.addToList(fOutput);
   //std::cout <<"TerminateSlave regionmassplots wrote" << std::endl; 
   //for(auto obj: vcp) obj.AddToList(fOutput);
   //std::cout <<"TerminateSlave vcp added to list" << std::endl;
}

void HSCPSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   //calling Print on fOutput create an error - don't do that !
   //fOutput->Print();
   //create the output file
   std::cout << "We are in Terminate function, on client side" << std::endl;
   fout = new TFile(oFile_.c_str(),"RECREATE");
   cout<<"Numbers of objects to be stored: "<<fOutput->GetSize()<<endl;

   //loop over all elements stored
   TIter next(fOutput);
   TObject* object = 0;
   while ((object = next())){
   	     object->Write();
   }

   //TH1F* h = dynamic_cast<TH1F *>(fOutput->FindObject("massFromTreetoto"));
   //write and close the output file
   fout->Write();
   fout->Close();

}
