import os
import ROOT
import sys

sys.path.append('../')

import CMS_lumi
ROOT.gROOT.SetBatch(ROOT.kTRUE)

CMS_lumi.lumi_13TeV = "2018 - 59.7 fb^{-1}"
CMS_lumi.writeExtraText = False
CMS_lumi.extraText = ""
CMS_lumi.cmsText = "Private work"
CMS_lumi.cmsTextFont = 52
CMS_lumi.lumiTextSize = 0.3
CMS_lumi.cmsTextSize  = 0.3

iPos = 0
if iPos == 0:
    CMS_lumi.relPosX = 0.12
iPeriod = 4


mass = "557"
sType = '\\tilde{\\tau} (M=' + mass + ' GeV)'

topMargin = 0.015

def getRangeGraph(hist):
    min_x = hist.GetXaxis().GetBinLowEdge(1)
    max_x = hist.GetXaxis().GetBinLowEdge(hist.GetNbinsX()+1)
    return (min_x,max_x)

def MakeCanvasFit(name,hist2d,output,min_X,max_X,x_axis_title):

    print("Will perform the double gaussian fit in [{},{}]".format(min_X,max_X))
    specRange = [i for i in range(7,hist2d.GetNbinsX()-3)]
    #specRange = [7]
    fit_results = [] 

    fit_means = []
    fit_means_errors = []
    fit_sigmas = []
    fit_sigmas_errors = []
    for i in specRange:
        plot = hist2d.ProjectionY("_px"+str(i-6),i,i)
        plot.GetXaxis().SetRangeUser(3,5)
        print(plot)
        renorm(plot)
        plot.SetStats(False)
        xmin = str(hist2d.GetXaxis().GetBinLowEdge(i))
        xmax = str(hist2d.GetXaxis().GetBinUpEdge(i))

        if i >= 16:
            plot.SetMarkerColor(i-5)
        else:
            plot.SetMarkerColor(i-6)

        plot.SetMarkerStyle(20)
        plot.SetMarkerSize(0.7)
        print("Fitting in range {} - {}".format(min_X, max_X))       

        fit_func = ROOT.TF1("gaussian_fits"+str(i), "[0]*exp(-0.5*((x-[1])/[2])*((x-[1])/[2])) + [3]*exp(-0.5*((x-[4])/[5])*((x-[4])/[5]))", min_X, max_X)

        print("Defined fit func 1")
        #fit_func.FixParameter(0,0.02)

        fit_func.SetParLimits(0,2e-3,2e-1)
        fit_func.SetParLimits(1,2.5,4)
        fit_func.SetParLimits(2,0.1,0.5)
        fit_func.SetParLimits(3,2.1e-3,2.1e-1)
        fit_func.SetParLimits(4,2.55,4)
        fit_func.SetParLimits(5,0.11,0.51)

        print("Set par limits")
        plot.Fit(fit_func, "R")
        print("fitted plot hist")
        fit = plot.GetFunction("gaussian_fits"+str(i))
        chi2 = fit.GetChisquare()
        ndof = fit.GetNDF()
        prob = fit.GetProb()
        latex = ROOT.TLatex()
        latex.SetNDC()
        latex.SetTextAngle(0)
        latex.SetTextColor(2)
        latex.SetTextFont(52)
        latex.SetTextAlign(11)
        latex.SetTextSize(0.04)
        legMu = '#chi^{2}/N_{dof} = ' + '{:.1f}'.format(chi2) + '/ {:.1f}'.format(ndof) +' = '

        print(fit_func.GetParameter(0))
        print(fit_func)
        print("Fpixels in {} - {}".format(xmin,xmax))
        nbPar = fit_func.GetNpar()
        fit_params = fit_func.GetParameters()
        fit_errors = fit_func.GetParErrors()
        for k in range(nbPar):
            param_value = fit_params[k]
            print("Parameter {} = : {} +/- {}".format(k,param_value,fit_func.GetParError(k)))

        
        fit_results.append({
            "F^pix": "F^{pix}" + xmin + "-" + xmax,
            "Chi2": fit_func.GetChisquare(),
            "NDF": fit_func.GetNDF(),
            "Mean": fit_func.GetParameter(1),
            "Mean_error": fit_func.GetParError(1),
            "Sigma": fit_func.GetParameter(2),
            "Sigma_error": fit_func.GetParError(2)
        })
        fit_means.append(fit_func.GetParameter(1))
        fit_means_errors.append(fit_func.GetParError(1))
        fit_sigmas.append(fit_func.GetParameter(2))
        fit_sigmas_errors.append(fit_func.GetParError(2))

        canvas = ROOT.TCanvas(name+'_fit'+str(i), name+'_fit'+str(i), 600, 600)
        canvas.SetLogy(1)   
        legend = ROOT.TLegend(0.5, 0.5, 0.84, 0.8)
        legend.AddEntry(fit, "Summed gaussians fit", "l")
        pad1 = ROOT.TPad("histograms"+str(i), "Histograms"+str(i), 0, 0.1, 1, 0.92)
        pad1.Draw()
        pad1.cd()

        pad1.SetTopMargin(topMargin)
        plot.SetMarkerColor(1)
        plot.Draw("SAME P")
        legend.AddEntry(plot,"F^{pixels} ["+xmin+"-"+xmax+']', "PE1")
        latex.DrawLatex(0.5, 0.36, legMu + '{:.2f}'.format(chi2/ndof))

        legend.Draw("SAME")
        canvas.cd()
    
        CMS_lumi.CMS_lumi(canvas, iPeriod, iPos)
        namepng = 'Fits_ih/'+output+str(i)+'.png'
        namepdf = 'Fits_ih/'+output+str(i)+'.pdf'
        nameroot = 'Fits_ih/'+output+str(i)+'.root'
        canvas.SaveAs(namepng)
        canvas.SaveAs(namepdf)
        canvas.SaveAs(nameroot)


    name2 = 'Fits_ih/'+output + "_means"
    canvas2 = ROOT.TCanvas(name2, name2, 600, 600)
    legend2 = ROOT.TLegend(0.15, 0.75, 0.4, 0.95)
    pad1 = ROOT.TPad("Std devs", "Std devs", 0, 0.1, 1, 0.92)
    pad1.Draw()
    pad1.cd()

    pad1.SetTopMargin(topMargin)
    plot2x = hist2d.ProjectionX("_py_default",2,2)
    plot2x.Reset()
    plot2x.GetXaxis().SetRangeUser(0.3,0.8)
    for i in range(len(fit_means)):
        plot2x.SetBinContent(i+7,fit_means[i])
        plot2x.SetBinError(i+7,fit_means_errors[i])
        xmin = str(hist2d.GetXaxis().GetBinLowEdge(i))
        xmax = str(hist2d.GetXaxis().GetBinUpEdge(i))

        if i >= 16:
            plot2x.SetMarkerColor(i-5)
        else:
            plot2x.SetMarkerColor(i-6)
 
    legend2.AddEntry(plot2x,"#mu_{fit}", "PE1")
    plot2x.SetStats(False)
    plot2x.GetYaxis().SetRangeUser(3.1,3.4)
    plot2x.SetMarkerStyle(20)
    plot2x.SetMarkerSize(0.7)
    plot2x.SetMarkerColor(46)
    plot2x.SetLineColor(38)
    plot2x.Draw("SAME P")
    plot2x.GetXaxis().SetTitle("F^{pixels}")
    plot2x.GetXaxis().SetTitleSize(0.045)
    plot2x.GetXaxis().SetTitleOffset(1.1)

    plot2x.GetYaxis().SetTitle("#mu (gaussian fit)")
    plot2x.GetYaxis().SetTitleSize(0.045)
    plot2x.GetYaxis().SetTitleOffset(1.1)

    fit_range_min2 = 0.3
    fit_range_max2 = 0.8
    constant_func2 = ROOT.TF1("constant_func2", "[0]", fit_range_min2, fit_range_max2)
    fit_result2 = plot2x.Fit(constant_func2, "S")
    chi22 = constant_func2.GetChisquare()
    ndof2 = constant_func2.GetNDF()
    print("MEAN VALUES : Chi-squared value: ", chi22/ndof2) 

    legend2.AddEntry(constant_func2,"Fit","l")
    '''
    fit1 = ROOT.TF1("fit_func", "[0]", 0.3, 0.8)
    fit1.SetParameter(0,2.235)
    plot2x.Fit(fit1, "QRSL","",0.3,0.8)
    fit1.SetLineColor(2)
    fit1.SetLineWidth(1)
    fit1.Draw("same")
    fit_function = plot2x.GetFunction("fit_func")
    chi2 = fit_function.GetChisquare()
    ndof = fit_function.GetNDF()
    prob = fit_function.GetProb()
    print("Linear fit of mu has chi 2 = {}, ndof = {}".format(chi2,ndof))
    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(2)
    latex.SetTextFont(52)
    latex.SetTextAlign(11)
    latex.SetTextSize(0.04)
    legMu = '#chi^{2}/N_{dof} = '
    latex.DrawLatex(0.58, 0.26, legMu + '{:.2f}'.format(chi2/ndof))
    legend2.AddEntry(fit1, "Linear fit", "l")
    '''
    legend2.SetTextSize(0.06)
    legend2.Draw("SAME")
    canvas2.cd()
    
    CMS_lumi.CMS_lumi(canvas2, iPeriod, iPos)
    namepng = 'Fits/'+output+'_mean.png'
    namepdf = 'Fits/'+output+'_mean.pdf'
    nameroot = 'Fits/' + output+'_mean.root'
    canvas2.SaveAs(namepng)
    canvas2.SaveAs(namepdf)
    canvas2.SaveAs(nameroot)


    name3 = 'Fits/'+output + "_stddev"
    canvas3 = ROOT.TCanvas(name3, name3, 600, 600)
    legend3 = ROOT.TLegend(0.15, 0.75, 0.4, 0.95)
    pad2 = ROOT.TPad("Std devs", "Std devs", 0, 0.1, 1, 0.92)
    pad2.Draw()
    pad2.cd()

    pad2.SetTopMargin(topMargin)
    plot3x = hist2d.ProjectionX("_py_default_sigma",2,2)
    plot3x.Reset()
    plot3x.GetXaxis().SetRangeUser(0.3,0.8)
   
    for i in range(len(fit_means)):
        plot3x.SetBinContent(i+7,fit_sigmas[i])
        plot3x.SetBinError(i+7,fit_sigmas_errors[i])
        xmin = str(hist2d.GetXaxis().GetBinLowEdge(i))
        xmax = str(hist2d.GetXaxis().GetBinUpEdge(i))

        if i >= 16:
            plot3x.SetMarkerColor(i-5)
        else:
            plot3x.SetMarkerColor(i-6)
 
    legend3.AddEntry(plot2x,"#sigma_{fit}", "PE1")
    plot3x.SetStats(False)
    plot3x.GetYaxis().SetRangeUser(0.1,0.35)
    plot3x.SetMarkerStyle(20)
    plot3x.SetMarkerSize(0.7)
    plot3x.SetMarkerColor(46)
    plot3x.SetLineColor(38)
    plot3x.GetXaxis().SetTitle("F^{pixels}")
    plot3x.GetXaxis().SetTitleSize(0.045)
    plot3x.GetXaxis().SetTitleOffset(1.1)

    plot3x.GetYaxis().SetTitle("#sigma (gaussian fit)")
    plot3x.GetYaxis().SetTitleSize(0.045)
    plot3x.GetYaxis().SetTitleOffset(1.1)

    fit_range_min = 0.3
    fit_range_max = 0.8
    constant_func = ROOT.TF1("constant_func", "[0]", fit_range_min, fit_range_max)
    fit_result = plot3x.Fit(constant_func, "S")
    chi2 = constant_func.GetChisquare()
    ndof = constant_func.GetNDF()
    print("STD DEVS : Chi-squared value: ", chi2/ndof) 
    legend3.AddEntry(constant_func,"Fit","l")

    plot3x.Draw("SAME P")
    fit2 = ROOT.TF1("fit_func2", "[0]", 0.3, 0.8)
    fit2.SetParameter(0,0.235)
    '''
    plot3x.Fit(fit2, "QRSL","",0.3,0.8)
    fit2.SetLineColor(2)
    fit2.SetLineWidth(1)
    fit2.Draw("same")
    fit_function2 = plot3x.GetFunction("fit_func2")
    chi22 = fit_function2.GetChisquare()
    ndof2 = fit_function2.GetNDF()
    prob2 = fit_function2.GetProb()
    print("Linear fit of #sigma has chi 2 = {}, ndof = {}".format(chi22,ndof2))
    latex2 = ROOT.TLatex()
    latex2.SetNDC()
    latex2.SetTextAngle(0)
    latex2.SetTextColor(2)
    latex2.SetTextFont(52)
    latex2.SetTextAlign(11)
    latex2.SetTextSize(0.04)
    legMu2 = '#chi^{2}/N_{dof} = '
    latex2.DrawLatex(0.58, 0.26, legMu2 + '{:.2f}'.format(chi22/ndof2))
    legend3.AddEntry(fit2, "Linear fit", "l")
    '''

    legend3.SetTextSize(0.06)
    legend3.Draw("SAME")
    canvas3.cd()
    
    CMS_lumi.CMS_lumi(canvas3, iPeriod, iPos)
    namepng2 = 'Fits/'+output+'_stddev.png'
    nameroot2 = 'Fits/' + output+'_stddev.root'
    namepdf2 = 'Fits/' + output+'_stddev.pdf'
    canvas3.SaveAs(namepng2)
    canvas3.SaveAs(namepdf2)
    canvas3.SaveAs(nameroot2)




    for result in fit_results:
        print("Histogram: {}, Chi2/NDF: {}/{} = {}".format(result["F^pix"], result["Chi2"], result["NDF"],(float(result["Chi2"])/float(result["NDF"]))))
        print("Mean: {} +/- {}, Sigma: {} +/- {}".format(result["Mean"],result["Mean_error"],result["Sigma"],result["Sigma_error"]))
        print("-" * 30)




def MakeCanvas(name,hist2d,output,min_X,max_X,x_axis_title):
    spec = False
    specRange = []
    if spec:
        specRange = [7,hist2d.GetNbinsX()-4]
    else:
        specRange = [i for i in range(7,hist2d.GetNbinsX()-3)]
    specRange = [7,12,16]
    specColor = [38,46,30]
    specStyle = [23,20,39]
    canvas = ROOT.TCanvas(name, name, 600, 600)
    canvas.SetLogy(1)   
    legend = ROOT.TLegend(0.5, 0.6, 0.8, 0.9)
    
    pad1 = ROOT.TPad("histograms", "Histograms", 0, 0.1, 1, 0.92)
    pad1.Draw()
    pad1.cd()
    pad1.SetLogy(1)
    a = ROOT.TH1D("a","a",6,3,5.5)
    a.SetMarkerSize(0)
    a.SetStats(False)
    a.GetYaxis().SetRangeUser(0.00001,0.06)
    a.SetTitle("")
    a.GetXaxis().SetTitle("I_{h} [MeV/cm]")
    a.GetXaxis().SetTitleSize(0.045)
    a.GetYaxis().SetTitleSize(0.04)
    a.GetXaxis().SetTitleOffset(1.1)
    a.GetYaxis().SetTitleOffset(1.15)
    a.GetXaxis().SetLabelSize(0.03)
    a.GetYaxis().SetLabelSize(0.03)
    a.GetYaxis().SetTitle("Normalized tracks / 0.025")
    a.Draw("P")
    pad1.SetTopMargin(topMargin)
    
    #for i in range(7,hist2d.GetNbinsX()-3):
    ctC = 0
    for i in specRange:       
        plot = hist2d.ProjectionY("_px"+str(i-6),i,i)
        plot.GetXaxis().SetRangeUser(3,6)
        renorm(plot)
        #plot.GetYaxis().SetRangeUser(0,0.08)
        plot.SetStats(False)
        xmin = str(hist2d.GetXaxis().GetBinLowEdge(i))
        xmax = str(hist2d.GetXaxis().GetBinUpEdge(i))
        plot.SetMarkerColor(specColor[ctC]) 
        plot.SetLineWidth(0) 
        plot.SetMarkerStyle(specStyle[ctC]) 
        '''
        if i >= 16:
            plot.SetMarkerColor(i-5)
        else:
            plot.SetMarkerColor(i-6)
        plot.SetMarkerStyle(20)
        '''
        plot.Rebin(2)
        plot.SetMarkerSize(0.8)


        plot.Draw("SAME P")
        legend.AddEntry(plot,"F^{pixels} ["+xmin+"-"+xmax+"]", "PE1")
        ctC+=1

    legend.SetTextSize(0.04)
    legend.Draw("SAME")
    canvas.cd()
    
    CMS_lumi.CMS_lumi(canvas, iPeriod, iPos)
    namepng = 'Fpix_Ih/'+output+'.png'
    namepdf = 'Fpix_Ih/'+output+'.pdf'
    nameroot = 'Fpix_Ih/'+output+'.root'
    canvas.SaveAs(namepng)
    canvas.SaveAs(namepdf)
    canvas.SaveAs(nameroot)

    if spec:
        ks_statistic = hist2d.ProjectionY("_px"+str(i-6),7,7).KolmogorovTest(hist2d.ProjectionY("_px"+str(i-6),hist2d.GetNbinsX()-4,hist2d.GetNbinsX()-4), "X")
        ks_significance = ROOT.TMath.KolmogorovProb(ks_statistic)
        print("Kolmogorov-Smirnov test statistic:", ks_statistic)
        print("Kolmogorov-Smirnov test significance:", ks_significance)

    name2 = name + "_stddev"
    canvas2 = ROOT.TCanvas(name2, name2, 900, 1200)
    canvas2.SetLogy(1)   
    legend2 = ROOT.TLegend(0.2, 0.3, 0.3, 0.4)
    pad1s = ROOT.TPad("Std devs", "Std devs", 0, 0.4, 1, 0.92)
    pad1s.Draw()
    pad1s.cd()

    pad1s.SetTopMargin(topMargin)
    plot2x = hist2d.ProjectionX("_py_default",2,2)
    plot2x.Reset()
    #for i in range(7,hist2d.GetNbinsX()-3):
    for i in specRange:
        plot2 = hist2d.ProjectionY("_px"+str(i-6),i,i)
        clone = plot2x.Clone("stddev")
        clone.Reset()

        stddev = plot2.GetStdDev()
        stddev_err = plot2.GetStdDevError() 
        plot2x.SetBinContent(i,stddev)
        plot2x.SetBinError(i,stddev_err)
        
        xmin = str(hist2d.GetXaxis().GetBinLowEdge(i))
        xmax = str(hist2d.GetXaxis().GetBinUpEdge(i))

        if i >= 16:
            plot2x.SetMarkerColor(i-5)
        else:
            plot2x.SetMarkerColor(i-6)
 
    legend2.AddEntry(plot2x,"I_h std dev", "PE1")
    plot2x.SetStats(False)
    plot2x.SetMarkerStyle(20)
    plot2x.SetMarkerSize(0.7)
    plot2x.SetMarkerColor(46)
    plot2x.Draw("SAME P")

    legend2.Draw("SAME")
    canvas2.cd()
    
    CMS_lumi.CMS_lumi(canvas2, iPeriod, iPos)
    namepng = 'Fpix_Ih/'+output+'_stddev.png'
    namepdf = 'Fpix_Ih/'+output+'_stddev.pdf'
    nameroot = 'Fpix_Ih/'+output+'_stddev.root'
    canvas2.SaveAs(namepng)
    canvas2.SaveAs(namepdf)
    canvas2.SaveAs(nameroot)

def renorm(hist):
    if hist.Integral() > 0:
        hist.Scale(1.0 / hist.Integral())
    else:
        print("Hist {} has no entries".format(hist))



def main():    
    data_path = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/4_october_data_validation_pt100/"
    dataHyp = "Mu2018_massCut_0_pT100_V7p0_Fpix.root" 

    dataRoot_file = ROOT.TFile.Open(os.path.join(data_path, dataHyp),"READ")
    print(dataRoot_file)
    data_histogram_fpix_ih = dataRoot_file.Get("fpix_ih_regionC_3fp8_testIhPt")
    print(data_histogram_fpix_ih)

    data_1dX = data_histogram_fpix_ih.ProjectionY("x_proj",7,7) 
    range_x = getRangeGraph(data_1dX)
     
    #MakeCanvas("canvas_fpix_ih_extrem",data_histogram_fpix_ih,"Fpix_ih_correlations",range_x[0],range_x[1],"Ih [MeV/cm]")
    
    MakeCanvasFit("canvas_fits_ih_fpix_extrem", data_histogram_fpix_ih, "Fpix_ih_fits", range_x[0], range_x[1], "Ih [MeV/cm]")

if __name__ == "__main__":
    main()
