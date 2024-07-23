'''
    Description:
        - Plot histograms for comparison
          given two ROOT files
    Requirements:
        - json file "histos.json"
        - ROOT
    ________________________

    json file needs : name of histograms, and full path within the root files

    <raphael.julien.haberle@cern.ch>
'''
import os, sys, argparse, json, copy, array, numpy, struct
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
parser = argparse.ArgumentParser(prog='python '+sys.argv[0])
parser.add_argument('ref', metavar='ref-rootfile', help='Ref ROOT file with full path')
parser.add_argument('output', metavar='output-dir', help='output directory')
parser.add_argument('display', metavar='display-option', help='display option : itg to see integral values')
parser.add_argument('maxBorn', metavar='max-born-hists', help='display option : set the max value for histograms, pulls, ratios etc...')
parser.add_argument('ptcut', metavar='pt-cut-regions', help='display option : set the value for pt cut when defining signal/control regions (Fpix cuts are fixed)')


args = parser.parse_args()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
import ROOT
import math
import CMS_lumi, tdrstyle
from ROOT import gROOT, gStyle, TFile, TH1F, TLegend, TCanvas, TPad, TLine, TH1D, TH2F, TPaveText, gPad, gDirectory,TFitResultPtr

from binningVar import *
from SignalXS import *

from makeDataCards import *
from datetime import datetime

from FpixConstants import regions_extension as extension, extDataCards
from ValidationFunctionsData  import *


if __name__ == "__main__":

    gROOT.SetBatch()
    gStyle.SetOptStat(0)


    ref = args.ref           # PedestalEqualizationReference.root'
    display = args.display
    maxDisplay = int(args.maxBorn)
    pTcut = int(args.ptcut)
    outdir = args.output

    try:
        os.makedirs(outdir)
    except OSError:
        if not os.path.isdir(outdir):
            raise

    fref = TFile(ref, "READ")

    year = '2018'
    if '2018' in ref:
        year = '2018'
    if '2017' in ref:
        year = '2017'
    if 'merge' in ref:
        year = '2017and2018'
    
    now = datetime.now()
    day = now.strftime("%d")
    month = now.strftime("%m")
    date = str(day) + "_" + str(month)

    prefixA = "mass_regionA_"
    prefixB = "mass_regionB_"
    prefixC = "mass_regionC_"
    prefixD = "mass_regionD_"

    nameSR0 = 'mass_regionD_8fp10' + extension
    nameSR1 = 'mass_regionD_9fp10' + extension
    nameSR2 = 'mass_regionD_99fp10' + extension
    nameSR3 = 'mass_regionD_999fp10' + extension
 

    nameRootSR = [year+"_SR0_predMass.root",year+"_SR1_predMass.root",year+"_SR2_predMass.root",year+"_SR3_predMass.root"]
    namePredSR = [year+"_SR0_predMass",year+"_SR1_predMass",year+"_SR2_predMass",year+"_SR3_predMass"]
    namePredRaw = [year+"_SR0_predMass_raw",year+"_SR1_predMass_raw",year+"_SR2_predMass_raw",year+"_SR3_predMass_raw"]


    scalePredMass = [ROOT.TFile.Open(outdir + "/" + name, "RECREATE") for name in nameRootSR]
    CRMass = ROOT.TFile.Open(outdir + "/CR_Mass_"+year+".root", "RECREATE")
    namesSR= ["SR0","SR1","SR2","SR3"]

  
    histoNamesA_cr = ['mass_regionA_3fp8'+extension]
    histoNamesC_cr = ['mass_regionC_3fp8'+extension]

    histoNamesD_vr = ['mass_regionC_8fp9'+extension]
    histoNamesB_vr = ['mass_regionB_8fp9'+extension]


    histoNamesD_sr0 = ['mass_regionD_8fp10'+extension]
    histoNamesB_sr0 = ['mass_regionB_8fp10'+extension]

    histoNamesD_sr1 = ['mass_regionD_9fp10'+extension]
    histoNamesB_sr1 = ['mass_regionB_9fp10'+extension]

    histoNamesD_sr2 = ['mass_regionD_99fp10'+extension]
    histoNamesB_sr2 = ['mass_regionB_99fp10'+extension]

    histoNamesD_sr3 = ['mass_regionD_999fp10'+extension]
    histoNamesB_sr3 = ['mass_regionB_999fp10'+extension]

    regA3fp8 = getHistogram(fref, histoNamesA_cr[0], 'regA3fp8')
    regC3fp8 = getHistogram(fref, histoNamesC_cr[0], 'regC3fp8')

    regC3fp8_scaleVR = regC3fp8.Clone("toCheck_VR_8f9")

    regC3f8_SRs = [regC3fp8.Clone("CR_scaled_SR0"),regC3fp8.Clone("CR_scaled_SR1"),regC3fp8.Clone("CR_scaled_SR2"),regC3fp8.Clone("CR_scaled_SR3")]

    regC3f8_RAW = [regC3fp8.Clone("CR_raw_SR0"),regC3fp8.Clone("CR_raw_SR1"),regC3fp8.Clone("CR_raw_SR2"),regC3fp8.Clone("CR_raw_SR3")]

    regB8fp9 = getHistogram(fref, histoNamesB_vr[0], 'regB8fp9')
    regD8fp9 = getHistogram(fref, histoNamesD_vr[0], 'regD8fp9')

 
    regB_SRs = [getHistogram(fref, histoNamesB_sr0[0], 'regB8fp10'),getHistogram(fref, histoNamesB_sr1[0], 'regB9fp10'),getHistogram(fref, histoNamesB_sr2[0], 'regB99fp10'),getHistogram(fref, histoNamesB_sr3[0], 'regB999fp10')]
    regD_SRs = [getHistogram(fref, histoNamesD_sr0[0], 'regD8fp10'),getHistogram(fref, histoNamesD_sr1[0], 'regD9fp10'),getHistogram(fref, histoNamesD_sr2[0], 'regD99fp10'),getHistogram(fref, histoNamesD_sr3[0], 'regD999fp10')]


    predVR = regB8fp9.GetEntries()*regC3fp8.GetEntries()/regA3fp8.GetEntries()
    sfVR = regC3fp8.GetEntries()/predVR
    print("for year {} :".format(year))
    print("Scale factor from ABCD method between control region C 0.3 - 0.8 and validation region D 0.8 - 0.9 ={}".format(sfVR))
    scale_histogram_with_poissonian_errors(regC3fp8_scaleVR, 1./sfVR)

    predD_sr0 = regB_SRs[0].GetEntries()*regC3fp8.GetEntries()/regA3fp8.GetEntries()
    ScaleFacSR0 = regC3fp8.GetEntries() / predD_sr0

    predD_sr1 = regB_SRs[1].GetEntries()*regC3fp8.GetEntries()/regA3fp8.GetEntries()
    ScaleFacSR1 = regC3fp8.GetEntries() / predD_sr1

    predD_sr2 = regB_SRs[2].GetEntries()*regC3fp8.GetEntries()/regA3fp8.GetEntries()
    ScaleFacSR2 = regC3fp8.GetEntries() / predD_sr2

    predD_sr3 = regB_SRs[3].GetEntries()*regC3fp8.GetEntries()/regA3fp8.GetEntries()
    ScaleFacSR3 = regC3fp8.GetEntries() / predD_sr3

    sf_SRs = [ScaleFacSR0,ScaleFacSR1,ScaleFacSR2,ScaleFacSR3]

    nameSFtxt =outdir + '/' + year + "_SF_CR_SRs_stau.txt"
    with open(nameSFtxt, "w") as file:
        for p in range(len(namesSR)):
            line = namesSR[p] + '_' + year + '_ScaleFactor_CR_SR_stau : ' + str( (1./sf_SRs[p]) ) + '\n' 
            file.write(line)

    for i in range(len(namesSR)):
        print("Scale factor from ABCD method between regions C 0.3 - 0.8 and {} ={}".format(namesSR[i],sf_SRs[i]))

    #scale_histogram_with_poissonian_errors(regC3fp8, 1./ScaleFacSR1)

    
    scale_histogram_with_poissonian_errors(regC3f8_SRs[0], 1./ScaleFacSR0)
    scale_histogram_with_poissonian_errors(regC3f8_SRs[1], 1./ScaleFacSR1)
    scale_histogram_with_poissonian_errors(regC3f8_SRs[2], 1./ScaleFacSR2)
    scale_histogram_with_poissonian_errors(regC3f8_SRs[3], 1./ScaleFacSR3)
    
    crlow = "3"
    crup = "8"
    vrlow = "8"
    vrup = "9"
  
    lejDataCR = "CR F^{pix}" + "[0."+crlow+" - 0."+ crup + "]"
    lejDataVR = "VR F^{pix}" + "[0."+vrlow+" - 0."+ vrup + "]"

    lejDataSR0 = "SR F^{pix}" + "[0.8 - 1]"
    lejDataSR1 = "SR F^{pix}" + "[0.9 - 1]"
    lejDataSR2 = "SR F^{pix}" + "[0.99 - 1]"
    lejDataSR3 = "SR F^{pix}" + "[0.999 - 1]"

    
    regA3fp8 = regA3fp8.Rebin(sizeRebinning,'regA3fp8',rebinning)
    #regC3fp8 = regC3fp8.Rebin(sizeRebinning,lejDataCR,rebinning)


    regB8fp9 = regB8fp9.Rebin(sizeRebinning,'regB8fp9',rebinning)
    regD8fp9 = regD8fp9.Rebin(sizeRebinning,lejDataVR,rebinning)


    regB_SRs[0] = regB_SRs[0].Rebin(sizeRebinning,'regB8fp10',rebinning)
    regD_SRs[0] = regD_SRs[0].Rebin(sizeRebinning,lejDataSR0,rebinning)

    regB_SRs[1] = regB_SRs[1].Rebin(sizeRebinning,'regB9fp10',rebinning)
    regD_SRs[1] = regD_SRs[1].Rebin(sizeRebinning,lejDataSR1,rebinning)

    regB_SRs[2] = regB_SRs[2].Rebin(sizeRebinning,'regB99fp10',rebinning)
    regD_SRs[2] = regD_SRs[2].Rebin(sizeRebinning,lejDataSR2,rebinning)
 
    regB_SRs[3] = regB_SRs[3].Rebin(sizeRebinning,'regB999fp10',rebinning)
    regD_SRs[3] = regD_SRs[3].Rebin(sizeRebinning,lejDataSR3,rebinning)


    for k in range(len(regC3f8_SRs)):
        scalePredMass[k].cd()
        print("SR{} has mass prediction number of events = {}".format(k,regC3f8_SRs[k].Integral()))
        regC3f8_SRs[k].Write(str(namePredSR[k]))
        regC3f8_RAW[k].Write(str(namePredRaw[k])) 
        scalePredMass[k].Close()

    CRMass.cd()
    regC3fp8.GetXaxis().SetRangeUser(0,1400)
    regC3fp8.GetYaxis().SetRangeUser(0.001,1000000)
    regC3fp8.Write(str(year+"_CR_Mass_"+year))
    CRMass.Close()
