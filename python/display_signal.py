import ROOT
import sys
import os
from binningVar import *
from USE_DATE import USED_DATE, VERSION 


# Define the list of mass points
massPoints = ["200","247","308","432","557","651","745","871","1029","1218","1409","1599"]

# Signal type : stau
sType = ['$\\tilde{\\tau}$ (M='+i+' GeV)' for i in massPoints]


#types
iMarkers = [39,22,23,8]
#colors
iColors = [26,30,41,46]
pt_name = ["70","100","200","300"]

if  len(sys.argv) != 3:
    print("Usage : python display_signal.py <PT cut> <Search Region (SR0...)>")

version = VERSION
ptCUT = sys.argv[1]
SearchRegion = sys.argv[2]

pathSig = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V"+version+"/HSCPpairStau_V"+version+"/Merged_HSCPpairStau/"
input_files = [pathSig + "HSCPpairStau_M-"+massPoints[i]+"_merged.root" for i in range(len(massPoints))]




allNames = ["HSCPpairStau_M-"+massPoints[i] for i in range(len(massPoints))]
allNamesSysPUDown = ["HSCPpairStau_M-"+massPoints[i]+"_Pileup_down" for i in range(len(massPoints))]
allNamesSysPUUp = ["HSCPpairStau_M-"+massPoints[i]+"_Pileup_up" for i in range(len(massPoints))]
allNamesSysPtDown = ["HSCPpairStau_M-"+massPoints[i]+"_Pt_down" for i in range(len(massPoints))]
allNamesSysPtUp = ["HSCPpairStau_M-"+massPoints[i]+"_Pt_up" for i in range(len(massPoints))]
allNamesSysTriggerDown = ["HSCPpairStau_M-"+massPoints[i]+"_Trigger_down" for i in range(len(massPoints))]
allNamesSysTriggerUp = ["HSCPpairStau_M-"+massPoints[i]+"_Trigger_up" for i in range(len(massPoints))]
allNamesSysKDown = ["HSCPpairStau_M-"+massPoints[i]+"_K_down" for i in range(len(massPoints))]
allNamesSysKUp = ["HSCPpairStau_M-"+massPoints[i]+"_K_up" for i in range(len(massPoints))]
allNamesSysCDown = ["HSCPpairStau_M-"+massPoints[i]+"_C_down" for i in range(len(massPoints))]
allNamesSysCUp = ["HSCPpairStau_M-"+massPoints[i]+"_C_up" for i in range(len(massPoints))]



nameFiles = [allNames[i].split("_")[0]+allNames[i].split("_")[1]+ '_Sys.root' for i in range(len(allNames))]

nameRootSR ="sigMass_"+SearchRegion+"_pT"+ptCUT+".root"
path_direc = "V"+version+"/SignalMass_V"+version+SearchRegion+"_pT"+ptCUT

if not os.path.exists(path_direc):
    os.makedirs(path_direc)


signalFiles = [ROOT.TFile.Open(path_direc+"/" +name , "RECREATE") for name in nameFiles]



rFiles = [ROOT.TFile(input_files[i]) for i in range(len(input_files))]


idirSignal='HSCParticleAnalyzer/BaseName/'

massDistribSR = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt"+ptCUT+"_Fpix_Mass") for i in range(len(rFiles))]
#SysPU
sysPileUpUpDistribSR = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt"+ptCUT+"_Fpix_Mass_Pileup_up") for i in range(len(rFiles))]
sysPileUpDownDistribSR = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt"+ptCUT+"_Fpix_Mass_Pileup_down") for i in range(len(rFiles))]
#SysPT
sysPTUpDistribSR = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt"+ptCUT+"_Fpix_Mass_Pt_up") for i in range(len(rFiles))]
sysPTDownDistribSR = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt"+ptCUT+"_Fpix_Mass_Pt_down") for i in range(len(rFiles))]
#SysTRIGGER
sysTriggerUpDistribSR = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt"+ptCUT+"_Fpix_Mass_Trigger_up") for i in range(len(rFiles))]
sysTriggerDownDistribSR = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt"+ptCUT+"_Fpix_Mass_Trigger_down") for i in range(len(rFiles))]
#SysK
sysFacKUpDistribSR = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt"+ptCUT+"_Fpix_Mass_K_up1") for i in range(len(rFiles))]
sysFacKDownDistribSR = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt"+ptCUT+"_Fpix_Mass_K_down1") for i in range(len(rFiles))]
#SysC
sysFacCUpDistribSR = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt"+ptCUT+"_Fpix_Mass_C_up1") for i in range(len(rFiles))]
sysFacCDownDistribSR = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt"+ptCUT+"_Fpix_Mass_C_down1") for i in range(len(rFiles))]



'''
#---------------------------------------------- PT cut 100 -------------------------------------------------------------
massDistribSRpt100 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt100_Fpix_Mass") for i in range(len(rFiles))]
#SysPU
sysPileUpUpDistribSRpt100 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt100_Fpix_Mass_Pileup_up") for i in range(len(rFiles))]
sysPileUpDownDistribSRpt100 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt100_Fpix_Mass_Pileup_down") for i in range(len(rFiles))]
#SysPT
sysPTUpDistribSRpt100 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt100_Fpix_Mass_Pt_up") for i in range(len(rFiles))]
sysPTDownDistribSRpt100 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt100_Fpix_Mass_Pt_down") for i in range(len(rFiles))]
#SysTRIGGER
sysTriggerUpDistribSRpt100 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt100_Fpix_Mass_Trigger_up") for i in range(len(rFiles))]
sysTrigerDownDistribSRpt100 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt100_Fpix_Mass_Trigger_down") for i in range(len(rFiles))]
#SysK
sysFacKUpDistribSRpt100 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt100_Fpix_Mass_K_up1") for i in range(len(rFiles))]
sysFacKDownDistribSRpt100 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt100_Fpix_Mass_K_down1") for i in range(len(rFiles))]
#SysC
sysFacCUpDistribSRpt100 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt100_Fpix_Mass_C_up1") for i in range(len(rFiles))]
sysFacCDownDistribSRpt100 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt100_Fpix_Mass_C_down1") for i in range(len(rFiles))]

#---------------------------------------------- PT cut 200 -------------------------------------------------------------

massDistribSRpt200 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt200_Fpix_Mass") for i in range(len(rFiles))]

massDistribSRpt300 = [rFiles[i].Get(idirSignal + "PostS_"+SearchRegion+"_pt300_Fpix_Mass") for i in range(len(rFiles))]
'''

for i in range(len(massDistribSR)):
    massDistribSR[i] = massDistribSR[i].Rebin(sizeRebinning,'pt'+ptCUT+str(i),rebinning)
    sysPileUpUpDistribSR[i] = sysPileUpUpDistribSR[i].Rebin(sizeRebinning,'PileUpUp'+ptCUT+str(i),rebinning)
    sysPileUpDownDistribSR[i] = sysPileUpDownDistribSR[i].Rebin(sizeRebinning,'PileUpDown'+ptCUT+str(i),rebinning)
    sysPTUpDistribSR[i] = sysPTUpDistribSR[i].Rebin(sizeRebinning,'PtUp'+ptCUT+str(i),rebinning)
    sysPTDownDistribSR[i] = sysPTDownDistribSR[i].Rebin(sizeRebinning,'PtDown'+ptCUT+str(i),rebinning)
    sysTriggerUpDistribSR[i] = sysTriggerUpDistribSR[i].Rebin(sizeRebinning,'TriggerUp'+ptCUT+str(i),rebinning)
    sysTriggerDownDistribSR[i] = sysTriggerDownDistribSR[i].Rebin(sizeRebinning,'TriggerDown'+ptCUT+str(i),rebinning)
    sysFacKUpDistribSR[i] = sysFacKUpDistribSR[i].Rebin(sizeRebinning,'KUp'+ptCUT+str(i),rebinning)
    sysFacKDownDistribSR[i] = sysFacKDownDistribSR[i].Rebin(sizeRebinning,'KDown'+ptCUT+str(i),rebinning)
    sysFacCUpDistribSR[i] = sysFacCUpDistribSR[i].Rebin(sizeRebinning,'CUp'+ptCUT+str(i),rebinning)
    sysFacCDownDistribSR[i] = sysFacCDownDistribSR[i].Rebin(sizeRebinning,'CDown'+ptCUT+str(i),rebinning)

    '''
    massDistribSRpt100[i] = massDistribSRpt100[i].Rebin(sizeRebinning,'pt100'+str(i),rebinning)
    massDistribSRpt200[i] = massDistribSRpt200[i].Rebin(sizeRebinning,'pt200'+str(i),rebinning)
    massDistribSRpt300[i] = massDistribSRpt300[i].Rebin(sizeRebinning,'pt300'+str(i),rebinning)
    '''

#Set style of signal histos
for i in range(len(massDistribSR)):
    massDistribSR[i].SetMarkerStyle(iMarkers[0])
    massDistribSR[i].SetMarkerColor(iColors[0])

    '''
    massDistribSRpt100[i].SetMarkerStyle(iMarkers[1])
    massDistribSRpt100[i].SetMarkerColor(iColors[1])
    
    massDistribSRpt200[i].SetMarkerStyle(iMarkers[2])
    massDistribSRpt200[i].SetMarkerColor(iColors[2])

    massDistribSRpt300[i].SetMarkerStyle(iMarkers[3])
    massDistribSRpt300[i].SetMarkerColor(iColors[3])
    '''


for i in range(len(signalFiles)):
    signalFiles[i].cd()
    #for i in range(len(massDistribSR)):
    massDistribSR[i].Write(str(allNames[i]))
    sysPileUpUpDistribSR[i].Write(str(allNamesSysPUUp[i]))
    sysPileUpDownDistribSR[i].Write(str(allNamesSysPUDown[i]))
    sysPTUpDistribSR[i].Write(str(allNamesSysPtUp[i]))
    sysPTDownDistribSR[i].Write(str(allNamesSysPtDown[i]))
    sysTriggerUpDistribSR[i].Write(str(allNamesSysTriggerUp[i]))
    sysTriggerDownDistribSR[i].Write(str(allNamesSysTriggerDown[i]))
    sysFacKUpDistribSR[i].Write(str(allNamesSysKUp[i]))
    sysFacKDownDistribSR[i].Write(str(allNamesSysKDown[i]))
    sysFacCUpDistribSR[i].Write(str(allNamesSysCUp[i]))
    sysFacCDownDistribSR[i].Write(str(allNamesSysCDown[i]))

    signalFiles[i].Close()

# Create a canvas
'''
for i in range(len(massDistribSRpt100)):
    canvas = ROOT.TCanvas("CvStauM-"+massPoints[i], "Mass Distribution for "+sType[i], 800, 600)
    canvas.SetLogy()

    massDistribSRpt70[i].SetStats(False)
    massDistribSRpt70[i].Draw("P")

    massDistribSRpt100[i].SetStats(False)
    massDistribSRpt100[i].Draw("SAME")

    massDistribSRpt200[i].SetStats(False)
    massDistribSRpt200[i].Draw("SAME")

    massDistribSRpt300[i].SetStats(False)
    massDistribSRpt300[i].Draw("SAME")

    legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    legend.AddEntry(massDistribSRpt70[i], sType[i]+"_pT > 70", "EP1")
    legend.AddEntry(massDistribSRpt100[i], sType[i]+"_pT > 100", "EP1")
    legend.AddEntry(massDistribSRpt200[i], sType[i]+"_pT > 200", "EP1")
    legend.AddEntry(massDistribSRpt300[i], sType[i]+"_pT > 300", "EP1")
    legend.Draw()


    canvas.SaveAs("stauSignals/"+SearchRegion+"_StauM-"+massPoints[i]+".root")
'''
#cleanup
for i in range(len(rFiles)):
    rFiles[i].Close()
