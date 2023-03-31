//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Mar 31 10:54:37 2023 by ROOT version 6.14/09
// from TTree HscpCandidates/HscpCandidates
// found on file: /opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/prodJan2023_CMSSW_10_6_30/HSCP_framework/UL2017/UL2017_RunF/Histos_2017F_merged.root
//////////////////////////////////////////////////////////

#ifndef HSCPSelector_h
#define HSCPSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <vector>

#include "inc/RegionMassPlot.h"

class HSCPSelector : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   //Test
   TFile* fout;
   RegionMassPlot* mrp;
  

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<UInt_t> Trig = {fReader, "Trig"};
   TTreeReaderValue<UInt_t> Run = {fReader, "Run"};
   TTreeReaderValue<ULong64_t> Event = {fReader, "Event"};
   TTreeReaderValue<UInt_t> Lumi = {fReader, "Lumi"};
   TTreeReaderValue<UInt_t> PileUp = {fReader, "PileUp"};
   TTreeReaderArray<int> BunchXing = {fReader, "BunchXing"};
   TTreeReaderArray<int> nPU = {fReader, "nPU"};
   TTreeReaderArray<float> nPUmean = {fReader, "nPUmean"};
   TTreeReaderValue<UInt_t> nofVtx = {fReader, "nofVtx"};
   TTreeReaderValue<Int_t> npv = {fReader, "npv"};
   TTreeReaderArray<float> pvX = {fReader, "pvX"};
   TTreeReaderArray<float> pvY = {fReader, "pvY"};
   TTreeReaderArray<float> pvZ = {fReader, "pvZ"};
   TTreeReaderArray<float> pvRho = {fReader, "pvRho"};
   TTreeReaderArray<int> pvNdof = {fReader, "pvNdof"};
   TTreeReaderArray<float> pvChi2 = {fReader, "pvChi2"};
   TTreeReaderArray<float> pvSumPt2 = {fReader, "pvSumPt2"};
   TTreeReaderValue<UInt_t> Hscp = {fReader, "Hscp"};
   TTreeReaderValue<UInt_t> nMuons = {fReader, "nMuons"};
   TTreeReaderValue<UInt_t> njets = {fReader, "njets"};
   TTreeReaderValue<Float_t> Weight = {fReader, "Weight"};
   TTreeReaderValue<Float_t> GeneratorWeight = {fReader, "GeneratorWeight"};
   TTreeReaderValue<Float_t> GeneratorBinningValues = {fReader, "GeneratorBinningValues"};
   TTreeReaderValue<vector<bool>> triggerDecision = {fReader, "triggerDecision"};
   TTreeReaderArray<int> triggerHLTPrescale = {fReader, "triggerHLTPrescale"};
   TTreeReaderArray<vector<float>> triggerObjectE = {fReader, "triggerObjectE"};
   TTreeReaderArray<vector<float>> triggerObjectPt = {fReader, "triggerObjectPt"};
   TTreeReaderArray<vector<float>> triggerObjectEta = {fReader, "triggerObjectEta"};
   TTreeReaderArray<vector<float>> triggerObjectPhi = {fReader, "triggerObjectPhi"};
   TTreeReaderValue<Bool_t> HLT_Mu50 = {fReader, "HLT_Mu50"};
   TTreeReaderValue<Bool_t> HLT_PFMET120_PFMHT120_IDTight = {fReader, "HLT_PFMET120_PFMHT120_IDTight"};
   TTreeReaderValue<Bool_t> HLT_PFHT500_PFMET100_PFMHT100_IDTight = {fReader, "HLT_PFHT500_PFMET100_PFMHT100_IDTight"};
   TTreeReaderValue<Bool_t> HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60 = {fReader, "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"};
   TTreeReaderValue<Bool_t> HLT_MET105_IsoTrk50 = {fReader, "HLT_MET105_IsoTrk50"};
   TTreeReaderValue<Float_t> RecoCaloMET = {fReader, "RecoCaloMET"};
   TTreeReaderValue<Float_t> RecoCaloMET_phi = {fReader, "RecoCaloMET_phi"};
   TTreeReaderValue<Float_t> RecoCaloMET_sigf = {fReader, "RecoCaloMET_sigf"};
   TTreeReaderValue<Float_t> RecoPFMET = {fReader, "RecoPFMET"};
   TTreeReaderValue<Float_t> RecoPFMET_phi = {fReader, "RecoPFMET_phi"};
   TTreeReaderValue<Float_t> RecoPFMET_sigf = {fReader, "RecoPFMET_sigf"};
   TTreeReaderValue<Float_t> RecoPFMHT = {fReader, "RecoPFMHT"};
   TTreeReaderValue<Float_t> HLTCaloMET = {fReader, "HLTCaloMET"};
   TTreeReaderValue<Float_t> HLTCaloMET_phi = {fReader, "HLTCaloMET_phi"};
   TTreeReaderValue<Float_t> HLTCaloMET_sigf = {fReader, "HLTCaloMET_sigf"};
   TTreeReaderValue<Float_t> HLTCaloMETClean = {fReader, "HLTCaloMETClean"};
   TTreeReaderValue<Float_t> HLTCaloMETClean_phi = {fReader, "HLTCaloMETClean_phi"};
   TTreeReaderValue<Float_t> HLTCaloMETClean_sigf = {fReader, "HLTCaloMETClean_sigf"};
   TTreeReaderValue<Float_t> HLTCaloMHT = {fReader, "HLTCaloMHT"};
   TTreeReaderValue<Float_t> HLTCaloMHT_phi = {fReader, "HLTCaloMHT_phi"};
   TTreeReaderValue<Float_t> HLTCaloMHT_sigf = {fReader, "HLTCaloMHT_sigf"};
   TTreeReaderValue<Float_t> HLTPFMET = {fReader, "HLTPFMET"};
   TTreeReaderValue<Float_t> HLTPFMET_phi = {fReader, "HLTPFMET_phi"};
   TTreeReaderValue<Float_t> HLTPFMET_sigf = {fReader, "HLTPFMET_sigf"};
   TTreeReaderValue<Float_t> HLTPFMHT = {fReader, "HLTPFMHT"};
   TTreeReaderValue<Float_t> HLTPFMHT_phi = {fReader, "HLTPFMHT_phi"};
   TTreeReaderValue<Float_t> HLTPFMHT_sigf = {fReader, "HLTPFMHT_sigf"};
   TTreeReaderValue<Bool_t> matchedMuonWasFound = {fReader, "matchedMuonWasFound"};
   TTreeReaderArray<int> gParticleId = {fReader, "gParticleId"};
   TTreeReaderArray<int> gParticleStatus = {fReader, "gParticleStatus"};
   TTreeReaderArray<float> gParticleE = {fReader, "gParticleE"};
   TTreeReaderArray<float> gParticlePt = {fReader, "gParticlePt"};
   TTreeReaderArray<float> gParticlePz = {fReader, "gParticlePz"};
   TTreeReaderArray<float> gParticleEta = {fReader, "gParticleEta"};
   TTreeReaderArray<float> gParticlePhi = {fReader, "gParticlePhi"};
   TTreeReaderArray<float> gParticleBeta = {fReader, "gParticleBeta"};
   TTreeReaderArray<int> gParticleCharge = {fReader, "gParticleCharge"};
   TTreeReaderArray<float> gParticleProdVertexX = {fReader, "gParticleProdVertexX"};
   TTreeReaderArray<float> gParticleProdVertexY = {fReader, "gParticleProdVertexY"};
   TTreeReaderArray<float> gParticleProdVertexZ = {fReader, "gParticleProdVertexZ"};
   TTreeReaderArray<int> gParticleMotherId = {fReader, "gParticleMotherId"};
   TTreeReaderArray<int> gParticleMotherIndex = {fReader, "gParticleMotherIndex"};
   TTreeReaderArray<float> eleE = {fReader, "eleE"};
   TTreeReaderArray<float> elePt = {fReader, "elePt"};
   TTreeReaderArray<float> eleEta = {fReader, "eleEta"};
   TTreeReaderArray<float> elePhi = {fReader, "elePhi"};
   TTreeReaderArray<float> eleCharge = {fReader, "eleCharge"};
   TTreeReaderArray<float> eleE_SC = {fReader, "eleE_SC"};
   TTreeReaderArray<float> eleEta_SC = {fReader, "eleEta_SC"};
   TTreeReaderArray<float> elePhi_SC = {fReader, "elePhi_SC"};
   TTreeReaderArray<float> eleSigmaIetaIeta = {fReader, "eleSigmaIetaIeta"};
   TTreeReaderArray<float> eleFull5x5SigmaIetaIeta = {fReader, "eleFull5x5SigmaIetaIeta"};
   TTreeReaderArray<float> eleR9 = {fReader, "eleR9"};
   TTreeReaderArray<float> ele_dEta = {fReader, "ele_dEta"};
   TTreeReaderArray<float> ele_dPhi = {fReader, "ele_dPhi"};
   TTreeReaderArray<float> ele_HoverE = {fReader, "ele_HoverE"};
   TTreeReaderArray<float> ele_d0 = {fReader, "ele_d0"};
   TTreeReaderArray<float> ele_dZ = {fReader, "ele_dZ"};
   TTreeReaderArray<float> ele_pileupIso = {fReader, "ele_pileupIso"};
   TTreeReaderArray<float> ele_chargedIso = {fReader, "ele_chargedIso"};
   TTreeReaderArray<float> ele_photonIso = {fReader, "ele_photonIso"};
   TTreeReaderArray<float> ele_neutralHadIso = {fReader, "ele_neutralHadIso"};
   TTreeReaderArray<int> ele_MissHits = {fReader, "ele_MissHits"};
   TTreeReaderValue<vector<bool>> ele_passCutBasedIDVeto = {fReader, "ele_passCutBasedIDVeto"};
   TTreeReaderValue<vector<bool>> ele_passCutBasedIDLoose = {fReader, "ele_passCutBasedIDLoose"};
   TTreeReaderValue<vector<bool>> ele_passCutBasedIDMedium = {fReader, "ele_passCutBasedIDMedium"};
   TTreeReaderValue<vector<bool>> ele_passCutBasedIDTight = {fReader, "ele_passCutBasedIDTight"};
   TTreeReaderValue<vector<bool>> ele_passMVAIsoIDWP80 = {fReader, "ele_passMVAIsoIDWP80"};
   TTreeReaderValue<vector<bool>> ele_passMVAIsoIDWP90 = {fReader, "ele_passMVAIsoIDWP90"};
   TTreeReaderValue<vector<bool>> ele_passMVAIsoIDWPHZZ = {fReader, "ele_passMVAIsoIDWPHZZ"};
   TTreeReaderValue<vector<bool>> ele_passMVAIsoIDWPLoose = {fReader, "ele_passMVAIsoIDWPLoose"};
   TTreeReaderValue<vector<bool>> ele_passMVANoIsoIDWP80 = {fReader, "ele_passMVANoIsoIDWP80"};
   TTreeReaderValue<vector<bool>> ele_passMVANoIsoIDWP90 = {fReader, "ele_passMVANoIsoIDWP90"};
   TTreeReaderValue<vector<bool>> ele_passMVANoIsoIDWPLoose = {fReader, "ele_passMVANoIsoIDWPLoose"};
   TTreeReaderValue<vector<bool>> ele_PassConvVeto = {fReader, "ele_PassConvVeto"};
   TTreeReaderArray<float> ele_OneOverEminusOneOverP = {fReader, "ele_OneOverEminusOneOverP"};
   TTreeReaderArray<float> muonE = {fReader, "muonE"};
   TTreeReaderArray<float> muonPt = {fReader, "muonPt"};
   TTreeReaderArray<float> muonEta = {fReader, "muonEta"};
   TTreeReaderArray<float> muonPhi = {fReader, "muonPhi"};
   TTreeReaderArray<int> muonCharge = {fReader, "muonCharge"};
   TTreeReaderValue<vector<bool>> muonIsLoose = {fReader, "muonIsLoose"};
   TTreeReaderValue<vector<bool>> muonIsMedium = {fReader, "muonIsMedium"};
   TTreeReaderValue<vector<bool>> muonIsTight = {fReader, "muonIsTight"};
   TTreeReaderArray<float> muon_d0 = {fReader, "muon_d0"};
   TTreeReaderArray<float> muon_d0Err = {fReader, "muon_d0Err"};
   TTreeReaderArray<float> muon_dZ = {fReader, "muon_dZ"};
   TTreeReaderArray<float> muon_ip3d = {fReader, "muon_ip3d"};
   TTreeReaderArray<float> muon_ip3dSignificance = {fReader, "muon_ip3dSignificance"};
   TTreeReaderArray<unsigned int> muonType = {fReader, "muonType"};
   TTreeReaderArray<unsigned int> muonQuality = {fReader, "muonQuality"};
   TTreeReaderArray<float> muon_pileupIso = {fReader, "muon_pileupIso"};
   TTreeReaderArray<float> muon_chargedIso = {fReader, "muon_chargedIso"};
   TTreeReaderArray<float> muon_photonIso = {fReader, "muon_photonIso"};
   TTreeReaderArray<float> muon_neutralHadIso = {fReader, "muon_neutralHadIso"};
   TTreeReaderArray<float> muon_validFractionTrackerHits = {fReader, "muon_validFractionTrackerHits"};
   TTreeReaderArray<float> muTree_muon_normChi2onE = {fReader, "muTree_muon_normChi2onE"};
   TTreeReaderArray<float> muon_chi2LocalPosition = {fReader, "muon_chi2LocalPosition"};
   TTreeReaderArray<float> muon_kinkFinder = {fReader, "muon_kinkFinder"};
   TTreeReaderArray<float> muon_segmentCompatability = {fReader, "muon_segmentCompatability"};
   TTreeReaderArray<float> muon_trkIso = {fReader, "muon_trkIso"};
   TTreeReaderArray<float> muon_tuneP_Pt = {fReader, "muon_tuneP_Pt"};
   TTreeReaderArray<float> muon_tuneP_PtErr = {fReader, "muon_tuneP_PtErr"};
   TTreeReaderArray<float> muon_tuneP_Eta = {fReader, "muon_tuneP_Eta"};
   TTreeReaderArray<float> muon_tuneP_Phi = {fReader, "muon_tuneP_Phi"};
   TTreeReaderArray<int> muon_tuneP_MuonBestTrackType = {fReader, "muon_tuneP_MuonBestTrackType"};
   TTreeReaderValue<vector<bool>> muon_isHighPtMuon = {fReader, "muon_isHighPtMuon"};
   TTreeReaderValue<vector<bool>> muon_isTrackerHighPtMuon = {fReader, "muon_isTrackerHighPtMuon"};
   TTreeReaderArray<float> Jet_pt = {fReader, "Jet_pt"};
   TTreeReaderArray<float> Jet_eta = {fReader, "Jet_eta"};
   TTreeReaderArray<float> Jet_phi = {fReader, "Jet_phi"};
   TTreeReaderArray<float> Jet_mass = {fReader, "Jet_mass"};
   TTreeReaderArray<float> Jet_energy = {fReader, "Jet_energy"};
   TTreeReaderArray<float> Jet_pdgId = {fReader, "Jet_pdgId"};
   TTreeReaderArray<float> Jet_et = {fReader, "Jet_et"};
   TTreeReaderArray<float> Jet_chargedEmEnergyFraction = {fReader, "Jet_chargedEmEnergyFraction"};
   TTreeReaderArray<float> Jet_neutralEmEnergyFraction = {fReader, "Jet_neutralEmEnergyFraction"};
   TTreeReaderArray<float> Jet_chargedHadronEnergyFraction = {fReader, "Jet_chargedHadronEnergyFraction"};
   TTreeReaderArray<float> Jet_neutralHadronEnergyFraction = {fReader, "Jet_neutralHadronEnergyFraction"};
   TTreeReaderArray<float> Jet_muonEnergyFraction = {fReader, "Jet_muonEnergyFraction"};
   TTreeReaderArray<int> Jet_chargedMultiplicity = {fReader, "Jet_chargedMultiplicity"};
   TTreeReaderArray<int> Jet_neutralMultiplicity = {fReader, "Jet_neutralMultiplicity"};
   TTreeReaderArray<float> Jet_jetArea = {fReader, "Jet_jetArea"};
   TTreeReaderArray<float> Jet_pileupE = {fReader, "Jet_pileupE"};
   TTreeReaderArray<float> mT = {fReader, "mT"};
   TTreeReaderValue<vector<bool>> passCutPt55 = {fReader, "passCutPt55"};
   TTreeReaderValue<vector<bool>> passPreselection = {fReader, "passPreselection"};
   TTreeReaderValue<vector<bool>> passPreselectionSept8 = {fReader, "passPreselectionSept8"};
   TTreeReaderValue<vector<bool>> passSelection = {fReader, "passSelection"};
   TTreeReaderValue<vector<bool>> isPFMuon = {fReader, "isPFMuon"};
   TTreeReaderValue<vector<bool>> PFMuonPt = {fReader, "PFMuonPt"};
   TTreeReaderArray<float> Charge = {fReader, "Charge"};
   TTreeReaderArray<float> Pt = {fReader, "Pt"};
   TTreeReaderArray<float> PtErr = {fReader, "PtErr"};
   TTreeReaderArray<float> Is_StripOnly = {fReader, "Is_StripOnly"};
   TTreeReaderArray<float> Ias = {fReader, "Ias"};
   TTreeReaderArray<float> Ias_noTIBnoTIDno3TEC = {fReader, "Ias_noTIBnoTIDno3TEC"};
   TTreeReaderArray<float> Ias_PixelOnly = {fReader, "Ias_PixelOnly"};
   TTreeReaderArray<float> Ias_StripOnly = {fReader, "Ias_StripOnly"};
   TTreeReaderArray<float> Ias_PixelOnly_noL1 = {fReader, "Ias_PixelOnly_noL1"};
   TTreeReaderArray<float> Ih = {fReader, "Ih"};
   TTreeReaderArray<float> Ick = {fReader, "Ick"};
   TTreeReaderArray<float> Fmip = {fReader, "Fmip"};
   TTreeReaderArray<float> ProbXY = {fReader, "ProbXY"};
   TTreeReaderArray<float> ProbXY_noL1 = {fReader, "ProbXY_noL1"};
   TTreeReaderArray<float> ProbQ = {fReader, "ProbQ"};
   TTreeReaderArray<float> ProbQ_noL1 = {fReader, "ProbQ_noL1"};
   TTreeReaderArray<float> Ndof = {fReader, "Ndof"};
   TTreeReaderArray<float> Chi2 = {fReader, "Chi2"};
   TTreeReaderArray<int> QualityMask = {fReader, "QualityMask"};
   TTreeReaderValue<vector<bool>> isHighPurity = {fReader, "isHighPurity"};
   TTreeReaderArray<float> EoverP = {fReader, "EoverP"};
   TTreeReaderValue<vector<bool>> isMuon = {fReader, "isMuon"};
   TTreeReaderValue<vector<bool>> isPhoton = {fReader, "isPhoton"};
   TTreeReaderValue<vector<bool>> isElectron = {fReader, "isElectron"};
   TTreeReaderValue<vector<bool>> isChHadron = {fReader, "isChHadron"};
   TTreeReaderValue<vector<bool>> isNeutHadron = {fReader, "isNeutHadron"};
   TTreeReaderValue<vector<bool>> isPfTrack = {fReader, "isPfTrack"};
   TTreeReaderValue<vector<bool>> isUndefined = {fReader, "isUndefined"};
   TTreeReaderArray<float> ECAL_energy = {fReader, "ECAL_energy"};
   TTreeReaderArray<float> HCAL_energy = {fReader, "HCAL_energy"};
   TTreeReaderArray<float> TOF = {fReader, "TOF"};
   TTreeReaderArray<float> TOFErr = {fReader, "TOFErr"};
   TTreeReaderArray<unsigned int> TOF_ndof = {fReader, "TOF_ndof"};
   TTreeReaderArray<float> DTTOF = {fReader, "DTTOF"};
   TTreeReaderArray<float> DTTOFErr = {fReader, "DTTOFErr"};
   TTreeReaderArray<unsigned int> DTTOF_ndof = {fReader, "DTTOF_ndof"};
   TTreeReaderArray<float> CSCTOF = {fReader, "CSCTOF"};
   TTreeReaderArray<float> CSCTOFErr = {fReader, "CSCTOFErr"};
   TTreeReaderArray<unsigned int> CSCTOF_ndof = {fReader, "CSCTOF_ndof"};
   TTreeReaderArray<float> Mass = {fReader, "Mass"};
   TTreeReaderArray<float> MassErr = {fReader, "MassErr"};
   TTreeReaderArray<float> dZ = {fReader, "dZ"};
   TTreeReaderArray<float> dXY = {fReader, "dXY"};
   TTreeReaderArray<float> dR = {fReader, "dR"};
   TTreeReaderArray<float> p = {fReader, "p"};
   TTreeReaderArray<float> eta = {fReader, "eta"};
   TTreeReaderArray<float> phi = {fReader, "phi"};
   TTreeReaderArray<unsigned int> NOH = {fReader, "NOH"};
   TTreeReaderArray<unsigned int> NOPH = {fReader, "NOPH"};
   TTreeReaderArray<float> FOVH = {fReader, "FOVH"};
   TTreeReaderArray<unsigned int> NOMH = {fReader, "NOMH"};
   TTreeReaderArray<float> FOVHD = {fReader, "FOVHD"};
   TTreeReaderArray<unsigned int> NOM = {fReader, "NOM"};
   TTreeReaderArray<float> matchTrigMuon_minDeltaR = {fReader, "matchTrigMuon_minDeltaR"};
   TTreeReaderArray<float> matchTrigMuon_pT = {fReader, "matchTrigMuon_pT"};
   TTreeReaderArray<float> iso_TK = {fReader, "iso_TK"};
   TTreeReaderArray<float> iso_ECAL = {fReader, "iso_ECAL"};
   TTreeReaderArray<float> iso_HCAL = {fReader, "iso_HCAL"};
   TTreeReaderArray<float> track_genTrackMiniIsoSumPt = {fReader, "track_genTrackMiniIsoSumPt"};
   TTreeReaderArray<float> HSCP_tuneP_Pt = {fReader, "HSCP_tuneP_Pt"};
   TTreeReaderArray<float> HSCP_tuneP_PtErr = {fReader, "HSCP_tuneP_PtErr"};
   TTreeReaderArray<float> HSCP_tuneP_Eta = {fReader, "HSCP_tuneP_Eta"};
   TTreeReaderArray<float> HSCP_tuneP_Phi = {fReader, "HSCP_tuneP_Phi"};
   TTreeReaderArray<int> HSCP_tuneP_MuonBestTrackType = {fReader, "HSCP_tuneP_MuonBestTrackType"};
   TTreeReaderArray<int> HSCP_ErrorHisto_bin = {fReader, "HSCP_ErrorHisto_bin"};
   TTreeReaderArray<int> HSCP_type = {fReader, "HSCP_type"};
   TTreeReaderArray<float> PFMiniIso_relative = {fReader, "PFMiniIso_relative"};
   TTreeReaderArray<float> PFMiniIso_wMuon_relative = {fReader, "PFMiniIso_wMuon_relative"};
   TTreeReaderArray<float> TrackPFIsolationR005_sumChargedHadronPt = {fReader, "TrackPFIsolationR005_sumChargedHadronPt"};
   TTreeReaderArray<float> TrackPFIsolationR005_sumNeutralHadronPt = {fReader, "TrackPFIsolationR005_sumNeutralHadronPt"};
   TTreeReaderArray<float> TrackPFIsolationR005_sumPhotonPt = {fReader, "TrackPFIsolationR005_sumPhotonPt"};
   TTreeReaderArray<float> TrackPFIsolationR005_sumPUPt = {fReader, "TrackPFIsolationR005_sumPUPt"};
   TTreeReaderArray<float> TrackPFIsolationR01_sumChargedHadronPt = {fReader, "TrackPFIsolationR01_sumChargedHadronPt"};
   TTreeReaderArray<float> TrackPFIsolationR01_sumNeutralHadronPt = {fReader, "TrackPFIsolationR01_sumNeutralHadronPt"};
   TTreeReaderArray<float> TrackPFIsolationR01_sumPhotonPt = {fReader, "TrackPFIsolationR01_sumPhotonPt"};
   TTreeReaderArray<float> TrackPFIsolationR01_sumPUPt = {fReader, "TrackPFIsolationR01_sumPUPt"};
   TTreeReaderArray<float> TrackPFIsolationR03_sumChargedHadronPt = {fReader, "TrackPFIsolationR03_sumChargedHadronPt"};
   TTreeReaderArray<float> TrackPFIsolationR03_sumNeutralHadronPt = {fReader, "TrackPFIsolationR03_sumNeutralHadronPt"};
   TTreeReaderArray<float> TrackPFIsolationR03_sumPhotonPt = {fReader, "TrackPFIsolationR03_sumPhotonPt"};
   TTreeReaderArray<float> TrackPFIsolationR03_sumPUPt = {fReader, "TrackPFIsolationR03_sumPUPt"};
   TTreeReaderArray<float> TrackPFIsolationR05_sumChargedHadronPt = {fReader, "TrackPFIsolationR05_sumChargedHadronPt"};
   TTreeReaderArray<float> TrackPFIsolationR05_sumNeutralHadronPt = {fReader, "TrackPFIsolationR05_sumNeutralHadronPt"};
   TTreeReaderArray<float> TrackPFIsolationR05_sumPhotonPt = {fReader, "TrackPFIsolationR05_sumPhotonPt"};
   TTreeReaderArray<float> TrackPFIsolationR05_sumPUPt = {fReader, "TrackPFIsolationR05_sumPUPt"};
   TTreeReaderArray<float> MuonPFIsolationR03_sumChargedHadronPt = {fReader, "MuonPFIsolationR03_sumChargedHadronPt"};
   TTreeReaderArray<float> MuonPFIsolationR03_sumNeutralHadronPt = {fReader, "MuonPFIsolationR03_sumNeutralHadronPt"};
   TTreeReaderArray<float> MuonPFIsolationR03_sumPhotonPt = {fReader, "MuonPFIsolationR03_sumPhotonPt"};
   TTreeReaderArray<float> MuonPFIsolationR03_sumPUPt = {fReader, "MuonPFIsolationR03_sumPUPt"};
   TTreeReaderArray<float> Ih_noL1 = {fReader, "Ih_noL1"};
   TTreeReaderArray<float> Ih_15drop = {fReader, "Ih_15drop"};
   TTreeReaderArray<float> Ih_StripOnly = {fReader, "Ih_StripOnly"};
   TTreeReaderArray<float> Ih_StripOnly_15drop = {fReader, "Ih_StripOnly_15drop"};
   TTreeReaderArray<float> Ih_PixelOnly_noL1 = {fReader, "Ih_PixelOnly_noL1"};
   TTreeReaderArray<float> Ih_SaturationCorrectionFromFits = {fReader, "Ih_SaturationCorrectionFromFits"};
   TTreeReaderArray<vector<float>> clust_charge = {fReader, "clust_charge"};
   TTreeReaderArray<vector<float>> clust_pathlength = {fReader, "clust_pathlength"};
   TTreeReaderArray<vector<unsigned int>> clust_nstrip = {fReader, "clust_nstrip"};
   TTreeReaderArray<vector<bool>> clust_sat254 = {fReader, "clust_sat254"};
   TTreeReaderArray<vector<bool>> clust_sat255 = {fReader, "clust_sat255"};
   TTreeReaderArray<vector<unsigned int>> clust_detid = {fReader, "clust_detid"};
   TTreeReaderArray<vector<bool>> clust_isStrip = {fReader, "clust_isStrip"};
   TTreeReaderArray<vector<bool>> clust_isPixel = {fReader, "clust_isPixel"};
   TTreeReaderArray<float> GenId = {fReader, "GenId"};
   TTreeReaderArray<float> GenCharge = {fReader, "GenCharge"};
   TTreeReaderArray<float> GenMass = {fReader, "GenMass"};
   TTreeReaderArray<float> GenPt = {fReader, "GenPt"};
   TTreeReaderArray<float> GenEta = {fReader, "GenEta"};
   TTreeReaderArray<float> GenPhi = {fReader, "GenPhi"};


   HSCPSelector(TTree * /*tree*/ =0) { }
   virtual ~HSCPSelector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(HSCPSelector,0);

};

#endif

#ifdef HSCPSelector_cxx
void HSCPSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t HSCPSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef HSCPSelector_cxx
