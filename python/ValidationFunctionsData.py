import os, sys, argparse, json, copy, array, numpy
import ROOT
import math
import CMS_lumi, tdrstyle
from ROOT import gROOT, gStyle, TFile, TH1F, TLegend, TCanvas, TPad, TLine, TH1D, TH2F, TPaveText, gPad, gDirectory,TGraphErrors

from SignalXS import crossSectionArray,sampleArray,nbEventsSig,sampleNames
from binningVar import *

CMS_lumi.lumi_sqrtS = "13 TeV"

#CMS_lumi.lumi_13TeV = "2018 - 59.7 fb^{-1}"
#CMS_lumi.lumi_13TeV = "2017 - 41.3 fb^{-1}"
CMS_lumi.lumi_13TeV = "2017+2018 - 101 fb^{-1}"

CMS_lumi.writeExtraText = True
CMS_lumi.extraText = "In progress"

iPos = 0

if( iPos==0 ): CMS_lumi.relPosX = 0.12
iPeriod = 4

def SaveMassPredScaledRootFile(file_name,predMass,outdir):
    root_file = ROOT.TFile.Open(file_name, "RECREATE")
    if not root_file or root_file.IsZombie():
        print("Error: Could not create the ROOT file.")
        return

    root_file.cd()
    predMass.Write()
    root_file.Close()


def get_pull_plot(hist1,hist2,minVal,maxVal):
    pull_plot = hist1.Clone("pull")
    pull_plot.Reset()

    if hist1.GetNbinsX() != hist2.GetNbinsX():
        print("Not the same number of bins in both histograms, exiting program..")
        exit()

    for i in range(hist1.FindBin(minVal),hist1.FindBin(maxVal)+1):        
        content1 = hist1.GetBinContent(i)
        content2 = hist2.GetBinContent(i)
        error1 = hist1.GetBinError(i)
        error2 = hist2.GetBinError(i)
        
        pull = 0
        if(error1 != 0 or error2 != 0):
            pull = (content1 - content2) / ROOT.TMath.Sqrt(error1**2 + error2**2)
        if(error1 == 0 and error2 == 0):
            pull = 0
        if content1 == 0 or content2 ==0:
            pull = 0

        pull_plot.SetBinContent(i,pull)
        #print("Filling pull plot at bin {} with {}".format(i,pull))

    return pull_plot

def get_ratio_plot(hist1,hist2,minVal,maxVal):
    ratio_plot = hist1.Clone("ratio")
    ratio_plot.Reset()

    if hist1.GetNbinsX() != hist2.GetNbinsX():
        print("Not the same number of bins in both histograms, exiting program..")
        exit()

    for i in range(hist1.FindBin(minVal),hist1.FindBin(maxVal)+1):        
        content1 = hist1.GetBinContent(i)
        content2 = hist2.GetBinContent(i)
        error1 = hist1.GetBinError(i)
        error2 = hist2.GetBinError(i)
        
        ratio_val = 0
        ratio_error = 0
        
        if(content2 != 0 and content1 !=0):
            ratio_val = content1/content2
            ratio_error = ratio_val * ROOT.TMath.Sqrt( (error1/content1)**2 + (error2/content2)**2)

        ratio_plot.SetBinContent(i,ratio_val)
        ratio_plot.SetBinError(i,ratio_error)

    ratio_plot.SetMarkerStyle(20)
    return ratio_plot


def MakeCanvas(h1,h2,namecanv,namereg1,namereg2,xmin,xmax,outdir):
    frameR = h1.Clone("FRAME_RATIO")
    frameR.Reset()
    frameP = h1.Clone("FRAME_PULL")
    frameP.Reset()

    canvas = ROOT.TCanvas(namecanv, namecanv, 900, 1200)
    pad1 = ROOT.TPad("pad1", "Histograms", 0, 0.42, 1, 0.92)
    pad1.SetLogy(1)
    pad1.Draw()
    pad1.cd()
    h1.SetStats(False)
    h1.GetYaxis().SetRangeUser(0.001,1000000)
    h1.GetXaxis().SetRangeUser(xmin,xmax)
    h2.GetYaxis().SetRangeUser(0.001,1000000)
    h2.GetXaxis().SetRangeUser(xmin,xmax)
    h1.SetMarkerStyle(20)
    h1.SetMarkerSize(0.7)
    h1.SetMarkerColor(2)
    h2.SetStats(False)
    h2.SetMarkerStyle(20)
    h2.SetMarkerSize(0.7)
    h2.SetMarkerColor(1)

    h1.Draw("SAME P")
    h2.Draw("SAME P")


    lej = ROOT.TLegend(0.6,0.6,0.8,0.9)
    lej.SetFillStyle(0)
    lej.SetBorderSize(0)
    lej.SetTextFont(43)
    lej.SetTextSize(13)
    lej.AddEntry(h1, namereg1,"PE1")
    lej.AddEntry(h2, namereg2,"PE1")
    lej.Draw("SAME")

    canvas.cd() 
    pad2 = ROOT.TPad("pad2", "Ratio", 0, 0.26, 1, 0.41)
    pad2.Draw()
    pad2.cd()
    frameR = SetFrameColor(frameR,510,510,510,0,"","","#frac{scaled CR}{CR}",0,2.5,43,12,43,14,43,10,43,20,3.5,1.75)
    frameR.Draw("SAME AXIS")
    ratioPlot = get_ratio_plot(h1,h2,xmin,xmax)
    ratioPlot.SetStats(False)
    ratioPlot.SetMarkerStyle(20)
    ratioPlot.SetMarkerSize(0.7)
    ratioPlot.SetMarkerColor(1)
    ratioPlot.Draw("SAME EP")
    lineOne = ROOT.TLine(0, 1, xmax, 1)
    lineOne.SetLineColor(2)
    lineOne.SetLineStyle(2)
    lineOne.Draw("SAME")
    lineAb = ROOT.TLine(0,0.8, xmax,0.8)
    lineAb.SetLineColor(40)
    lineAb.SetLineStyle(2)
    lineAb.Draw("SAME")
    lineAb2 = ROOT.TLine(0,1.2, xmax,1.2)
    lineAb2.SetLineColor(40)
    lineAb2.SetLineStyle(2)
    lineAb2.Draw("SAME")

    canvas.cd() 
    pad3 = ROOT.TPad("pad3", "Pulls", 0, 0.10, 1, 0.25)
    pad3.Draw()
    pad3.cd()
    frameP = SetFrameColor(frameP,510,510,510,0,"","","#frac{scaled CR}{CR}",-3.5,3.5,43,12,43,14,43,10,43,20,3.5,1.75)
    frameP.Draw("SAME AXIS")
    pullPlot = get_pull_plot(h1,h2,xmin,xmax)
    pullPlot.GetYaxis().SetRangeUser(-3.5,3.5)
    pullPlot.SetStats(False)
    pullPlot.SetMarkerStyle(20)
    pullPlot.SetMarkerSize(0.7)
    pullPlot.SetFillColor(38)
    pullPlot.Draw("SAME HIST")
    lineP0 = ROOT.TLine(0, 0, xmax, 0)
    lineP0.SetLineColor(2)
    lineP0.SetLineStyle(2)
    lineP0.Draw("SAME")
    lineP1 = ROOT.TLine(0, 1, xmax, 1)
    lineP1.SetLineColor(40)
    lineP1.SetLineStyle(2)
    lineP1.Draw("SAME")
    lineP2 = ROOT.TLine(0,2, xmax,2)
    lineP2.SetLineColor(40)
    lineP2.SetLineStyle(2)
    lineP2.Draw("SAME")
    lineP3 = ROOT.TLine(0,3, xmax,3)
    lineP3.SetLineColor(40)
    lineP3.SetLineStyle(2)
    lineP3.Draw("SAME")
    lineP4 = ROOT.TLine(0,-1, xmax,-1)
    lineP4.SetLineColor(40)
    lineP4.SetLineStyle(2)
    lineP4.Draw("SAME")
    lineP5 = ROOT.TLine(0,-2, xmax,-2)
    lineP5.SetLineColor(40)
    lineP5.SetLineStyle(2)
    lineP5.Draw("SAME")
    lineP6 = ROOT.TLine(0,-3, xmax,-3)
    lineP6.SetLineColor(40)
    lineP6.SetLineStyle(2)
    lineP6.Draw("SAME")

    CMS_lumi.CMS_lumi(canvas, iPeriod, iPos)
    namePng = namecanv+".png"
    nameRoot = namecanv+".root"
    canvas.SaveAs(namePng)
    canvas.SaveAs(nameRoot)

def SignalRegion(h1,h2,name,maxDisplay,outdir,blind,maxBlind,stau308,stau432,stau557,stau651):

    hist1 = h1.Clone(h1.GetName())
    hist2 = h2.Clone(h2.GetName())


    setColorAndMarker(stau308,42,41,1)
    setColorAndMarker(stau432,45,33,1)
    setColorAndMarker(stau557,33,23,1)
    setColorAndMarker(stau651,36,39,1)

    if(blind):
        #hist2 = BlindHist(hist2)
        hist2 = BlindHist(hist2,int(maxBlind))    

    hist1.GetXaxis().SetRangeUser(0,maxDisplay)
    hist2.GetXaxis().SetRangeUser(0,maxDisplay)

    print("Hist 1 has {} nbins, an hist  2 has {}".format(hist1.GetNbinsX(),hist2.GetNbinsX()))

    print("Making hists/ratio/pull for histograms ", hist1.GetName(), " and " , hist2.GetName())
    setColorAndMarkerNoFill(hist1,1,20,0.7)
    setColorAndMarkerNoFill(hist2,2,20,0.7)
  
    pull_graph = hist1.Clone("pulls_hist")
    pull_graph.GetXaxis().SetRangeUser(0,maxBlind)
    pull_graph.Reset()

    ratio_graph = hist1.Clone("ratio_hist")
    ratio_graph.GetXaxis().SetRangeUser(0,maxBlind)
    ratio_graph.Reset()


    ratio_itg = hist1.Clone("ratio_integrals")
    ratio_itg.GetXaxis().SetRangeUser(0,maxBlind)
    ratio_itg.Reset()

    for i in range(1,hist1.FindBin(maxBlind)+1):
        err1 = ROOT.Double(0)
        err2 = ROOT.Double(0)
        
        it1 = hist1.IntegralAndError(i,hist1.FindBin(maxBlind),err2)
        it2 = hist2.IntegralAndError(i,hist2.FindBin(maxBlind),err2)
        itg_ratio = 0
        itg_err_ratio = 0

        if(it1!=0 and it2!=0):
            itg_ratio = it1/it2
            itg_err_ratio = itg_ratio * math.sqrt( ((err1/it1)**2) + ((err2/it2)**2))

        if(it1!=0 and it2==0):
            itg_err_ratio = 0

        ratio_itg.SetBinContent(i,itg_ratio)
        ratio_itg.SetBinError(i,itg_err_ratio)


        content1 = hist1.GetBinContent(i)
        content2 = hist2.GetBinContent(i)
        error1 = hist1.GetBinError(i)
        error2 = hist2.GetBinError(i)
        
        ratio_val = 0
        ratio_error = 0
        
        if(content2 != 0 and content1 !=0):
            ratio_val = content1/content2
            ratio_error = ratio_val * ROOT.TMath.Sqrt( (error1/content1)**2 + (error2/content2)**2)

        ratio_graph.SetBinContent(i,ratio_val)
        ratio_graph.SetBinError(i,ratio_error)


        if(error1 != 0 or error2 != 0):
            pull = (content1 - content2) / ROOT.TMath.Sqrt(error1**2 + error2**2)
        if(error1 == 0 and error2 == 0):
            pull = 0

        pull_graph.SetBinContent(i,pull)

    
    max_entries = hist1.GetMaximum()*100
    min_entries = 0.00001

    hist1.GetYaxis().SetRangeUser(min_entries,max_entries)

    # Calculate chi2/ndf
    '''
    chi2, ndf = hist1.Chi2Test(hist2, "WU"), hist1.GetNbinsX() - 1
    print("Chi2 / NDF  = {}/{}".format(chi2,ndf))
    chi2ndf = chi2 / ndf
    chi2ndf_string = "Chi2/NDF = {:.2f}".format(chi2ndf)
    '''

    cv = TCanvas(name, name, 800, 800)
    print("After canvas")
    padhc1 = TPad("pad1", "Histograms", 0, 0.5, 1, 0.95)
    padhc1.SetLogy(1)
    padhc1.Draw()
    padhc1.cd()

    hist1.SetLineColor(1)
    hist2.SetLineColor(2)
    hist1.Draw("SAME E1")
    hist2.Draw("SAME E1")
    stau308.Draw("SAME P")
    stau432.Draw("SAME P")
    stau557.Draw("SAME P")
    stau651.Draw("SAME P")

    lej = TLegend(0.6,0.9,0.4,0.7)
    lej.SetFillStyle(0)
    lej.SetBorderSize(0)
    lej.SetTextFont(43)
    lej.SetTextSize(13)

    lej.AddEntry(hist1, "CR","PE1")
    srBlind = "SR - Blind > " + str(maxBlind)
    lej.AddEntry(hist2, srBlind ,"P")
    #lej.AddEntry("", chi2ndf_string, "")
    lej.AddEntry(stau308, "pair. #tilde{#tau} (M=308 GeV)","PE1")

    lej.Draw("same")
    print("After legend")

    lej2 = TLegend(0.8,0.9,0.6,0.7)
    lej2.SetFillStyle(0)
    lej2.SetBorderSize(0)
    lej2.SetTextFont(43)
    lej2.SetTextSize(13)

    lej2.AddEntry(stau432, "pair. #tilde{#tau} (M=432 GeV)","PE1")
    lej2.AddEntry(stau557, "pair. #tilde{#tau} (M=557 GeV)","PE1")
    lej2.AddEntry(stau651, "pair. #tilde{#tau} (M=651 GeV)","PE1")
    lej2.Draw("same")

    LineLastBin=TLine(maxBlind,min_entries,maxBlind,max_entries)
    LineLastBin.SetLineColor(1)
    LineLastBin.SetLineStyle(1)
    LineLastBin.Draw("same")
    print("After legend 2")

    cv.cd() 

    paditg = TPad("padintegral", "RatioIntegral", 0, 0.34, 1, 0.46)
    paditg.Draw()
    paditg.cd()

    frameR3 = ROOT.TH1D("frameR3", "frameR3",nxmass,xbinMass)
    frameR3 = SetFrameColor(frameR3,510,510,510,0,"","","Integral ratio-R",0,2.5,43,12,43,14,43,10,43,20,3.5,1.75)
    frameR3.GetXaxis().SetRangeUser(0,maxDisplay)
    frameR3.Draw("SAME AXIS")

    print("After frame R3")

    ratio_itg.SetMarkerStyle(20)
    ratio_itg.Draw("SAME EP")

    lineitg1 = ROOT.TLine(0, 1,maxDisplay, 1)
    lineitg1.SetLineColor(2)
    lineitg1.SetLineStyle(4)

    lineitg1.Draw("SAME")

    lineitg2 = ROOT.TLine(0, 0.8, maxDisplay, 0.8)
    lineitg2.SetLineColor(1)
    lineitg2.SetLineStyle(4)
    lineitg2.Draw("SAME")

    lineitg3 = ROOT.TLine(0, 1.2, maxDisplay, 1.2)
    lineitg3.SetLineColor(1)
    lineitg3.SetLineStyle(4)
    lineitg3.Draw("SAME")
   
    LineLastBinitg=TLine(maxBlind,0,maxBlind,2.5)
    LineLastBinitg.SetLineColor(1)
    LineLastBinitg.SetLineStyle(4)
    LineLastBinitg.Draw("same")

    print("After paditg")
    cv.cd()
    padhc2 = TPad("pad2", "Ratio", 0, 0.2, 1, 0.32)
    padhc2.Draw()
    padhc2.cd()


    frameR2 = ROOT.TH1D("frameR2", "frameR2",nxmass,xbinMass)
    frameR2 = SetFrameColor(frameR2,510,510,510,0,"","","#frac{data}{fit}",0,2.5,43,12,43,14,43,10,43,20,3.5,1.75)
    frameR2.GetXaxis().SetRangeUser(0,maxDisplay)
    frameR2.Draw("SAME AXIS")

    ratio_graph.SetMarkerStyle(20)
    ratio_graph.Draw("SAME AP")
    linehc1 = ROOT.TLine(0, 1,maxDisplay, 1)
    SetLineStyle(linehc1,2,4)
    linehc1.Draw("SAME")

    linehc2 = ROOT.TLine(0, 0.8, maxDisplay, 0.8)
    SetLineStyle(linehc2,1,4)
    linehc2.Draw("SAME")

    linehc3 = ROOT.TLine(0, 1.2, maxDisplay, 1.2)
    SetLineStyle(linehc3,1,4)
    linehc3.Draw("SAME")
   
    LineLastBinRatio=TLine(maxBlind,0,maxBlind,2.5)
    SetLineStyle(LineLastBinRatio,1,3)
    LineLastBinRatio.Draw("same")
    print("After pad ratio")

    cv.cd()

    padhc3 = TPad("pad3", "Pull", 0, 0.12, 1, 0.2)
    padhc3.Draw()
    padhc3.cd()
    frameR=ROOT.TH1D("frameR", "frameR", nxmass,xbinMass)

    frameR = SetFrameColor(frameR,510,510,510,0,"","","#frac{data-fit}{#sigma_{stat}}",-3.5,3.5,43,12,43,14,43,10,43,14,5,1.75)
    frameR.GetXaxis().SetRangeUser(0,maxDisplay)
    frameR.Draw("SAME AXIS")

    pull_graph.SetMarkerStyle(20)
    pull_graph.SetFillColor(38)
    pull_graph.Draw("SAME HIST")

    linehp1 = ROOT.TLine(0, 0,maxDisplay, 0)
    SetLineStyle(linehp1,2,4)

    linehp2 = ROOT.TLine(0, 1, maxDisplay, 1)
    SetLineStyle(linehp2,1,4)

    linehp2b = ROOT.TLine(0, -1, maxDisplay, -1)
    SetLineStyle(linehp2b,1,4)

    linehp3 = ROOT.TLine(0, 2, maxDisplay, 2)
    SetLineStyle(linehp3,1,4)

    linehp3b = ROOT.TLine(0, -2, maxDisplay, -2)
    SetLineStyle(linehp3b,1,4)

    linehp1.Draw("SAME")
    linehp2.Draw("SAME")
    linehp2b.Draw("SAME")
    linehp3.Draw("SAME")
    linehp3b.Draw("SAME")
    LineLastBinPull=TLine(maxBlind,-3.5,maxBlind,3.5)
    LineLastBinPull.SetLineColor(1)
    LineLastBinPull.SetLineStyle(3)
    LineLastBinPull.Draw("same")
    print("After last line, before CMS Canv")
    cv.cd()
    CMS_lumi.CMS_lumi(cv, iPeriod, iPos)
    #CMS_lumi.CMS_lumi(cv, iPeriod, iPos)
    #cv.Update()
    #cv.Draw()
    nameRoot = outdir + '/' + name + '.root'
    namePdf = outdir + '/' + name + '.pdf'
    cv.SaveAs(nameRoot)
    cv.SaveAs(namePdf)

def CompareRegions(h1,h2,allSigContam,name,maxDisplay,outdir,blind,addContamination):

    hist1 = h1.Clone(h1.GetName())
    hist2 = h2.Clone(h2.GetName())

    '''
    if(blind):
        hist2 = BlindHist(hist2)    
    '''

    if(addContamination):
        setColorAndMarker(allSigContam[9],42,41,1)
        setColorAndMarker(allSigContam[10],45,33,1)
        setColorAndMarker(allSigContam[11],33,23,1)
        setColorAndMarker(allSigContam[12],36,39,1)

    hist1.GetXaxis().SetRangeUser(0,maxDisplay)
    hist2.GetXaxis().SetRangeUser(0,maxDisplay)

    print("Hist 1 has {} nbins, an hist  2 has {}".format(hist1.GetNbinsX(),hist2.GetNbinsX()))

    print("Making hists/ratio/pull for histograms ", hist1.GetName(), " and " , hist2.GetName())
    setColorAndMarkerNoFill(hist1,1,20,0.7)
    setColorAndMarkerNoFill(hist2,2,20,0.7)
  
    pull_graph = hist1.Clone("pulls_hist")
    pull_graph.GetXaxis().SetRangeUser(0,maxDisplay)
    pull_graph.Reset()

    ratio_graph = hist1.Clone("ratio_hist")
    ratio_graph.GetXaxis().SetRangeUser(0,maxDisplay)
    ratio_graph.Reset()


    ratio_itg = hist1.Clone("ratio_integrals")
    ratio_itg.GetXaxis().SetRangeUser(0,maxDisplay)
    ratio_itg.Reset()

    for i in range(1,hist1.GetNbinsX()+1):
        err1 = ROOT.Double(0)
        err2 = ROOT.Double(0)
        
        it1 = hist1.IntegralAndError(i,hist1.GetNbinsX()+1,err2)
        it2 = hist2.IntegralAndError(i,hist2.GetNbinsX()+1,err2)

        itg_ratio = 0
        itg_err_ratio = 0

        if(it1!=0 and it2!=0):
            itg_ratio = it1/it2
            itg_err_ratio = itg_ratio * math.sqrt( ((err1/it1)**2) + ((err2/it2)**2))

        if(it1!=0 and it2==0):
            itg_err_ratio = 0

        ratio_itg.SetBinContent(i,itg_ratio)
        ratio_itg.SetBinError(i,itg_err_ratio)


        content1 = hist1.GetBinContent(i)
        content2 = hist2.GetBinContent(i)
        error1 = hist1.GetBinError(i)
        error2 = hist2.GetBinError(i)
        
        ratio_val = 0
        ratio_error = 0
        
        if(content2 != 0 and content1 !=0):
            ratio_val = content1/content2
            ratio_error = ratio_val * ROOT.TMath.Sqrt( (error1/content1)**2 + (error2/content2)**2)

        ratio_graph.SetBinContent(i,ratio_val)
        ratio_graph.SetBinError(i,ratio_error)


        if(error1 != 0 or error2 != 0):
            pull = (content1 - content2) / ROOT.TMath.Sqrt(error1**2 + error2**2)
        if(error1 == 0 and error2 == 0):
            pull = 0

        pull_graph.SetBinContent(i,pull)

    
    max_entries = hist1.GetMaximum()*100
    min_entries = 0.001

    hist1.GetYaxis().SetRangeUser(min_entries,max_entries)

    '''
    # Calculate chi2/ndf
    chi2, ndf = hist1.Chi2Test(hist2, "WU"), hist1.GetNbinsX() - 1
    print("Chi2 / NDF  = {}/{}".format(chi2,ndf))
    chi2ndf = chi2 / ndf
    chi2ndf_string = "Chi2/NDF = {:.2f}".format(chi2ndf)
    '''

    print("Before canvas") 

    cv = TCanvas(name, name, 800, 800)

    padhc1 = TPad("pad1", "Histograms", 0, 0.5, 1, 0.95)
    padhc1.SetLogy(1)
    padhc1.Draw()
    padhc1.cd()

    hist1.SetLineColor(1)
    hist2.SetLineColor(2)
    hist1.Draw("SAME E1")
    hist2.Draw("SAME E1")
    if(addContamination):
        allSigContam[9].Draw("SAME P")
        allSigContam[10].Draw("SAME P")
        allSigContam[11].Draw("SAME P")
        allSigContam[12].Draw("SAME P")


        lej = TLegend(0.6,0.9,0.4,0.7)
        lej.SetFillStyle(0)
        lej.SetBorderSize(0)
        lej.SetTextFont(43)
        lej.SetTextSize(13)
    
        lej.AddEntry(hist1, hist1.GetName(),"PE1")
        lej.AddEntry(hist2, hist2.GetName() ,"P")
        lej.AddEntry(allSigContam[9], "pair. #tilde{#tau} (M=308 GeV)","PE1")
    
        lej.Draw("same")
    
        lej2 = TLegend(0.8,0.9,0.6,0.7)
        lej2.SetFillStyle(0)
        lej2.SetBorderSize(0)
        lej2.SetTextFont(43)
        lej2.SetTextSize(13)
    
        lej2.AddEntry(allSigContam[10], "pair. #tilde{#tau} (M=432 GeV)","PE1")
        lej2.AddEntry(allSigContam[11], "pair. #tilde{#tau} (M=557 GeV)","PE1")
        lej2.AddEntry(allSigContam[12], "pair. #tilde{#tau} (M=651 GeV)","PE1")
        lej2.Draw("same")
    

    else:
        lej = TLegend(0.7,0.9,0.5,0.7)
        lej.SetFillStyle(0)
        lej.SetBorderSize(0)
        lej.SetTextFont(43)
        lej.SetTextSize(13)

        lej.AddEntry(hist1,hist1.GetName() ,"P")
        lej.AddEntry(hist2, hist2.GetName() ,"P")
        #lej.AddEntry("", chi2ndf_string, "")
        lej.Draw("same")
   
    cv.cd() 

    print("Before canvas itg") 
    paditg = TPad("paditg", "RatioIntegral", 0, 0.34, 1, 0.46)
    paditg.Draw()
    paditg.cd()

    frameR3 = ROOT.TH1D("frameR3", "frameR3",nxmass,xbinMass)
    frameR3 = SetFrameColor(frameR3,510,510,510,0,"","","Integral ratio-R",0,2.5,43,12,43,14,43,10,43,20,3.5,1.75)
    frameR3.GetXaxis().SetRangeUser(0,maxDisplay)
    frameR3.Draw("SAME AXIS")

    ratio_itg.SetMarkerStyle(20)
    ratio_itg.Draw("SAME EP")

    lineitg1 = ROOT.TLine(0, 1,maxDisplay, 1)

    lineitg1.SetLineColor(2)
    lineitg1.SetLineStyle(4)

    lineitg1.Draw("SAME")

    lineitg2 = ROOT.TLine(0, 0.8, maxDisplay, 0.8)
    lineitg2.SetLineColor(2)
    lineitg2.SetLineStyle(4)
    lineitg2.Draw("SAME")

    lineitg3 = ROOT.TLine(0, 1.2, maxDisplay, 1.2)

    lineitg3.SetLineColor(1)
    lineitg3.SetLineStyle(4)
    lineitg3.Draw("SAME")


    cv.cd()

    print("Before canvasRatio") 
    padhc2 = TPad("pad2", "Ratio", 0, 0.2, 1, 0.32)
    padhc2.Draw()
    padhc2.cd()


    frameR2 = ROOT.TH1D("frameR2", "frameR2",nxmass,xbinMass)
    frameR2 = SetFrameColor(frameR2,510,510,510,0,"","","#frac{data}{fit}",0,2.5,43,12,43,14,43,10,43,20,3.5,1.75)
    frameR2.GetXaxis().SetRangeUser(0,maxDisplay)
    frameR2.Draw("SAME AXIS")

    ratio_graph.SetMarkerStyle(20)
    ratio_graph.Draw("SAME AP")
    linehc1 = ROOT.TLine(0, 1,maxDisplay, 1)
    linehc1.SetLineColor(2)
    linehc1.SetLineStyle(4)
    linehc1.Draw("SAME")

    linehc2 = ROOT.TLine(0, 0.8, maxDisplay, 0.8)
    linehc2.SetLineColor(1)
    linehc2.SetLineStyle(4)
    linehc2.Draw("SAME")

    linehc3 = ROOT.TLine(0, 1.2, maxDisplay, 1.2)
    linehc3.SetLineColor(1)
    linehc3.SetLineStyle(4)
    linehc3.Draw("SAME")

    cv.cd()

    print("Before canvas Pull") 

    padhc3 = TPad("pad3", "Pull", 0, 0.12, 1, 0.2)
    padhc3.Draw()
    padhc3.cd()
    print("Created canvas pull") 

    frameR=ROOT.TH1D("frameR", "frameR", nxmass,xbinMass)
    frameR = SetFrameColor(frameR,510,510,510,0,"","","#frac{data-fit}{#sigma_{stat}}",-3.5,3.5,43,12,43,14,43,10,43,14,5,1.75)
    frameR.GetXaxis().SetRangeUser(0,maxDisplay)
    frameR.Draw("SAME AXIS")

    pull_graph.SetMarkerStyle(20)
    pull_graph.SetFillColor(38)
    pull_graph.Draw("SAME HIST")

    linehp1 = ROOT.TLine(0, 0,maxDisplay, 0)
    linehp1.SetLineColor(2)
    linehp1.SetLineStyle(4)
    SetLineStyle(linehp1,2,4)

    linehp2 = ROOT.TLine(0, 1, maxDisplay, 1)
    linehp2.SetLineColor(1)
    linehp2.SetLineStyle(4)


    linehp2b = ROOT.TLine(0, -1, maxDisplay, -1)
    linehp2b.SetLineColor(1)
    linehp2b.SetLineStyle(4)


    linehp3 = ROOT.TLine(0, 2, maxDisplay, 2)
    linehp3.SetLineColor(1)
    linehp3.SetLineStyle(4)


    linehp3b = ROOT.TLine(0, -2, maxDisplay, -2)
    linehp3b.SetLineColor(1)
    linehp3b.SetLineStyle(4)

    linehp1.Draw("SAME")
    linehp2.Draw("SAME")
    linehp2b.Draw("SAME")
    linehp3.Draw("SAME")
    linehp3b.Draw("SAME")

    print("before last canvas.cd()") 
    cv.cd()
    print("After canvas pull") 
    CMS_lumi.CMS_lumi(cv, iPeriod, iPos)
    #cv.Update()
    #cv.Draw()
    nameRoot = outdir + '/' + name + '.root'
    namePdf = outdir + '/' + name + '.pdf'
    cv.SaveAs(nameRoot)
    cv.SaveAs(namePdf)
    print("End function HistCanvases")



def DisplayAllRegions(regions,name,maxDisplay,outdir):

    vReg = [region.Clone() for region in regions]
    
    for i in range(len(vReg)):
        vReg[i] = renorm(vReg[i])   
        setColorAndMarker(vReg[i],i+1,20,0.6)
        vReg[i].GetXaxis().SetRangeUser(0,maxDisplay)
    
    
    max_entries = 1.5
    min_entries = 0.0000001

    vReg[0].GetYaxis().SetRangeUser(min_entries,max_entries)
    cv = TCanvas(name, name, 800, 800)

    padhc1 = TPad("pad1", "Histograms", 0, 0.2, 1, 0.95)
    padhc1.SetLogy(1)
    padhc1.Draw()
    padhc1.cd()
    for i in range(len(vReg)):
        vReg[i].Draw("SAME E1")

    lej = TLegend(0.7,0.9,0.5,0.7)
    lej.SetFillStyle(0)
    lej.SetBorderSize(0)
    lej.SetTextFont(43)
    lej.SetTextSize(13)
    for i in range(len(vReg)):
        lej.AddEntry(vReg[i],vReg[i].GetName() ,"P")
       
    lej.Draw("same")
    cv.cd()   
    CMS_lumi.CMS_lumi(cv, iPeriod, iPos)

    nameRoot = outdir + '/' + name + '.root'
    namePdf = outdir + '/' + name + '.pdf'
    cv.SaveAs(nameRoot)
    cv.SaveAs(namePdf)




def SetFrameColor(frame,ndiv,xndiv,yndiv,stats,title,xtitle,ytitle,minf,maxf,ylabelfont,ylabelsize,ytitlefont,ytitlesize,xlabelfont,xlabelsize,xtitlefont,xtitlesize,xoffset,yoffset):
    
    #General Frame
    tmpFrame = frame.Clone()
    tmpFrame.SetTitle(title)
    tmpFrame.SetStats(stats) 
    tmpFrame.SetMaximum(maxf)
    tmpFrame.SetMinimum(minf)

    #Y axis
    tmpFrame.GetYaxis().SetTitle(ytitle)
    tmpFrame.GetYaxis().SetLabelFont(ylabelfont)
    tmpFrame.GetYaxis().SetLabelSize(ylabelsize)
    tmpFrame.GetYaxis().SetTitleFont(ytitlefont) 
    tmpFrame.GetYaxis().SetTitleSize(ytitlesize)
    tmpFrame.GetYaxis().SetTitleOffset(yoffset)
    tmpFrame.GetYaxis().SetNdivisions(yndiv)
    #X axis
    tmpFrame.GetXaxis().SetTitle(xtitle)
    tmpFrame.GetXaxis().SetNdivisions(ndiv)
    tmpFrame.GetXaxis().SetLabelFont(xlabelfont)
    tmpFrame.GetXaxis().SetLabelSize(xlabelsize)
    tmpFrame.GetXaxis().SetTitleFont(xtitlefont) 
    tmpFrame.GetXaxis().SetTitleSize(xtitlesize)
    tmpFrame.GetXaxis().SetTitleOffset(xoffset)

    return tmpFrame

def setColorAndMarkerNoFill(h1,color,markerstyle,markersize):
    h1.SetLineColor(color)
    h1.SetMarkerColor(color)
    h1.SetMarkerStyle(markerstyle)
    h1.SetMarkerSize(markersize)

def setColorAndMarker(h1,color,markerstyle,markersize):
    h1.SetLineColor(color)
    h1.SetMarkerColor(color)
    h1.SetFillColor(color)
    h1.SetMarkerStyle(markerstyle)
    h1.SetMarkerSize(markersize)

def SetLineStyle(line,color,style):
    line.SetLineColor(color)
    line.SetLineStyle(style)

def scale_histogram_with_poissonian_errors(histogram, scale_factor):
    # Loop over the histogram bins
    for i in range(1, histogram.GetNbinsX() + 1):
        # Scale the bin content
        histogram.SetBinContent(i, histogram.GetBinContent(i) * scale_factor)

        # Scale the bin error while preserving Poissonian nature
        bin_error = histogram.GetBinError(i)
        scaled_error = bin_error * scale_factor
        ''' 
        if bin_error > 0.0:
            scaled_error = scaled_error / bin_error  # Preserve the relative error
        '''
        histogram.SetBinError(i, scaled_error)


def makeOutputForDataCards(infile_path,output_file_path,SF):

    with open(infile_path, "r") as file:
        lines = file.readlines()

    numbers = []
    for line in lines:
        line = line.strip()  # Remove leading/trailing whitespaces and newline characters
        if line.startswith("#") or not line:  # Skip empty lines and lines starting with "#"
            continue
        modified_line = line[11:]
        val = int(modified_line)/SF
        numbers.append(val)

    with open(output_file_path, "w") as output_file:
        for p in range(len(numbers)):
            output_file.write(str(numbers[p])+"\n")


def RescaleSignals(size,year,region,regspec):

    print("#" * 40) 
    print("In the ", region, " region ",regspec , ", here are the weights for signals samples")
    print("#" * 40) 
    weights = []


    lumi = ROOT.Double(0.0)
    print("Year = ", year)
    if year == '2017':
        lumi = 41300.0
    elif year == '2018':
        lumi = 59700.0
    elif year == '2017and2018':
        lumi = 101000.0

    for index in range(size):
        #print("Will scale with name = " ,sampleArray[index] , " , entries =" , nbEventsSig[index] , ", lumi  = ", lumi, " , cross section = ",crossSectionArray.get(sampleArray[index]))
        name = sampleNames[index]
        entries = nbEventsSig[index]
        tmpWeight = lumi*crossSectionArray.get(sampleArray[index])/entries
        print("Name = " , name, " has " , entries , " entries and the weight = ", lumi, "*",crossSectionArray.get(sampleArray[index]),"/",entries, " = ", tmpWeight )
        weights.append(tmpWeight)

    return weights

def overflowInLastBin(h):
    res=h.Clone()
    res.SetBinContent(h.GetNbinsX(),h.GetBinContent(h.GetNbinsX())+h.GetBinContent(h.GetNbinsX()+1))
    res.SetBinContent(h.GetNbinsX()+1,0)
    res.SetBinError(res.GetNbinsX(),math.sqrt(res.GetBinContent(res.GetNbinsX())))
    res.SetBinError(res.GetNbinsX()+1,0)
    return res

def BlindHist(h,maxBlind):
    hist = h.Clone("blinded_hist")
    for i in range(hist.GetNbinsX()+1):
        if(hist.GetXaxis().GetBinLowEdge(i) >= maxBlind):
            hist.SetBinContent(i,0)
            hist.SetBinError(i,0)

    return hist


def cleanSingleHist(hist):
   h = hist.Clone()

   for bin in range(1, h.GetNbinsX() + 1):
       h.SetBinError(bin, 0.0)

   return h


def cleanHist(signals):
   cleaned_sigs = []
   for index,hist in enumerate(signals):
       for bin in range(1, hist.GetNbinsX() + 1):
           hist.SetBinError(bin, 0.0)
       cleaned_sigs.append(hist)
   return cleaned_sigs

def getHistogram(tfile, name, ref, normalize=False):

    h = tfile.Get(name)
    if not h:
        raise Exception("Failed to load histogram {0}.".format(name))
    h.SetName(name+ref)
    if normalize:
        N = h.GetEntries()
        nbinsx = h.GetNbinsX()
        xmin = h.GetXaxis().GetXmin()
        xmax = h.GetXaxis().GetXmax()
        hname = h.GetName()
        h_norm = TH1F(hname, hname, nbins, xmin, xmax)
        for i in range(1,nbins+1):
            y = h.GetBinContent(i)
            yy = float(y)/N
            h_norm.SetBinContent(i, yy)
        return h_norm

    return h

def renorm(h):
    hist = h.Clone()
    hist.Scale(1./hist.Integral())
    return hist
