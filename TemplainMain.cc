#include <TProof.h>
#include <TChain.h>

int toto(){
   //use proof-lite: running on all nodes of the machine
   TProof* proof = TProof::Open("");
   proof->SetParallel(1);
   TChain* chain = new TChain("HSCParticleAnalyzer/BaseName/HscpCandidates");
   chain->AddFile("/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/prodJan2023_CMSSW_10_6_30/HSCP_framework/UL2017/UL2017_RunF/Histos_2017F_merged.root");
   //parallelize with proof
   chain->SetProof();

   chain->Process("HSCPSelector.C+");

}
