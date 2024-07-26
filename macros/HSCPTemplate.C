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

int year(2018);

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
   //Extension name for the .root file
   std::string ext = "_TestGael.root";

   std::string ptname( ((TObjString *)(tx->At(1)))->String().Data());
   int ptInt = std::stoi(ptname);

   //oFile will be the name of the output root file 
   oFile_ = ((TObjString *)(tx->At(9)))->String().Data();
   oFile_ += "_massCut_";
   oFile_ += ((TObjString *)(tx->At(6)))->String().Data();
   oFile_ += "_pT";
   oFile_ += to_string(ptInt);
   oFile_ += "_";
   oFile_ += ((TObjString *)(tx->At(10)))->String().Data();
   oFile_ += ext;
   
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


   //OutputTree is a new tree that is produced when reading the TTree from HSCP analyzer, which is a skimmed version to study the different mass recontructions for instance (for quick validation in FPix regions)

   outputTree = new TTree("outputTree", "OutputTree");
   outputTree->Branch("TreeInverseBeta", &TreeInverseBeta, "TreeInverseBeta/F");
   outputTree->Branch("TreeIh", &TreeIh, "TreeIh/F");

   outputTree->Branch("TreeMassIh", &TreeMassIh, "TreeMassIh/F");
   outputTree->Branch("TreeMuonMassIh", &TreeMuonMassIh, "TreeMuonMassIh/F");

   outputTree->Branch("TreeMuonMassAtlas", &TreeMuonMassAtlas, "TreeMuonMassAtlas/F");

   outputTree->Branch("TreeMassBeta", &TreeMassBeta, "TreeMassBeta/F");
   outputTree->Branch("TreeMuonMassBeta", &TreeMuonMassBeta, "TreeMuonMassBeta/F");

   outputTree->Branch("TreeMassCombined", &TreeMassCombined, "TreeMassCombined/F");
   outputTree->Branch("TreeMuonMassCombined", &TreeMuonMassCombined, "TreeMuonMassCombined/F");
 
   outputTree->Branch("TreePt", &TreePt, "TreePt/F");
   outputTree->Branch("TreeFpix", &TreeFpix, "TreeFpix/F");

   outputTree->Branch("TreeChi2", &TreeChi2, "TreeChi2/F");


   //Printing the content of option

   //Here we read every TString and transform them into strings
   
   std::string tofTmp( ((TObjString *)(tx->At(0)))->String().Data());
   std::string ptTmp( ((TObjString *)(tx->At(1)))->String().Data());
   std::string etaTmp( ((TObjString *)(tx->At(2)))->String().Data());
   std::string ihTmp( ((TObjString *)(tx->At(3)))->String().Data());
   std::string pTmp( ((TObjString *)(tx->At(4)))->String().Data());
   std::string massTmp( ((TObjString *)(tx->At(5)))->String().Data());
   std::string massCutTmp( ((TObjString *)(tx->At(6)))->String().Data());

   std::string tofCutTmp( ((TObjString *)(tx->At(7)))->String().Data());
   std::string fpixCutTmp( ((TObjString *)(tx->At(8)))->String().Data());

   std::string dataset( ((TObjString *)(tx->At(9)))->String().Data());
   std::string version( ((TObjString *)(tx->At(10)))->String().Data());

   do1Dplots = true;
   do2Dplots = true;
   doPreselPlots = true;

   //old TRUE

   CalibrationZmumu = true;
   correctEstimators = true;
   FillTree = true; 

   computeAtlasMass = true;
   //Finally, transform strings into int for cuts/binning, and make them correspond to header defined variables (binning and pt cut)
   tofcut_ = std::stod(tofTmp);
   ptcut_ = std::stod(ptTmp);
   etabins_ = std::stoi(etaTmp);
   ihbins_ = std::stoi(ihTmp); 
   pbins_ = std::stoi(pTmp);
   massbins_ = std::stoi(massTmp);
   masscut_ = std::stoi(massCutTmp);
   tofbins_ = std::stoi(tofCutTmp);
   fpixbins_ = std::stoi(fpixCutTmp);
   dataset_ = dataset;
   version_ = version; 

   //isSimulation will detect automatically if you run on data or not, and make histograms accordingly
   isSimulation = false;

   //Change here to add debug informations in the log of each worker in /~.proof
   debug = false;
   debugSignal = false;

   if(dataset_ != "Mu2017" && dataset_ != "Mu2018"){
       isSimulation = true;
       std::cout <<"This is SIMULATION running" << std::endl;
   }
   else{
       std::cout <<"This is DATA running" << std::endl;
   }

   //Do this to create region based on FPIXELS slices (0.3 - 0.4) (0.4 - 0.5) ectc.. (not needed anymore but for validation slices, it can be used)
   UseFpixel = false;
   //This is for regions based on Gstrip (mass method)
   UseGstrip = false;


   muBadReco = 0;
   muGoodReco = 0;
   muGoodRecoGoodError = 0;
   muGoodRecoButBadError = 0;
   tot = 0;
   PFMu = 0; 
   GlobalMu = 0; 
   passedSel =0;
   for (char c : dataset_){
       if (isdigit(c)){
           numbersMpoint_ += c;
       }
   }
   //Find the mass of the signal studied if you run on simulation
   massPointSig_ = std::stoi(numbersMpoint_);


   //create all objects that will be used by the methods Process    
   std::string firstThreeChars = dataset_.substr(0, 3);

   if(dataset_ == "highmass2017B"){
       K = K_data2017;
       C = C_data2017;
   }

   if(dataset_ == "Mu2017" || dataset_ == "Ele2017"){ 
       K = K_data2017;
       C = C_data2017;
       year = 2017;
   }
   if(dataset_ == "Mu2018"){ 
       K = K_data2018;
       C = C_data2018;
       year = 2018;
   }
   if(firstThreeChars == "Glu" || firstThreeChars == "pSt"){ 
       K = K_signal2018;
       C = C_signal2018;
   }
   if(dataset_ == "qcd170_300" || dataset_ == "wjets_0j"){
       K = K_bckg2017;
       C = C_bckg2017;
   }

   std::cout << "Dataset is : " << dataset_ << " , mass point then is : " << massPointSig_ <<  " , C = "<< C << " and K = " << K << std::endl;
   std::cout << " Starting SLAVE ---------PT  cut for regions = " << ptcut_ << " tof cut for SR = " << tofcut_ << " , Eta #bin : "<< etabins_ << ", Ih #bins" << ihbins_ << " , P #bins " << pbins_ << " , mass #bins " << massbins_ << " TOF #bins " << tofbins_ <<" , Fpix #bins " << fpixbins_  <<  ", mass cut : " << masscut_ << " , sel labels size : " << selLabels_.size() << std::endl;



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
      if(UseFpixel){
          //Create names for each plot in each regions (slices in Fpixels)
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
    
          //Create objects RegionMassPlot using the names defined above 
          RegionMassPlot regAll(label_FpixAll.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 


          RegionMassPlot regA_3f6(label_FpixA_3f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regA_6f9(label_FpixA_6f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
          RegionMassPlot regA_3f8(label_FpixA_3f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regA_3f9(label_FpixA_3f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 


          RegionMassPlot regA_3f4(label_FpixA_3f4.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regA_4f5(label_FpixA_4f5.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regA_5f6(label_FpixA_5f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regA_6f7(label_FpixA_6f7.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regA_7f8(label_FpixA_7f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
          RegionMassPlot regA_8f9(label_FpixA_8f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regA_9f10(label_FpixA_9f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regA_99f10(label_FpixA_99f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regA_999f10(label_FpixA_999f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 

    
          RegionMassPlot regC_3f8(label_FpixC_3f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regC_3f9(label_FpixC_3f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
    
          RegionMassPlot regC_3f4(label_FpixC_3f4.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regC_4f5(label_FpixC_4f5.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regC_5f6(label_FpixC_5f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regC_6f7(label_FpixC_6f7.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regC_7f8(label_FpixC_7f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regC_8f9(label_FpixC_8f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 

          RegionMassPlot regC_3f6(label_FpixC_3f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regC_6f9(label_FpixC_6f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
    
          RegionMassPlot regD_6f9(label_FpixD_6f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 

          RegionMassPlot regD_3f4(label_FpixD_3f4.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regD_4f5(label_FpixD_4f5.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regD_5f6(label_FpixD_5f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regD_6f7(label_FpixD_6f7.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regD_7f8(label_FpixD_7f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);     
          RegionMassPlot regD_8f9(label_FpixD_8f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 

          RegionMassPlot regD_8f10(label_FpixD_8f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regD_9f10(label_FpixD_9f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regD_99f10(label_FpixD_99f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regD_999f10(label_FpixD_999f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
    
          RegionMassPlot regB_3f6(label_FpixB_3f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regB_6f9(label_FpixB_6f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regB_3f8(label_FpixB_3f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regB_3f9(label_FpixB_3f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regB_3f4(label_FpixB_3f4.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regB_4f5(label_FpixB_4f5.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regB_5f6(label_FpixB_5f6.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regB_6f7(label_FpixB_6f7.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regB_7f8(label_FpixB_7f8.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);     
          RegionMassPlot regB_8f9(label_FpixB_8f9.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 

          RegionMassPlot regB_8f10(label_FpixB_8f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regB_9f10(label_FpixB_9f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regB_99f10(label_FpixB_99f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
          RegionMassPlot regB_999f10(label_FpixB_999f10.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_); 
   
          //push the RegionMassPlot objects created in vectors, because you have a set of different plot for each region, for each selection (defined in the configuration file) 
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
    
      //Definition of regions D with all quantiles and different names
      if(UseGstrip){
          std::string labelD_50ias60 = rD_50 + "_" + selLabels_[i];
          RegionMassPlot regD_50ias60(labelD_50ias60.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelD_60ias70 = rD_60 + "_" + selLabels_[i];
          RegionMassPlot regD_60ias70(labelD_60ias70.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelD_70ias80 = rD_70 + "_" + selLabels_[i];
          RegionMassPlot regD_70ias80(labelD_70ias80.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelD_80ias90 = rD_80 + "_" + selLabels_[i];
          RegionMassPlot regD_80ias90(labelD_80ias90.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelD_50ias90 = rD_50_90 + "_" + selLabels_[i];
          RegionMassPlot regD_50ias90(labelD_50ias90.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelD_50ias99 = rD_50_99 + "_" + selLabels_[i];
          RegionMassPlot regD_50ias99(labelD_50ias99.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelD_50ias999 = rD_50_999 + "_" + selLabels_[i];
          RegionMassPlot regD_50ias999(labelD_50ias999.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelD_90ias100 = rD_90_100 + "_" + selLabels_[i];
          RegionMassPlot regD_90ias100(labelD_90ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelD_99ias100 = rD_99_100 + "_" + selLabels_[i];
          RegionMassPlot regD_99ias100(labelD_99ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelD_999ias100 = rD_999_100 + "_" + selLabels_[i];
          RegionMassPlot regD_999ias100(labelD_999ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          //Definition of regions A with all quantiles and different names
          std::string labelA_50ias = rA_med + "_" + selLabels_[i];
          RegionMassPlot regA_ias50(labelA_50ias.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
          std::string labelA_80ias = rA_80 + "_" + selLabels_[i];
          RegionMassPlot regA_ias80(labelA_80ias.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
          std::string labelA_90ias = rA_90 + "_" + selLabels_[i];
          RegionMassPlot regA_ias90(labelA_90ias.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
    
          //Definition of regions C with all quantiles and different names
          std::string labelC_50ias = rC_med + "_" + selLabels_[i];
          RegionMassPlot regC_ias50(labelC_50ias.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
          std::string labelC_80ias = rC_80 + "_" + selLabels_[i];
          RegionMassPlot regC_ias80(labelC_80ias.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
          std::string labelC_90ias = rC_90 + "_" + selLabels_[i];
          RegionMassPlot regC_ias90(labelC_90ias.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
    
          //Definition of regions B with all quantiles and different names
          std::string labelB_50ias60 = rB_50 + "_" + selLabels_[i];
          RegionMassPlot regB_50ias60(labelB_50ias60.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelB_60ias70 = rB_60 + "_" + selLabels_[i];
          RegionMassPlot regB_60ias70(labelB_60ias70.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelB_70ias80 = rB_70 + "_" + selLabels_[i];
          RegionMassPlot regB_70ias80(labelB_70ias80.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelB_80ias90 = rB_80 + "_" + selLabels_[i];
          RegionMassPlot regB_80ias90(labelB_80ias90.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelB_50ias90 = rB_50_90 + "_" + selLabels_[i];
          RegionMassPlot regB_50ias90(labelB_50ias90.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelB_50ias100 = rB_50_100 + "_" + selLabels_[i];
          RegionMassPlot regB_50ias100(labelB_50ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelB_50ias99 = rB_50_99 + "_" + selLabels_[i];
          RegionMassPlot regB_50ias99(labelB_50ias99.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelB_50ias999 = rB_50_999 + "_" + selLabels_[i];
          RegionMassPlot regB_50ias999(labelB_50ias999.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelB_90ias100 = rB_90_100 + "_" + selLabels_[i];
          RegionMassPlot regB_90ias100(labelB_90ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
    
          std::string labelB_99ias100 = rB_99_100 + "_" + selLabels_[i];
          RegionMassPlot regB_99ias100(labelB_99ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);
          
          std::string labelB_999ias100 = rB_999_100 + "_" + selLabels_[i];
          RegionMassPlot regB_999ias100(labelB_999ias100.c_str(),etabins_,ihbins_,pbins_,massbins_,tofbins_,fpixbins_);


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
         
      CPlots plots;
      plots.AddHisto1D(selLabels_[i]+"_PreTrigEvt",3,0,1);

      //Not used, just a check for eta reweighting, do not take it in consideration
      std::ifstream fileEta("/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/python/thesis/chap5/EtaReweightingTofndof/eta_ratios.txt");
      std::string lineEta;
      if(selLabels_[i] == "testCalibration"){
          while (std::getline(fileEta, lineEta)) {
              std::stringstream iss(lineEta);
              double minEta, maxEta, weightEta;
              char delimiter1, delimiter2, delimiter3;
              
              if (iss >> minEta >> delimiter1 >> maxEta >> delimiter2 >> weightEta && delimiter1 == '|' && delimiter2 == ':') {
                  etaMin.push_back(minEta); 
                  etaMax.push_back(maxEta); 
                  etaWeights.push_back(weightEta); 
              }
              else{
                  std::cout << "Error parsing a line from the eta weights ->" << std::endl;
              }
          }
          std::cout << "Map etaweigths has size = " << etaWeights.size() << std::endl;
      }
      fileEta.close();

      std::ifstream fileToferr("/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/python/thesis/chap5/TOF_sigmaTof_SF/output_ratios.txt");
      std::string lineToferr;
      if(selLabels_[i] == "testIhPt"){
          while (std::getline(fileToferr, lineToferr)) {
              std::stringstream iss(lineToferr);
              double minToferr, maxToferr, weightToferr;
              char delimiter1, delimiter2, delimiter3;
              
              if (iss >> minToferr >> delimiter1 >> maxToferr >> delimiter2 >> weightToferr && delimiter1 == '-' && delimiter2 == ':') {
                  toferrMin.push_back(minToferr); 
                  toferrMax.push_back(maxToferr); 
                  toferrWeights.push_back(weightToferr); 
              }
              else{
                  std::cout << "Error parsing a line from the Toferr weights ->" << std::endl;
              }
          }
          std::cout << "Map Toferrweigths has size = " << toferrWeights.size() << std::endl;
      }
      fileToferr.close();

      //Retrieve the (Ih, BetaGamma) couples from my new parametrization
      std::ifstream file("/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/PASSATION_POWER/MassParametrisationSignal/map_dedx_bg_atlas_parallel_factor10_witherror_SIMULATION.txt");
      std::string line;
      if(selLabels_[i] == "testIhPt"){
          while (std::getline(file, line)) {
              std::stringstream ss(line);
              double Ih, P,errorP;
              char comma1,comma2;
              if (ss >> Ih >> comma1 >> P >> comma2 >> errorP && comma1 == ',' && comma2 == ',') {
                  ih_betagamma_error.push_back(std::make_tuple(Ih, P,errorP));
                  //std::cout << "Adding tuple to vector beta gamma = " << Ih << " - " << P << " +/- " << errorP << std::endl;
              }
              else{
                  std::cout << "Error parsing a line from the 2d map Ih -> betagamma" << std::endl;
              }
          }
          std::cout << "Map betagama vs Ih with ATLAS parametrisation has size = " << ih_betagamma_error.size() << std::endl;
      }
      file.close();
      //If you add values in Ih_cut_values and beta_cut_values, it will make all possible pairs and create mass histograms for each pair cut, it was just a test      
      Ih_cut_values = {};
      beta_cut_values = {};
      //Ih_cut_values = {3.35,3.6,3.75,3.85,3.95};
      //beta_cut_values = {1.01,1.02,1.03,1.04,1.05,1.06,1.07,1.08};

      //Need to add plots here
      //ADD-CPLOTS-HERE

      if(do1Dplots){ 
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionA_3fp8_ih3p60_beta1p10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionC_3fp8_ih3p60_beta1p10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionB_8fp9_ih3p60_beta1p10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionD_8fp9_ih3p60_beta1p10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionB_9fp10_ih3p60_beta1p10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionD_9fp10_ih3p60_beta1p10",800,0,4000);

          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionA_3fp8_ih3p60_beta1p10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionC_3fp8_ih3p60_beta1p10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionB_8fp9_ih3p60_beta1p10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionD_8fp9_ih3p60_beta1p10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionB_9fp10_ih3p60_beta1p10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionD_9fp10_ih3p60_beta1p10",800,0,4000);


          plots.AddHisto1D(selLabels_[i]+"_hmass_regionA_3fp8_ih3p70_beta1p12",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionC_3fp8_ih3p70_beta1p12",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionB_8fp9_ih3p70_beta1p12",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionD_8fp9_ih3p70_beta1p12",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionB_9fp10_ih3p70_beta1p12",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionD_9fp10_ih3p70_beta1p12",800,0,4000);

          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionA_3fp8_ih3p70_beta1p12",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionC_3fp8_ih3p70_beta1p12",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionB_8fp9_ih3p70_beta1p12",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionD_8fp9_ih3p70_beta1p12",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionB_9fp10_ih3p70_beta1p12",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionD_9fp10_ih3p70_beta1p12",800,0,4000);



          plots.AddHisto1D(selLabels_[i]+"_hmass_regionA_3fp8_ih3p89_beta1p13",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionC_3fp8_ih3p89_beta1p13",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionB_8fp9_ih3p89_beta1p13",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionD_8fp9_ih3p89_beta1p13",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionB_9fp10_ih3p89_beta1p13",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionD_9fp10_ih3p89_beta1p13",800,0,4000);


          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionA_3fp8_ih3p89_beta1p13",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionC_3fp8_ih3p89_beta1p13",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionB_8fp9_ih3p89_beta1p13",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionD_8fp9_ih3p89_beta1p13",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionB_9fp10_ih3p89_beta1p13",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionD_9fp10_ih3p89_beta1p13",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionA_3fp8_ih3p95_beta1p15",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionC_3fp8_ih3p95_beta1p15",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionB_8fp9_ih3p95_beta1p15",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionD_8fp9_ih3p95_beta1p15",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionB_9fp10_ih3p95_beta1p15",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionD_9fp10_ih3p95_beta1p15",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionA_3fp8_ih3p95_beta1p15",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionC_3fp8_ih3p95_beta1p15",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionB_8fp9_ih3p95_beta1p15",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionD_8fp9_ih3p95_beta1p15",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionB_9fp10_ih3p95_beta1p15",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionD_9fp10_ih3p95_beta1p15",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionA_3fp8_ih4p04_beta1p17",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionC_3fp8_ih4p04_beta1p17",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionB_8fp9_ih4p04_beta1p17",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionD_8fp9_ih4p04_beta1p17",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionB_9fp10_ih4p04_beta1p17",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmass_regionD_9fp10_ih4p04_beta1p17",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionA_3fp8_ih4p04_beta1p17",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionC_3fp8_ih4p04_beta1p17",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionB_8fp9_ih4p04_beta1p17",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionD_8fp9_ih4p04_beta1p17",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionB_9fp10_ih4p04_beta1p17",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassnoP_regionD_9fp10_ih4p04_beta1p17",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hTOT",2,-0.5,1.5);
          plots.AddHisto1D(selLabels_[i]+"_hEta",200,-5,5);
          plots.AddHisto1D(selLabels_[i]+"_hEtaReweighted",200,-5,5);
          plots.AddHisto1D(selLabels_[i]+"_hPhi",200,-5,5);
          plots.AddHisto1D(selLabels_[i]+"_hNOMH",20,-0.5,19.5);
          plots.AddHisto1D(selLabels_[i]+"_hNOM",40,-0.5,39.5);
          plots.AddHisto1D(selLabels_[i]+"_hNOPH",8,-0.5,7.5);
          plots.AddHisto1D(selLabels_[i]+"_hNOMinIh",20,-0.5,19.5);
          plots.AddHisto1D(selLabels_[i]+"_hFOVH",100,0,1);
          plots.AddHisto1D(selLabels_[i]+"_hdZ",100,0,1);
          plots.AddHisto1D(selLabels_[i]+"_hdXY",100,0,1);
          plots.AddHisto1D(selLabels_[i]+"_hChi2",100,0,100);
          plots.AddHisto1D(selLabels_[i]+"_hNdof",100,0,100);
          plots.AddHisto1D(selLabels_[i]+"_hChi2PerNdof",300,0,30);
          plots.AddHisto1D(selLabels_[i]+"_hpt",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hTOF",200,0,5);
          plots.AddHisto1D(selLabels_[i]+"_hBetaError",200,0,0.5);
          plots.AddHisto1D(selLabels_[i]+"_hBetaErrorCorrected",200,0,0.5);
          plots.AddHisto1D(selLabels_[i]+"_hinvTOF",200,0,5);
          plots.AddHisto1D(selLabels_[i]+"_hIh",600,0,30);
          plots.AddHisto1D(selLabels_[i]+"_hPtErr",400,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hPtErrOverPt",160,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hPtErrOverPt2",80,0,0.003);
          plots.AddHisto1D(selLabels_[i]+"_hFPIX",20,0,1);
          plots.AddHisto1D(selLabels_[i]+"_hGstrip",20,0,1);
          plots.AddHisto1D(selLabels_[i]+"_hnbHSCP",20,0,10);
          plots.AddHisto1D(selLabels_[i]+"_hrecoPFMET",200,0,2000);
          plots.AddHisto1D(selLabels_[i]+"_hBeta",40,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hDiMuMass_Calib",800,0,200);
          plots.AddHisto1D(selLabels_[i]+"_hDiMuMass_CalibWide",6000,0,6000);
          plots.AddHisto1D(selLabels_[i]+"_hmuPtErr_over_muPt",400,0,4);
          plots.AddHisto1D(selLabels_[i]+"_hTrackPtErr_over_TrackPt",400,0,4);
          plots.AddHisto1D(selLabels_[i]+"_hmuMatchedHSCP_dr0p15",3,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hTrack_MiniIsoSumPtFixCone",200,0,50);
          plots.AddHisto1D(selLabels_[i]+"_hTrack_MiniIsoSumPtVarCone",200,0,50);
          plots.AddHisto1D(selLabels_[i]+"_hPFMiniIso_relative",300,0,1.5);
          plots.AddHisto1D(selLabels_[i]+"_hmatchTrigMuon_minDeltaR",100,0,5);
          plots.AddHisto1D(selLabels_[i]+"_hTrackPt_minus_MatchedMuonPt_dr03",100,-5,5);
          plots.AddHisto1D(selLabels_[i]+"_hHLTMu50",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hisPFMuon",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hDedxError",200,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hBetaError",100,0,1);
          plots.AddHisto1D(selLabels_[i]+"_hMassDedxError",1000,0,100);
          plots.AddHisto1D(selLabels_[i]+"_hMassBetaError",1000,0,100);
          plots.AddHisto1D(selLabels_[i]+"_hMassCombinedError",1000,0,100);
          plots.AddHisto1D(selLabels_[i]+"_hMass_VR_3fp8_etabin1",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassAtlas_VR_3fp8_etabin1",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassBeta_VR_3fp8_etabin1",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassCombined_VR_3fp8_etabin1",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassMuon_VR_3fp8_etabin1",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassMuonBeta_VR_3fp8_etabin1",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_VR_3fp8_etabin1",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMass_VR_3fp8_etabin2",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassBeta_VR_3fp8_etabin2",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassCombined_VR_3fp8_etabin2",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassDedxPow2_CR_3fp8",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassDedxPow5third_CR_3fp8",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hpoverm_VR_3fp8",100,0,5);
          plots.AddHisto1D(selLabels_[i]+"_hpoverm_SR_9fp10",100,0,5);
          plots.AddHisto1D(selLabels_[i]+"_hBeta_VR_3fp8",100,0,5);
          plots.AddHisto1D(selLabels_[i]+"_hpoverm",100,0,5);
          plots.AddHisto1D(selLabels_[i]+"_hMass_SR1_9fp10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassMuon_SR1_9fp10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassAtlas_SR1_9fp10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassBeta_SR1_9fp10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassMuonBeta_SR1_9fp10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassCombined_SR1_9fp10",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10",800,0,4000);
          //Signal systematics 
          if(isSimulation){
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_fpixUp",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_fpixDown",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_toferrUp",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_toferrDown",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_puUp",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_puDown",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_pTUp",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_pTDown",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_triggerUp",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_triggerDown",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_muTriggerUp",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_muTriggerDown",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_muRecoUp",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_muRecoDown",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_muIdUp",800,0,4000);
              plots.AddHisto1D(selLabels_[i]+"_hMassMuonCombined_SR1_9fp10_muIdDown",800,0,4000);
          } 

          plots.AddHisto1D(selLabels_[i]+"_hMassBetaMinusTheo_div_MassBeta_SR1_9fp10",200,-100,100);
          plots.AddHisto1D(selLabels_[i]+"_hMassdEdxMinusTheo_div_MassdEdx_SR1_9fp10",200,-100,100);
          plots.AddHisto1D(selLabels_[i]+"_hMassCombMinusTheo_div_MassComb_SR1_9fp10",200,-100,100);
          plots.AddHisto1D(selLabels_[i]+"_hMassATLAS",200,0,4);
          plots.AddHisto1D(selLabels_[i]+"_hMassIH",200,0,4);
          plots.AddHisto1D(selLabels_[i]+"_hMassBETA",200,0,4);
          plots.AddHisto1D(selLabels_[i]+"_hMassCombinedATLASBETA",200,0,4);
          plots.AddHisto1D(selLabels_[i]+"_hEventWeight",200,0,4);

          if(isSimulation){
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin1",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin2",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin3",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin4",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin5",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin6",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin7",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin8",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin9",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin10",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin11",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin12",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin13",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin14",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin15",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin1_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin2_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin3_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin4_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin5_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin6_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin7_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin8_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin9_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin10_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin11_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin12_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin13_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin14_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBeta_bin15_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullBetaZmumu",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullinvBetaZmumu",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin1",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin2",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin3",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin4",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin5",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin6",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin7",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin8",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin9",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin10",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin11",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin12",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin13",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin14",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin15",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin1_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin2_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin3_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin4_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin5_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin6_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin7_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin8_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin9_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin10_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin11_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin12_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin13_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin14_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPt_bin15_gen",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonP",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPtZmumu",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMuonPZmumu",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hinvBetaZmumu_MC",200,0,2);
              plots.AddHisto1D(selLabels_[i]+"_hBetaZmumu_MC",200,0,2);
              plots.AddHisto1D(selLabels_[i]+"_hTOFerrZmumu_MC",200,0,0.2);
              plots.AddHisto1D(selLabels_[i]+"_hTOFndofZmumu_MC",120,0,120);
              plots.AddHisto1D(selLabels_[i]+"_hTOFerr",200,0,0.2);
              plots.AddHisto1D(selLabels_[i]+"_hTOFndof",1200,0,120);
              plots.AddHisto2D(selLabels_[i]+"_hEta_vs_TOFerr_Zmumu_MC",40,-2.6,2.6,40,0.,1.);
              plots.AddHisto1D(selLabels_[i]+"_hinvBetaHSCP_MC",200,0,2);
              plots.AddHisto1D(selLabels_[i]+"_hTOFerrHSCP_MC",200,0,0.2);
              plots.AddHisto1D(selLabels_[i]+"_hTOFndofHSCP_MC",120,0,120);
              plots.AddHisto1D(selLabels_[i]+"_hpullMassBeta_CR_3fp8",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMassDedx_CR_3fp8",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMassAtlas_CR_3fp8",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMassComb_CR_3fp8",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMassCombTest_CR_3fp8",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMassBeta_SR1_9fp10",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMassDedx_SR1_9fp10",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMassAtlas_SR1_9fp10",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMassComb_SR1_9fp10",160,-20,20);
              plots.AddHisto1D(selLabels_[i]+"_hpullMassCombTest_SR1_9fp10",160,-20,20);
          }
          if(!isSimulation){
              plots.AddHisto1D(selLabels_[i]+"_hinvBetaZmumu_Data",200,0,2);
              plots.AddHisto1D(selLabels_[i]+"_hBetaZmumu_Data",200,0,2);
              plots.AddHisto1D(selLabels_[i]+"_hTOFerrZmumu_Data",200,0,0.2);
              plots.AddHisto1D(selLabels_[i]+"_hTOFndofZmumu_Data",120,0,120);

              plots.AddHisto1D(selLabels_[i]+"_hinvBetaHSCP_Data",200,0,2);
              plots.AddHisto1D(selLabels_[i]+"_hTOFerrHSCP_Data",200,0,0.2);
              plots.AddHisto1D(selLabels_[i]+"_hTOFndofHSCP_Data",120,0,120);
              plots.AddHisto2D(selLabels_[i]+"_hEta_vs_TOFerr_Zmumu_Data",40,-2.6,2.6,40,0.,1.);
          }



          plots.AddHisto1D(selLabels_[i]+"_hTOF_CR_3fp8",200,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hIh_CR_3fp8",1000,0,40);

          plots.AddHisto1D(selLabels_[i]+"_hTOF_SR_9fp10",200,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hIh_SR_9fp10",1000,0,40);
          
          plots.AddHisto1D(selLabels_[i]+"_hmassAtlas_CR_3fp8",800,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hmassIh_CR_3fp8",800,0,4000);

          plots.AddHisto1D(selLabels_[i]+"_hmassAtlas_lowMass",200,0,4);
          plots.AddHisto1D(selLabels_[i]+"_hmassIh_lowMass",200,0,4);


          plots.AddHisto1D(selLabels_[i]+"_hMassBetaMinusTheo_div_MassBeta_CR_3fp8",200,-100,100);
          plots.AddHisto1D(selLabels_[i]+"_hMassdEdxMinusTheo_div_MassdEdx_CR_3fp8",200,-100,100);
          plots.AddHisto1D(selLabels_[i]+"_hMassCombMinusTheo_div_MassComb_CR_3fp8",200,-100,100);



          plots.AddHisto1D(selLabels_[i]+"_hChi2_dedx_beta_comb_tight_preCut",800,0,800);
          plots.AddHisto1D(selLabels_[i]+"_hChi2_dedx_beta_comb_tight",800,0,100);

          plots.AddHisto1D(selLabels_[i]+"_hChi2MuonSquare_dedx_beta_comb_large",800,0,800);
          plots.AddHisto1D(selLabels_[i]+"_hChi2MuonNoSquare_dedx_beta_comb_large",800,0,800);


          plots.AddHisto1D(selLabels_[i]+"_hChi2MuonSquare_dedx_beta_comb_large_SR",800,0,800);
          plots.AddHisto1D(selLabels_[i]+"_hChi2MuonSquare_dedx_beta_comb_large_CR",800,0,800);

          plots.AddHisto1D(selLabels_[i]+"_hChi2MuonNoSquare_dedx_beta_comb_large_SR",800,0,800);
          plots.AddHisto1D(selLabels_[i]+"_hChi2MuonNoSquare_dedx_beta_comb_large_CR",800,0,800);



          plots.AddHisto1D(selLabels_[i]+"_hChi2_dedx_beta_exp_tight_preCut",800,0,800);
          plots.AddHisto1D(selLabels_[i]+"_hChi2_dedx_beta_exp_large",800,0,800);
          plots.AddHisto1D(selLabels_[i]+"_hChi2_dedx_beta_exp_tight",800,0,100);


          plots.AddHisto1D(selLabels_[i]+"_hChi2MuonSquare_dedx_beta_comb_tight_CR_FAIL",800,0,100);
          plots.AddHisto1D(selLabels_[i]+"_hChi2MuonSquare_dedx_beta_comb_tight_CR_PASS",800,0,100);
          plots.AddHisto1D(selLabels_[i]+"_hChi2MuonSquare_dedx_beta_comb_tight_CR_ALL",800,0,100);

          plots.AddHisto1D(selLabels_[i]+"_hChi2_dedx_beta_exp_tight_CR_FAIL",800,0,100);
          plots.AddHisto1D(selLabels_[i]+"_hChi2_dedx_beta_exp_tight_CR_PASS",800,0,100);

          plots.AddHisto1D(selLabels_[i]+"_hChi2MuonSquare_dedx_beta_comb_ultra_tight_long",64000,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hChi2MuonSquare_dedx_beta_comb_ultra_tight_long_CR",64000,0,4000);
          plots.AddHisto1D(selLabels_[i]+"_hChi2MuonSquare_dedx_beta_comb_ultra_tight_long_SR",64000,0,4000);


          plots.AddHisto1D(selLabels_[i]+"_hChi2_dedx_beta_exp_ultra_tight_long",64000,0,4000);

          plots.AddHisto1D(selLabels_[i]+"_hQuad2_dedx_beta_large",400,-100,100);
          plots.AddHisto1D(selLabels_[i]+"_hQuad2_dedx_beta_tight",800,-100,100);
          plots.AddHisto1D(selLabels_[i]+"_hQuad2_dedx_beta_ultra_tight",1600,-100,100);

          plots.AddHisto1D(selLabels_[i]+"_hQuad2_dedx_beta_large_SR",400,-100,100);
          plots.AddHisto1D(selLabels_[i]+"_hQuad2_dedx_beta_tight_SR",800,-100,100);
          plots.AddHisto1D(selLabels_[i]+"_hQuad2_dedx_beta_ultra_tight_SR",1600,-100,100);

          plots.AddHisto1D(selLabels_[i]+"_hQuad2_dedx_beta_large_CR",400,-100,100);
          plots.AddHisto1D(selLabels_[i]+"_hQuad2_dedx_beta_tight_cR",800,-100,100);
          plots.AddHisto1D(selLabels_[i]+"_hQuad2_dedx_beta_ultra_tight_CR",1600,-100,100);

          plots.AddHisto1D(selLabels_[i]+"_hSubstraction_dedx_beta_quadraticErrors_SR1",3200,-500,500);
          plots.AddHisto1D(selLabels_[i]+"_hSubstraction_dedx_beta_quadraticErrors_CR",3200,-500,500);

          /*
          for (double Ih_cut : Ih_cut_values) {
              for (double beta_cut : beta_cut_values) {
                  const HistogramInfo& info = cut_hist_map.at(std::make_pair(Ih_cut, beta_cut));
                  plots.AddHisto1D(selLabels_[i]+info.histName_betaCR.c_str(), 800, 0, 4000);
                  plots.AddHisto1D(selLabels_[i]+info.histName_dedxCR.c_str(), 800, 0, 4000);
                  plots.AddHisto1D(selLabels_[i]+info.histName_combCR.c_str(), 800, 0, 4000);
                  plots.AddHisto1D(selLabels_[i]+info.histName_betaSR.c_str(), 800, 0, 4000);
                  plots.AddHisto1D(selLabels_[i]+info.histName_dedxSR.c_str(), 800, 0, 4000);
                  plots.AddHisto1D(selLabels_[i]+info.histName_combSR.c_str(), 800, 0, 4000);
              }
          }
          */
          if(selLabels_[i] == "testIhPt"){
              for (double Ih_cut : Ih_cut_values) {
                  std::stringstream Ih_cut_ss;
                  Ih_cut_ss << Ih_cut;
                  std::string Ih_cut_str = Ih_cut_ss.str();
                  for (char &c : Ih_cut_str) {
                      if (c == '.') {
                          c = 'p';
                          break;
                      }
                  }
    
                  for (double beta_cut : beta_cut_values) {
                      std::stringstream beta_cut_ss;
                      beta_cut_ss << beta_cut;
                      std::string beta_cut_str = beta_cut_ss.str();
                      for (char &c : beta_cut_str) {
                          if (c == '.') {
                              c = 'p';
                              break;
                          }
                      }
    
                      
                      std::string tmpBetaCR = selLabels_[i]+"_hMassBeta_C_3fp8_ih" + Ih_cut_str + "_beta" + beta_cut_str;
                      std::string tmpDedxCR = selLabels_[i]+"_hMassdEdx_C_3fp8_ih" + Ih_cut_str + "_beta" + beta_cut_str;
                      std::string tmpCombCR = selLabels_[i]+"_hMassCombined_C_3fp8_ih" + Ih_cut_str + "_beta" + beta_cut_str;
                      std::string tmpAtlasCR = selLabels_[i]+"_hMassAtlas_C_3fp8_ih" + Ih_cut_str + "_beta" + beta_cut_str;
                      cut_hist_map_beta_CR[std::make_pair(Ih_cut, beta_cut)] = tmpBetaCR;
                      cut_hist_map_dedx_CR[std::make_pair(Ih_cut, beta_cut)] = tmpDedxCR;
                      cut_hist_map_comb_CR[std::make_pair(Ih_cut, beta_cut)] = tmpCombCR;
                      cut_hist_map_atlas_CR[std::make_pair(Ih_cut, beta_cut)] = tmpAtlasCR;
    
                      std::cout << "Adding plot mass beta CR with name : " << tmpBetaCR << std::endl;
                      std::cout << "Adding plot mass combined CR with name : " << tmpCombCR << std::endl;
                      std::cout << "Adding plot mass dedx CR with name : " << tmpDedxCR << std::endl;
                      std::cout << "Adding plot mass ATLAS CR with name : " << tmpAtlasCR << std::endl;
                      plots.AddHisto1D(tmpBetaCR, 800, 0, 4000);
                      plots.AddHisto1D(tmpDedxCR, 800, 0, 4000);
                      plots.AddHisto1D(tmpCombCR, 800, 0, 4000);
                      plots.AddHisto1D(tmpAtlasCR, 800, 0, 4000);
    
                      std::string tmpBetaSR = selLabels_[i]+"_hMassBeta_SR1_9fp10_ih" + Ih_cut_str + "_beta" + beta_cut_str;
                      std::string tmpDedxSR = selLabels_[i]+"_hMassdEdx_SR1_9fp10_ih" + Ih_cut_str + "_beta" + beta_cut_str;
                      std::string tmpCombSR = selLabels_[i]+"_hMassCombined_SR1_9fp10_ih" + Ih_cut_str + "_beta" + beta_cut_str;
                      std::string tmpAtlasSR = selLabels_[i]+"_hMassAtlas_SR1_9fp10_ih" + Ih_cut_str + "_beta" + beta_cut_str;
    
                      cut_hist_map_beta_SR[std::make_pair(Ih_cut, beta_cut)] = tmpBetaSR;
                      cut_hist_map_dedx_SR[std::make_pair(Ih_cut, beta_cut)] = tmpDedxSR;
                      cut_hist_map_comb_SR[std::make_pair(Ih_cut, beta_cut)] = tmpCombSR;
                      cut_hist_map_atlas_SR[std::make_pair(Ih_cut, beta_cut)] = tmpAtlasSR;
                      std::cout << "Adding plot mass beta SR with name : " << tmpBetaSR << std::endl;
                      std::cout << "Adding plot mass combined SR with name : " << tmpCombSR << std::endl;
                      std::cout << "Adding plot mass dedx SR with name : " << tmpDedxSR << std::endl;
                      std::cout << "Adding plot mass atlas SR with name : " << tmpAtlasSR << std::endl;
    
                      plots.AddHisto1D(tmpBetaSR, 800, 0, 4000);
                      plots.AddHisto1D(tmpDedxSR, 800, 0, 4000);
                      plots.AddHisto1D(tmpCombSR, 800, 0, 4000);
                      plots.AddHisto1D(tmpAtlasSR, 800, 0, 4000);
                  
                  }
    
              }
          }

      }
      
      if(do2Dplots){ 

          plots.AddHisto2D(selLabels_[i]+"_hmuonPt_vs_trackPt",2000,0,4000,2000,0,4000);
          plots.AddHisto2D(selLabels_[i]+"_hHSCPmuEta_vs_HSCPTrackEta",80,-2,2,80,-2,2);



          plots.AddHisto2D(selLabels_[i]+"_hTOFerr_vs_TOF",40,0,0.2,40,0.5,1.5);
          plots.AddHisto2D(selLabels_[i]+"_hEta_vs_TOFerr",40,-2.6,2.6,40,0.,1.);
          plots.AddHisto2D(selLabels_[i]+"_hEta_vs_TOFndof",40,-2.6,2.6,60,0.,120.);
          plots.AddHisto2D(selLabels_[i]+"_hTOFndof_vs_TOF",60,0.,120.,40,0.5,1.5);
          plots.AddHisto2D(selLabels_[i]+"_hTOFndof_vs_TOFerr",60,0.,120.,200,0.,1);


          plots.AddHisto2D(selLabels_[i]+"_hDTTOFerr_vs_DTTOF",40,0,0.2,40,0.5,1.5);
          plots.AddHisto2D(selLabels_[i]+"_hEta_vs_DTTOFerr",40,-2.6,2.6,40,0.,1.);
          plots.AddHisto2D(selLabels_[i]+"_hEta_vs_DTTOFndof",40,-2.6,2.6,60,0.,120.);
          plots.AddHisto2D(selLabels_[i]+"_hDTTOFndof_vs_DTTOF",60,0.,120.,40,0.5,1.5);

          plots.AddHisto2D(selLabels_[i]+"_hCSCTOFerr_vs_CSCTOF",40,0,0.2,40,0.5,1.5);
          plots.AddHisto2D(selLabels_[i]+"_hEta_vs_CSCTOFerr",40,-2.6,2.6,40,0.,1.);
          plots.AddHisto2D(selLabels_[i]+"_hEta_vs_CSCTOFndof",40,-2.6,2.6,60,0.,120.);
          plots.AddHisto2D(selLabels_[i]+"_hCSCTOFndof_vs_CSCTOF",60,0.,120.,40,0.5,1.5);

          plots.AddHisto2D(selLabels_[i]+"_hIh_vs_TOF",200,3,8,40,0.5,1.5);
          plots.AddHisto2D(selLabels_[i]+"_hBetaGamma_vs_Ih",100,0,5,200,0,13);
          plots.AddHisto2D(selLabels_[i]+"_hBetaGamma_vs_Ih_SR_9fp10",100,0,5,200,0,13);

          plots.AddHisto2D(selLabels_[i]+"_hIh_vs_P",600,0,30,800,0,4000);

          plots.AddHisto2D(selLabels_[i]+"_hTOF_vs_Fpix",40,0.5,1.5,11,0,1);

          plots.AddHisto2D(selLabels_[i]+"_hmuPt_vs_muPtErr",1000,0,2000,1000,0,2000);

          plots.AddHisto2D(selLabels_[i]+"_hTOF_vs_Ih_CR_3fp8",1000,0,3,1000,0,50);
	  plots.AddHisto2D(selLabels_[i]+"_hHSCPmuPt_vs_muPtErr",1000,0,2000,1000,0,2000);

	  plots.AddHisto2D(selLabels_[i]+"_hHSCPmuPt_vs_muPtErr_fine",4000,0,1000,4000,0,1000);
	  plots.AddHisto2D(selLabels_[i]+"_hHSCPmuPt_vs_muPtErrovermuPt",1000,0,2000,200,0,1);
	  plots.AddHisto2D(selLabels_[i]+"_hHSCPmuPt_vs_muPtErrovermuPt_fine",4000,0,1000,200,0,1);
          plots.AddHisto2D(selLabels_[i]+"_hTOF_vs_Ih_SR_9fp10",1000,0,3,1000,0,50);

          if(isSimulation){
              plots.AddHisto2D(selLabels_[i]+"_hgenP_vs_muP",1000,0,2000,1000,0,2000);
              plots.AddHisto2D(selLabels_[i]+"_hgenP_vs_trackP",1000,0,2000,1000,0,2000);
              plots.AddHisto2D(selLabels_[i]+"_hgenPt_vs_muPt",1000,0,2000,1000,0,2000);
              plots.AddHisto2D(selLabels_[i]+"_hgenPt_vs_muPtZmumu",1000,0,2000,1000,0,2000);
              plots.AddHisto2D(selLabels_[i]+"_hgenPt_vs_trackPt",1000,0,2000,1000,0,2000);
              plots.AddHisto2D(selLabels_[i]+"_hgenPt_vs_trackPtZmumu",1000,0,2000,1000,0,2000);
              plots.AddHisto2D(selLabels_[i]+"_hgenBeta_vs_Beta",200,0,2,200,0,2);


              plots.AddHisto2D(selLabels_[i]+"_hBeta_vs_betaMuon",400,0,2,400,0,2);


              plots.AddHisto2D(selLabels_[i]+"_hgenBeta_vs_pullBeta",200,0,2,160,-20,20);
              plots.AddHisto2D(selLabels_[i]+"_hrecoBeta_vs_pullBeta",200,0,2,160,-20,20);

              plots.AddHisto2D(selLabels_[i]+"_hgenPt_vs_pullMuonPt",1000,0,1000,160,-20,20);
              plots.AddHisto2D(selLabels_[i]+"_hrecoPt_vs_pullMuonPt",1000,0,1000,160,-20,20);

              plots.AddHisto2D(selLabels_[i]+"_hPMuonOverM_vs_Ih",600,0,30,1000,0,30);
              plots.AddHisto2D(selLabels_[i]+"_hPTrackOverM_vs_Ih",600,0,30,1000,0,30);
          }

          plots.AddHisto2D(selLabels_[i]+"_hMass_vs_TOF",800,0,4000,40,0.5,1.5);

          plots.AddHisto2D(selLabels_[i]+"_hMass_vs_Fpix",800,0,4000,11,0,1);


          plots.AddHisto2D(selLabels_[i]+"_hMassBeta_vs_ErrMassBeta_CR3fp8",800,0,4000,800,0,4000);
          plots.AddHisto2D(selLabels_[i]+"_hMassdEdx_vs_ErrMassdEdx_CR3fp8",800,0,4000,800,0,4000);

          plots.AddHisto2D(selLabels_[i]+"_hMassBeta_vs_ErrMassBeta_SR9fp10",800,0,4000,800,0,4000);
          plots.AddHisto2D(selLabels_[i]+"_hMassdEdx_vs_ErrMassdEdx_SR9fp10",800,0,4000,800,0,4000);

          plots.AddHisto2D(selLabels_[i]+"_hMassBeta_vs_Mass",800,0,4000,800,0,4000);

          plots.AddHisto2D(selLabels_[i]+"_hMassBeta_vs_theor_CR3fp8",800,0,4000,800,0,4000);
          plots.AddHisto2D(selLabels_[i]+"_hMassdEdx_vs_theor_CR3fp8",800,0,4000,800,0,4000);
          plots.AddHisto2D(selLabels_[i]+"_hMassCombined_vs_theor_CR3fp8",800,0,4000,800,0,4000);



	  plots.AddHisto2D(selLabels_[i]+"_hmassDedxInit_vs_eta_CR_3fp8",800,0,4000,80,-1,1);
	  plots.AddHisto2D(selLabels_[i]+"_hEta_vs_IhStripOnly_CR_3fp8",80,-1,1,100,0,8);

	  plots.AddHisto2D(selLabels_[i]+"_hpullBeta_vs_massBeta_CR_3fp8",80,-20,20,800,0,4000);
	  plots.AddHisto2D(selLabels_[i]+"_hpullDedx_vs_massDedx_CR_3fp8",80,-20,20,800,0,4000);
	  plots.AddHisto2D(selLabels_[i]+"_hpullComb_vs_massComb_CR_3fp8",80,-20,20,800,0,4000);

	  plots.AddHisto2D(selLabels_[i]+"_hpullBeta_vs_eta_CR_3fp8",80,-20,20,60,-2.5,2.5);
	  plots.AddHisto2D(selLabels_[i]+"_hpullDedx_vs_eta_CR_3fp8",80,-20,20,60,-2.5,2.5);
	  plots.AddHisto2D(selLabels_[i]+"_hpullComb_vs_eta_CR_3fp8",80,-20,20,60,-2.5,2.5);

	  plots.AddHisto2D(selLabels_[i]+"_hpullBeta_vs_p_CR_3fp8",80,-20,20,800,0,4000);
	  plots.AddHisto2D(selLabels_[i]+"_hpullDedx_vs_p_CR_3fp8",80,-20,20,800,0,4000);
	  plots.AddHisto2D(selLabels_[i]+"_hpullComb_vs_p_CR_3fp8",80,-20,20,800,0,4000);

          plots.AddHisto2D(selLabels_[i]+"_hMassBeta_MassBetaErr_CR",1000,0,2000,1000,0,2000);
          plots.AddHisto2D(selLabels_[i]+"_hMassDedx_MassDedxErr_CR",1000,0,2000,1000,0,2000);
          plots.AddHisto2D(selLabels_[i]+"_hMassComb_MassCombErr_CR",1000,0,2000,1000,0,2000);

          plots.AddHisto2D(selLabels_[i]+"_hMassCombineWeighted_vs_MassCombineAverage",800,0,4000,800,0,4000);
          plots.AddHisto2D(selLabels_[i]+"_hMassBetaClean_vs_Mass",800,0,4000,800,0,4000);

          plots.AddHisto2D(selLabels_[i]+"_hFpix_vs_PFinal_CR_3fp8",20,0,1,800,0,4000);
          plots.AddHisto2D(selLabels_[i]+"_hFpix_vs_BetaFinal_CR_3fp8",20,0,1,200,0,2);
          


          float runMin,runMax;
          if(year == 2017){
              runMin = 296000.5;
              runMax = 306500.5;
          }
          if(year == 2018){
              runMin = 315000.5;
              runMax = 326000.5;
          }
          int binRuns = int(runMax - runMin);

          plots.AddHisto2D(selLabels_[i]+"_hPTrack_vs_Ih",800,0,4000,1000,0,30);
          plots.AddHisto2D(selLabels_[i]+"_hPMuon_vs_Ih",800,0,4000,1000,0,30);

          plots.AddHisto2D(selLabels_[i]+"_hNPV_vs_PU",100,0,100,100,0,100);
          plots.AddHisto2D(selLabels_[i]+"_hPU_vs_NPV",100,0,100,100,0,100);

          plots.AddHisto2D(selLabels_[i]+"_hRUN_vs_TOF",binRuns,runMin,runMax,40,0.5,1.5);
          plots.AddHisto2D(selLabels_[i]+"_hRUN_vs_DTTOF",binRuns,runMin,runMax,40,0.5,1.5);
          plots.AddHisto2D(selLabels_[i]+"_hRUN_vs_CSCTOF",binRuns,runMin,runMax,40,0.5,1.5);

          plots.AddHisto2D(selLabels_[i]+"_hEta_vs_TOF",40,-2.6,2.6,40,0.5,1.5);

          plots.AddHisto2D(selLabels_[i]+"_hTOF_vs_MassDeDx_CRbeta_3fp8",20,0.5,1.5,800,0,4000);

          plots.AddHisto2D(selLabels_[i]+"_hTOF_vs_MassDeDxAbove400_CRbeta_3fp8",20,0.5,1.5,800,0,4000);

          plots.AddHisto2D(selLabels_[i]+"_hMassDedx_vs_MassBeta_chi2FAIL_CR",800,0,4000,800,0,4000);
          plots.AddHisto2D(selLabels_[i]+"_hMassDedx_vs_MassBeta_chi2FAIL_neg_CR",800,-2000,2000,800,-2000,2000);

          plots.AddHisto2D(selLabels_[i]+"_hMassDedx_vs_MassBeta_chi2FAIL_SR",800,0,4000,800,0,4000);
          plots.AddHisto2D(selLabels_[i]+"_hMassDedx_vs_MassBeta_chi2FAIL_neg_SR",800,-2000,2000,800,-2000,2000);


          plots.AddHisto2D(selLabels_[i]+"_hMassDedx_vs_MassBeta_chi2PASS_CR",800,0,4000,800,0,4000);
          plots.AddHisto2D(selLabels_[i]+"_hMassDedx_vs_MassBeta_chi2ALL_CR",800,0,4000,800,0,4000);

          plots.AddHisto2D(selLabels_[i]+"_hMassDedx_vs_MassBeta_chi2PASS_SR",800,0,4000,800,0,4000);
          plots.AddHisto2D(selLabels_[i]+"_hMassDedx_vs_MassBeta_chi2ALL_SR",800,0,4000,800,0,4000);


      }


      if(doPreselPlots && selLabels_[i] =="allInclusive"){

          plots.AddHisto1D(selLabels_[i]+"_hEff_Trigger",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_PFMuon",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_Pt",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_Eta",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_NOPH",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_FOVH",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_NOM",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_HighPurity",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_Chi2oNDF",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_dZ",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_dXY",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_MiniRelIso",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_TkAbsIso",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_EoP",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_PtErroPtPt",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_PtErroPt1",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_PtErroPt0",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_ProbQ",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_TOFerr",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_TOFndof",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hEff_TOFinf1",2,0,2);

          plots.AddHisto1D(selLabels_[i]+"_hN1_Trigger",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_PFMuon",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_Pt",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_Eta",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_NOPH",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_FOVH",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_NOM",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_HighPurity",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_Chi2oNDF",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_dZ",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_dXY",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_MiniRelIso",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_TkAbsIso",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_EoP",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_PtErroPtPt",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_PtErroPt1",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_PtErroPt0",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_ProbQ",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_TOFerr",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_TOFndof",2,0,2);
          plots.AddHisto1D(selLabels_[i]+"_hN1_TOFinf1",2,0,2);
           

          plots.AddHisto1D(selLabels_[i]+"_hN1_CutFlow",3,-0.5,2.5);

      }
      //std::cout << "After plots.Addhisto1D" << std::endl;
      vcp.push_back(std::move(plots));
      //std::cout << "Pushed plots to vcp vector" << std::endl;
      
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
   vector<bool> foundOnePF(selLabels_.size(),false); 
   vector<bool> foundOneGlobal(selLabels_.size(),false); 
   vector<bool> passedThisSel(selLabels_.size(),false); 
   //std::cout << "Before loop on all HSCP, selections size =" << selections_.size() << " and sel label size : " << selLabels_.size() << std::endl; 
   
   vcp[0].FillHisto1D(selLabels_[0]+"_PreTrigEvt",1);

   for(unsigned int i=0;i<Pt.GetSize();i++){
      //std::cout<<"HSCP #" << i << " has PF muon = " << ((*isMuon.Get())[i])  << " and global muon = " << ((*isGlobalMuon.Get())[i]) << std::endl;
      //TAKE MOST IONIZING CANDIDATE

      for(int s=0;s<selections_.size();s++){
         bool (HSCPSelector::*ptr)(int);
	 ptr = selections_[s];
	 if((this->*ptr)(i)){
	    if(Ih_StripOnly[i]>maxIh[s]){
                maxIh[s]=Ih_StripOnly[i];
                iCand[s]=i;
            }
         }
      } 
      //TAKE MOST IONIZING PF AND GLOBAL MUON CANDIDATE
      /*
      for(int s=0;s<selections_.size();s++){
         bool (HSCPSelector::*ptr)(int);
	 ptr = selections_[s];
	 if((this->*ptr)(i)){
            bool hscpPFmuon = (*isMuon.Get())[i];
            bool hscpGlobalMuon = (*isGlobalMuon.Get())[i];
            if(hscpPFmuon) foundOnePF[s] = true;
            if(hscpGlobalMuon) foundOneGlobal[s] = true;
            passedThisSel[s] = true;
	    vcp[s].FillHisto1D(selLabels_[s]+"_hIh",Ih_StripOnly[i]);
	    if(Ih_StripOnly[i]>maxIh[s] && ((*isMuon.Get())[i]) && ((*isGlobalMuon.Get())[i])){
                maxIh[s]=Ih_StripOnly[i];
                iCand[s]=i;
            }
         }
      }
      */
   }

   tot+=1;
   if(passedThisSel[2]){
       passedSel+=1;
   }
   if(foundOnePF[2]){
       PFMu+=1;
   } 
   if(foundOneGlobal[2]){
       GlobalMu+=1;
   } 
   //std::cout << "After defining moost ionizing HSCP" << std::endl; 
   //End of loop over all HSCP candidates

   // Plots for the most ionizing candidate per event passing the preselection
   for(int s=0;s<selections_.size();s++){
	vcp[s].FillHisto1D(selLabels_[s]+"_hTOT",0);
        
        int i=iCand[s];
	if(i<0){
            continue;
        }
        //FILL-CPLOTS-HERE
        float invBetaErr = TOFErr[i];
        float BetaErr = (((invBetaErr)/(pow(TOF[i],2))));
	vcp[s].FillHisto1D(selLabels_[s]+"_hBetaError",BetaErr);


        double betaErrSF = SF_betaError((1/TOF[i]));

        if(betaErrSF != -1 && correctEstimators) BetaErr*=betaErrSF; 
	vcp[s].FillHisto1D(selLabels_[s]+"_hBetaErrorCorrected",BetaErr);

        
        float dedxErr = 0.27;
        if(correctEstimators) dedxErr = 0.55; 

	vcp[s].FillHisto1D(selLabels_[s]+"_hTOT",1);


        float tmpDr = 9999; 
        float tmpIdx = -1;
        float idMuonHSCP = -1;
        for(int k=0;k<muonPt.GetSize();k++){
            float dr = deltaR(eta[i],phi[i],muonEta[k],muonPhi[k]);
            if(dr < tmpDr){
                tmpDr = dr;
                tmpIdx = k;
            }
        }

        float hscpMuonP = -1;
        float hscpMuonEta = -9999;
        float hscpMuonPhi = -9999;
        float hscpMuonBeta = -9999;
        float hscpMuonBetaGamma = -9999;
        float hscpMuonPt = -1;
        float hscpMuonPtUp = -1;
        float hscpMuonPtDown = -1;

        float hscpMuonPtErr = -1;
        bool muonMatched = false;
        bool isLooseMuon = false; 
        if(tmpDr < 0.001){
            muonMatched = true;
            idMuonHSCP = tmpIdx;
            if ( (*muonIsLoose.Get())[idMuonHSCP]) isLooseMuon = true;

            if(debug) std::cout << "Global track muon pt > 200 GeV" << std::endl;
            hscpMuonP = muonPt[idMuonHSCP]*cosh(muonEta[idMuonHSCP]);
            hscpMuonPt = muonPt[idMuonHSCP];

            //hscpMuonP = globalTrackMuonPt[idMuonHSCP]*cosh(muonEta[idMuonHSCP]);
            //hscpMuonPt = globalTrackMuonPt[idMuonHSCP];

            if(debug) std::cout << "Pt = " << hscpMuonPt << std::endl;

            /*
            if(isSimulation){
                hscpMuonPtUp = rescaledPtUpGlobalMuon[idMuonHSCP];
                if(debug) std::cout << "Pt UP sys = " << hscpMuonPtUp << std::endl;
                hscpMuonPtDown = rescaledPtDownGlobalMuon[idMuonHSCP];
                if(debug) std::cout << "Pt DOWN sys = " << hscpMuonPtDown << std::endl;
            }
            */
 
            hscpMuonEta = muonEta[idMuonHSCP];
            hscpMuonPhi = muonPhi[idMuonHSCP];
            hscpMuonBeta = hscpMuonP/muonE[idMuonHSCP];
            hscpMuonBetaGamma = muonBeta[idMuonHSCP];
            hscpMuonPtErr = muonPtErr[idMuonHSCP];
            vcp[s].FillHisto2D(selLabels_[s]+"_hHSCPmuPt_vs_muPtErr",hscpMuonPt,hscpMuonPtErr);
            vcp[s].FillHisto2D(selLabels_[s]+"_hHSCPmuPt_vs_muPtErr_fine",hscpMuonPt,hscpMuonPtErr);

            vcp[s].FillHisto2D(selLabels_[s]+"_hHSCPmuPt_vs_muPtErrovermuPt",hscpMuonPt,hscpMuonPtErr/hscpMuonPt);
            vcp[s].FillHisto2D(selLabels_[s]+"_hHSCPmuPt_vs_muPtErrovermuPt_fine",hscpMuonPt,hscpMuonPtErr/hscpMuonPt);

            vcp[s].FillHisto1D(selLabels_[s]+"_hinvBetaHSCP_Data",TOF[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hTOFerrHSCP_Data",TOFErr[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hTOFndofHSCP_Data",TOF_ndof[i]);

        }
        if(selLabels_[s] == "testIhPt" && muonMatched){
            muGoodReco +=1;
            if(hscpMuonPtErr / hscpMuonPt < 0.125){
                muGoodRecoGoodError +=1;
            }
            else{
               muGoodRecoButBadError+=1; 
            } 
        }
        else if(selLabels_[s] == "testIhPt" && !muonMatched){
            muBadReco +=1;
            continue;
        }

        double Ias = Ias_StripOnly[i];
        float P = 10000./(Pt[i]*cosh(eta[i]));
        float hscpMuonPErr = cosh(hscpMuonEta)*hscpMuonPtErr;
        float PErr = cosh(eta[i])*PtErr[i];

        float Ih = Ih_StripOnly[i];
        float genP = -1;
        float genPt = -1;
        if(isSimulation){
            genPt = GenPt[i];
            genP = GenPt[i] * cosh(GenEta[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hgenP_vs_muP",genP,hscpMuonP);
            vcp[s].FillHisto2D(selLabels_[s]+"_hgenP_vs_trackP",genP,Pt[i]*cosh(eta[i]));
            vcp[s].FillHisto2D(selLabels_[s]+"_hgenPt_vs_muPt",genPt,hscpMuonPt);
            vcp[s].FillHisto2D(selLabels_[s]+"_hgenPt_vs_trackPt",genPt,Pt[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hgenBeta_vs_Beta",GenBeta[i],(1/TOF[i]));
          
            vcp[s].FillHisto2D(selLabels_[s]+"_hHSCPmuEta_vs_HSCPTrackEta",genPt,Pt[i]);

            vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta",((1/TOF[i])-GenBeta[i])/BetaErr);

            if((1/TOF[i]) >= 0.45 && (1/TOF[i]) < 0.5) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin1",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 0.5 && (1/TOF[i]) < 0.55) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin2",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 0.55 && (1/TOF[i]) < 0.6) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin3",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 0.6 && (1/TOF[i]) < 0.65) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin4",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 0.65 && (1/TOF[i]) < 0.7) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin5",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 0.7 && (1/TOF[i]) < 0.75) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin6",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 0.75 && (1/TOF[i]) < 0.8) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin7",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 0.8 && (1/TOF[i]) < 0.85) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin8",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 0.85 && (1/TOF[i]) < 0.9) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin9",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 0.9 && (1/TOF[i]) < 0.95) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin10",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 0.95 && (1/TOF[i]) < 1.0) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin11",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 1.0 && (1/TOF[i]) < 1.05) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin12",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 1.05 && (1/TOF[i]) < 1.10) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin13",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 1.10 && (1/TOF[i]) < 1.15) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin14",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 1.15) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin15",((1/TOF[i])-GenBeta[i])/BetaErr);



            if(GenBeta[i] >= 0.45 && GenBeta[i]< 0.5) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin1_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if((1/TOF[i]) >= 0.5 && GenBeta[i] < 0.55) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin2_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i]>= 0.55 && GenBeta[i] < 0.6) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin3_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i] >= 0.6 && GenBeta[i] < 0.65) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin4_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i] >= 0.65 && GenBeta[i] < 0.7) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin5_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i] >= 0.7 && GenBeta[i] < 0.75) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin6_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i] >= 0.75 && GenBeta[i] < 0.8) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin7_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i] >= 0.8 && GenBeta[i] < 0.85) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin8_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i] >= 0.85 && GenBeta[i] < 0.9) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin9_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i] >= 0.9 && GenBeta[i] < 0.95) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin10_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i] >= 0.95 && GenBeta[i] < 1.0) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin11_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i] >= 1.0 && GenBeta[i] < 1.05) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin12_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i] >= 1.05 && GenBeta[i] < 1.10) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin13_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i] >= 1.10 && GenBeta[i] < 1.15) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin14_gen",((1/TOF[i])-GenBeta[i])/BetaErr);
            if(GenBeta[i] >= 1.15) vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta_bin15_gen",((1/TOF[i])-GenBeta[i])/BetaErr);





            vcp[s].FillHisto1D(selLabels_[s]+"_hpullBeta",((1/TOF[i])-GenBeta[i])/BetaErr);

            vcp[s].FillHisto2D(selLabels_[s]+"_hgenBeta_vs_pullBeta",GenBeta[i],((1/TOF[i])-GenBeta[i])/BetaErr);
            vcp[s].FillHisto2D(selLabels_[s]+"_hrecoBeta_vs_pullBeta",(1/TOF[i]),((1/TOF[i])-GenBeta[i])/BetaErr);


            if(hscpMuonPt != -1){
                vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt > 0 && hscpMuonPt < 100) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin1",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 100 && hscpMuonPt < 200) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin2",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 200 && hscpMuonPt < 300) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin3",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 300 && hscpMuonPt < 400) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin4",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 400 && hscpMuonPt < 500) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin5",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 500 && hscpMuonPt < 600) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin6",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 600 && hscpMuonPt < 700) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin7",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 700 && hscpMuonPt < 800) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin8",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 800 && hscpMuonPt < 900) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin9",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 900 && hscpMuonPt < 1000) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin10",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 1000 && hscpMuonPt < 1100) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin11",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 1100 && hscpMuonPt < 1200) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin12",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 1200 && hscpMuonPt < 1300) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin13",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 1300 && hscpMuonPt < 1400) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin14",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(hscpMuonPt >= 1400) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin15",(hscpMuonPt-genPt)/hscpMuonPtErr);


                if(genPt > 0 && genPt < 100) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin1_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 100 && genPt < 200) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin2_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 200 && genPt < 300) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin3_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 300 && genPt < 400) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin4_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 400 && genPt < 500) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin5_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 500 && genPt < 600) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin6_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 600 && genPt < 700) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin7_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 700 && genPt < 800) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin8_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 800 && genPt < 900) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin9_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 900 && genPt < 1000) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin10_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 1000 && genPt < 1100) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin11_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 1100 && genPt < 1200) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin12_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 1200 && genPt < 1300) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin13_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 1300 && genPt < 1400) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin14_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);
                if(genPt >= 1400) vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPt_bin15_gen",(hscpMuonPt-genPt)/hscpMuonPtErr);


                vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonP",(hscpMuonP-genP)/hscpMuonPErr);

                vcp[s].FillHisto2D(selLabels_[s]+"_hgenPt_vs_pullMuonPt",genPt,((hscpMuonPt-genPt)/hscpMuonPtErr));
                vcp[s].FillHisto2D(selLabels_[s]+"_hrecoPt_vs_pullMuonPt",hscpMuonPt,((hscpMuonPt-genPt)/hscpMuonPtErr));


            }
        }

        float Fpix = float(1.0 - ProbQ_noL1[i]);
        float FpixDown = Fpix * 0.995;
        float FpixUp = Fpix*1.005;

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

        bool matchedmu = *matchedMuonWasFound.Get();
        float RECO_PFMET = *RecoPFMET;

        unsigned long long EVENT = *Event;
        float weight = *Weight; 

        //Weights for systematics PU and trigger
        
        float puSysUp = 1;
        float puSysDown = 1;
        float triggerSysUp = 1;
        float triggerSysDown = 1;
        float muTriggerSysUp = 1;
        float muTriggerSysDown = 1;
        float muRecoSysUp = 1;
        float muRecoSysDown = 1;
        float muIdSysUp = 1;
        float muIdSysDown = 1;
        /*
        if(isSimulation){
            puSysUp = *PileUpSystUp;
            puSysDown = *PileUpSystDown;
            triggerSysUp = *triggerSystFactorUp;
            triggerSysDown = *triggerSystFactorDown;
            muTriggerSysUp = *muonTriggerSFsUpEff;
            muTriggerSysDown = *muonTriggerSFsDownEff;
            muRecoSysUp = *muonRecoSFsUpEff;
            muRecoSysDown = *muonRecoSFsDownEff;
            muIdSysUp = *muonIdSFsUpEff;
            muIdSysDown = *muonIdSFsDownEff;
        }
        */
 
        unsigned int RUN = *Run;
        unsigned int PU = *PileUp;
        unsigned int NPV = *nofVtx;

        vcp[s].FillHisto2D(selLabels_[s]+"_hNPV_vs_PU",NPV,PU);
        vcp[s].FillHisto2D(selLabels_[s]+"_hPU_vs_NPV",PU,NPV);
        
        unsigned int LUMI = *Lumi;
        unsigned int nbMu = *nMuons;
        bool HLTmu = *HLT_Mu50.Get();
        //bool isPFmu = (*isMuon.Get())[i];

        float massIhnoL1 = GetMass(pt*cosh(eta[i]),Ih_noL1[i],K,C);

        double massDedx = GetMass(pt*cosh(eta[i]),Ih_StripOnly[i],K,C);
        double massDedxInit = GetMass(pt*cosh(eta[i]),Ih_StripOnly[i],K,C);

        double massDedxInitPow3_2 = GetMassPow(pt*cosh(eta[i]),Ih_StripOnly[i],K,C,1.667);

        double massBeta = GetMassBeta(pt*cosh(eta[i]),(1./TOF[i]));

        double massCombined = GetCombMass(massDedx,massBeta);

        double massAtlas=-1;
        double massAtlaspTUp = -1;
        double massAtlaspTDown = -1;
 
        double sigmaMassAtlas = -1;

        int badSel = 0;

        if(selLabels_[s] == "testIhPt" && computeAtlasMass){
            if(Ih_StripOnly[i]>=3.1945 && Ih_StripOnly[i]<40){
                auto closestXIterator = std::min_element(ih_betagamma_error.begin(), ih_betagamma_error.end(), [&](const auto& t1, const auto& t2) {
                    return std::abs(std::get<0>(t1) - Ih_StripOnly[i]) < std::abs(std::get<0>(t2) - Ih_StripOnly[i]);
                });
                

                if (closestXIterator != ih_betagamma_error.end()) {
                    double closestX = std::get<0>(*closestXIterator);
                    double associatedBG = std::get<1>(*closestXIterator);
                    double error = std::get<2>(*closestXIterator);
                    if (closestXIterator != ih_betagamma_error.begin()) {
                        auto prevIterator = closestXIterator - 1;
                        if (std::abs(Ih_StripOnly[i] - std::get<0>(*closestXIterator)) > std::abs(Ih_StripOnly[i] - std::get<0>(*prevIterator))) {
                            closestXIterator = prevIterator;
                            closestX = std::get<0>(*closestXIterator);
                            associatedBG = std::get<1>(*closestXIterator);
                            error = std::get<2>(*closestXIterator);
                        }
                    }  
                    massAtlas= (hscpMuonPt*cosh(hscpMuonEta))*1.0/associatedBG;
                    /*
                    if(isSimulation){
                        massAtlaspTUp = (hscpMuonPtUp*cosh(hscpMuonEta))*1.0/associatedBG;
                        massAtlaspTDown = (hscpMuonPtDown*cosh(hscpMuonEta))*1.0/associatedBG;
                    }
                    */
                    double partial_pT = cosh(hscpMuonEta) / associatedBG;
                    //double df_dedx = partial_df_dedx(associatedBG,0.0296054219691,-16.7347572514,1.31501838859,5.42756057418);
                    double df_dp = 1.0;
                    //sigmaMassAtlas = calculateErrorOnMass(df_dedx, df_dp, dedxErr, hscpMuonPtErr);
                    sigmaMassAtlas = fabs(partial_pT) * hscpMuonPtErr;

 
                }

            } 
        }
        else{
            badSel++;
        }



 
        //USING MUONS 
        double massDedxMuonInit = GetMass(hscpMuonPt*cosh(hscpMuonEta),Ih_StripOnly[i],K,C); 
        double massMuonBeta = GetMassBeta(hscpMuonPt*cosh(hscpMuonEta),(1./TOF[i]));


        float sigmaMassBetaNoPMuon = errorMassBeta(hscpMuonPt*cosh(hscpMuonEta), (1./TOF[i]), BetaErr);

        float sigmaMassBetaMuon = errorMassBetaWithP(hscpMuonPt*cosh(hscpMuonEta), (1./TOF[i]), BetaErr,hscpMuonPErr);


        float sigmaMassDedxNoPMuon = errorMassDedx(hscpMuonPt*cosh(hscpMuonEta),Ih_StripOnly[i],K,C,dedxErr);
        
        float sigmaMassDedxMuon = errorMassWithP(hscpMuonPt*cosh(hscpMuonEta),Ih_StripOnly[i],K,C,dedxErr,hscpMuonPErr);

        //JUST TO SHOW BEFORE / AFTER
        float sigmaMassBetaBasicTrack = errorMassBeta(Pt[i]*cosh(eta[i]), (1./TOF[i]), BetaErr);
        float sigmaMassBetaBasicTrackWithP = errorMassBetaWithP(Pt[i]*cosh(eta[i]), (1./TOF[i]), BetaErr,PErr);

        float sigmaMassDedxBasicTrack = errorMassDedx(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C,dedxErr);
        float sigmaMassDedxBasicTrackWithP = errorMassWithP(Pt[i]*cosh(eta[i]),Ih_StripOnly[i],K,C,dedxErr,PErr);

        //WITH P ERROR
        float CorrelationFactorMuon = 0.0;
        float CorrelationFactor = 0.0;
  
        float sigmaMassCombMuon = sqrt( (sigmaMassBetaNoPMuon*sigmaMassBetaNoPMuon) + (sigmaMassDedxNoPMuon*sigmaMassDedxNoPMuon) + 2 * CorrelationFactorMuon * sigmaMassBetaNoPMuon * sigmaMassDedxNoPMuon);

        float sigmaMassCombMuonWithP = sqrt( (sigmaMassBetaMuon*sigmaMassBetaMuon) + (sigmaMassDedxMuon*sigmaMassDedxMuon) + 2 * CorrelationFactorMuon * sigmaMassBetaNoPMuon * sigmaMassDedxNoPMuon);
        
        float sigmaMassComb = sqrt( (sigmaMassBetaBasicTrackWithP*sigmaMassBetaBasicTrackWithP) + (sigmaMassDedxBasicTrackWithP*sigmaMassDedxBasicTrackWithP) + 2 * CorrelationFactor * sigmaMassBetaBasicTrackWithP * sigmaMassDedxBasicTrackWithP );



        double massCombinedWeighted = GetCombMassWeighted(massBeta, massDedx,sigmaMassBetaBasicTrackWithP, sigmaMassDedxBasicTrackWithP);

        double massCombinedMuonWeighted = GetCombMassWeighted(massMuonBeta, massDedxMuonInit,sigmaMassBetaMuon, sigmaMassDedxMuon);
 
        double massCombinedMuonWeightednoP = GetCombMassWeighted(massMuonBeta, massDedxMuonInit,sigmaMassBetaNoPMuon, sigmaMassDedxNoPMuon); 
        //HERE TO CHANGE USING ATLAS MASS

        massCombinedMuonWeighted = GetCombMassWeighted(massMuonBeta, massAtlas,sigmaMassBetaNoPMuon, sigmaMassAtlas);
        double massCombinedMuonWeightedWithP = GetCombMassWeighted(massMuonBeta, massAtlas,sigmaMassBetaMuon, sigmaMassAtlas);

        double testErrorMCombined = 1.0 / sqrt( (1.0/(sigmaMassBetaMuon*sigmaMassBetaMuon) ) + (1.0 /( sigmaMassAtlas*sigmaMassAtlas)));
         
        double massForRegions = massCombinedMuonWeighted;
        
        double massCombinedMuonWeightedpTUp =  -1;
        double massCombinedMuonWeightedpTDown =  -1;
        /*
        if(isSimulation){
            massCombinedMuonWeightedpTUp =  GetCombMassWeighted(massMuonBeta, massAtlaspTUp,sigmaMassBetaNoPMuon, sigmaMassAtlas);
            massCombinedMuonWeightedpTDown =  GetCombMassWeighted(massMuonBeta, massAtlaspTDown,sigmaMassBetaNoPMuon, sigmaMassAtlas);
        }
        */

        massDedxInit = massAtlas;
        massBeta = massMuonBeta;
        massCombinedWeighted = massCombinedMuonWeighted;

        float combErr = sqrt(BetaErr*BetaErr + dedxErr*dedxErr); 
 

        float chi2CombTrackNoSquare =  (pow((massBeta-massCombinedWeighted),2)/sigmaMassBetaBasicTrackWithP) + (pow((massDedxInit-massCombinedWeighted),2)/sigmaMassDedxBasicTrackWithP);
        float chi2CombTrackSquare =  (pow((massBeta-massCombinedWeighted),2)/pow(sigmaMassBetaBasicTrackWithP,2)) + (pow((massDedxInit-massCombinedWeighted),2)/pow(sigmaMassDedxBasicTrackWithP,2));


        float chi2CombMuonNoSquare =  (pow((massMuonBeta-massCombinedMuonWeighted),2)/sigmaMassBetaNoPMuon) + (pow((massDedxMuonInit-massCombinedMuonWeighted),2)/sigmaMassDedxNoPMuon);
        float chi2CombMuonSquare =  (pow((massMuonBeta-massCombinedMuonWeighted),2)/pow(sigmaMassBetaNoPMuon,2)) + (pow((massDedxMuonInit-massCombinedMuonWeighted),2)/pow(sigmaMassDedxNoPMuon,2));

        //if(selLabels_[s] == "testIhPt") std::cout << "This is signal and chi2CombMuonSquare = " << chi2CombMuonSquare << std::endl;

        if(selLabels_[s] == "testIhPt" && debug){
            std::cout << "Candidate has muon p = " << hscpMuonPt*cosh(hscpMuonEta) << " with p error = " << hscpMuonPErr << " , muon TOF = " << 1/TOF[i] << " with TOF error = " << BetaErr << std::endl;

            std::cout << "MASSES AND ERRORS --- : " << std::endl;
            std::cout << "   Using muons : Mass beta = " << massMuonBeta << " +/-" << sigmaMassBetaNoPMuon << " << | Mass DEDX = " << massDedxMuonInit << " +/- " << sigmaMassDedxNoPMuon << " | Mass COMBINED = " << massCombinedMuonWeighted << " +/- " << sigmaMassCombMuon << std::endl;
            std::cout << "          CHI2 bottom squared :  = " << chi2CombMuonSquare << std::endl;
            std::cout << "          CHI2 bottom not squared :  = " << chi2CombMuonNoSquare << std::endl;
            std::cout << "   Using tracks : Mass beta = " << massBeta << " +/-" << sigmaMassBetaBasicTrack << " << | Mass DEDX = " << massDedxInit << " +/- " << sigmaMassDedxBasicTrack << " | Mass COMBINED = " << massCombinedWeighted << " +/- " <<  sigmaMassComb <<std::endl;
        }


        vcp[s].FillHisto2D(selLabels_[s]+"_hPMuonOverM_vs_Ih",( (hscpMuonPt*cosh(hscpMuonEta))/massPointSig_*1.0),IhStrip);
        vcp[s].FillHisto2D(selLabels_[s]+"_hPTrackOverM_vs_Ih",( (hscpMuonPt*cosh(hscpMuonEta))/massPointSig_*1.0),IhStrip);

        vcp[s].FillHisto2D(selLabels_[s]+"_hPTrack_vs_Ih",pt*cosh(eta[i]),IhStrip);
        vcp[s].FillHisto2D(selLabels_[s]+"_hPMuon_vs_Ih",hscpMuonPt*cosh(hscpMuonEta),IhStrip);

        vcp[s].FillHisto2D(selLabels_[s]+"_hMassDedx_vs_MassBeta_chi2ALL",massDedxInit,massBeta);

        //vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonSquare_dedx_beta_comb_tight_preCut",chi2CombMuonSquare);

        bool passCR = (hscpMuonPt >= ptcut_ && (Fpix > fpix3) && (Fpix <= fpix8));
        bool passSR = (hscpMuonPt >= ptcut_ && (Fpix > fpix9) && Fpix <= fpix10);

        bool passSRfpixUp = false;
        bool passSRfpixDown = false;
        bool passSRpTUp = false;
        bool passSRpTDown = false;

        /*
        if(isSimulation){
            passSRfpixUp = (hscpMuonPt >= ptcut_ && (FpixUp > fpix9) && Fpix <= fpix10);
            passSRfpixDown = (hscpMuonPt >= ptcut_ && (FpixDown > fpix9) && Fpix <= fpix10);

            passSRpTUp = (hscpMuonPtUp >= ptcut_ && (FpixUp > fpix9) && Fpix <= fpix10);
            passSRpTDown = (hscpMuonPtDown >= ptcut_ && (FpixUp > fpix9) && Fpix <= fpix10);
        }
        */
        
        vcp[s].FillHisto1D(selLabels_[s]+"_hmassAtlas_lowMass",massAtlas);
        vcp[s].FillHisto1D(selLabels_[s]+"_hmassIh_lowMass",massDedx);
        if(hscpMuonPt > ptcut_){
            if((Fpix > fpix3) && (Fpix <= fpix8)){
                if(TOF[i] > 1.1 && Ih_StripOnly[i] > 3.6) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionC_3fp8_ih3p60_beta1p10",massCombinedMuonWeighted);
                if(TOF[i] > 1.12 && Ih_StripOnly[i] > 3.7) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionC_3fp8_ih3p70_beta1p12",massCombinedMuonWeighted);
                if(TOF[i] > 1.13 && Ih_StripOnly[i] > 3.89) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionC_3fp8_ih3p89_beta1p13",massCombinedMuonWeighted);
                if(TOF[i] > 1.15 && Ih_StripOnly[i] > 3.95) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionC_3fp8_ih3p95_beta1p15",massCombinedMuonWeighted);
                if(TOF[i] > 1.17 && Ih_StripOnly[i] > 4.04) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionC_3fp8_ih4p04_beta1p17",massCombinedMuonWeighted);

                if(TOF[i] > 1.1 && Ih_StripOnly[i] > 3.6) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionC_3fp8_ih3p60_beta1p10",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.12 && Ih_StripOnly[i] > 3.7) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionC_3fp8_ih3p70_beta1p12",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.13 && Ih_StripOnly[i] > 3.89) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionC_3fp8_ih3p89_beta1p13",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.15 && Ih_StripOnly[i] > 3.95) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionC_3fp8_ih3p95_beta1p15",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.17 && Ih_StripOnly[i] > 4.04) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionC_3fp8_ih4p04_beta1p17",massCombinedMuonWeightednoP);

            }
            if((Fpix > fpix8) && (Fpix <= fpix9)){
                if(TOF[i] > 1.1 && Ih_StripOnly[i] > 3.6) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionD_8fp9_ih3p60_beta1p10",massCombinedMuonWeighted);
                if(TOF[i] > 1.12 && Ih_StripOnly[i] > 3.7) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionD_8fp9_ih3p70_beta1p12",massCombinedMuonWeighted);
                if(TOF[i] > 1.13 && Ih_StripOnly[i] > 3.89) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionD_8fp9_ih3p89_beta1p13",massCombinedMuonWeighted);
                if(TOF[i] > 1.15 && Ih_StripOnly[i] > 3.95) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionD_8fp9_ih3p95_beta1p15",massCombinedMuonWeighted);
                if(TOF[i] > 1.17 && Ih_StripOnly[i] > 4.04) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionD_8fp9_ih4p04_beta1p17",massCombinedMuonWeighted);

                if(TOF[i] > 1.1 && Ih_StripOnly[i] > 3.6) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionD_8fp9_ih3p60_beta1p10",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.12 && Ih_StripOnly[i] > 3.7) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionD_8fp9_ih3p70_beta1p12",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.13 && Ih_StripOnly[i] > 3.89) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionD_8fp9_ih3p89_beta1p13",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.15 && Ih_StripOnly[i] > 3.95) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionD_8fp9_ih3p95_beta1p15",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.17 && Ih_StripOnly[i] > 4.04) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionD_8fp9_ih4p04_beta1p17",massCombinedMuonWeightednoP);
            }
            if((Fpix > fpix9) && (Fpix <= fpix10)){
                if(TOF[i] > 1.1 && Ih_StripOnly[i] > 3.6) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionD_9fp10_ih3p60_beta1p10",massCombinedMuonWeighted);
                if(TOF[i] > 1.12 && Ih_StripOnly[i] > 3.7) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionD_9fp10_ih3p70_beta1p12",massCombinedMuonWeighted);
                if(TOF[i] > 1.13 && Ih_StripOnly[i] > 3.89) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionD_9fp10_ih3p89_beta1p13",massCombinedMuonWeighted);
                if(TOF[i] > 1.15 && Ih_StripOnly[i] > 3.95) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionD_9fp10_ih3p95_beta1p15",massCombinedMuonWeighted);
                if(TOF[i] > 1.17 && Ih_StripOnly[i] > 4.04) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionD_9fp10_ih4p04_beta1p17",massCombinedMuonWeighted);

                if(TOF[i] > 1.1 && Ih_StripOnly[i] > 3.6) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionD_9fp10_ih3p60_beta1p10",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.12 && Ih_StripOnly[i] > 3.7) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionD_9fp10_ih3p70_beta1p12",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.13 && Ih_StripOnly[i] > 3.89) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionD_9fp10_ih3p89_beta1p13",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.15 && Ih_StripOnly[i] > 3.95) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionD_9fp10_ih3p95_beta1p15",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.17 && Ih_StripOnly[i] > 4.04) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionD_9fp10_ih4p04_beta1p17",massCombinedMuonWeightednoP);
            }
        }
        if(hscpMuonPt <= ptcut_){
            if((Fpix > fpix3) && (Fpix <= fpix8)){
                if(TOF[i] > 1.1 && Ih_StripOnly[i] > 3.6) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionA_3fp8_ih3p60_beta1p10",massCombinedMuonWeighted);
                if(TOF[i] > 1.12 && Ih_StripOnly[i] > 3.7) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionA_3fp8_ih3p70_beta1p12",massCombinedMuonWeighted);
                if(TOF[i] > 1.13 && Ih_StripOnly[i] > 3.89) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionA_3fp8_ih3p89_beta1p13",massCombinedMuonWeighted);
                if(TOF[i] > 1.15 && Ih_StripOnly[i] > 3.95) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionA_3fp8_ih3p95_beta1p15",massCombinedMuonWeighted);
                if(TOF[i] > 1.17 && Ih_StripOnly[i] > 4.04) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionA_3fp8_ih4p04_beta1p17",massCombinedMuonWeighted);

                if(TOF[i] > 1.1 && Ih_StripOnly[i] > 3.6) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionA_3fp8_ih3p60_beta1p10",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.12 && Ih_StripOnly[i] > 3.7) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionA_3fp8_ih3p70_beta1p12",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.13 && Ih_StripOnly[i] > 3.89) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionA_3fp8_ih3p89_beta1p13",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.15 && Ih_StripOnly[i] > 3.95) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionA_3fp8_ih3p95_beta1p15",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.17 && Ih_StripOnly[i] > 4.04) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionA_3fp8_ih4p04_beta1p17",massCombinedMuonWeightednoP);

            }
            if((Fpix > fpix8) && (Fpix <= fpix9)){
                if(TOF[i] > 1.1 && Ih_StripOnly[i] > 3.6) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionB_8fp9_ih3p60_beta1p10",massCombinedMuonWeighted);
                if(TOF[i] > 1.12 && Ih_StripOnly[i] > 3.7) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionB_8fp9_ih3p70_beta1p12",massCombinedMuonWeighted);
                if(TOF[i] > 1.13 && Ih_StripOnly[i] > 3.89) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionB_8fp9_ih3p89_beta1p13",massCombinedMuonWeighted);
                if(TOF[i] > 1.15 && Ih_StripOnly[i] > 3.95) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionB_8fp9_ih3p95_beta1p15",massCombinedMuonWeighted);
                if(TOF[i] > 1.17 && Ih_StripOnly[i] > 4.04) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionB_8fp9_ih4p04_beta1p17",massCombinedMuonWeighted);

                if(TOF[i] > 1.1 && Ih_StripOnly[i] > 3.6) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionB_8fp9_ih3p60_beta1p10",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.12 && Ih_StripOnly[i] > 3.7) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionB_8fp9_ih3p70_beta1p12",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.13 && Ih_StripOnly[i] > 3.89) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionB_8fp9_ih3p89_beta1p13",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.15 && Ih_StripOnly[i] > 3.95) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionB_8fp9_ih3p95_beta1p15",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.17 && Ih_StripOnly[i] > 4.04) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionB_8fp9_ih4p04_beta1p17",massCombinedMuonWeightednoP);

            }
            if((Fpix > fpix9) && (Fpix <= fpix10)){
                if(TOF[i] > 1.1 && Ih_StripOnly[i] > 3.6) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionB_9fp10_ih3p60_beta1p10",massCombinedMuonWeighted);
                if(TOF[i] > 1.12 && Ih_StripOnly[i] > 3.7) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionB_9fp10_ih3p70_beta1p12",massCombinedMuonWeighted);
                if(TOF[i] > 1.13 && Ih_StripOnly[i] > 3.89) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionB_9fp10_ih3p89_beta1p13",massCombinedMuonWeighted);
                if(TOF[i] > 1.15 && Ih_StripOnly[i] > 3.95) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionB_9fp10_ih3p95_beta1p15",massCombinedMuonWeighted);
                if(TOF[i] > 1.17 && Ih_StripOnly[i] > 4.04) vcp[s].FillHisto1D(selLabels_[s]+"_hmass_regionB_9fp10_ih4p04_beta1p17",massCombinedMuonWeighted);

                if(TOF[i] > 1.1 && Ih_StripOnly[i] > 3.6) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionB_9fp10_ih3p60_beta1p10",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.12 && Ih_StripOnly[i] > 3.7) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionB_9fp10_ih3p70_beta1p12",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.13 && Ih_StripOnly[i] > 3.89) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionB_9fp10_ih3p89_beta1p13",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.15 && Ih_StripOnly[i] > 3.95) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionB_9fp10_ih3p95_beta1p15",massCombinedMuonWeightednoP);
                if(TOF[i] > 1.17 && Ih_StripOnly[i] > 4.04) vcp[s].FillHisto1D(selLabels_[s]+"_hmassnoP_regionB_9fp10_ih4p04_beta1p17",massCombinedMuonWeightednoP);
            }
        }

        if(passCR){
            vcp[s].FillHisto1D(selLabels_[s]+"_hTOF_CR_3fp8",TOF[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hIh_CR_3fp8",Ih_StripOnly[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hmassAtlas_CR_3fp8",massAtlas);
            vcp[s].FillHisto1D(selLabels_[s]+"_hmassIh_CR_3fp8",massDedx);

            vcp[s].FillHisto2D(selLabels_[s]+"_hFpix_vs_PFinal_CR_3fp8",Fpix,hscpMuonPt*cosh(hscpMuonEta));
            vcp[s].FillHisto2D(selLabels_[s]+"_hFpix_vs_BetaFinal_CR_3fp8",Fpix,TOF[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hTOF_vs_Ih_CR_3fp8",TOF[i],Ih_StripOnly[i]);

        }
        if(passSR){
            vcp[s].FillHisto2D(selLabels_[s]+"_hTOF_vs_Ih_SR_9fp10",TOF[i],Ih_StripOnly[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hTOF_SR_9fp10",TOF[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hIh_SR_9fp10",Ih_StripOnly[i]);

        }

        if(passCR) vcp[s].FillHisto2D(selLabels_[s]+"_hMassBeta_MassBetaErr_CR",massBeta,sigmaMassBetaNoPMuon);
        if(passCR) vcp[s].FillHisto2D(selLabels_[s]+"_hMassDedx_MassDedxErr_CR",massDedxInit,sigmaMassDedxNoPMuon);
        if(passCR) vcp[s].FillHisto2D(selLabels_[s]+"_hMassComb_MassCombErr_CR",massCombinedWeighted,sigmaMassComb);

        if(passCR) vcp[s].FillHisto2D(selLabels_[s]+"_hMassDedx_vs_MassBeta_chi2ALL_CR",massDedxInit,massBeta);
        if(passSR) vcp[s].FillHisto2D(selLabels_[s]+"_hMassDedx_vs_MassBeta_chi2ALL_SR",massDedxInit,massBeta);

        
        if(chi2CombMuonSquare > 55){            
            if(passCR) vcp[s].FillHisto2D(selLabels_[s]+"_hMassDedx_vs_MassBeta_chi2FAIL_CR",massDedxInit,massBeta);
            if(passCR) vcp[s].FillHisto2D(selLabels_[s]+"_hMassDedx_vs_MassBeta_chi2FAIL_neg_CR",massDedxInit,massBeta);
            if(passSR) vcp[s].FillHisto2D(selLabels_[s]+"_hMassDedx_vs_MassBeta_chi2FAIL_SR",massDedxInit,massBeta);
            if(passSR) vcp[s].FillHisto2D(selLabels_[s]+"_hMassDedx_vs_MassBeta_chi2FAIL_neg_SR",massDedxInit,massBeta);
            //continue;
        }


        if(chi2CombMuonSquare < 55){
            if(passCR){
                vcp[s].FillHisto2D(selLabels_[s]+"_hMassDedx_vs_MassBeta_chi2PASS_CR",massDedxInit,massBeta);
            }
            if(passSR){
                vcp[s].FillHisto2D(selLabels_[s]+"_hMassDedx_vs_MassBeta_chi2PASS_SR",massDedxInit,massBeta);
            }
        }

        float diffDivQuadraticErr = ( (massBeta - massDedxInit)*1.0) / sqrt( (sigmaMassBetaNoPMuon*sigmaMassBetaNoPMuon) + (sigmaMassDedxNoPMuon*sigmaMassDedxNoPMuon) );

        //Number of tracks before any trigger or preselection, ann inclusive        
        if(doPreselPlots && selLabels_[s] =="allInclusive") vcp[s].FillHisto1D(selLabels_[s]+"_hN1_CutFlow",0);

        if(doPreselPlots && selLabels_[s] == "allInclusive"){
            bool passedCutsArray[2];
            std::fill(std::begin(passedCutsArray), std::end(passedCutsArray),false);
            passedCutsArray[0]  = HLTmu;
            passedCutsArray[1]  = (NOPH[i] >= 2) && (FOVH[i] > 0.8) && (NOM[i] >= 10) && ((*isHighPurity.Get())[i]) && ((Chi2[i]/Ndof[i]) < 5.0) && (abs(dZ[i]) < 0.1) && (abs(dXY[i]) < 0.02) && (PFMiniIso_relative[i] < 0.02) && (EoverP[i] < 0.3) && ((PtErr[i]/(Pt[i]*Pt[i]))<0.0008) && ((PtErr[i]/(Pt[i]))<1) && (track_genTrackAbsIsoSumPtFix[i] < 15.0) && (Ih_StripOnly[i]>3.14) && (abs(eta[i]) < 1) && Pt[i] > 55 && Ias > quan999 && (ProbQ_noL1[i] < 0.7);

            for (size_t k=0;k<sizeof(passedCutsArray);k++) {
                bool allCutsPassedSoFar = true;
                for (size_t j=0;j<=k;j++) {
                    if(!passedCutsArray[j]) {
                        allCutsPassedSoFar = false;
                        break;
                    }
                }
                if (allCutsPassedSoFar) {
                   vcp[s].FillHisto1D(selLabels_[s]+"_hN1_CutFlow",k+1);  
                }
            }
        } 

        //bool isPFmuon =  (*isMuon.Get())[i];

        double dimuMass = -1;
        double idMu1 = -1;
        double idMu2 = -1;
        if(CalibrationZmumu){
           double minZm;
           double maxZm;
           if(dataset_ == "zmumu50_120"){
               minZm = 88;
               maxZm = 94;
           }
           if(dataset_ == "zmumu200_400"){
               minZm = 190;
               maxZm = 240;
           }
           if(dataset_ == "zmumu400_800"){
               minZm = 390;
               maxZm = 440;
           }
           if(dataset_ == "zmumu800_1400"){
               minZm = 780;
               maxZm = 850;
           }
           //Prendre un cut en pT > 25 GeV | deux muons dos a dos, reconstruire la masse invariante 
           bool foundZmumu = false;
           TLorentzVector muon1, muon2;
           for(unsigned int nm1 = 0; nm1 < nbMu; nm1++){
               for(unsigned int nm2 = nm1+1; nm2 < nbMu; nm2++){
                  if (muonPt[nm1] > 25 && muonPt[nm2] > 25){
                      muon1.SetPtEtaPhiM(muonPt[nm1], muonEta[nm1], muonPhi[nm1], 0.105658);
                      muon2.SetPtEtaPhiM(muonPt[nm2], muonEta[nm2], muonPhi[nm2], 0.105658);
                  }
                  if (muon1.DeltaR(muon2) > M_PI - 0.1) {
                      TLorentzVector dimuon = muon1 + muon2;
                      dimuMass = dimuon.M();

                      if(!isSimulation){
                          if (dimuMass > 88 && dimuMass < 94){ 
                              idMu1 = nm1;
                              idMu2 = nm2;
                              foundZmumu = true;
                              break;
                          }
                      }
                      else{
                          if (dimuMass > minZm && dimuMass < maxZm){ 
                              idMu1 = nm1;
                              idMu2 = nm2;
                              foundZmumu = true;
                              break;
                          }
                 
                      }
                  }  
               }
               if(foundZmumu) break;
           }
           int test = 0;
        }

        //ID FOR BOTH GEN MUON MATCHED TO Z MUMU 
        int muon1GenIndex = -1;
        float mindrMu1 = 9999;
        int muon2GenIndex = -1;
        float mindrMu2 = 9999;
         
        if( (isSimulation==true) && (idMu1 != -1) && (idMu2 != -1) ){

            vcp[s].FillHisto1D(selLabels_[s]+"_hinvBetaHSCP_MC",TOF[i]); 
            vcp[s].FillHisto1D(selLabels_[s]+"_hTOFerrHSCP_MC",TOFErr[i]); 
            vcp[s].FillHisto1D(selLabels_[s]+"_hTOFndofHSCP_MC",TOF_ndof[i]); 

            for (unsigned int genp = 0; genp < gParticlePhi.GetSize(); genp++) {
                if(abs(gParticleEta[genp])>10 || abs(gParticlePhi[genp]) > 10) continue;
                float tempDr1 = deltaR(gParticleEta[genp],gParticlePhi[genp],muonEta[idMu1],muonPhi[idMu1]);
                float tempDr2 = deltaR(gParticleEta[genp],gParticlePhi[genp],muonEta[idMu2],muonPhi[idMu2]);
                if (tempDr1 < mindrMu1){
                    mindrMu1 = tempDr1;
                    muon1GenIndex = genp;
                }
                if (tempDr2 < mindrMu2){
                    mindrMu2 = tempDr2;
                    muon2GenIndex = genp;
                }
            }

            float drMu1HSCP = deltaR(hscpMuonEta,hscpMuonPhi,muonEta[idMu1],muonPhi[idMu1]);
            float drMu2HSCP = deltaR(hscpMuonEta,hscpMuonPhi,muonEta[idMu2],muonPhi[idMu2]);

            if(drMu1HSCP<0.001 && mindrMu1 < 0.001){
                vcp[s].FillHisto1D(selLabels_[s]+"_hpullBetaZmumu",((1/TOF[i])-gParticleBeta[muon1GenIndex])/BetaErr);
                vcp[s].FillHisto1D(selLabels_[s]+"_hpullinvBetaZmumu",((TOF[i])-(1/gParticleBeta[muon1GenIndex]))/TOFErr[i]);
                vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPtZmumu",(hscpMuonPt-gParticlePt[muon1GenIndex])/hscpMuonPtErr);
                vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPZmumu",(hscpMuonP-(gParticlePt[muon1GenIndex]*cosh(gParticleEta[muon1GenIndex])) )/hscpMuonPErr);

                vcp[s].FillHisto1D(selLabels_[s]+"_hinvBetaZmumu_MC",TOF[i]); 
                vcp[s].FillHisto1D(selLabels_[s]+"_hBetaZmumu_MC",1.0/TOF[i]); 
                vcp[s].FillHisto1D(selLabels_[s]+"_hTOFerrZmumu_MC",TOFErr[i]); 
                if(selLabels_[i] == "testCalibration"){

                    double WeightForEta = 1.0;
                    for(size_t i = 0; i < etaMin.size(); ++i) {
                        if (hscpMuonEta >= etaMin[i] && hscpMuonEta < etaMax[i]) {
                            WeightForEta = etaWeights[i]; 
                            break;
                        }
                    }
                    if(WeightForEta != 0){
                        vcp[s].FillHisto1D(selLabels_[s]+"_hTOFndofZmumu_MC",TOF_ndof[i],WeightForEta);
                        vcp[s].FillHisto1D(selLabels_[s]+"_hEtaReweighted",hscpMuonEta,WeightForEta);
                    }
                } 
                else{
                    vcp[s].FillHisto1D(selLabels_[s]+"_hTOFndofZmumu_MC",TOF_ndof[i]); 
                }

                vcp[s].FillHisto2D(selLabels_[s]+"_hBeta_vs_betaMuon",(1/TOF[i]),hscpMuonBeta); 
                vcp[s].FillHisto2D(selLabels_[i]+"_hEta_vs_TOFerr_Zmumu_MC",hscpMuonEta,TOFErr[i]);

                vcp[s].FillHisto2D(selLabels_[s]+"_hgenPt_vs_muPtZmumu",genPt,hscpMuonPt);
                vcp[s].FillHisto2D(selLabels_[s]+"_hgenPt_vs_trackPtZmumu",genPt,Pt[i]);
            }

            else if(drMu2HSCP<0.001 && mindrMu2 < 0.001 ){
                vcp[s].FillHisto1D(selLabels_[s]+"_hpullBetaZmumu",((1/TOF[i])-gParticleBeta[muon2GenIndex])/BetaErr);
                vcp[s].FillHisto1D(selLabels_[s]+"_hpullinvBetaZmumu",((TOF[i])-(1/gParticleBeta[muon2GenIndex]))/TOFErr[i]);
                vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPtZmumu",(hscpMuonPt-gParticlePt[muon2GenIndex])/hscpMuonPtErr);
                vcp[s].FillHisto1D(selLabels_[s]+"_hpullMuonPZmumu",(hscpMuonP-(gParticlePt[muon2GenIndex]*cosh(gParticleEta[muon2GenIndex])) )/hscpMuonPErr);
                vcp[s].FillHisto1D(selLabels_[s]+"_hinvBetaZmumu_MC",TOF[i]); 
                vcp[s].FillHisto1D(selLabels_[s]+"_hBetaZmumu_MC",1.0/TOF[i]); 
                vcp[s].FillHisto1D(selLabels_[s]+"_hTOFerrZmumu_MC",TOFErr[i]); 
                vcp[s].FillHisto1D(selLabels_[s]+"_hTOFndofZmumu_MC",TOF_ndof[i]); 

                vcp[s].FillHisto2D(selLabels_[s]+"_hBeta_vs_betaMuon",(1.0/TOF[i]),hscpMuonBeta); 
                vcp[s].FillHisto2D(selLabels_[i]+"_hEta_vs_TOFerr_Zmumu_MC",hscpMuonEta,TOFErr[i]);

                vcp[s].FillHisto2D(selLabels_[s]+"_hgenPt_vs_muPtZmumu",genPt,hscpMuonPt);
                vcp[s].FillHisto2D(selLabels_[s]+"_hgenPt_vs_trackPtZmumu",genPt,Pt[i]);
            }
        } 
        if(!isSimulation && idMu1 != -1 && idMu2 !=-1){
            float data_drmu1ZHSCP = deltaR(hscpMuonEta,hscpMuonPhi,muonEta[idMu1],muonPhi[idMu1]);
            float data_drmu2ZHSCP = deltaR(hscpMuonEta,hscpMuonPhi,muonEta[idMu2],muonPhi[idMu2]);
            if(data_drmu1ZHSCP < 0.001){
                vcp[s].FillHisto1D(selLabels_[s]+"_hinvBetaZmumu_Data",TOF[i]); 
                vcp[s].FillHisto1D(selLabels_[s]+"_hBetaZmumu_Data",1.0/TOF[i]); 
                vcp[s].FillHisto1D(selLabels_[s]+"_hTOFerrZmumu_Data",TOFErr[i]); 
                vcp[s].FillHisto1D(selLabels_[s]+"_hTOFndofZmumu_Data",TOF_ndof[i]); 
                vcp[s].FillHisto2D(selLabels_[i]+"_hEta_vs_TOFerr_Zmumu_Data",hscpMuonEta,TOFErr[i]);
            }
            if(data_drmu2ZHSCP < 0.001){
                vcp[s].FillHisto1D(selLabels_[s]+"_hinvBetaZmumu_Data",TOF[i]); 
                vcp[s].FillHisto1D(selLabels_[s]+"_hBetaZmumu_Data",1.0/TOF[i]); 
                vcp[s].FillHisto1D(selLabels_[s]+"_hTOFerrZmumu_Data",TOFErr[i]); 
                vcp[s].FillHisto1D(selLabels_[s]+"_hTOFndofZmumu_Data",TOF_ndof[i]); 
                vcp[s].FillHisto2D(selLabels_[i]+"_hEta_vs_TOFerr_Zmumu_Data",hscpMuonEta,TOFErr[i]);
            }
        }


        if(do2Dplots){ 

            if (idMuonHSCP != -1){
                vcp[s].FillHisto2D(selLabels_[s]+"_hmuonPt_vs_trackPt",muonPt[idMuonHSCP],Pt[i]);
                vcp[s].FillHisto2D(selLabels_[s]+"_hHSCPmuEta_vs_HSCPTrackEta",muonEta[idMuonHSCP],eta[i]);
                vcp[s].FillHisto1D(selLabels_[s]+"_hmuMatchedHSCP_dr0p15",1);
            }
            else{
                vcp[s].FillHisto1D(selLabels_[s]+"_hmuMatchedHSCP_dr0p15",0);
            }

            vcp[s].FillHisto2D(selLabels_[s]+"_hTOFerr_vs_TOF",TOFErr[i],TOF[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hEta_vs_TOFerr",eta[i],TOFErr[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hEta_vs_TOFndof",eta[i],TOF_ndof[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hTOFndof_vs_TOF",TOF_ndof[i],TOF[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hTOFndof_vs_TOFerr",TOF_ndof[i],TOFErr[i]);


            vcp[s].FillHisto2D(selLabels_[s]+"_hDTTOFerr_vs_DTTOF",DTTOFErr[i],DTTOF[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hEta_vs_DTTOFerr",eta[i],DTTOFErr[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hEta_vs_DTTOFndof",eta[i],DTTOF_ndof[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hDTTOFndof_vs_DTTOF",DTTOF_ndof[i],DTTOF[i]);


            vcp[s].FillHisto2D(selLabels_[s]+"_hCSCTOFerr_vs_CSCTOF",CSCTOFErr[i],CSCTOF[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hEta_vs_CSCTOFerr",eta[i],CSCTOFErr[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hEta_vs_CSCTOFndof",eta[i],CSCTOF_ndof[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hCSCTOFndof_vs_CSCTOF",CSCTOF_ndof[i],CSCTOF[i]);


            vcp[s].FillHisto2D(selLabels_[s]+"_hIh_vs_TOF",Ih_StripOnly[i],TOF[i]);

            vcp[s].FillHisto2D(selLabels_[s]+"_hBetaGamma_vs_Ih",(Pt[i]*cosh(eta[i]))/massPointSig_*1.0,Ih_StripOnly[i]);


            vcp[s].FillHisto2D(selLabels_[s]+"_hIh_vs_P",Ih_StripOnly[i],Pt[i]*cosh(eta[i]));

            vcp[s].FillHisto2D(selLabels_[s]+"_hTOF_vs_Fpix",TOF[i],Fpix);
            vcp[s].FillHisto2D(selLabels_[s]+"_hMass_vs_TOF",massDedxInit,TOF[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hMass_vs_Fpix",massDedxInit,Fpix);

            vcp[s].FillHisto2D(selLabels_[s]+"_hMassBeta_vs_Mass",massBeta,massDedxInit);

            vcp[s].FillHisto2D(selLabels_[s]+"_hMassCombineWeighted_vs_MassCombineAverage",massCombinedWeighted,massCombined);

            vcp[s].FillHisto2D(selLabels_[s]+"_hRUN_vs_TOF",RUN,TOF[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hRUN_vs_DTTOF",RUN,DTTOF[i]);
            vcp[s].FillHisto2D(selLabels_[s]+"_hRUN_vs_CSCTOF",RUN,CSCTOF[i]);

            vcp[s].FillHisto2D(selLabels_[s]+"_hEta_vs_TOF",eta[i],TOF[i]);

            vcp[s].FillHisto2D(selLabels_[s]+"_hEta_vs_TOF",eta[i],TOF[i]);

            

           
            if(TOFErr[i]<0.1 && TOF[i] > 0.5 && TOF[i] < 1.5){
                vcp[s].FillHisto2D(selLabels_[s]+"_hMassBetaClean_vs_Mass",massBeta,massDedxInit);
            }
        }
        if(do1Dplots){ 
            vcp[s].FillHisto1D(selLabels_[s]+"_hpoverm",(Pt[i]*cosh(eta[i]))/massPointSig_);
            vcp[s].FillHisto1D(selLabels_[s]+"_hEta",eta[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hPhi",phi[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hNOMH",NOMH[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hNOM",NOM[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hNOMinIh",NOM[i]-NOPH[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hNOPH",NOPH[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hFOVH",FOVH[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hdZ",dZ[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hdXY",dXY[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hdChi2",Chi2[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hdNdof",Ndof[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hChi2PerNdof",(Chi2[i]*1.0/Ndof[i]));
            vcp[s].FillHisto1D(selLabels_[s]+"_hTOFerr",TOFErr[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hTOFndof",TOF_ndof[i]);
    
	    vcp[s].FillHisto1D(selLabels_[s]+"_hpt",pt);
	    vcp[s].FillHisto1D(selLabels_[s]+"_hTOF",TOF[i]);
	    vcp[s].FillHisto1D(selLabels_[s]+"_hinvTOF",1/TOF[i]);
	    //vcp[s].FillHisto1D(selLabels_[s]+"_hIh",Ih_StripOnly[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hPtErr",PtErr[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hFPIX",Fpix);
            vcp[s].FillHisto1D(selLabels_[s]+"_hGstrip",Ias);
            vcp[s].FillHisto1D(selLabels_[s]+"_hnbHSCP",Pt.GetSize());
            vcp[s].FillHisto1D(selLabels_[s]+"_hrecoPFMET",RECO_PFMET);
            vcp[s].FillHisto1D(selLabels_[s]+"_hBeta",(1./TOF[i]));
            vcp[s].FillHisto1D(selLabels_[s]+"_hDiMuMass_Calib",dimuMass);
            vcp[s].FillHisto1D(selLabels_[s]+"_hDiMuMass_CalibWide",dimuMass);



            if(CalibrationZmumu){
                if(idMu1 != -1){
                    vcp[s].FillHisto1D(selLabels_[s]+"_hmuPtErr_over_muPt",(muonPtErr[idMu1]/muonPt[idMu1]));
                    vcp[s].FillHisto2D(selLabels_[s]+"_hmuPt_vs_muPtErr",muonPt[idMu1],muonPtErr[idMu1]);
                    vcp[s].FillHisto2D(selLabels_[s]+"_hpullBetaZmumu",muonPt[idMu1],muonPtErr[idMu1]);
                }
                if(idMu2 != -1){
                    vcp[s].FillHisto1D(selLabels_[s]+"_hmuPtErr_over_muPt",(muonPtErr[idMu2]/muonPt[idMu2]));
                    vcp[s].FillHisto2D(selLabels_[s]+"_hmuPt_vs_muPtErr",muonPt[idMu2],muonPtErr[idMu2]);
                    vcp[s].FillHisto2D(selLabels_[s]+"_hpullBetaZmumu",muonPt[idMu1],muonPtErr[idMu1]);
                }
 
            }
            vcp[s].FillHisto1D(selLabels_[s]+"_hPtErrOverPt",PtErr[i]/pt);
            vcp[s].FillHisto1D(selLabels_[s]+"_hPtErrOverPt2",PtErr[i]/(pt*pt));
            vcp[s].FillHisto1D(selLabels_[s]+"_hTrack_MiniIsoSumPtFixCone",track_genTrackAbsIsoSumPtFix[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hTrack_MiniIsoSumPtVarCone",track_genTrackMiniIsoSumPt[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hPFMiniIso_relative",PFMiniIso_relative[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hmatchTrigMuon_minDeltaR",matchTrigMuon_minDeltaR[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hisPFMuon",(*isMuon.Get())[i]);
            vcp[s].FillHisto1D(selLabels_[s]+"_hHLTMu50",HLTmu);


            vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonSquare_dedx_beta_comb_large",chi2CombMuonSquare);
            vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonNoSquare_dedx_beta_comb_large",chi2CombMuonNoSquare);

            vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonSquare_dedx_beta_comb_tight",chi2CombMuonSquare);
            vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonSquare_dedx_beta_comb_ultra_tight_long",chi2CombMuonSquare);

            if(passSR){
                vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonSquare_dedx_beta_comb_large_SR",chi2CombMuonSquare);
                vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonNoSquare_dedx_beta_comb_large_SR",chi2CombMuonNoSquare);
                vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonSquare_dedx_beta_comb_ultra_tight_long_SR",chi2CombMuonSquare);
            }
            if(passCR){
                vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonSquare_dedx_beta_comb_large_CR",chi2CombMuonSquare);
                vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonNoSquare_dedx_beta_comb_large_CR",chi2CombMuonNoSquare);
                vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonSquare_dedx_beta_comb_ultra_tight_long_CR",chi2CombMuonSquare);
            }


            vcp[s].FillHisto1D(selLabels_[s]+"_hDedxError",dedxErr);
            vcp[s].FillHisto1D(selLabels_[s]+"_hBetaError",BetaErr);


            vcp[s].FillHisto1D(selLabels_[s]+"_hMassDedxError",sigmaMassDedxNoPMuon);
            vcp[s].FillHisto1D(selLabels_[s]+"_hMassBetaError",sigmaMassBetaNoPMuon);
            vcp[s].FillHisto1D(selLabels_[s]+"_hMassCombinedError",sigmaMassCombMuon);




            vcp[s].FillHisto1D(selLabels_[s]+"_hQuad2_dedx_beta_large",diffDivQuadraticErr);
            vcp[s].FillHisto1D(selLabels_[s]+"_hQuad2_dedx_beta_tight",diffDivQuadraticErr);
            vcp[s].FillHisto1D(selLabels_[s]+"_hQuad2_dedx_beta_ultra_tight",diffDivQuadraticErr);
            if(passCR){
                vcp[s].FillHisto1D(selLabels_[s]+"_hQuad2_dedx_beta_large_CR",diffDivQuadraticErr);
                vcp[s].FillHisto1D(selLabels_[s]+"_hQuad2_dedx_beta_tight_CR",diffDivQuadraticErr);
                vcp[s].FillHisto1D(selLabels_[s]+"_hQuad2_dedx_beta_ultra_tight_CR",diffDivQuadraticErr);
            }
            if(passSR){
                vcp[s].FillHisto1D(selLabels_[s]+"_hQuad2_dedx_beta_large_SR",diffDivQuadraticErr);
                vcp[s].FillHisto1D(selLabels_[s]+"_hQuad2_dedx_beta_tight_SR",diffDivQuadraticErr);
                vcp[s].FillHisto1D(selLabels_[s]+"_hQuad2_dedx_beta_ultra_tight_SR",diffDivQuadraticErr);
            }

            if(matchTrigMuon_minDeltaR[i] < 0.3){
                vcp[s].FillHisto1D(selLabels_[s]+"_hTrackPt_minus_MatchedMuonPt_dr03",(pt-matchTrigMuon_pT[i])/(pt+matchTrigMuon_pT[i]));
            }
            

        }
         
        newWeight = weight;
        
        if(do1Dplots){
            if(passSR){
                vcp[s].FillHisto1D(selLabels_[s]+"_hpoverm_SR_9fp10",(Pt[i]*cosh(eta[i]))/massPointSig_);
                vcp[s].FillHisto1D(selLabels_[s]+"_hMass_SR1_9fp10",massDedxInit);
                vcp[s].FillHisto1D(selLabels_[s]+"_hMassBeta_SR1_9fp10",massBeta);
                vcp[s].FillHisto1D(selLabels_[s]+"_hMassCombined_SR1_9fp10",massCombinedWeighted);
    
                vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuon_SR1_9fp10",massDedxMuonInit);
                vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonBeta_SR1_9fp10",massMuonBeta);

                vcp[s].FillHisto1D(selLabels_[s]+"_hEventWeight",weight);
                
                /*
                if(isSimulation){
                    double WeightForTofErr = 1.0;
                    if(selLabels_[s] == "testIhPt"){
                        for(size_t tw = 0; tw < toferrMin.size(); tw++) {
                            if (TOFErr[i] >= toferrMin[tw] && TOFErr[i] < toferrMax[tw]) {
                                WeightForTofErr = toferrWeights[tw]; 
                                break;
                            }
                        }
                        if(WeightForTofErr != 0){
                            filltofErrUp +=1; 
                            vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_toferrUp",massCombinedMuonWeighted,weight*WeightForTofErr*WeightForTofErr);
                            vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_toferrDown",massCombinedMuonWeighted,weight);
                            weight = weight * WeightForTofErr;
                        }
                        else{
                            toferrUpEqualsZero+=1;
                            std::cout << "Weight equals 0, this is weird !!" << std::endl;
                            vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_toferrUp",massCombinedMuonWeighted,weight);
                            vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_toferrDown",massCombinedMuonWeighted,weight);
                        }
                    }

                    if(passSRfpixUp){
                        vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_fpixUp",massCombinedMuonWeighted,weight);
                    }
                    if(passSRfpixDown){
                        vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_fpixDown",massCombinedMuonWeighted,weight);
                    }
                    if(passSRpTUp){
                        vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_pTUp",massCombinedMuonWeightedpTUp,weight);
                    }
                    if(passSRpTDown){
                        vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_pTDown",massCombinedMuonWeightedpTDown,weight);
                    }
 

 
                    if(selLabels_[s] == "testIhPt") fillNominal +=1;
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10",massCombinedMuonWeighted,weight);

                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_puUp",massCombinedMuonWeighted,weight*puSysUp);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_puDown",massCombinedMuonWeighted,weight*puSysDown);

                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_triggerUp",massCombinedMuonWeighted,weight*triggerSysUp);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_triggerDown",massCombinedMuonWeighted,weight*triggerSysDown);

                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_muTriggerUp",massCombinedMuonWeighted,weight*muTriggerSysUp);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_muTriggerDown",massCombinedMuonWeighted,weight*muTriggerSysDown);

                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_muRecoUp",massCombinedMuonWeighted,weight*muRecoSysUp);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_muRecoDown",massCombinedMuonWeighted,weight*muRecoSysDown);

                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_muIdUp",massCombinedMuonWeighted,weight*muIdSysUp);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_SR1_9fp10_muIdDown",massCombinedMuonWeighted,weight*muIdSysDown);
                }
                */

                vcp[s].FillHisto1D(selLabels_[s]+"_hMassAtlas_SR1_9fp10",massAtlas);
    
                vcp[s].FillHisto1D(selLabels_[s]+"_hMassBetaMinusTheo_div_MassBeta_SR1_9fp10",(massBeta- massPointSig_)*1.0/massBeta);
                vcp[s].FillHisto1D(selLabels_[s]+"_hMassdEdxMinusTheo_div_MassdEdx_SR1_9fp10",(massDedxInit-massPointSig_)*1.0 / massDedxInit );
                vcp[s].FillHisto1D(selLabels_[s]+"_hMassCombMinusTheo_div_MassComb_SR1_9fp10",(massCombinedWeighted-massPointSig_)*1.0 / massCombinedWeighted );
    
                if(isSimulation){
                    vcp[s].FillHisto1D(selLabels_[s]+"_hpullMassBeta_SR1_9fp10",((massBeta- massPointSig_)*1.0)/sigmaMassBetaMuon);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hpullMassDedx_SR1_9fp10",((massDedx-massPointSig_)*1.0) / sigmaMassDedxMuon);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hpullMassComb_SR1_9fp10",((massCombinedWeighted-massPointSig_)*1.0)/sigmaMassCombMuonWithP );
                    vcp[s].FillHisto1D(selLabels_[s]+"_hpullMassAtlas_SR1_9fp10",((massAtlas-massPointSig_)*1.0)/sigmaMassAtlas);

                    vcp[s].FillHisto1D(selLabels_[s]+"_hpullMassCombTest_SR1_9fp10",((massCombinedMuonWeightedWithP-massPointSig_)*1.0)/testErrorMCombined);
                }
    
                vcp[s].FillHisto1D(selLabels_[s]+"_hSubstraction_dedx_beta_quadraticErrors_SR1",diffDivQuadraticErr);
    
                if(selLabels_[s] == "testIhPt"){
                    for (double Ih_cut : Ih_cut_values) {             
                        for (double beta_cut : beta_cut_values) {
                            std::string tmpBetaSR = cut_hist_map_beta_SR[std::make_pair(Ih_cut, beta_cut)];
                            std::string tmpDedxSR = cut_hist_map_dedx_SR[std::make_pair(Ih_cut, beta_cut)];
                            std::string tmpCombSR = cut_hist_map_comb_SR[std::make_pair(Ih_cut, beta_cut)];
                            std::string tmpAtlasSR = cut_hist_map_atlas_SR[std::make_pair(Ih_cut, beta_cut)];
                            if( (Ih_StripOnly[i] > Ih_cut) && (TOF[i] > beta_cut)){
                                vcp[s].FillHisto1D(tmpCombSR,massCombinedWeighted);
                                vcp[s].FillHisto1D(tmpDedxSR,massDedxInit);
                                vcp[s].FillHisto1D(tmpBetaSR,massBeta);
                                vcp[s].FillHisto1D(tmpAtlasSR,massAtlas);
                            }
                        }
                    }
                }
            }
            if(passCR){

                vcp[s].FillHisto1D(selLabels_[s]+"_hSubstraction_dedx_beta_quadraticErrors_CR",diffDivQuadraticErr);
                vcp[s].FillHisto1D(selLabels_[s]+"_hMassBetaMinusTheo_div_MassBeta_CR_3fp8",(massBeta- massPointSig_)*1.0/massBeta);
                vcp[s].FillHisto1D(selLabels_[s]+"_hMassdEdxMinusTheo_div_MassdEdx_CR_3fp8",(massDedxInit-massPointSig_)*1.0 / massDedxInit );
                vcp[s].FillHisto1D(selLabels_[s]+"_hMassCombMinusTheo_div_MassComb_CR_3fp8",(massCombinedWeighted-massPointSig_)*1.0 / massCombinedWeighted );


                if(isSimulation){
                    vcp[s].FillHisto1D(selLabels_[s]+"_hpullMassBeta_CR_3fp8",((massBeta- massPointSig_)*1.0)/sigmaMassBetaMuon);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hpullMassDedx_CR_3fp8",((massDedxInit-massPointSig_)*1.0)/sigmaMassDedxMuon);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hpullMassAtlas_CR_3fp8",((massAtlas-massPointSig_)*1.0)/sigmaMassAtlas);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hpullMassComb_CR_3fp8",((massCombinedWeighted-massPointSig_)*1.0)/sigmaMassCombMuonWithP );
                    vcp[s].FillHisto1D(selLabels_[s]+"_hpullMassCombTest_CR_3fp8",((massCombinedMuonWeightedWithP-massPointSig_)*1.0)/testErrorMCombined );
                }


                vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonSquare_dedx_beta_comb_tight_CR_ALL",chi2CombMuonSquare);

                if(chi2CombMuonNoSquare < 40) vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonSquare_dedx_beta_comb_tight_CR_PASS",chi2CombMuonSquare);
                if(chi2CombMuonNoSquare > 40) vcp[s].FillHisto1D(selLabels_[s]+"_hChi2MuonSquare_dedx_beta_comb_tight_CR_FAIL",chi2CombMuonSquare);

                vcp[s].FillHisto1D(selLabels_[s]+"_hMassDedxPow2_CR_3fp8",massDedxInit);
                vcp[s].FillHisto1D(selLabels_[s]+"_hMassDedxPow5third_CR_3fp8",massDedxInitPow3_2);


                if(abs(eta[i]) < 1){
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMass_VR_3fp8_etabin1",massDedxInit);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassAtlas_VR_3fp8_etabin1",massAtlas);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassBeta_VR_3fp8_etabin1",massBeta);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassCombined_VR_3fp8_etabin1",massCombinedWeighted);

                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuon_VR_3fp8_etabin1",massDedxMuonInit);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonBeta_VR_3fp8_etabin1",massMuonBeta);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassMuonCombined_VR_3fp8_etabin1",massCombinedMuonWeighted);
                }

                if(abs(eta[i]) >= 1){
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMass_VR_3fp8_etabin2",massDedxInit);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassBeta_VR_3fp8_etabin2",massBeta);
                    vcp[s].FillHisto1D(selLabels_[s]+"_hMassCombined_VR_3fp8_etabin2",massCombinedMuonWeighted);
                }

                vcp[s].FillHisto1D(selLabels_[s]+"_hpoverm_VR_3fp8",(Pt[i]*cosh(eta[i]))/massPointSig_);
                vcp[s].FillHisto1D(selLabels_[s]+"_hBeta_VR_3fp8",(1./TOF[i]));

                if(selLabels_[s] == "testIhPt"){

                    for (double Ih_cut : Ih_cut_values) {
                        for (double beta_cut : beta_cut_values) {
                            std::string tmpBetaCR = cut_hist_map_beta_CR[std::make_pair(Ih_cut, beta_cut)];
                            std::string tmpDedxCR = cut_hist_map_dedx_CR[std::make_pair(Ih_cut, beta_cut)];
                            std::string tmpCombCR = cut_hist_map_comb_CR[std::make_pair(Ih_cut, beta_cut)];
                            std::string tmpAtlasCR = cut_hist_map_atlas_CR[std::make_pair(Ih_cut, beta_cut)];
                            if( (Ih_StripOnly[i] > Ih_cut) && (TOF[i] > beta_cut)){
                                vcp[s].FillHisto1D(tmpCombCR,massCombinedWeighted);
                                vcp[s].FillHisto1D(tmpDedxCR,massDedxInit);
                                vcp[s].FillHisto1D(tmpBetaCR,massBeta);
                                vcp[s].FillHisto1D(tmpAtlasCR,massAtlas);
                            }
                        }
                    }
                }
            }
        }        

        if(do2Dplots){ 
            if(passSR){
            
                vcp[s].FillHisto2D(selLabels_[s]+"_hBetaGamma_vs_Ih_SR_9fp10",(Pt[i]*cosh(eta[i]))/massPointSig_,Ih_StripOnly[i]);
                vcp[s].FillHisto2D(selLabels_[s]+"_hMassBeta_vs_ErrMassBeta_SR9fp10",massBeta,sigmaMassBetaNoPMuon);
                vcp[s].FillHisto2D(selLabels_[s]+"_hMassdEdx_vs_ErrMassdEdx_SR9fp10",massDedxInit,sigmaMassDedxNoPMuon);
                vcp[s].FillHisto2D(selLabels_[s]+"_hMassBeta_vs_Massdedx_SR9fp10",massBeta,massDedxInit);
            }
            if(passCR){
                vcp[s].FillHisto2D(selLabels_[s]+"_hmassDedxInit_vs_eta_CR_3fp8",massDedxInit,eta[i]);
                vcp[s].FillHisto2D(selLabels_[s]+"_hEta_vs_IhStripOnly_CR_3fp8",eta[i],Ih_StripOnly[i]);

                vcp[s].FillHisto2D(selLabels_[s]+"_hMassBeta_vs_ErrMassBeta_CR3fp8",massBeta,sigmaMassBetaNoPMuon);
                vcp[s].FillHisto2D(selLabels_[s]+"_hMassdEdx_vs_ErrMassdEdx_CR3fp8",massDedxInit,sigmaMassDedxNoPMuon);
                vcp[s].FillHisto2D(selLabels_[s]+"_hMassBeta_vs_Massdedx_CR3fp8",massBeta,massDedxInit);

                vcp[s].FillHisto2D(selLabels_[s]+"_hMassBeta_vs_theor_CR3fp8",massBeta,massPointSig_);
                vcp[s].FillHisto2D(selLabels_[s]+"_hMassdEdx_vs_theor_CR3fp8",massDedxInit,massPointSig_);
                vcp[s].FillHisto2D(selLabels_[s]+"_hMassCombined_vs_theor_CR3fp8",massCombinedWeighted,massPointSig_);

                vcp[s].FillHisto2D(selLabels_[s]+"_hpullBeta_vs_massBeta_CR_3fp8",((massBeta- massPointSig_)*1.0)/sigmaMassBetaNoPMuon, massBeta);
                vcp[s].FillHisto2D(selLabels_[s]+"_hpullDedx_vs_massDedx_CR_3fp8",((massDedxInit- massPointSig_)*1.0)/sigmaMassDedxNoPMuon, massDedxInit);
                vcp[s].FillHisto2D(selLabels_[s]+"_hpullComb_vs_massComb_CR_3fp8",((massCombinedWeighted- massPointSig_)*1.0)/sigmaMassCombMuon, massCombinedWeighted);

                vcp[s].FillHisto2D(selLabels_[s]+"_hpullBeta_vs_eta_CR_3fp8",((massBeta- massPointSig_)*1.0)/sigmaMassBetaNoPMuon, eta[i]);
                vcp[s].FillHisto2D(selLabels_[s]+"_hpullDedx_vs_eta_CR_3fp8",((massDedxInit- massPointSig_)*1.0)/sigmaMassDedxNoPMuon, eta[i]);
                vcp[s].FillHisto2D(selLabels_[s]+"_hpullComb_vs_eta_CR_3fp8",((massCombinedWeighted- massPointSig_)*1.0)/sigmaMassCombMuon, eta[i]);


                vcp[s].FillHisto2D(selLabels_[s]+"_hpullBeta_vs_p_CR_3fp8",((massBeta- massPointSig_)*1.0)/sigmaMassBetaNoPMuon, (pt*cosh(eta[i])));
                vcp[s].FillHisto2D(selLabels_[s]+"_hpullDedx_vs_p_CR_3fp8",((massDedxInit- massPointSig_)*1.0)/sigmaMassDedxNoPMuon, (pt*cosh(eta[i])));
                vcp[s].FillHisto2D(selLabels_[s]+"_hpullComb_vs_p_CR_3fp8",((massCombinedWeighted- massPointSig_)*1.0)/sigmaMassCombMuon, (pt*cosh(eta[i])));

            }

        }

        if(UseFpixel){           
            vmrp_regionFpix_all[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);

           if(pt <= ptcut_){
               
               if( (Fpix > fpix3) && (Fpix <= fpix4) ) vmrp_regionA_3f4[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix4) && (Fpix <= fpix5) ) vmrp_regionA_4f5[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix5) && (Fpix <= fpix6) ) vmrp_regionA_5f6[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix7) ) vmrp_regionA_6f7[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix7) && (Fpix <= fpix8) ) vmrp_regionA_7f8[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix8) && (Fpix <= fpix9) ) vmrp_regionA_8f9[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);

               if( (Fpix > fpix9) && (Fpix <= fpix10) ) vmrp_regionA_9f10[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix99) && (Fpix <= fpix10) ) vmrp_regionA_99f10[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix999) && (Fpix <= fpix10) ) vmrp_regionA_999f10[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);



               if( (Fpix > fpix3) && (Fpix <= fpix6) ) vmrp_regionA_3f6[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix9) ) vmrp_regionA_6f9[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix8) ) vmrp_regionA_3f8[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix9) ) vmrp_regionA_3f9[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);

               if( (Fpix > fpix3) && (Fpix <= fpix6) ) vmrp_regionB_3f6[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix9) ) vmrp_regionB_6f9[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix8) ) vmrp_regionB_3f8[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix9) ) vmrp_regionB_3f9[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix4) ) vmrp_regionB_3f4[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix4) && (Fpix <= fpix5) ) vmrp_regionB_4f5[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix5) && (Fpix <= fpix6) ) vmrp_regionB_5f6[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix7) ) vmrp_regionB_6f7[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix7) && (Fpix <= fpix8) ) vmrp_regionB_7f8[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix8) && (Fpix <= fpix9) ) vmrp_regionB_8f9[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);

               if( (Fpix > fpix8) && (Fpix <= fpix10) ) vmrp_regionB_8f10[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix9) && (Fpix <= fpix10) ) vmrp_regionB_9f10[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix99) && (Fpix <= fpix10) ) vmrp_regionB_99f10[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix999) && (Fpix <= fpix10) ) vmrp_regionB_999f10[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               
           }
           else{  

               if( (Fpix > fpix6) && (Fpix <= fpix9) ) vmrp_regionD_6f9[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix4) ) vmrp_regionD_3f4[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix4) && (Fpix <= fpix5) ) vmrp_regionD_4f5[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix5) && (Fpix <= fpix6) ) vmrp_regionD_5f6[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix7) ) vmrp_regionD_6f7[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix7) && (Fpix <= fpix8) ) vmrp_regionD_7f8[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix8) && (Fpix <= fpix9) ) vmrp_regionD_8f9[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);


               if( (Fpix > fpix8) && (Fpix <= fpix10) ) vmrp_regionD_8f10[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix9) && (Fpix <= fpix10) ) vmrp_regionD_9f10[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix99) && (Fpix <= fpix10) ) vmrp_regionD_99f10[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix999) && (Fpix <= fpix10) ) vmrp_regionD_999f10[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix8) ) vmrp_regionC_3f8[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix9) ) vmrp_regionC_3f9[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix3) && (Fpix <= fpix4) ) vmrp_regionC_3f4[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix4) && (Fpix <= fpix5) ) vmrp_regionC_4f5[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix5) && (Fpix <= fpix6) ) vmrp_regionC_5f6[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix7) ) vmrp_regionC_6f7[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix7) && (Fpix <= fpix8) ) vmrp_regionC_7f8[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix8) && (Fpix <= fpix9) ) vmrp_regionC_8f9[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               
               if( (Fpix > fpix3) && (Fpix <= fpix6) ) vmrp_regionC_3f6[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               if( (Fpix > fpix6) && (Fpix <= fpix9) ) vmrp_regionC_6f9[s].fill(eta[i],NOM[i],P,pt,PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massForRegions,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
               
           }
        }
     
        if(UseGstrip){

            if(Pt[i] <= ptcut_){               
                if(Ias < quan50) vmrp_regionA_ias50[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
                if(Ias < quan80) vmrp_regionA_ias80[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
                if(Ias < quan90) vmrp_regionA_ias90[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
    
    
                if(Ias >= quan50 && Ias < quan60){vmrp_regionB_50ias60[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan60 && Ias < quan70){vmrp_regionB_60ias70[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan70 && Ias < quan80){vmrp_regionB_70ias80[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan80 && Ias < quan90){vmrp_regionB_80ias90[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan50 && Ias < quan90){vmrp_regionB_50ias90[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan50 && Ias < quan99){vmrp_regionB_50ias99[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                
                if(Ias >= quan50 && Ias < quan999){vmrp_regionB_50ias999[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
    
                if(Ias >= quan50){vmrp_regionB_50ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                
                if(Ias >= quan90){vmrp_regionB_90ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan99){vmrp_regionB_99ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan999){vmrp_regionB_999ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                 
            }
            else{
                
                if(Ias < quan50) vmrp_regionC_ias50[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
                if(Ias < quan80) vmrp_regionC_ias80[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
                if(Ias < quan90) vmrp_regionC_ias90[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1);
    
    
                if(Ias >= quan50 && Ias < quan60){vmrp_regionD_50ias60[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan60 && Ias < quan70){vmrp_regionD_60ias70[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan70 && Ias < quan80){vmrp_regionD_70ias80[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan80 && Ias < quan90){vmrp_regionD_80ias90[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan50 && Ias < quan90){vmrp_regionD_50ias90[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan50 && Ias < quan99){vmrp_regionD_50ias99[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan50 && Ias < quan999){vmrp_regionD_50ias999[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
            
                if(Ias >= quan90 ){vmrp_regionD_90ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan99 ){vmrp_regionD_99ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
                if(Ias >= quan999 ){vmrp_regionD_999ias100[s].fill(eta[i],NOM[i],P,Pt[i],PtErr[i],Ih_StripOnly[i],Ias_StripOnly[i],-1,massDedx,TOF[i],*nofVtx.Get(),Fpix,newWeight,1); }
    
            }
            
        }  
        //TEST NEW OUTPUT TTREE
        TreeInverseBeta=TOF[i];
        TreeIh = Ih_StripOnly[i];

        TreeMassIh = massDedxInit; 
        TreeMuonMassIh = massDedxMuonInit;

        TreeMassBeta = massBeta;
        TreeMuonMassBeta = massMuonBeta;

        TreeMassCombined = massCombinedWeighted;
        TreeMuonMassCombined = massCombinedMuonWeighted;

        TreeMuonMassAtlas = massAtlas;
        TreePt = hscpMuonPt;
        TreeFpix = Fpix;
        TreeChi2 = chi2CombMuonNoSquare;
        //std::cout << "Setting inverse beta to = " << inverseBeta <<std::endl;
        if(selLabels_[s] == "testIhPt" && FillTree){        
            outputTree->Fill();
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
   cout << "Total number of events  = " << tot << std::endl;
   cout <<"Total events passing preselection testIhPt = " << passedSel << std::endl;
   cout <<"Number of events where one of the HSCP passing presel testIhPt is a PF muon = " << PFMu << std::endl;
   cout <<"Number of events where one of the HSCP passing presel testIhPt is a Global muon = " << GlobalMu << std::endl;
   cout << "There was " << muBadReco << " events where candidate being PF muon is not associated to a muon in the event (weird)" << std::endl;
   cout << "There was " << muGoodReco << "events where we found a muon linked to the highest muon passing presel (as expected)" << std::endl;
   cout << "    Out of these " << muGoodReco << "events, " << muGoodRecoGoodError << " Had a sigmaPt/pt < 0.125, and " << muGoodRecoButBadError << " had one above 0.125, which means the efficiency of sigma pt/pt for muon passing presel is " << (muGoodRecoGoodError - muGoodRecoButBadError)/muGoodRecoGoodError << std::endl;

   cout << "Filled toferr up hist " << filltofErrUp << " times, and filled nominal hist " << fillNominal << " times." << std::endl;
   cout << "TofErr up weight equals 0 in " << toferrUpEqualsZero << " events" << std::endl;
   if(FillTree) fOutput->Add(outputTree);
   //treeTest->Close();
   //delete treeTest;

   for(auto obj: vcp) obj.AddToList(fOutput);
   if(UseFpixel){
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
       //for(auto obj: vmrp_regionA_99f10) obj.addToList(fOutput);
       //for(auto obj: vmrp_regionA_999f10) obj.addToList(fOutput);


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
       //for(auto obj: vmrp_regionB_99f10) obj.addToList(fOutput);
       //for(auto obj: vmrp_regionB_999f10) obj.addToList(fOutput);


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
       //for(auto obj: vmrp_regionD_99f10) obj.addToList(fOutput);
       //for(auto obj: vmrp_regionD_999f10) obj.addToList(fOutput);
   }

   if(UseGstrip){ 
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
   //TEST OUTPUT TTREE

   fout->Write();
   fout->Close();
   /*
   treeTest = new TFile("testForTree.root","RECREATE");
   outputTree->Write();
   treeTest->Write();
   treeTest->Close();
   */
}
