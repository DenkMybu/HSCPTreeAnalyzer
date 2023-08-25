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

    namesValidation = ["3fp4","4fp5","5fp6","6fp7","7fp8","8fp9"]
     
    regionsValidation = ["3fp4"+extension,"4fp5"+extension,"5fp6"+extension,"6fp7"+extension,"7fp8"+extension,"8fp9"+extension]

    
    namesA = [(prefixA+regionsValidation[i]) for i in range(len(regionsValidation))]
    namesB = [(prefixB+regionsValidation[i]) for i in range(len(regionsValidation))]
    namesC = [(prefixC+regionsValidation[i]) for i in range(len(regionsValidation))]
    namesD = [(prefixC+regionsValidation[i]) for i in range(len(regionsValidation))]
    
    namesDtemp = [(prefixD+regionsValidation[i]) for i in range(len(regionsValidation))]
    
    regionsA = [getHistogram(fref, namesA[i], '') for i in range(len(regionsValidation))]
    regionsB = [getHistogram(fref, namesB[i], '') for i in range(len(regionsValidation))]
    regionsC = [getHistogram(fref, namesC[i], '') for i in range(len(regionsValidation))]
    regionsD = [getHistogram(fref, namesD[i], '') for i in range(len(regionsValidation))]


    signalsFiles = [ROOT.TFile(path, "READ") for path in sampleArray]
    nameSR1 = 'mass_regionD_9fp10' + extension
    nameSR2 = 'mass_regionD_99fp10' + extension
    nameSR3 = 'mass_regionD_999fp10' + extension
 

    nameRootSR = [year+"_SR1_predMass.root",year+"_SR2_predMass.root",year+"_SR3_predMass.root"]
    namePredSR = [year+"_SR1_predMass",year+"_SR2_predMass",year+"_SR3_predMass"]

    scalePredMass = [ROOT.TFile.Open(outdir + "/" + name, "RECREATE") for name in nameRootSR]


    weightsSIG = RescaleSignals(len(signalsFiles),year,"search region",nameSR1)

    sigArray = [[] for _ in range(len(signalsFiles))]

    for fileIndex, rootFile in enumerate(signalsFiles):
        name = sampleNames[fileIndex]
        weight = weightsSIG[fileIndex]
        data = {"name": name, "histograms": []}

        for histName in namesC:
            hist = rootFile.Get(histName)
            if hist:
                scale_histogram_with_poissonian_errors(hist, weight)
                #hist.Scale(weight)
                #hist = cleanSingleHist(hist)
                data["histograms"].append(hist)
            else:
                print("Warning: Histogram {} not found in root file {}".format(histName,name))
	 
        sigArray[fileIndex] = data
    
    arraySlices = ["0.3","0.4","0.5","0.6","0.7","0.8","0.9"]
    lejSlices = []
    
    for i in range(len(arraySlices)-1):
        lejSlices.append("Data VR F^{pix} ["+arraySlices[i] + " - " + arraySlices[i+1] + "]")
    

    for i in range (len(regionsA)):
        #regionsA[i] = regionsA[i].Rebin(sizeRebinning,namesA[i],rebinning)
        regionsA[i] = regionsA[i].Rebin(sizeRebinning,lejSlices[i],rebinning)
    for i in range (len(regionsB)):
        regionsB[i] = regionsB[i].Rebin(sizeRebinning,lejSlices[i],rebinning)
    for i in range (len(regionsC)):
        regionsC[i] = regionsC[i].Rebin(sizeRebinning,lejSlices[i],rebinning)
    for i in range (len(regionsD)):
        regionsD[i] = regionsD[i].Rebin(sizeRebinning,lejSlices[i],rebinning)

    nameAllReg = year+"_allCR_sameCanvas_"
    DisplayAllRegions(regionsC,nameAllReg,maxDisplay,outdir)

    scaleFactors = []
    nameSF = []

    for i in range(1,len(regionsValidation)):
       print("Calculation predicted #events in region {} , = events in {} * events in {} / events in {}".format(namesD[i],namesB[i],namesC[i-1],namesA[i-1]))
       pred = regionsB[i].GetEntries()*regionsC[i-1].GetEntries()/regionsA[i-1].GetEntries()
       sf = regionsC[i-1].GetEntries()/pred
       print("Scale factor = {}".format(sf))
       nameSF.append(year+"_SF_"+namesD[i]+"_vs_"+namesC[i-1])
       scaleFactors.append(sf)

    

    for i in range(len(scaleFactors)):
        scale_histogram_with_poissonian_errors(regionsC[i], 1./scaleFactors[i])
        print("Scaled region {} by {}, and check the differences with region {}".format(namesC[i],1./scaleFactors[i],namesD[i+1]))
        name = "validation_region_"+namesValidation[i+1]

        givenRegAllSig = []
        for data in sigArray:
            histograms = data["histograms"]
            givenRegAllSig.append(histograms[i])

        CompareRegions(regionsC[i],regionsD[i+1],givenRegAllSig,nameSF[i],maxDisplay,outdir,False,True)

    
    #Opening every root file contained in SignalXS.py
    for idx,name in enumerate(sampleArray):
        print("File {} has name {}".format(idx,name)) 

    selSigSR1 = [getHistogram(file, nameSR1, '') for file in signalsFiles]
    selSigSR2 = [getHistogram(file, nameSR2, '') for file in signalsFiles]
    selSigSR3 = [getHistogram(file, nameSR3, '') for file in signalsFiles]


  
    for i in range(len(selSigSR1)):
        weight = weightsSIG[i]
        print("Scaling signal {} by {}".format(sampleNames[i],weight))
        #histogram.Scale(weight)
        scale_histogram_with_poissonian_errors(selSigSR1[i], weight)
        scale_histogram_with_poissonian_errors(selSigSR2[i], weight)
        scale_histogram_with_poissonian_errors(selSigSR3[i], weight)
         
    allScaledSelSR = [selSigSR1,selSigSR2,selSigSR3] 
    namesSR= ["SR1","SR2","SR3"]

    print("#" * 40) 
    print("# Fpix region :", nameSR1)
    print("#" * 40)
    for u in range(len(allScaledSelSR)):
        print("Looking within {} \n".format(namesSR[u]))
        for i in range(len(sampleNames)):
            print("Signal: {}, Entries: {}, Mean: {}, std dev : {}".format(sampleNames[i],allScaledSelSR[u][i].GetEntries(),allScaledSelSR[u][i].GetMean(),allScaledSelSR[u][i].GetStdDev()))


    meanSignals = [ [] for _ in range(3)]
    stddevSignals = [ [] for _ in range(3)]
    for u in range(3):
        meanSignals[u] = [allScaledSelSR[u][i].GetMean() for i in range(len(allScaledSelSR[u]))]
        stddevSignals[u] = [allScaledSelSR[u][i].GetStdDev() for i in range(len(allScaledSelSR[u]))]
  

  
    histoNamesA_cr = ['mass_regionA_3fp8'+extension]
    histoNamesC_cr = ['mass_regionC_3fp8'+extension]

    histoNamesD_vr = ['mass_regionC_8fp9'+extension]
    histoNamesB_vr = ['mass_regionB_8fp9'+extension]


    histoNamesD_sr = ['mass_regionD_9fp10'+extension]
    histoNamesB_sr = ['mass_regionB_9fp10'+extension]

    histoNamesD_sr2 = ['mass_regionD_99fp10'+extension]
    histoNamesB_sr2 = ['mass_regionB_99fp10'+extension]

    histoNamesD_sr3 = ['mass_regionD_999fp10'+extension]
    histoNamesB_sr3 = ['mass_regionB_999fp10'+extension]

    regA3fp8 = getHistogram(fref, histoNamesA_cr[0], 'regA3fp8')
    regC3fp8 = getHistogram(fref, histoNamesC_cr[0], 'regC3fp8')

    regC3fp8_scaleVR = regC3fp8.Clone("toCheck_VR_8f9")

    regC3f8_SRs = [regC3fp8.Clone("CR_scaled_SR1"),regC3fp8.Clone("CR_scaled_SR2"),regC3fp8.Clone("CR_scaled_SR3")]


    regB8fp9 = getHistogram(fref, histoNamesB_vr[0], 'regB8fp9')
    regD8fp9 = getHistogram(fref, histoNamesD_vr[0], 'regD8fp9')

 
    regB_SRs = [getHistogram(fref, histoNamesB_sr[0], 'regB9fp10'),getHistogram(fref, histoNamesB_sr2[0], 'regB99fp10'),getHistogram(fref, histoNamesB_sr3[0], 'regB999fp10')]
    regD_SRs = [getHistogram(fref, histoNamesD_sr[0], 'regD9fp10'),getHistogram(fref, histoNamesD_sr2[0], 'regD99fp10'),getHistogram(fref, histoNamesD_sr3[0], 'regD999fp10')]




    predVR = regB8fp9.GetEntries()*regC3fp8.GetEntries()/regA3fp8.GetEntries()
    sfVR = regC3fp8.GetEntries()/predVR
    print("Scale factor from ABCD method between control region C 0.3 - 0.8 and validation region D 0.8 - 0.9 ={}".format(sfVR))
    scale_histogram_with_poissonian_errors(regC3fp8_scaleVR, 1./sfVR)

    predD_sr1 = regB_SRs[0].GetEntries()*regC3fp8.GetEntries()/regA3fp8.GetEntries()
    ScaleFac = regC3fp8.GetEntries() / predD_sr1

    predD_sr2 = regB_SRs[1].GetEntries()*regC3fp8.GetEntries()/regA3fp8.GetEntries()
    ScaleFacSR2 = regC3fp8.GetEntries() / predD_sr2

    predD_sr3 = regB_SRs[2].GetEntries()*regC3fp8.GetEntries()/regA3fp8.GetEntries()
    ScaleFacSR3 = regC3fp8.GetEntries() / predD_sr3

    sf_SRs = [ScaleFac,ScaleFacSR3,ScaleFacSR3]

    nameSFtxt =outdir + '/' + year + "_SF_CR_SRs_stau.txt"
    with open(nameSFtxt, "w") as file:
        for p in range(len(namesSR)):
            line = namesSR[p] + '_' + year + '_ScaleFactor_CR_SR_stau : ' + str(sf_SRs[p]) + '\n' 
            file.write(line)

    for i in range(len(namesSR)):
        print("Scale factor from ABCD method between regions C 0.3 - 0.8 and {} ={}".format(namesSR[i],sf_SRs[i]))

    scale_histogram_with_poissonian_errors(regC3fp8, 1./ScaleFac)

    scale_histogram_with_poissonian_errors(regC3f8_SRs[0], 1./ScaleFac)
    scale_histogram_with_poissonian_errors(regC3f8_SRs[1], 1./ScaleFacSR2)
    scale_histogram_with_poissonian_errors(regC3f8_SRs[2], 1./ScaleFacSR3)


        
    years = ['2017','2018']
    extPt = "_pt" + str(pTcut)
       
 
    for i in years:
        for idx,u in enumerate(namesSR):
            infile_path = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/final_data_validation"+extPt+"/BCKG_3f8_masswinwo_" + i + extPt + ".txt"
            outfile_path = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/final_data_validation"+extPt+"/scaled_bckg_" + i +"_"+ u + extPt + ".txt"
            makeOutputForDataCards(infile_path,outfile_path,sf_SRs[idx])
    

    scaledBkg3f8_2017_path = [ [] for i in range(len(namesSR))]
    scaledBkg3f8_2018_path = [ [] for i in range(len(namesSR))]

    for i in range(len(namesSR)):    
        scaledBkg3f8_2017_path[i] = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/final_data_validation"+extPt+"/scaled_bckg_2017_" +namesSR[i] +extPt+".txt" 
        scaledBkg3f8_2018_path[i] = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/final_data_validation"+extPt+"/scaled_bckg_2018_" + namesSR[i]+extPt + ".txt"

    bckg3f8_2017 = [[] for i in range(len(namesSR))] 
    bckg3f8_2018 = [[] for i in range(len(namesSR))] 

    for i in range(len(namesSR)):
        with open(scaledBkg3f8_2017_path[i], "r") as file:
            for line in file:
                value = float(line.strip())
                bckg3f8_2017[i].append(value)

        with open(scaledBkg3f8_2018_path[i], "r") as file:
            for line in file:
                value = float(line.strip())
                bckg3f8_2018[i].append(value)

    

    ''' 
    print("2017 background predicted in region C 0.3 - 0.8 , scaled to signal region D by {}".format(ScaleFac))
    print(bckg3f8_2017)
    print("2018 background predicted in region C 0.3 - 0.8 , scaled to signal region D by {}".format(ScaleFac))
    print(bckg3f8_2018)
    '''

    crlow = "3"
    crup = "8"
    vrlow = "8"
    vrup = "9"
  
    lejDataCR = "CR F^{pix}" + "[0."+crlow+" - 0."+ crup + "]"
    lejDataVR = "VR F^{pix}" + "[0."+vrlow+" - 0."+ vrup + "]"

    lejDataSR = "SR F^{pix}" + "[0.9 - 1]"
    lejDataSR2 = "SR F^{pix}" + "[0.99 - 1]"
    lejDataSR3 = "SR F^{pix}" + "[0.999 - 1]"

    
    regA3fp8 = regA3fp8.Rebin(sizeRebinning,'regA3fp8',rebinning)
    regC3fp8 = regC3fp8.Rebin(sizeRebinning,lejDataCR,rebinning)

    regC3fp8_scaleVR = regC3fp8_scaleVR.Rebin(sizeRebinning,lejDataCR,rebinning)

    for i in range(len(regC3f8_SRs)):
        regC3f8_SRs[i] = regC3f8_SRs[i].Rebin(sizeRebinning,lejDataCR,rebinning)

    regB8fp9 = regB8fp9.Rebin(sizeRebinning,'regB8fp9',rebinning)
    regD8fp9 = regD8fp9.Rebin(sizeRebinning,lejDataVR,rebinning)

    regB_SRs[0] = regB_SRs[0].Rebin(sizeRebinning,'regB9fp10',rebinning)
    regD_SRs[0] = regD_SRs[0].Rebin(sizeRebinning,lejDataSR,rebinning)

    regB_SRs[1] = regB_SRs[1].Rebin(sizeRebinning,'regB99fp10',rebinning)
    regD_SRs[1] = regD_SRs[1].Rebin(sizeRebinning,lejDataSR2,rebinning)
 
    regB_SRs[2] = regB_SRs[2].Rebin(sizeRebinning,'regB999fp10',rebinning)
    regD_SRs[2] = regD_SRs[2].Rebin(sizeRebinning,lejDataSR3,rebinning)



    name =year + "_CR"+crlow+"to"+crup+"_vs_VR"+vrlow+"to"+vrup
    
    CompareRegions(regC3fp8_scaleVR,regD8fp9,None,name,maxDisplay,outdir,False,False)
    

    meanSignalsSR = [ [] for _ in range(3)]

    datacardDir = [(pathDC +namesSR[i] + "_datacards_2channels_test_" + date + "/") for i in range(len(namesSR))]
    datacardDirMerged = [(pathDC + namesSR[i] +"datacards_merged_test_" + date + "/") for i in range(len(namesSR))]
    
    for i in range(len(datacardDir)):
        try:
            os.makedirs(datacardDir[i])
        except OSError:
            if not os.path.isdir(datacardDir[i]):
                raise

        try:
            os.makedirs(datacardDirMerged[i])
        except OSError:
            if not os.path.isdir(datacardDirMerged[i]):
                raise

    for u in range(len(datacardDir)):
        print("Search Region : {}\n".format(namesSR[u]))
        for signal in range(len(meanSignals[u])):
        
            up_window = meanSignals[u][signal] + 2*stddevSignals[u][signal]
            low_window = meanSignals[u][signal] - stddevSignals[u][signal]
    
            width_window = (up_window - low_window)
            print("Signal hypothese is ", sampleNames[signal])         
            print("Mean of signal = {}, std dev = {}".format(meanSignals[u][signal],stddevSignals[u][signal]))
            print("Upper value = {}+2*{} = {}".format(meanSignals[u][signal], stddevSignals[u][signal],up_window))
            print("Lower value = {}-{} = {}".format(meanSignals[u][signal],stddevSignals[u][signal], low_window))
            print("Mass window = [{} - {}]".format(low_window,up_window))
            print("Window width = {}".format(width_window))
    
            ## DIVIDING BY BIN WIIDTH
            #HERE CHANGED FITFUNC BY FINAL FIT
            
            itg_bck_err = ROOT.Double(0.0)
            itg_bck = regC3f8_SRs[u].IntegralAndError(regC3f8_SRs[u].FindBin(low_window),regC3f8_SRs[u].FindBin(up_window),itg_bck_err)
    
            print("Width of window = {}".format(width_window))
            if(year == '2017'):
                print("Direct prediction (CR scaled) in mass window {}-{} = {} +/- {}".format(low_window,up_window,bckg3f8_2017[u][signal], itg_bck_err))
                print("Integral of prediction from binned histo {}-{} = {} +/- {}".format(low_window,up_window,itg_bck, itg_bck_err))
            if(year == '2018'):
                print("Direct prediction (CR scaled) in mass window {}-{} = {} +/- {}".format(low_window,up_window,bckg3f8_2018[u][signal], itg_bck_err))
                print("Integral of prediction from binned histo {}-{} = {} +/- {}".format(low_window,up_window,itg_bck, itg_bck_err))
    
                 
            itg_sig_err = ROOT.Double(0.0)
            itg_sig = allScaledSelSR[u][signal].IntegralAndError(allScaledSelSR[u][signal].FindBin(low_window),allScaledSelSR[u][signal].FindBin(up_window),itg_sig_err)
    
            print("Integral of signal in mass window {}-{} = {} +\- {}".format(allScaledSelSR[u][signal].GetBinCenter(allScaledSelSR[u][signal].FindBin(low_window)),allScaledSelSR[u][signal].GetBinCenter(allScaledSelSR[u][signal].FindBin(up_window)),itg_sig,itg_sig_err))
            print("\n" * 2)
    
            signalUncert = 1.2
            bckgUncert = 1.

            if(year == '2017'):
                make_datacard_2channels_hscp(datacardDir[u], str(sampleNames[signal]+extDataCards), itg_sig, bckg3f8_2017[u][signal],0,signalUncert, bckgUncert)
            elif(year == '2018'):
                add_2018_to_datacards_2channels(datacardDir[u],str(sampleNames[signal]+extDataCards),itg_sig,bckg3f8_2018[u][signal])
            elif(year == '2017and2018'):
                make_datacard_hscp_merged(datacardDirMerged[u], str(sampleNames[signal]+extDataCards), itg_sig, itg_bck,0,1.5, 1.8)
             

        print("-"*50)
        print("-"*50)


    for i in range(len(selSigSR1)):
        selSigSR1[i] = selSigSR1[i].Rebin(sizeRebinning,sampleNames[i],rebinning)
        selSigSR2[i] = selSigSR2[i].Rebin(sizeRebinning,sampleNames[i],rebinning)
        selSigSR3[i] = selSigSR3[i].Rebin(sizeRebinning,sampleNames[i],rebinning)
    

    for i in range(len(selSigSR1)):
        selSigSR1[i] = overflowInLastBin(selSigSR1[i])
        selSigSR2[i] = overflowInLastBin(selSigSR2[i])
        selSigSR3[i] = overflowInLastBin(selSigSR3[i])

    #selSigSR1 = cleanHist(selSigSR1)
    nameSR = ["SR1_pred_datablind","SR2_pred_datablind","SR3_pred_datablind"]

    blind = True
    maxBlind = 0
   

    for k in range(3):
        scalePredMass[k].cd()
        regC3f8_SRs[k].Write(str(namePredSR[k]))
        scalePredMass[k].Close()


 
    #TO DEBUG
    #SignalRegion(regC3f8_SRs[0],regD_SRs[0],nameSR[0],maxDisplay,outdir,blind,maxBlind,selSigSR1[9],selSigSR1[10],selSigSR1[11],selSigSR1[12])
   
    #SignalRegion(regC3f8_SRs[1],regD_SRs[1],nameSR[1],maxDisplay,outdir,blind,maxBlind,selSigSR2[9],selSigSR2[10],selSigSR2[11],selSigSR2[12])

    #SignalRegion(regC3f8_SRs[2],regD_SRs[2],nameSR[2],maxDisplay,outdir,blind,maxBlind,selSigSR3[9],selSigSR3[10],selSigSR3[11],selSigSR3[12])

