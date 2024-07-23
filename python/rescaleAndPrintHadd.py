import ROOT, sys, os, time, re, numpy
from optparse import OptionParser
from tqdm import tqdm
from USE_DATE import USED_DATE, VERSION
#just the number, like 18p2

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetPadRightMargin(.15)
ROOT.gStyle.SetPadTopMargin(0.1);
ROOT.gStyle.SetPadBottomMargin(0.14);
ROOT.gStyle.SetPadLeftMargin(0.15);


version = VERSION

#pathSig = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/TriggerStudies/Seeds_infos/HSCP_glu_stau/stau/"
#pathSigStau = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/prodSeptember2023_CMSSW_10_6_30/14_september/"
pathSigStau = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPpairStau_V"+version+"/Merged_HSCPpairStau/"
pathSigGluino = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPgluino_V"+version+"/Merged_HSCPgluino/"
pathSigHSCPtauPrimeCharge2e = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPtauPrimeCharge2e_V"+version+"/Merged_HSCPtauPrimeCharge2e/"
pathSigHSCPtauPrimeCharge1e = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPtauPrimeCharge1e_V"+version+"/Merged_HSCPtauPrimeCharge1e/"
pathSigHSCPstop = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPstop_V"+version+"/Merged_HSCPstop/"
pathSigHSCPgmsbStau = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPgmsbStau_V"+version+"/Merged_HSCPgmsbStau/"
pathSigHSCPstopOnlyNeutral = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPstopOnlyNeutral_V"+version+"/Merged_HSCPstopOnlyNeutral/"
pathSigHSCPgluinoOnlyNeutral = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPgluinoOnlyNeutral_V"+version+"/Merged_HSCPgluinoOnlyNeutral/"

pathSigHSCPZPrime_tauM200 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPtauPrime2e-200_V"+version+"/Merged_HSCPtauPrime2e-200/"
pathSigHSCPZPrime_tauM400 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPtauPrime2e-400_V"+version+"/Merged_HSCPtauPrime2e-400/"
pathSigHSCPZPrime_tauM600 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPtauPrime2e-600_V"+version+"/Merged_HSCPtauPrime2e-600/"
pathSigHSCPZPrime_tauM800 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPtauPrime2e-800_V"+version+"/Merged_HSCPtauPrime2e-800/"
pathSigHSCPZPrime_tauM1000 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPtauPrime2e-1000_V"+version+"/Merged_HSCPtauPrime2e-1000/"
pathSigHSCPZPrime_tauM1200 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPtauPrime2e-1200_V"+version+"/Merged_HSCPtauPrime2e-1200/"
pathSigHSCPZPrime_tauM1400 = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPtauPrime2e-1400_V"+version+"/Merged_HSCPtauPrime2e-1400/"

#print("Recaling signal histograms from production within {}".format(pathSig[-39:]))
'''
BackgroundSamples = [
pathSigStau + "HSCPpairStau_M-200_merged.root",
pathSigStau + "HSCPpairStau_M-247_merged.root",
pathSigStau + "HSCPpairStau_M-308_merged.root",
pathSigStau + "HSCPpairStau_M-432_merged.root",
pathSigStau + "HSCPpairStau_M-557_merged.root",
pathSigStau + "HSCPpairStau_M-651_merged.root",
pathSigStau + "HSCPpairStau_M-745_merged.root",
pathSigStau + "HSCPpairStau_M-871_merged.root",
pathSigStau + "HSCPpairStau_M-1029_merged.root",
pathSigStau + "HSCPpairStau_M-1218_merged.root",
pathSigStau + "HSCPpairStau_M-1409_merged.root",
pathSigStau + "HSCPpairStau_M-1599_merged.root",
pathSigStau + "HSCPgmsbStau_M-557_merged.root",
pathSigStau + "HSCPgmsbStau_M-871_merged.root",
pathSigGluino + "HSCPgluino_M-500_merged.root",
pathSigGluino + "HSCPgluino_M-800_merged.root",
pathSigGluino + "HSCPgluino_M-1000_merged.root",
pathSigGluino + "HSCPgluino_M-1400_merged.root",
pathSigGluino + "HSCPgluino_M-1600_merged.root",
pathSigGluino + "HSCPgluino_M-1800_merged.root",
pathSigGluino + "HSCPgluino_M-2000_merged.root",
pathSigGluino + "HSCPgluino_M-2200_merged.root",
pathSigGluino + "HSCPgluino_M-2400_merged.root",
pathSigGluino + "HSCPgluino_M-2600_merged.root",
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-200_merged.root",
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-400_merged.root",
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-500_merged.root",
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-1000_merged.root",
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-1400_merged.root",
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-1800_merged.root",
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-2200_merged.root",
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-2600_merged.root",

pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-100_merged.root",
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-200_merged.root",
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-400_merged.root",
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-500_merged.root",
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-800_merged.root",
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-1000_merged.root",
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-1400_merged.root",
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-1800_merged.root",
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-2200_merged.root",
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-2600_merged.root",
pathSigHSCPstop + "HSCPstop_M-500_merged.root",
pathSigHSCPstop + "HSCPstop_M-800_merged.root",
pathSigHSCPstop + "HSCPstop_M-1000_merged.root",
pathSigHSCPstop + "HSCPstop_M-1200_merged.root",
pathSigHSCPstop + "HSCPstop_M-1400_merged.root",
pathSigHSCPstop + "HSCPstop_M-1600_merged.root",
pathSigHSCPstop + "HSCPstop_M-1800_merged.root",
pathSigHSCPstop + "HSCPstop_M-2000_merged.root",
pathSigHSCPstop + "HSCPstop_M-2200_merged.root",
pathSigHSCPstop + "HSCPstop_M-2400_merged.root",
pathSigHSCPstop + "HSCPstop_M-2600_merged.root",
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-200_merged.root",
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-247_merged.root",
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-308_merged.root",
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-432_merged.root",
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-557_merged.root",
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-651_merged.root",
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-745_merged.root",
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-871_merged.root",
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-1029_merged.root",
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-1218_merged.root",
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-1409_merged.root",
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-1599_merged.root",
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-500_merged.root",
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-800_merged.root",
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-1000_merged.root",
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-1200_merged.root",
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-1400_merged.root",
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-1600_merged.root",
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-1800_merged.root",
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-2000_merged.root",
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-2200_merged.root",
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-2400_merged.root",
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-2600_merged.root",
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-500_merged.root",
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-800_merged.root",
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-1000_merged.root",
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-1200_merged.root",
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-1400_merged.root",
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-1600_merged.root",
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-1800_merged.root",
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-2000_merged.root",
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M_2200_merged.root",
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-2400_merged.root",
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-2600_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-7000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-7000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-7000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-7000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-7000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-7000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-7000_merged.root",
]
'''

BackgroundSamples = [
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-3000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-4000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-5000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-6000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-7000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-7000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-7000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-7000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-7000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-7000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-7000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-3000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-4000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-5000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-6000_merged.root",
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-7000_merged.root",
]
#intLumi = 4598.37 #10.0 #
intLumi = 101000.0 #101/fb
#intLumi = 54300.0 #100/fb
#intLumi = 30000.0

crossSectionArray = {
pathSigStau+ 'HSCPpairStau_M-200_merged.root' : 0.0303143567,
pathSigStau+ 'HSCPpairStau_M-247_merged.root' : 0.01354210931669562,
pathSigStau+ 'HSCPpairStau_M-308_merged.root' : 0.005617969677491303,
pathSigStau+ 'HSCPpairStau_M-432_merged.root' : 0.0013205993021946,
pathSigStau+ 'HSCPpairStau_M-557_merged.root' : 0.0003982919391389629,
pathSigStau+ 'HSCPpairStau_M-651_merged.root' : 0.00018170273864904894,
pathSigStau+ 'HSCPpairStau_M-745_merged.root' : 8.760649473515753e-5,
pathSigStau+ 'HSCPpairStau_M-871_merged.root' : 3.548748280460506e-5,
pathSigStau+ 'HSCPpairStau_M-1029_merged.root' : 1.1684554271190975e-5,
pathSigStau+ 'HSCPpairStau_M-1218_merged.root' : 0.00057/1000., #fake
pathSigStau+ 'HSCPpairStau_M-1409_merged.root' : 0.00057/1000., #fake
pathSigStau+ 'HSCPpairStau_M-1599_merged.root' : 0.00057/1000.  ,#fake


pathSigGluino + "HSCPgluino_M-500_merged.root" : 33.800,
pathSigGluino + "HSCPgluino_M-800_merged.root" : 1.810,
pathSigGluino + "HSCPgluino_M-1000_merged.root" : 0.385,
pathSigGluino + "HSCPgluino_M-1400_merged.root" : 0.0284,
pathSigGluino + "HSCPgluino_M-1600_merged.root" : 0.00887,
pathSigGluino + "HSCPgluino_M-1800_merged.root" : 0.00293,
pathSigGluino + "HSCPgluino_M-2000_merged.root" : 0.00101,
pathSigGluino + "HSCPgluino_M-2200_merged.root" : 0.000356,
pathSigGluino + "HSCPgluino_M-2400_merged.root" : 0.000128,
pathSigGluino + "HSCPgluino_M-2600_merged.root" : 4.62e-5,
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-200_merged.root" : 7.332e-01,
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-400_merged.root" : 5.440e-02,
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-500_merged.root" : 2.50e-02,
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-1000_merged.root" : 6.553e-04,
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-1400_merged.root" : 7.477e-05,
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-1800_merged.root" : 1.056e-05,
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-2200_merged.root" : 1.670e-06,
pathSigHSCPtauPrimeCharge2e + "HSCPtauPrimeCharge2e_M-2600_merged.root" : 2.913e-07,

pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-200_merged.root" : 1.833e-01,
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-400_merged.root" : 1.361e-02,
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-500_merged.root" : 6.0e-03,
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-800_merged.root" : 5.697e-04,
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-1000_merged.root" : 1.638e-04,
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-1400_merged.root" : 1.869e-05,
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-1800_merged.root" : 2.641e-06,
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-2200_merged.root" : 4.175e-07,
pathSigHSCPtauPrimeCharge1e + "HSCPtauPrimeCharge1e_M-2600_merged.root" : 7.282e-08,

pathSigHSCPstop + "HSCPstop_M-500_merged.root" : 0.000257,
pathSigHSCPstop + "HSCPstop_M-800_merged.root" : 0.0326,
pathSigHSCPstop + "HSCPstop_M-1000_merged.root" : 0.00683,
pathSigHSCPstop + "HSCPstop_M-1200_merged.root" : 0.0017,
pathSigHSCPstop + "HSCPstop_M-1400_merged.root" : 0.000473,
pathSigHSCPstop + "HSCPstop_M-1600_merged.root" : 0.000142,
pathSigHSCPstop + "HSCPstop_M-1800_merged.root" : 4.51e-05,
pathSigHSCPstop + "HSCPstop_M-2000_merged.root" : 1.48e-05,
pathSigHSCPstop + "HSCPstop_M-2200_merged.root" : 5e-06,
pathSigHSCPstop + "HSCPstop_M-2400_merged.root" : 1.71e-06,
pathSigHSCPstop + "HSCPstop_M-2600_merged.root" : 5.9e-07,
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-200_merged.root" : 2.8E-01,
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-247_merged.root" : 8.8E-02,
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-308_merged.root" : 2.5E-02,
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-432_merged.root" : 3.9E-03,
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-557_merged.root" : 1.9E-03,
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-651_merged.root" : 4.1E-04,
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-745_merged.root" : 1.9E-04,
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-871_merged.root" : 6.9E-05,
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-1029_merged.root" : 2.2E-05,
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-1218_merged.root" : 6.4E-06,
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-1409_merged.root" : 2.0E-06,
pathSigHSCPgmsbStau + "HSCPgmsbStau_M-1599_merged.root" : 5.3E-07,
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-500_merged.root" : 0.000257,
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-800_merged.root" : 0.0326,
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-1000_merged.root" : 0.00683,
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-1200_merged.root" : 0.0017,
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-1400_merged.root" : 0.000473,
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-1600_merged.root" : 0.000142,
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-1800_merged.root" : 4.51e-05,
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-2000_merged.root" : 1.48e-05,
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-2200_merged.root" : 5e-06,
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-2400_merged.root" : 1.71e-06,
pathSigHSCPstopOnlyNeutral + "HSCPstopOnlyNeutral_M-2600_merged.root" : 5.9e-07,
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-500_merged.root" : 33.800,
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-800_merged.root" : 1.810,
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-1000_merged.root" : 0.385,
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-1200_merged.root" : 0.0985,
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-1400_merged.root" : 0.0284,
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-1600_merged.root" : 0.00887,
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-1800_merged.root" : 0.00293,
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-2000_merged.root" : 0.00101,
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M_2200_merged.root" : 0.000356,
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-2400_merged.root" : 0.000128,
pathSigHSCPgluinoOnlyNeutral + "HSCPgluinoOnlyNeutral_M-2600_merged.root" : 4.62e-5,

pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-3000_merged.root" : 0.000451036,
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-4000_merged.root" : 4.7749E-5,
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-5000_merged.root" : 5.34376E-6,
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-6000_merged.root" : 5.45292E-7,
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrime_M-7000_merged.root" : 5.2E-8,

pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-3000_merged.root" : 0.000451036,
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-4000_merged.root" : 4.7749E-5,
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-5000_merged.root" : 5.34376E-6,
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-6000_merged.root" : 5.45292E-7,
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrime_M-7000_merged.root" : 5.2E-8,

pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-3000_merged.root" : 0.000451036,
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-4000_merged.root" : 4.7749E-5,
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-5000_merged.root" : 5.34376E-6,
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-6000_merged.root" : 5.45292E-7,
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrime_M-7000_merged.root" : 5.2E-8,

pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-3000_merged.root" : 0.000451036,
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-4000_merged.root" : 4.7749E-5,
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-5000_merged.root" : 5.34376E-6,
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-6000_merged.root" : 5.45292E-7,
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrime_M-7000_merged.root" : 5.2E-8,

pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-3000_merged.root" : 0.000451036,
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-4000_merged.root" : 4.7749E-5,
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-5000_merged.root" : 5.34376E-6,
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-6000_merged.root" : 5.45292E-7,
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrime_M-7000_merged.root" : 5.2E-8,

pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-3000_merged.root" : 0.000451036,
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-4000_merged.root" : 4.7749E-5,
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-5000_merged.root" : 5.34376E-6,
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-6000_merged.root" : 5.45292E-7,
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrime_M-7000_merged.root" : 5.2E-8,

pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-3000_merged.root" : 0.000451036,
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-4000_merged.root" : 4.7749E-5,
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-5000_merged.root" : 5.34376E-6,
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-6000_merged.root" : 5.45292E-7,
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrime_M-7000_merged.root" : 5.2E-8,


pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-3000_merged.root" : 0.00147992,
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-4000_merged.root" : 0.000159354,
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-5000_merged.root" : 1.76729E-5,
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-6000_merged.root" : 1.80017E-6,
pathSigHSCPZPrime_tauM200 + "HSCPtauPrimeCharge2e_M-200_ZPrimeSSM_M-7000_merged.root" : 1.9E-7,

pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-3000_merged.root" : 0.00147992,
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-4000_merged.root" : 0.000159354,
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-5000_merged.root" : 1.76729E-5,
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-6000_merged.root" : 1.80017E-6,
pathSigHSCPZPrime_tauM400 + "HSCPtauPrimeCharge2e_M-400_ZPrimeSSM_M-7000_merged.root" : 1.9E-7,

pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-3000_merged.root" : 0.00147992,
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-4000_merged.root" : 0.000159354,
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-5000_merged.root" : 1.76729E-5,
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-6000_merged.root" : 1.80017E-6,
pathSigHSCPZPrime_tauM600 + "HSCPtauPrimeCharge2e_M-600_ZPrimeSSM_M-7000_merged.root" : 1.9E-7,

pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-3000_merged.root" : 0.00147992,
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-4000_merged.root" : 0.000159354,
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-5000_merged.root" : 1.76729E-5,
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-6000_merged.root" : 1.80017E-6,
pathSigHSCPZPrime_tauM800 + "HSCPtauPrimeCharge2e_M-800_ZPrimeSSM_M-7000_merged.root" : 1.9E-7,

pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-3000_merged.root" : 0.00147992,
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-4000_merged.root" : 0.000159354,
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-5000_merged.root" : 1.76729E-5,
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-6000_merged.root" : 1.80017E-6,
pathSigHSCPZPrime_tauM1000 + "HSCPtauPrimeCharge2e_M-1000_ZPrimeSSM_M-7000_merged.root" : 1.9E-7,

pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-3000_merged.root" : 0.00147992,
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-4000_merged.root" : 0.000159354,
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-5000_merged.root" : 1.76729E-5,
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-6000_merged.root" : 1.80017E-6,
pathSigHSCPZPrime_tauM1200 + "HSCPtauPrimeCharge2e_M-1200_ZPrimeSSM_M-7000_merged.root" : 1.9E-7,

pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-3000_merged.root" : 0.00147992,
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-4000_merged.root" : 0.000159354,
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-5000_merged.root" : 1.76729E-5,
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-6000_merged.root" : 1.80017E-6,
pathSigHSCPZPrime_tauM1400 + "HSCPtauPrimeCharge2e_M-1400_ZPrimeSSM_M-7000_merged.root" : 1.9E-7,

}

## TTBar is 832 pb (NNLO)
# gluino x-sections from
# https://github.com/fuenfundachtzig/xsec/blob/master/json/pp13_gluino_NNLO%2BNNLL.json#L1820

fileInArray = []
for sample in BackgroundSamples:
  print(sample)
  if not os.path.exists(sample): continue
  fileInArray.append(ROOT.TFile.Open(sample,"UPDATE"))

print("Files in array : \n",fileInArray)


with open("stau_weights.txt", "w") as file:
    for fileIn in fileInArray:
      if not (fileIn.Get("HSCParticleAnalyzer/BaseName/NumEvents")):
        print("NumEvents not found, exit for "+str(fileIn))
        continue
      print("Running on {}\n".format(fileIn)) 
      nEvetsPreTrig = fileIn.Get("HSCParticleAnalyzer/BaseName/NumEvents").GetBinContent(1)
      print("Number of events pre trigger = {}\n".format(nEvetsPreTrig))
      nEvetsPostTrig = fileIn.Get("HSCParticleAnalyzer/BaseName/NumEvents").GetBinContent(2)
      if (nEvetsPreTrig == 0):
        print("nEvetsPreTrig is zero, exiting")
        continue
    
      print(str(fileIn)) 
      start_index = str(fileIn).find('"') + 1
      print("start index = ", start_index)
      end_index = str(fileIn).find('.root')
      print("end index = ", end_index)
      nameFromTFile = str(fileIn)[start_index:end_index] + ".root"
      #nameFromTFile = str(fileIn)[str(fileIn).find("object")+3:str(fileIn).find("at")-3]
        
      #nameFromTFile = str(fileIn)[str(fileIn).find("Name")+6:str(fileIn).find("Title")-1]
      print nameFromTFile
      if not (crossSectionArray.get(nameFromTFile)) :
        print("No crossSectionArray for "+str(nameFromTFile))
        continue
      weight = intLumi*crossSectionArray.get(nameFromTFile)/nEvetsPreTrig
      start_idxWeights = str(fileIn).find('/stau/')
      end_idxWeights = str(fileIn).find('.root')
      name_idxWeights = str(fileIn)[start_idxWeights+6:end_idxWeights]
      print("Writing weight {} to txt file {}".format(weight,name_idxWeights))
      print("Weight for ")
      line = name_idxWeights + ' : ' + str(weight) + '\n'
      file.write(line)
 
      for i in range(0, fileIn.GetListOfKeys().GetEntries()):
        dirname = fileIn.GetListOfKeys().At(i).GetName()
        curr_dir = fileIn.GetDirectory(dirname)
    #    print("dirname: "+dirname)
        if not (curr_dir) :
          continue
        for i in range(0, curr_dir.GetListOfKeys().GetEntries()):
            keyname = curr_dir.GetListOfKeys().At(i).GetName()
            curr_dir2 = fileIn.GetDirectory(dirname+"/"+keyname)
            if not (curr_dir2) :
              continue
            for j in tqdm(range(0, curr_dir2.GetListOfKeys().GetEntries())):
              keyname2 = curr_dir2.GetListOfKeys().At(j).GetName()
    #          print("keyname2: "+keyname2)
              newname = dirname + "/" + keyname+ "/" + keyname2
              #print(newname)
              obj = fileIn.Get(newname)
              if not (obj) : continue
              if (obj.GetEntries() == 0 ) :
    #            print("obj.GetEntries() == 0")
                continue
              if (keyname2=="HscpCandidates" or keyname2=="GenHscpCandidates"):
                continue
              if obj.InheritsFrom("TObject"):
                obj.Scale(weight)
      fileIn.Write("",ROOT.TObject.kOverwrite)
      fileIn.Close()
    
    '''
    print("hadd crab_Analysis_2018_AllBackground_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-50To80_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-80To120_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-120To170_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-170To300_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-300To470_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-470To600_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-600To800_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-800To1000_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-1000_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_WJetsToLNu_*J_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_TTToHadronic_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_TTToSemiLeptonic_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_TTTo2L2Nu_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_ZToMuMu_M*_CodeV"+codeVersion+"_v1.root")
    
    #os.system
    print("hadd crab_Analysis_2018_AllQCD_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-50To80_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-80To120_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-120To170_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-170To300_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-300To470_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-470To600_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-600To800_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-800To1000_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_QCD_Pt-1000_MuEnrichedPt5_CodeV"+codeVersion+"_v1.root")
    
    print("hadd crab_Analysis_2018_AllTTbar_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_TTToHadronic_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_TTToSemiLeptonic_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_TTTo2L2Nu_CodeV"+codeVersion+"_v1.root")
    
    print("hadd crab_Analysis_2018_AllWJets_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_WJetsToLNu*J_CodeV"+codeVersion+"_v1.root ")
    
    print("hadd crab_Analysis_2018_AllZToMuMU_CodeV"+codeVersion+"_v1.root crab_Analysis_2018_ZToMuMu_M*_CodeV"+codeVersion+"_v1.root ")
    
    print("hadd crab_Analysis_SingleMuon_RunPhase1_CodeV"+codeVersion+"_v1.root *SingleMuon*_CodeV"+codeVersion+"_v1.root")
    '''
    
    #if not os.path.exists("CodeV"+codeVersion): os.mkdir("CodeV"+codeVersion)
    #
    #os.system("mv *"+codeVersion+"*root "+ "CodeV"+codeVersion+"/.")
    #os.system("cp CodeV"+codeVersion+"/crab_Analysis_2018_SingleMuon_Run2018C_CodeV*"+codeVersion+"_v1.root .")
    #os.system("cp CodeV"+codeVersion+"/crab_Analysis_2018_HSCPgluino_M-*_CodeV*"+codeVersion+"_v1.root .")
    #os.system("mv CodeV"+codeVersion+"/crab_Analysis_2018_All*"+codeVersion+"_v1.root .")
    
