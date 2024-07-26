 {
   gSystem->Load("../libTools.so");
   //use proof-lite: running on all nodes of the machine
   //TProof::Open("");
   TProof::Open("workers=3");
   //proof->SetParallel(1);
   
   ifstream ifile;
   ifile.open("/opt/sbg/cms/ui14_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/cfg/configFile.txt");
   if(!ifile) std::cout << "Error when opening config file " <<  std::endl;
   std::string line;
   std::string label;
   std::string dataset;
   std::string version;
   int etabins,ihbins,pbins,massbins,tofbins,fpixbins;
   double ptcut;
   double tofcut;
   int masscut;
   while(std::getline(ifile,line))// line per line
   {
       if(strncmp(line.c_str(),"#",1)==0) continue;
       std::cout << line << std::endl;
       std::stringstream ss(line);
       ss >> tofcut >> ptcut >> etabins >> ihbins >> pbins >> massbins >> masscut >> tofbins >> fpixbins >> dataset >> version;
   }
   ifile.close();

   std::cout << "Successfully closed config file "  << std::endl;

   TChain* chain;

   std::string pathHigh = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/SingleMuon/";
   std::string pathMC = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/MC/";
   std::string pathSignal = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V80p3/";


   std::string pathTestMC = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V80p5/";
   std::string pathData = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V80p6/";
   std::string pathTest = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V80p9/";
   std::string pathLast2017F = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V80p8/";

   //Change the TChain if needed
   chain = new TChain("HSCParticleAnalyzer/BaseName/HscpCandidates");


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
              
       std::string fileNamesMu2017[] = { (pathData+"SingleMuon2017_all.txt").c_str(),(pathLast2017F+"SingleMuon2017_all.txt").c_str()};
       //std::string fileNamesMu2017[] = { (pathData+"SingleMuon2017_1file.txt").c_str()};

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
       //ALL FILES 

       //std::string fileNamesMu2018[] = { (pathData+"singleMu2018B.txt").c_str()};

       std::string fileNamesMu2018[] = { (pathData+"SingleMuon2018_all.txt").c_str()};

       //std::string fileNamesMu2018[] = { (pathData+"singleMu2018B_1file.txt").c_str()};
       //std::string fileNamesMu2018[] = { (pathData+"singleMu2018B_50file.txt").c_str()};

       for (const std::string& fileName : fileNamesMu2018) {
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
   if(dataset == "qcd50_80"){
       std::string fileNamesQCD[] = {(pathMC+"QCDPt-50To80.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "qcd80_120"){
       std::string fileNamesQCD[] = {(pathMC+"QCDPt-80To120.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
 
 
   if(dataset == "qcd120_170"){
       std::string fileNamesQCD[] = {(pathMC+"QCDPt-120To170.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
 
   if(dataset == "qcd170_300"){
       std::string fileNamesQCD[] = {(pathMC+"QCDPt-170To300.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "qcd300_470"){
       std::string fileNamesQCD[] = {(pathMC+"QCDPt-300To470.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
 
   if(dataset == "qcd470_600"){
       std::string fileNamesQCD[] = {(pathMC+"QCDPt-470To600.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
 
   if(dataset == "qcd600_800"){
       std::string fileNamesQCD[] = {(pathMC+"QCDPt-600To800.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "qcd1000"){
       std::string fileNamesQCD[] = {(pathMC+"QCDPt-1000.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
 
 
 
   //RUNNING ON MC background WjetsToLNu 0jets
   if(dataset == "wjets_0j"){
       std::string fileNamesQCD[] = {(pathMC+"WJetsToLNu0J.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "wjets_1j"){
       std::string fileNamesQCD[] = {(pathMC+"WJetsToLNu1J.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "wjets_2j"){
       std::string fileNamesQCD[] = {(pathMC+"WJetsToLNu2J.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "zmumu50_120"){
       //std::string fileNamesQCD[] = {(pathMC+"ZToMuMuM-50To120.txt").c_str()};
       std::string fileNamesQCD[] = {(pathTestMC+"ZtoMuMu_50to120.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "zmumu200_400"){
       std::string fileNamesQCD[] = {(pathTestMC+"ZtoMuMu_200to400.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "zmumu400_800"){
       std::string fileNamesQCD[] = {(pathTestMC+"ZtoMuMu_400to800.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "zmumu800_1400"){
       std::string fileNamesQCD[] = {(pathTestMC+"ZtoMuMu_800to1400.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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

   if(dataset == "zmumu120_200"){
       std::string fileNamesQCD[] = {(pathTestMC+"ZToMuMu_120To200.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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


   if(dataset == "zmumu3500_4500"){
       std::string fileNamesQCD[] = {(pathTestMC+"ZtoMuMu_3500to4500.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "zmumu4500_6000"){
       std::string fileNamesQCD[] = {(pathMC+"ZToMuMuM-4500To6000.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   //RUNNING ON pairStau 557    
   
   if(dataset == "tauprime2e100"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge2e_M-100.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime2e200"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge2e_M-200.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime2e400"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge2e_M-400.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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

   if(dataset == "tauprime2e500"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge2e_M-500.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime2e800"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge2e_M-800.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime2e1400"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge2e_M-1400.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime2e1800"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge2e_M-1800.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime2e2200"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge2e_M-2200.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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

   if(dataset == "tauprime2e2600"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge2e_M-2600.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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


   if(dataset == "tauprime1e100"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge1e_M-100.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime1e200"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge1e_M-200.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime1e400"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge1e_M-400.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime1e500"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge1e_M-500.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime1e800"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge1e_M-800.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime1e1000"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge1e_M-1000.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime1e1800"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge1e_M-1800.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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

   if(dataset == "tauprime1e2200"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge1e_M-2200.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "tauprime1e2600"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPtauPrimeCharge1e_M-2600.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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






   if(dataset == "pStau200"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPpairStau_M-200.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "pStau247"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPpairStau_M-247.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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

   if(dataset == "pStau308"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPpairStau_M-308.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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

   if(dataset == "pStau432"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPpairStau_M-432.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
       //std::string p432 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/"+version+"/HSCPpairStau_"+version+"/Merged_HSCPpairStau/HSCPpairStau_M-432_merged.root";
       //chain->AddFile(p432.c_str());
   }

   if(dataset == "pStau557"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPpairStau_M-557.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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

   if(dataset == "pStau651"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPpairStau_M-651.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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

   if(dataset == "pStau745"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPpairStau_M-745.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
   if(dataset == "pStau871"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPpairStau_M-871.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
       //std::string p871 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/"+version+"/HSCPpairStau_"+version+"/Merged_HSCPpairStau/HSCPpairStau_M-871_merged.root";
       //chain->AddFile(p871.c_str());
   }
   if(dataset == "pStau1029"){
       std::string fileNamesQCD[] = {(pathTest+"HSCPpairStau_M-1029.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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


   if(dataset == "Glu800"){
       chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-800_CodeV7fn_v1/results/Gluino800_CodeV7fn.root");
   }
   if(dataset == "Glu1000"){
       std::string fileNamesQCD[] = {(pathSignal+"gluino1000.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
       //chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-1000_CodeV7fn_v1/results/Gluino1000_CodeV7fn.root");
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
       std::string fileNamesQCD[] = {(pathSignal+"gluino2000.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
       //chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-2000_CodeV7fn_v1/results/Gluino2000_CodeV7fn.root");
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

   if(dataset == "Stop1000"){
       std::string fileNamesQCD[] = {(pathSignal+"stop1000.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
       //chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-2600_CodeV7fn_v1/results/Gluino2600_CodeV7fn.root");
   }

   if(dataset == "Stop2000"){
       std::string fileNamesQCD[] = {(pathSignal+"stop2000.txt").c_str()};

       for (const std::string& fileName : fileNamesQCD) {
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
       //chain->AddFile("/opt/sbg/cms/ui2_data1/rhaeberl/CMSSW_10_6_30/src/crab_projects/crab_Analysis_2018_HSCPgluino_M-2600_CodeV7fn_v1/results/Gluino2600_CodeV7fn.root");
   }


   std::string binning = std::to_string(tofcut) + "," + std::to_string(ptcut) + "," + std::to_string(etabins) + "," + std::to_string(ihbins) + "," + std::to_string(pbins) + "," + std::to_string(massbins) + "," + std::to_string(masscut)  + "," +std::to_string(tofbins) + ","+std::to_string(fpixbins) + "," + dataset + "," + version;
 
   std::cout << "Running over dataset : " << dataset << " with code version : " << version << std::endl; 
   std::cout << "Defining regions A,B,C,D with pT cut = " << ptcut << " or tof cut = " << tofcut << std::endl;
   std::cout << "Eta #bin : "<< etabins << ", Ih #bins" << ihbins << " , P #bins " << pbins << " , mass #bins " << massbins << " ,  tof #bins " << tofbins << " , Fpix #bins " << fpixbins << " and mass cut = " << masscut << std::endl;
     

   //chain->StopCacheLearningPhase();  
   //parallelize with proof
   chain->SetProof();
   chain->Process("HSCPSelector.C+",binning.c_str());

   delete chain;
}
