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
   std::string version;
   int etabins,ihbins,pbins,massbins,tofbins;
   double ptcut;
   int masscut;
   while(std::getline(ifile,line))// line per line
   {
       if(strncmp(line.c_str(),"#",1)==0) continue;
       std::cout << line << std::endl;
       std::stringstream ss(line);
       ss >> ptcut >> etabins >> ihbins >> pbins >> massbins >> masscut >> tofbins >> dataset >> version;
   }
   ifile.close();

   std::cout << "Successfully closed config file "  << std::endl;

   TChain* chain;
   std::string pathPy = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/python/dpmDataPaths/";

   //std::string pathHigh = "/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/TEST/python/";
   std::string pathHigh = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/SingleMuon/";

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
   if(dataset == "highmass2017B"){
       std::string fileNamesMu2017[] = { (pathHigh+"all_root_files.txt").c_str()};

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
   }

   if(dataset == "Mu2017"){
       //Below is the addition on the TChain for files located on DPM (if no transfer is made)
       
       //std::string fileNamesMu2017[] = { (pathPy+"Mu2017Bachain.txt").c_str(),(pathPy+"Mu2017Cachain.txt").c_str(),(pathPy+"Mu2017Cbchain.txt").c_str(),(pathPy+"Mu2017Dachain.txt").c_str(),(pathPy+"Mu2017Eachain.txt").c_str(),(pathPy+"Mu2017Fachain.txt").c_str(),(pathPy+"Mu2017Fbchain.txt").c_str()};
       std::string fileNamesMu2017[] = { (pathHigh+"SingleMuon_Run2017B_"+version+".txt").c_str(),(pathHigh+"SingleMuon_Run2017C_"+version+".txt").c_str(),(pathHigh+"SingleMuon_Run2017D_"+version+".txt").c_str(),(pathHigh+"SingleMuon_Run2017E_"+version+".txt").c_str(),(pathHigh+"SingleMuon_Run2017F_"+version+".txt").c_str()};

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
   }
        
   //RUNNING ON 2018 SINGLE MUON WITH ABSOLUTE FIX CONE TK ISO

   if(dataset == "Mu2018"){
       //std::string fileNamesMu2017[] = { (pathPy+"Mu2018Aachain.txt").c_str(),(pathPy+"Mu2018Abchain.txt").c_str(),(pathPy+"Mu2018Bachain.txt").c_str(),(pathPy+"Mu2018Cachain.txt").c_str(),(pathPy+"Mu2018Dachain.txt").c_str(),(pathPy+"Mu2018Dbchain.txt").c_str(),(pathPy+"Mu2018Dcchain.txt").c_str()};
       std::string fileNamesMu2017[] = { (pathHigh+"SingleMuon_Run2018A_"+version+".txt").c_str(),(pathHigh+"SingleMuon_Run2018B_"+version+".txt").c_str(),(pathHigh+"SingleMuon_Run2018C_"+version+".txt").c_str(),(pathHigh+"SingleMuon_Run2018D_"+version+".txt").c_str()};

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

   if(dataset == "pStau200"){
       std::string p200 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/"+version+"/HSCPpairStau_"+version+"/Merged_HSCPpairStau/HSCPpairStau_M-200_merged.root";
       chain->AddFile(p200.c_str());
   }
   if(dataset == "pStau247"){
       std::string p247 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/"+version+"/HSCPpairStau_"+version+"/Merged_HSCPpairStau/HSCPpairStau_M-247_merged.root";
       chain->AddFile(p247.c_str());
   }

   if(dataset == "pStau308"){
       std::string p308 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/"+version+"/HSCPpairStau_"+version+"/Merged_HSCPpairStau/HSCPpairStau_M-308_merged.root";
       chain->AddFile(p308.c_str());
   }
   if(dataset == "pStau432"){
       std::string p432 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/"+version+"/HSCPpairStau_"+version+"/Merged_HSCPpairStau/HSCPpairStau_M-432_merged.root";
       chain->AddFile(p432.c_str());
   }
   if(dataset == "pStau557"){
       std::string p557 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/"+version+"/HSCPpairStau_"+version+"/Merged_HSCPpairStau/HSCPpairStau_M-557_merged.root";
       chain->AddFile(p557.c_str());
   }
   if(dataset == "pStau651"){
       std::string p651 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/"+version+"/HSCPpairStau_"+version+"/Merged_HSCPpairStau/HSCPpairStau_M-651_merged.root";
       chain->AddFile(p651.c_str());
   }
   if(dataset == "pStau745"){
       std::string p745 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/"+version+"/HSCPpairStau_"+version+"/Merged_HSCPpairStau/HSCPpairStau_M-745_merged.root";
       chain->AddFile(p745.c_str());
   }
   if(dataset == "pStau871"){
       std::string p871 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/"+version+"/HSCPpairStau_"+version+"/Merged_HSCPpairStau/HSCPpairStau_M-871_merged.root";
       chain->AddFile(p871.c_str());
   }
   if(dataset == "pStau1029"){
       std::string p1029 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/"+version+"/HSCPpairStau_"+version+"/Merged_HSCPpairStau/HSCPpairStau_M-1029_merged.root";
       chain->AddFile(p1029.c_str());
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



   std::string binning = std::to_string(ptcut) + "," + std::to_string(etabins) + "," + std::to_string(ihbins) + "," + std::to_string(pbins) + "," + std::to_string(massbins) + "," + std::to_string(masscut)  + "," +std::to_string(tofbins) + "," + dataset + "," + version;
 
   std::cout << "Running over dataset : " << dataset << " with code version : " << version << std::endl; 
   std::cout << "Defining regions A,B,C,D with pT cut = " << ptcut << std::endl;
   std::cout << "Eta #bin : "<< etabins << ", Ih #bins" << ihbins << " , P #bins " << pbins << " , mass #bins " << massbins <<  " and mass cut = " << masscut << std::endl;
     

   //chain->StopCacheLearningPhase();  
   //parallelize with proof
   chain->SetProof();
   chain->Process("HSCPSelector.C+",binning.c_str());

   delete chain;
}
