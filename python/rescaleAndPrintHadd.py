import ROOT, sys, os, time, re, numpy
from optparse import OptionParser
from tqdm import tqdm

#just the number, like 18p2

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetPadRightMargin(.15)
ROOT.gStyle.SetPadTopMargin(0.1);
ROOT.gStyle.SetPadBottomMargin(0.14);
ROOT.gStyle.SetPadLeftMargin(0.15);

#pathSig = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/TriggerStudies/Seeds_infos/HSCP_glu_stau/stau/"
pathSig = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/prodAugust2023_CMSSW_10_6_27/23_august/"

print("Recaling signal histograms from production within {}".format(pathSig[-39:]))

BackgroundSamples = [
pathSig + "HSCPpairStau_M-200_merged.root",
pathSig + "HSCPpairStau_M-247_merged.root",
pathSig + "HSCPpairStau_M-308_merged.root",
pathSig + "HSCPpairStau_M-432_merged.root",
pathSig + "HSCPpairStau_M-557_merged.root",
pathSig + "HSCPpairStau_M-651_merged.root",
pathSig + "HSCPpairStau_M-745_merged.root",
pathSig + "HSCPpairStau_M-871_merged.root",
pathSig + "HSCPpairStau_M-1029_merged.root",
pathSig + "HSCPpairStau_M-1218_merged.root",
pathSig + "HSCPpairStau_M-1409_merged.root",
pathSig + "HSCPpairStau_M-1599_merged.root",
pathSig + "HSCPgmsbStau_M-557_merged.root",
pathSig + "HSCPgmsbStau_M-871_merged.root",
pathSig + "HSCPtauPrimeCharge1e_M-800_merged.root",
pathSig + "HSCPtauPrimeCharge1e_M-1800_merged.root",
pathSig + "HSCPtauPrimeCharge2e_M-500_merged.root",
pathSig + "HSCPtauPrimeCharge2e_M-1800_merged.root",

]
#intLumi = 4598.37 #10.0 #
intLumi = 101000.0 #101/fb
#intLumi = 54300.0 #100/fb
#intLumi = 30000.0

crossSectionArray = {
pathSig+ 'HSCPpairStau_M-200_merged.root' : 0.0303143567,
pathSig+ 'HSCPpairStau_M-247_merged.root' : 0.01354210931669562,
pathSig+ 'HSCPpairStau_M-308_merged.root' : 0.005617969677491303,
pathSig+ 'HSCPpairStau_M-432_merged.root' : 0.0013205993021946,
pathSig+ 'HSCPpairStau_M-557_merged.root' : 0.0003982919391389629,
pathSig+ 'HSCPpairStau_M-651_merged.root' : 0.00018170273864904894,
pathSig+ 'HSCPpairStau_M-745_merged.root' : 8.760649473515753e-5,
pathSig+ 'HSCPpairStau_M-871_merged.root' : 3.548748280460506e-5,
pathSig+ 'HSCPpairStau_M-1029_merged.root' : 1.1684554271190975e-5,
pathSig+ 'HSCPpairStau_M-1218_merged.root' : 0.00057/1000., #fake
pathSig+ 'HSCPpairStau_M-1409_merged.root' : 0.00057/1000., #fake
pathSig+ 'HSCPpairStau_M-1599_merged.root' : 0.00057/1000.  ,#fake
pathSig+ 'HSCPgmsbStau_M-557_merged.root' : 1.9E-03,
pathSig+ 'HSCPgmsbStau_M-871_merged.root' : 6.9E-05,
pathSig+ 'HSCPtauPrimeCharge1e_M-800_merged.root' : 5.697e-04,
pathSig+ 'HSCPtauPrimeCharge1e_M-1800_merged.root' : 2.641e-06,
pathSig+ 'HSCPtauPrimeCharge2e_M-500_merged.root' : 2.280e-03,#fake
pathSig+ 'HSCPtauPrimeCharge2e_M-1800_merged.root' : 1.056e-05,

}

## TTBar is 832 pb (NNLO)
# gluino x-sections from
# https://github.com/fuenfundachtzig/xsec/blob/master/json/pp13_gluino_NNLO%2BNNLL.json#L1820

fileInArray = []
for sample in BackgroundSamples:
  if not os.path.exists(sample): continue
  fileInArray.append(ROOT.TFile.Open(sample,"UPDATE"))

with open("stau_weights.txt", "w") as file:
    for fileIn in fileInArray:
      if not (fileIn.Get("HSCParticleAnalyzer/BaseName/NumEvents")):
        print("NumEvents not found, exit for "+str(fileIn))
        continue
      
      nEvetsPreTrig = fileIn.Get("HSCParticleAnalyzer/BaseName/NumEvents").GetBinContent(1)
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
    
