import os
import ROOT
import sys
import re
import ctypes

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


mass = "557"
sType = '#tilde{#tau} (M=' + mass + ' GeV)'

massGlu = "1800"
sTypeGlu = '#tilde{g} (M=' + massGlu + ' GeV)'

color_signal = 8
color_signalGlu = 2
color_data = 4

def getRangeGraph(hist):
    min_x = hist.GetXaxis().GetBinLowEdge(1)
    max_x = hist.GetXaxis().GetBinLowEdge(hist.GetNbinsX()+1)
    return (min_x,max_x)

def MakeCanvas(name,sig,sig2,data,output,min_X,max_X,x_axis_title):
    canvas = ROOT.TCanvas(name, name, 700, 900)
    legend = ROOT.TLegend(0.5, 0.7, 0.8, 0.9)
    pad1 = ROOT.TPad("histograms", "Histograms", 0, 0.26, 1, 0.92)
    #pad1.SetLogy(1)
    pad1.Draw()
    pad1.cd()
    pad1.SetTopMargin(0.033)
    #pad1.SetBottomMargin(0)

    legend.AddEntry(sig,sType, "PE1")
    if sig2 is not None:
        legend.AddEntry(sig2,sTypeGlu, "PE1")
    legend.AddEntry(data,'Single Muon' , "PE1")
    sig.GetXaxis().SetTitle("")
    if sig2 is not None:
        sig2.GetXaxis().SetTitle("")
    data.GetXaxis().SetTitle("")

    sig.SetMarkerSize(0.7)
    if sig2 is not None:
        sig2.SetMarkerSize(0.7)
    data.SetMarkerSize(0.7)

    maxY = 0.8
    data.GetYaxis().SetRangeUser(0,maxY)
    sig.GetYaxis().SetRangeUser(0,maxY)
    if sig2 is not None:
        sig2.GetYaxis().SetRangeUser(0,maxY)
    sig.Draw("HIST")
    if sig2 is not None:
        sig2.Draw("same HIST")
    data.Draw("same HIST")
    legend.Draw("same")

    canvas.cd()
    cdfsig = CDF(sig,'$\\frac{1}{\\beta}',ROOT.kGreen,'TOF',max_X)
    if sig2 is not None:
        cdfsig2 = CDF(sig2,'$\\frac{1}{\\beta}',ROOT.kRed,'TOF',max_X)
    cdfdata = CDF(data,'$\\frac{1}{\\beta}',ROOT.kBlue,'TOF',max_X)    
    
    x_above_threshold = None

    if "TOF" in name:
        for i in range(1,cdfdata.GetN()):
            x =  ctypes.c_double(0)
            y =  ctypes.c_double(0)
            cdfdata.GetPoint(i, x, y)
            #print("Point {} has x = {} and y = {}".format(i,x,y))
            if y.value > 0.8:
                 x_above_threshold = x
                 print("X value for TOF where 80% of data is below = {}".format(x_above_threshold))
                 break

    cdfsig.SetMarkerColor(color_signal)
    if sig2 is not None:
        cdfsig2.SetMarkerColor(color_signalGlu)
    cdfdata.SetMarkerColor(color_data)

    cdfsig.SetMarkerStyle(20)
    if sig2 is not None:
        cdfsig2.SetMarkerStyle(20)
    cdfdata.SetMarkerStyle(20)

    cdfsig.SetMarkerSize(0.7)
    if sig2 is not None:
        cdfsig2.SetMarkerSize(0.7)
    cdfdata.SetMarkerSize(0.7)

    pad2 = ROOT.TPad("cdfs", "CDFs", 0, 0.12, 1, 0.25)
    pad2.Draw()
    pad2.SetBottomMargin(0.2)
    pad2.SetBorderSize(0)
    pad2.SetTopMargin(0)
    pad2.cd()
    frame = data.Clone("FRAME")
    frame.Reset()
    frame = SetFrameColor(frame,510,510,510,0,"",x_axis_title,"CDF",0,1,43,12,43,14,43,10,43,16,4.1,1.75) 
    frame.Draw("SAME AXIS")
    cdfsig.Draw("P")
    if sig2 is not None:
        cdfsig2.Draw("same P")
    cdfdata.Draw("same P")
  
    lineOne = ROOT.TLine(min_X, 0.5, max_X, 0.5)
    lineOne.SetLineColor(1)
    lineOne.SetLineStyle(2)
    lineOne.Draw("SAME")
    if "TOF" in name:
        lineThr = ROOT.TLine(float(x_above_threshold.value), 0, float(x_above_threshold.value), 1)
        lineThr.SetLineColor(9)
        lineThr.SetLineStyle(2)
        lineThr.SetLineWidth(2)
        lineThr.Draw("SAME")

    canvas.cd()
    
    CMS_lumi.CMS_lumi(canvas, iPeriod, iPos)
    namepng = output+'.png'
    nameroot = output+'.root'

    canvas.SaveAs(namepng)
    canvas.SaveAs(nameroot)

def histoStyle(hist,color):
    hist1 = hist.Clone()

    
    if hist1.Integral() > 0:
        hist1.Scale(1.0 / hist1.Integral())
    else:
        print("Hist {} has no entries".format(hist))
    
    hist1.SetStats(False)
    hist1.SetMarkerColor(color)
    hist1.SetLineColor(color)
    hist1.SetFillColor(color)
    hist1.SetMarkerSize(1)
    hist1.SetMarkerStyle(20)
    hist1.SetFillStyle(3004)
    return hist1

def extract_number(filename):
    match = re.search(r'(\d+)', filename)
    if match:
        return int(match.group(1))
    return -1  # Return -1 if no numerical part is found


def main():
    signal_path = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Signal_good/pStau_V7p0/"
    signal_path_glu = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Signal_good/gluino_V7p0/"

    data_path = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/4_october_data_validation_pt100/"

    outdir = "Signal_vs_data/"
    sigHyp = "pStau"+mass+"_massCut_0_pT100_V7p0_Fpix.root"
    sigHypGlu = "Glu"+massGlu+"_massCut_0_pT100_V7p0_Fpix.root"
    dataHyp = "Mu2018_massCut_0_pT100_V7p0_Fpix.root"    


    sigRoot_file = ROOT.TFile.Open(os.path.join(signal_path, sigHyp))
    sigRoot_fileGlu = ROOT.TFile.Open(os.path.join(signal_path_glu, sigHypGlu))
    dataRoot_file = ROOT.TFile.Open(os.path.join(data_path, dataHyp))

    sig_histogram_tof = sigRoot_file.Get("hTOF__regionC_3fp8_testIhPt")
    sig_histogram_mass = sigRoot_file.Get("mass_regionC_3fp8_testIhPt")

    sig_histogram_ih2d = sigRoot_file.Get("ih_npv_regionC_3fp8_testIhPt")
    sig_histogram_ih = sig_histogram_ih2d.ProjectionY("sig_ih") 

    sig_histogram_fpix2d = sigRoot_file.Get("fpix_ih_regionAll_testIhPt")
    sig_histogram_fpix = sig_histogram_fpix2d.ProjectionX("sig_fpix")

    sig_histogram_ias2d = sigRoot_file.Get("ias_pt_regionAll_testIhPt")
    sig_histogram_ias = sig_histogram_ias2d.ProjectionY("sig_gstrip")

    #sig_histogram_tof_glu = sigRoot_fileGlu.Get("hTOF__regionC_3fp8_testIhPt")
    sig_histogram_mass_glu = sigRoot_fileGlu.Get("mass_regionC_3fp8_testIhPt")

    sig_histogram_ih2d_glu = sigRoot_fileGlu.Get("ih_npv_regionC_3fp8_testIhPt")
    sig_histogram_ih_glu = sig_histogram_ih2d_glu.ProjectionY("sig_ih_glu") 

    sig_histogram_fpix2d_glu = sigRoot_fileGlu.Get("fpix_ih_regionAll_testIhPt")
    sig_histogram_fpix_glu = sig_histogram_fpix2d_glu.ProjectionX("sig_fpix_glu")

    sig_histogram_ias2d_glu = sigRoot_fileGlu.Get("ias_pt_regionAll_testIhPt")
    sig_histogram_ias_glu = sig_histogram_ias2d_glu.ProjectionY("sig_gstrip_glu")


    data_histogram_tof = dataRoot_file.Get("hTOF__regionC_3fp8_testIhPt")
    data_histogram_mass = dataRoot_file.Get("mass_regionC_3fp8_testIhPt")

    data_histogram_ih2d = dataRoot_file.Get("ih_npv_regionC_3fp8_testIhPt")
    data_histogram_ih = data_histogram_ih2d.ProjectionY("data_ih")

    data_histogram_fpix2d = dataRoot_file.Get("fpix_ih_regionAll_testIhPt")
    data_histogram_fpix = data_histogram_fpix2d.ProjectionX("data_fpix") 

    data_histogram_ias2d = dataRoot_file.Get("ias_pt_regionAll_testIhPt")
    data_histogram_ias = data_histogram_ias2d.ProjectionY("data_gstrip") 


    data_mean_TOF = data_histogram_tof.GetMean()
    sig_mean_TOF = sig_histogram_tof.GetMean()
    data_stddev_TOF = data_histogram_tof.GetStdDev()
    sig_stddev_TOF = sig_histogram_tof.GetStdDev()

    sig_histogram_mass.Rebin(8) 
    sig_histogram_tof.Rebin(1)
    sig_histogram_ih.Rebin(8)
    sig_histogram_ias.Rebin(40)

    sig_histogram_mass_glu.Rebin(8) 
    #sig_histogram_tof_glu.Rebin(1)
    sig_histogram_ih_glu.Rebin(8)
    sig_histogram_ias_glu.Rebin(40)

    data_histogram_mass.Rebin(8) 
    data_histogram_tof.Rebin(1)
    data_histogram_ih.Rebin(8)
    data_histogram_ias.Rebin(40)

    sig_histogram_tof=histoStyle(sig_histogram_tof,color_signal)
    sig_histogram_mass=histoStyle(sig_histogram_mass,color_signal)
    sig_histogram_ih=histoStyle(sig_histogram_ih,color_signal)
    sig_histogram_fpix=histoStyle(sig_histogram_fpix,color_signal)
    sig_histogram_ias=histoStyle(sig_histogram_ias,color_signal) 

    #sig_histogram_tof_glu=histoStyle(sig_histogram_tof_glu,color_signalGlu)
    sig_histogram_mass_glu=histoStyle(sig_histogram_mass_glu,color_signalGlu)
    sig_histogram_ih_glu=histoStyle(sig_histogram_ih_glu,color_signalGlu)
    sig_histogram_fpix_glu=histoStyle(sig_histogram_fpix_glu,color_signalGlu)
    sig_histogram_ias_glu = histoStyle(sig_histogram_ias_glu,color_signalGlu) 

    data_histogram_tof=histoStyle(data_histogram_tof,color_data)
    data_histogram_mass=histoStyle(data_histogram_mass,color_data)
    data_histogram_ih=histoStyle(data_histogram_ih,color_data)
    data_histogram_fpix=histoStyle(data_histogram_fpix,color_data)
    data_histogram_ias=histoStyle(data_histogram_ias,color_data)


    range_tof = getRangeGraph(sig_histogram_tof)
    range_ih = getRangeGraph(sig_histogram_ih)
    range_mass = getRangeGraph(sig_histogram_mass)
    range_fpix = getRangeGraph(sig_histogram_fpix)
    range_ias = getRangeGraph(sig_histogram_ias)

    sig_histogram_tof_glu = None 
    MakeCanvas("canvas_TOF",sig_histogram_tof,sig_histogram_tof_glu,data_histogram_tof,outdir+"TOF_distrib_stau_data",range_tof[0],range_tof[1],'$\\frac{1}{\\beta}')
    print("Data TOF MEAN = {}     /    Std dev = {}\nSignal TOF MEAN = {}      /       Std dev = {}".format(data_mean_TOF,data_stddev_TOF,sig_mean_TOF,sig_stddev_TOF)) 

    MakeCanvas("canvas_IH",sig_histogram_ih,sig_histogram_ih_glu,data_histogram_ih,outdir+"IH_distrib_stau_data",range_ih[0],range_ih[1],"IH [MeV/cm]")
    MakeCanvas("canvas_GSTRIP",sig_histogram_ias,sig_histogram_ias_glu,data_histogram_ias,outdir+"Gstrip_distrib_stau_data",range_ias[0],range_ias[1],"G^{strip}")
    MakeCanvas("canvas_FPIX",sig_histogram_fpix,sig_histogram_fpix_glu,data_histogram_fpix,outdir+"Fpix_distrib_stau_data",range_fpix[0],range_fpix[1],"F^{pix}")
    MakeCanvas("canvas_MASS",sig_histogram_mass,sig_histogram_mass_glu,data_histogram_mass,outdir+"Mass_distrib_stau_data",range_mass[0],range_mass[1],"Mass [GeV]")

if __name__ == "__main__":
    main()
