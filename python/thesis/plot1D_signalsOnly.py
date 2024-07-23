import os
import ROOT
import sys
import re

sys.path.append('../')
from allHists import CDF
from ValidationFunctionsData import *
import CMS_lumi

ROOT.gROOT.SetBatch(ROOT.kTRUE)

CMS_lumi.lumi_13TeV = "CMS Simulation"
CMS_lumi.writeExtraText = True
CMS_lumi.extraText = "Private work"

iPos = 0
if iPos == 0:
    CMS_lumi.relPosX = 0.12
iPeriod = 4

massPoints = ["200", "247", "308", "432", "557", "651", "745", "871", "1029"]
colors = [20, 28, 30, 36, 38, 41, 44, 46, 2]
sType = ['$\\tilde{\\tau}$ (M=' + i + ' GeV)' for i in massPoints]


def getRangeGraph(hist):
    min_x = hist.GetXaxis().GetBinLowEdge(1)
    max_x = hist.GetXaxis().GetBinLowEdge(hist.GetNbinsX()+1)
    return (min_x,max_x)

def MakeCanvas(name,array,output,min_X,max_X,x_axis_title):
    canvas = ROOT.TCanvas(name, name, 1200, 1200)
    legend = ROOT.TLegend(0.7, 0.4, 0.9, 0.9)
    pad1 = ROOT.TPad("histograms", "Histograms", 0, 0.32, 1, 0.92)
    pad1.Draw()
    pad1.cd()
    pad1.SetTopMargin(0.033)
    all_CDF = []
    for i in range(len(array)):
        cdf = CDF(array[i][0],'$\\frac{1}{\\beta}',ROOT.kRed,'TOF',max_X)
        all_CDF.append(cdf)
    
    for i in range(len(array)):
        all_CDF[i].SetMarkerColor(colors[array[i][1]])
        legend.AddEntry(array[i][0], sType[array[i][1]], "PE1")
        array[i][0].GetXaxis().SetTitle("")
        if i == 0:
            array[i][0].Draw("P")
        else:
            array[i][0].Draw("same P")

    legend.Draw("same")
    canvas.cd()
    pad2 = ROOT.TPad("histograms", "Histograms", 0, 0.1, 1, 0.3)
    pad2.Draw()
    pad2.cd()
    pad2.SetBottomMargin(0.2)
    frame = array[0][0].Clone("FRAME")
    frame.Reset()
    frame = SetFrameColor(frame,510,510,510,0,"",x_axis_title,"CDF",0,1,43,12,43,14,43,10,43,16,3.9,1.75) 
    frame.Draw("SAME AXIS")
    for i in range(len(all_CDF)):
        if i==0:
            all_CDF[i].Draw("P")
        else:
            all_CDF[i].Draw("same P")
  
    lineOne = ROOT.TLine(min_X, 0.5, max_X, 0.5)
    lineOne.SetLineColor(1)
    lineOne.SetLineStyle(2)
    lineOne.Draw("SAME")
    canvas.cd()
    
    CMS_lumi.CMS_lumi(canvas, iPeriod, iPos)
    namepng = output+'.png'
    nameroot = output+'.root'

    canvas.SaveAs(namepng)
    canvas.SaveAs(nameroot)

def histoStyle(hist,color):
    if hist.Integral() > 0:
        hist.Scale(1.0 / hist.Integral())

    hist.SetStats(False)
    hist.SetMarkerColor(color)
    hist.SetMarkerSize(1)
    hist.SetMarkerStyle(8)

def extract_number(filename):
    match = re.search(r'(\d+)', filename)
    if match:
        return int(match.group(1))
    return -1  # Return -1 if no numerical part is found


def main():    
    directory_path = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Signal_good/pStau_V7p0"

    outdir = "Signals_only/"    
    tofhistograms = []
    ihhistograms = []
    masshistograms = []

    file_paths = []
    root_files = []
    
    sorted_files = sorted(os.listdir(directory_path), key=extract_number)
    
    for counter, filename in enumerate(sorted_files):
        if filename.endswith(".root") and filename.startswith("pStau"):
            mass = filename.split("_")[0][5:]
            print("Mass = {}".format(mass))
            idxArray = massPoints.index(mass)
            file_path = os.path.join(directory_path, filename)
            file_paths.append( (file_path,idxArray) )
            root_file = ROOT.TFile.Open(file_path)
            root_files.append(root_file)
    
            histogram_tof = root_file.Get("hTOF__regionC_3fp8_testIhPt")
            histogram_mass = root_file.Get("mass_regionC_3fp8_testIhPt")
            histogram_ih = root_file.Get("ih_npv_regionC_3fp8_testIhPt").ProjectionY()
   
            histogram_mass.Rebin(8) 
            histogram_tof.Rebin(2)
            histogram_ih.Rebin(4)
    
            colorPoint = colors[idxArray]
    
            histoStyle(histogram_tof,colorPoint)
            histoStyle(histogram_mass,colorPoint)
            histoStyle(histogram_ih,colorPoint)
    
            tofhistograms.append((histogram_tof,idxArray))
            masshistograms.append((histogram_mass,idxArray))
            ihhistograms.append((histogram_ih,idxArray))
    
    range_tof = getRangeGraph(tofhistograms[0][0])
    range_ih = getRangeGraph(ihhistograms[0][0])
    range_mass = getRangeGraph(masshistograms[0][0])
    
    MakeCanvas("canvas_TOF",tofhistograms,outdir+"TOF_distrib_stau",range_tof[0],range_tof[1],'$\\frac{1}{\\beta}')
    MakeCanvas("canvas_IH",ihhistograms,outdir+"IH_distrib_stau",range_ih[0],range_ih[1],"IH [MeV/cm]")
    MakeCanvas("canvas_MASS",masshistograms,outdir+"Mass_distrib_stau",range_mass[0],range_mass[1],"Mass [GeV]")

if __name__ == "__main__":
    main()
