 {
   gSystem->Load("../libTools.so");
   //use proof-lite: running on all nodes of the machine
   //TProof::Open("");
   TProof::Open("workers=3");
   //proof->SetParallel(1);
   
   ifstream ifile;
   ifile.open("/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/cfg/configFile.txt");
   if(!ifile) std::cout << "Error when opening config file " <<  std::endl;
   std::string line;
   std::string label;
   std::string dataset;
   int etabins,ihbins,pbins,massbins;
   double ptcut;
   int masscut;
   while(std::getline(ifile,line))// line per line
   {
       if(strncmp(line.c_str(),"#",1)==0) continue;
       std::cout << line << std::endl;
       std::stringstream ss(line);
       ss >> ptcut >> etabins >> ihbins >> pbins >> massbins >> masscut >> dataset;
   }
   ifile.close();

   std::cout << "Successfully closed config file "  << std::endl;

   TChain* chain;
   std::string pathPy = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/python/dpmDataPaths/";

   //THIS IS WHEN WE RUN ON SKIMMED TREES (CopyOnlyTree) : muon 2017 or 2018, since we remove all useless branches
   /*
   if(dataset == "Mu2017" || dataset == "Mu2018"){
       chain = new TChain("HscpCandidates");
   }
   //THIS IS WHEN WE RUN ON FRAMEWORK PRODUCED FILEs
   else{
       chain = new TChain("HSCParticleAnalyzer/BaseName/HscpCandidates");
   }*/
   chain = new TChain("HSCParticleAnalyzer/BaseName/HscpCandidates");
   //chain = new TChain("HscpCandidates");

   //RUNNING ON 2017 SINGLE ELECTRON WITH ABSOLUTE FIX CONE TK ISO
   if(dataset == "Ele2017"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleElectron_Run2017B_CodeV3e_v1/results/all_2017B_ele_tree.root");  
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleElectron_Run2017C_CodeV3e_v1/results/all_2017C_ele_tree.root");  
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleElectron_Run2017D_CodeV3e_v1/results/all_2017D_ele_tree.root");  
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleElectron_Run2017E_CodeV3e_v1/results/all_2017E_ele_tree.root");  
   }


   //RUNNING ON 2017 SINGLE MUON WITH ABSOLUTE FIX CONE TK ISO

   if(dataset == "Mu2017"){
       //Below is the addition on the TChain for files located on DPM (if no transfer is made)
       
       std::string fileNamesMu2017[] = { (pathPy+"Mu2017Bachain.txt").c_str(),(pathPy+"Mu2017Cachain.txt").c_str(),(pathPy+"Mu2017Cbchain.txt").c_str(),(pathPy+"Mu2017Dachain.txt").c_str(),(pathPy+"Mu2017Eachain.txt").c_str(),(pathPy+"Mu2017Fachain.txt").c_str(),(pathPy+"Mu2017Fbchain.txt").c_str()};

       for (const std::string& fileName : fileNamesMu2017) {
           std::ifstream file(fileName);
           if (!file.is_open()) {
               std::cerr << "Failed to open file: " << fileName << std::endl;
               continue;
           }
           std::string line;
           while (std::getline(file, line)) {
               if (!line.empty() && line.back() == '\n') {
                   line.pop_back();
               }
               chain->AddFile(line.c_str());
           }

           file.close();
       }
       
       /*
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleMuon_Run2017B_CodeV1pM_v1/results/2017B_muons_skimtree.root");   
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleMuon_Run2017C_CodeV1pM_v1/results/2017C_muons_skimtree.root"); 
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleMuon_Run2017D_CodeV1pM_v1/results/2017D_muons_skimtree.root");
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleMuon_Run2017E_CodeV1pM_v1/results/2017E_muons_skimtree.root");
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleMuon_Run2017F_CodeV1pM_v1/results/2017F_muons_skimtree.root");
       */
       
   }
        
           
       

   //RUNNING ON 2018 SINGLE MUON WITH ABSOLUTE FIX CONE TK ISO

   if(dataset == "Mu2018"){
       std::string fileNamesMu2017[] = { (pathPy+"Mu2018Aachain.txt").c_str(),(pathPy+"Mu2018Abchain.txt").c_str(),(pathPy+"Mu2018Bachain.txt").c_str(),(pathPy+"Mu2018Cachain.txt").c_str(),(pathPy+"Mu2018Dachain.txt").c_str(),(pathPy+"Mu2018Dbchain.txt").c_str(),(pathPy+"Mu2018Dcchain.txt").c_str()};

       for (const std::string& fileName : fileNamesMu2017) {
           std::ifstream file(fileName);
           if (!file.is_open()) {
               std::cerr << "Failed to open file: " << fileName << std::endl;
               continue;
           }
           std::string line;
           while (std::getline(file, line)) {
               if (!line.empty() && line.back() == '\n') {
                   line.pop_back();
               }
               chain->AddFile(line.c_str());
           }

           file.close();
       }
       /*     
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleMuon_Run2018A_CodeV1pM_v1/results/2018A_muons_skimtree_p1.root");   
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleMuon_Run2018A_CodeV1pM_v1/results/0000/2018A_muons_skimtree_p2.root");
   
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleMuon_Run2018B_CodeV1pM_v1/results/2018B_muons_skimtree.root");
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleMuon_Run2018C_CodeV1pM_v1/results/2018C_muons_skimtree.root");   


       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleMuon_Run2018D_CodeV1pM_v1/results/2018D_muons_skimtree_p0.root");
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleMuon_Run2018D_CodeV1pM_v1/results/0000/2018D_muons_skimtree_p1.root");
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_SingleMuon_Run2018D_CodeV1pM_v1/results/0001/2018D_muons_skimtree_p2.root");
       */
   }


   
   //RUNNING ON MC background QCD Pt 170 to 300 MuEnrichedPt5
   if(dataset == "qcd170_300"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2017_QCD_Pt-170To300_MuEnrichedPt5_CodeV5_v1/results/2017_QCD_Pt-170To300_MuEnriched_all.root");
   }
 
   //RUNNING ON MC background WjetsToLNu 0jets
   if(dataset == "wjets_0j"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2017_WJetsToLNu_0J_CodeV5_v1/results/2017_WJetsToLNu_0J_all_skimmed.root");
   }
   //RUNNING ON pairStau 557    


   if(dataset == "pStau308"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPpairStau_M-308_CodeV7fn_v1/results/pStau308_CodeV7fn.root");
   }
   if(dataset == "pStau432"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPpairStau_M-432_CodeV7fn_v1/results/pStau432_CodeV7fn.root");
   }
   if(dataset == "pStau557"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPpairStau_M-557_CodeV7fn_v1/results/pStau557_CodeV7fn.root");
   }
   if(dataset == "pStau651"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPpairStau_M-651_CodeV6dylan_v1/results/pStau651_CodeV7fn.root");
   }
   if(dataset == "pStau745"){
       //chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPpairStau_M-745_CodeV7fn_v1/results/pStau745_CodeV7fn.root");
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPpairStau_M-745_CodeV1p0_v1/results/pStau745_CodeV1p0.root");
   }
   if(dataset == "pStau871"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPpairStau_M-871_CodeV7fn_v1/results/pStau871_CodeV7fn.root");
   }
   if(dataset == "pStau1029"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPpairStau_M-1029_CodeV7fn_v1/results/pStau1029_CodeV7fn.root");
   }


   if(dataset == "Glu800"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-800_CodeV7fn_v1/results/Gluino800_CodeV7fn.root");
   }
   if(dataset == "Glu1000"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-1000_CodeV7fn_v1/results/Gluino1000_CodeV7fn.root");
   }
   if(dataset == "Glu1400"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-1400_CodeV7fn_v1/results/Gluino1400_CodeV7fn.root");
   }

   if(dataset == "Glu1600"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-1600_CodeV7fn_v1/results/Gluino1600_CodeV7fn.root");
   }
   if(dataset == "Glu1800"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-1800_CodeV7fn_v1/results/Gluino1800_CodeV7fn.root");
   }

   if(dataset == "Glu2000"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-2000_CodeV7fn_v1/results/Gluino2000_CodeV7fn.root");
   }
   if(dataset == "Glu2200"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-2200_CodeV7fn_v1/results/Gluino2200_CodeV7fn.root");
   }
   if(dataset == "Glu2400"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-2400_CodeV7fn_v1/results/Gluino2400_CodeV7fn.root");
   }
   if(dataset == "Glu2600"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-2600_CodeV7fn_v1/results/Gluino2600_CodeV7fn.root");
   }



   std::string binning = std::to_string(ptcut) + "," + std::to_string(etabins) + "," + std::to_string(ihbins) + "," + std::to_string(pbins) + "," + std::to_string(massbins) + "," + std::to_string(masscut)  + "," + dataset;
 
   std::cout << "Running over dataset : " << dataset << std::endl; 
   std::cout << "Defining regions A,B,C,D with pT cut = " << ptcut << std::endl;
   std::cout << "Eta #bin : "<< etabins << ", Ih #bins" << ihbins << " , P #bins " << pbins << " , mass #bins " << massbins <<  " and mass cut = " << masscut << std::endl;
     

   //chain->StopCacheLearningPhase();  
   //parallelize with proof
   chain->SetProof();
   chain->Process("HSCPSelector.C+",binning.c_str());

   delete chain;
}
