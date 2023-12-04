import ROOT
from binningVar import *
import sys
from USE_DATE import USED_DATE, VERSION
import os

# Open the ROOT files
prefix = USED_DATE
#pt_ext = ["pt70/","pt100/","pt200/","pt300/"]
pt_ext = ["pt100/"]
#pt_name = ["70","100","200","300"]
pt_name = ["100"]
ROOT.gROOT.SetBatch(ROOT.kTRUE)
SearchReg = sys.argv[1]
year = sys.argv[2]

QT = 0

if year == "2017":
    QT = 0
else:
    QT = 1


nameRootSR = [year+"_"+SearchReg+"_predMass_pT"+pt_name[i]+".root" for i in range(len(pt_name))]
namePredSR = [year+"_"+SearchReg+"_predMass_pT"+pt_name[i] for i in range(len(pt_name))]
namePredSR_Raw = [year+"_"+SearchReg+"_predMass_pT"+pt_name[i]+"_raw" for i in range(len(pt_name))]

output_dir = "V"+VERSION+"/PredMass_"+prefix[:-1]
if not os.path.exists(output_dir):
    os.makedirs(output_dir)
    print("Output directory {} created".format(output_dir))
else:
    print("Output directory {} already exists".format(output_dir))

predRootFiles = [ROOT.TFile.Open("V"+VERSION+"/PredMass_"+prefix[:-1] +"/" + name, "RECREATE") for name in nameRootSR]

iDir = ["V"+VERSION+"/"+prefix + pt for pt in pt_ext]

nameHist2017 = "2017_"+SearchReg+"_predMass"
nameHist2018 = "2018_"+SearchReg+"_predMass"

nameHist2017raw = "2017_"+SearchReg+"_predMass_raw"
nameHist2018raw = "2018_"+SearchReg+"_predMass_raw"

iFile2017 = "2017_"+SearchReg+"_predMass.root"
iFile2018 = "2018_"+SearchReg+"_predMass.root"

iFileList2017 = [iDir[i] + iFile2017 for i in range(len(iDir))]
iFileList2018 = [iDir[i] + iFile2018 for i in range(len(iDir))]

files2017SR1 = [ROOT.TFile(item) for item in iFileList2017]
files2018SR1 = [ROOT.TFile(item) for item in iFileList2018]

# Get histograms from the files
histsPTall = []
for i in range(len(iDir)):
    histsPTall.append( (files2017SR1[i].Get(nameHist2017),files2018SR1[i].Get(nameHist2018) ) )
histsPTall = [list(pair) for pair in histsPTall]

histsPTall_raw = []
for i in range(len(iDir)):
    histsPTall_raw.append( (files2017SR1[i].Get(nameHist2017raw),files2018SR1[i].Get(nameHist2018raw) ) )
histsPTall_raw = [list(pair) for pair in histsPTall_raw]

'''
histsPt70 = [files2017SR1[0].Get(nameHist2017),files2018SR1[0].Get(nameHist2018)]
histsPt100 = [files2017SR1[1].Get(nameHist2017),files2018SR1[1].Get(nameHist2018)]
histsPt200 = [files2017SR1[2].Get(nameHist2017),files2018SR1[2].Get(nameHist2018)]
histsPt300 = [files2017SR1[3].Get(nameHist2017),files2018SR1[3].Get(nameHist2018)]

histsPt70_raw = [files2017SR1[0].Get(nameHist2017raw),files2018SR1[0].Get(nameHist2018raw)]
histsPt100_raw = [files2017SR1[1].Get(nameHist2017raw),files2018SR1[1].Get(nameHist2018raw)]
histsPt200_raw = [files2017SR1[2].Get(nameHist2017raw),files2018SR1[2].Get(nameHist2018raw)]
histsPt300_raw = [files2017SR1[3].Get(nameHist2017raw),files2018SR1[3].Get(nameHist2018raw)]
'''
print("Rebinning with this array :{}, with size {}".format(rebinning,sizeRebinning))


for i in range(2):
   for p in range(len(histsPTall)):
       print(histsPTall[p][i])
       if isinstance(histsPTall[p][i], ROOT.TH1F):
           histsPTall[p][i]=histsPTall[p][i].Rebin(sizeRebinning,pt_ext[p][:-1]+str(i),rebinning)
   for k in range(len(histsPTall_raw)):
       if isinstance(histsPTall_raw[k][i], ROOT.TH1F):
           histsPTall_raw[k][i]=histsPTall_raw[k][i].Rebin(sizeRebinning,pt_ext[k][:-1]+str(i),rebinning)
'''
for i in range(0,2):
    if isinstance(histsPt70[i], ROOT.TH1F):    
        histsPt70[i]=histsPt70[i].Rebin(sizeRebinning,'pt70'+str(i),rebinning)
    if isinstance(histsPt100[i], ROOT.TH1F):    
        histsPt100[i]=histsPt100[i].Rebin(sizeRebinning,'pt100'+str(i),rebinning)
    if isinstance(histsPt200[i], ROOT.TH1F):    
        histsPt200[i]=histsPt200[i].Rebin(sizeRebinning,'pt200'+str(i),rebinning)
    if isinstance(histsPt300[i], ROOT.TH1F):    
        histsPt300[i]=histsPt300[i].Rebin(sizeRebinning,'pt300'+str(i),rebinning)

    if isinstance(histsPt70_raw[i], ROOT.TH1F):    
        histsPt70_raw[i]=histsPt70_raw[i].Rebin(sizeRebinning,'pt70'+str(i),rebinning)
    if isinstance(histsPt100_raw[i], ROOT.TH1F):    
        histsPt100_raw[i]=histsPt100_raw[i].Rebin(sizeRebinning,'pt100'+str(i),rebinning)
    if isinstance(histsPt200_raw[i], ROOT.TH1F):    
        histsPt200_raw[i]=histsPt200_raw[i].Rebin(sizeRebinning,'pt200'+str(i),rebinning)
    if isinstance(histsPt300_raw[i], ROOT.TH1F):    
        histsPt300_raw[i]=histsPt300_raw[i].Rebin(sizeRebinning,'pt300'+str(i),rebinning)
'''

#allHists = [histsPTall[p][QT] for p in range(len(histsPTall))]
allHists = [pair[QT] for pair in histsPTall]
allHists_raw = [pair[QT] for pair in histsPTall_raw]
'''
allHists = [histsPt70[QT],histsPt100[QT],histsPt200[QT],histsPt300[QT]]
allHists_raw = [histsPt70_raw[QT],histsPt100_raw[QT],histsPt200_raw[QT],histsPt300_raw[QT]]
'''
print(allHists)
iMarkers = [39,22,23,8]
iColors = [26,30,41,46]

for i in range(2):
    for p in range(len(histsPTall)):
        if isinstance(histsPTall[p][i], ROOT.TH1F):
            histsPTall[p][i].SetMarkerStyle(iMarkers[p])
            histsPTall[p][i].SetMarkerColor(iColors[p])
            histsPTall[p][i].SetMarkerSize(1)
    for k in range(len(histsPTall_raw)):
        if isinstance(histsPTall_raw[k][i], ROOT.TH1F):
            histsPTall_raw[p][i].SetMarkerStyle(iMarkers[k])
            histsPTall_raw[p][i].SetMarkerColor(iColors[k])
            histsPTall_raw[p][i].SetMarkerSize(1)
'''
for i in range(len(histsPt100)):
    if isinstance(histsPt70[i], ROOT.TH1F):    
        histsPt70[i].SetMarkerStyle(iMarkers[0])
        histsPt70[i].SetMarkerColor(iColors[0])
        histsPt70[i].SetMarkerSize(1)
    if isinstance(histsPt100[i], ROOT.TH1F):    
        histsPt100[i].SetMarkerStyle(iMarkers[1])
        histsPt100[i].SetMarkerColor(iColors[1])
        histsPt100[i].SetMarkerSize(1)
    if isinstance(histsPt200[i], ROOT.TH1F):    
        histsPt200[i].SetMarkerStyle(iMarkers[2])
        histsPt200[i].SetMarkerColor(iColors[2])
        histsPt200[i].SetMarkerSize(1)
    if isinstance(histsPt300[i], ROOT.TH1F):    
        histsPt300[i].SetMarkerStyle(iMarkers[3])
        histsPt300[i].SetMarkerColor(iColors[3])
        histsPt300[i].SetMarkerSize(1)

    if isinstance(histsPt70_raw[i], ROOT.TH1F):    
        histsPt70_raw[i].SetMarkerStyle(iMarkers[0])
        histsPt70_raw[i].SetMarkerColor(iColors[0])
        histsPt70_raw[i].SetMarkerSize(1)
    if isinstance(histsPt100_raw[i], ROOT.TH1F):    
        histsPt100_raw[i].SetMarkerStyle(iMarkers[1])
        histsPt100_raw[i].SetMarkerColor(iColors[1])
        histsPt100_raw[i].SetMarkerSize(1)
    if isinstance(histsPt200_raw[i], ROOT.TH1F):    
        histsPt200_raw[i].SetMarkerStyle(iMarkers[2])
        histsPt200_raw[i].SetMarkerColor(iColors[2])
        histsPt200_raw[i].SetMarkerSize(1)
    if isinstance(histsPt300_raw[i], ROOT.TH1F):    
        histsPt300_raw[i].SetMarkerStyle(iMarkers[3])
        histsPt300_raw[i].SetMarkerColor(iColors[3])
        histsPt300_raw[i].SetMarkerSize(1)
'''



# Create a canvas
canvas2017 = ROOT.TCanvas("pred_SR1_"+year, "Histogram Comparison for"+SearchReg+" (ABCD scaled) background prediction,"+year+" data", 800, 600)
canvas2017.SetLogy()

# Draw hists on the canvas for 2017
maxX = rebinning[-1]


for p in range(len(histsPTall)):
    if isinstance(histsPTall[p][QT], ROOT.TH1F):
        histsPTall[p][QT].GetXaxis().SetRangeUser(0,maxX)
        if p == 1:
            histsPTall[p][QT].Draw() 
        else:
            histsPTall[p][QT].Draw("SAME")
        histsPTall[p][QT].SetStats(False)

'''
if isinstance(histsPt70[QT], ROOT.TH1F):    
    histsPt70[QT].GetXaxis().SetRangeUser(0,maxX)
    histsPt70[QT].Draw()
    histsPt70[QT].SetStats(False)
if isinstance(histsPt100[QT], ROOT.TH1F):    
    histsPt100[QT].GetXaxis().SetRangeUser(0,maxX)
    histsPt100[QT].Draw("SAME")
    histsPt100[QT].SetStats(False)
if isinstance(histsPt200[QT], ROOT.TH1F):    
    histsPt200[QT].GetXaxis().SetRangeUser(0,maxX)
    histsPt200[QT].Draw("SAME")
    histsPt200[QT].SetStats(False)
if isinstance(histsPt300[QT], ROOT.TH1F):    
    histsPt300[QT].GetXaxis().SetRangeUser(0,maxX)
    histsPt300[QT].Draw("SAME")
    histsPt300[QT].SetStats(False)
'''
# Add Title
pt = ROOT.TPaveText(0.1, 0.91, 0.4, 0.97, "NDC")
pt.SetFillColor(0)
pt.SetTextAlign(12)
pt.SetTextFont(42)
title = "Prediction in " + SearchReg + " for " + year + " data"
pt.AddText(title)
pt.Draw()

# Add legend
legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
for p in range(len(histsPTall)):
    if isinstance(histsPTall[p][QT], ROOT.TH1F):    
        legend.AddEntry(histsPTall[p][QT], "pT > "+pt_name[p], "EP1")
'''
if isinstance(histsPt70[QT], ROOT.TH1F):    
    legend.AddEntry(histsPt70[QT], "pT > 70", "EP1")
if isinstance(histsPt100[QT], ROOT.TH1F):    
    legend.AddEntry(histsPt100[QT], "pT > 100", "EP1")
if isinstance(histsPt200[QT], ROOT.TH1F):    
    legend.AddEntry(histsPt200[QT], "pT > 200", "EP1")
if isinstance(histsPt300[QT], ROOT.TH1F):    
    legend.AddEntry(histsPt300[QT], "pT > 300", "EP1")
'''
legend.Draw()

# Update the canvas
#canvas2017.Update()

# Save the canvas as "displayed_pred_SR.root"
canvas2017.SaveAs("displayed_pred_"+SearchReg+"_"+year+".root")
    
for k in range(len(pt_name)):
    predRootFiles[k].cd()    
    if isinstance(allHists[k], ROOT.TH1F):    
        print("Writing {} with name {}".format(allHists[k],str(namePredSR[k])))
        allHists[k].Write(str(namePredSR[k]))
        allHists_raw[k].Write(str(namePredSR_Raw[k]))
        
    predRootFiles[k].Close()


# Clean up
for i in range(len(files2017SR1)):
    files2017SR1[i].Close()
for i in range(len(files2018SR1)):
    files2018SR1[i].Close()
