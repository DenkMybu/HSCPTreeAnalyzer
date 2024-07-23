import os
import ROOT
import numpy as np

def GetLatexStddev(color):
    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(color)
    latex.SetTextFont(61)
    latex.SetTextAlign(11)
    latex.SetTextSize(0.03)
    latex.SetTextFont(52)
    return latex

def get_standard_deviation(root_file, hist_name):
    root_file = ROOT.TFile(root_file, "READ")
    histogram = root_file.Get(hist_name)
    if histogram:
        std_deviation = (histogram.GetStdDev(),histogram.GetStdDevError(),histogram.GetMean(),histogram.GetMeanError())
        root_file.Close()
        return std_deviation
    else:
        root_file.Close()
        return None

def plotRatios(ratios1,ratios2,mass_points,oDir):

    n = len(mass_points)
    x = np.array(mass_points,dtype=np.float64)
    r1 = np.array(ratios1,dtype=np.float64)
    r2 = np.array(ratios2,dtype=np.float64)
    xerr = np.array([0]*len(mass_points),dtype=np.float64)
    yerr = np.array([0]*len(mass_points),dtype=np.float64)

    print("x values : ")
    print(x)

    print("Ratios comb / dedx : ")
    print(r1)

    print("Ratios comb / beta : ")
    print(r2)

    graph_ratio1 = ROOT.TGraphErrors(n,x,r1,xerr,yerr)
    graph_ratio2 = ROOT.TGraphErrors(n,x,r2,xerr,yerr)
    

    graph_ratio1.SetMarkerStyle(20)
    graph_ratio1.SetMarkerColor(ROOT.kBlue)
    graph_ratio1.SetTitle("")
    graph_ratio1.GetXaxis().SetTitle("Mass Points r1")
    graph_ratio1.GetYaxis().SetTitle("Ratio1")

    graph_ratio2.SetMarkerStyle(20)
    graph_ratio2.SetMarkerColor(ROOT.kRed)
    graph_ratio2.SetTitle("Std Dev 1 / Std Dev 2")
    graph_ratio2.GetXaxis().SetTitle("Mass Points r2")
    graph_ratio2.GetYaxis().SetTitle("Ratio2")

    
    legend = ROOT.TLegend(0.15, 0.75, 0.65, 0.9)
    legend.SetTextFont(42)
    legend.SetTextSize(0.035)
    legend.SetMargin(0.15)
    legend.SetY1NDC(0.6)
    legend.SetY2NDC(0.3)
    legend.AddEntry(graph_ratio1, "Stddev M combined / stddev M dedx", "P")
    legend.AddEntry(graph_ratio2, "Stddev M combined / stddev M beta", "P")



    canvas = ROOT.TCanvas("ratiosStdDevs", "ratiosStdDevs", 800, 600)
    canvas.cd()
    pad1 = ROOT.TPad("std dev ratios", "std dev ratios", 0, 0, 1, 1)
    pad1.Draw()
    pad1.cd()
    pad1.SetTopMargin(0.05)
    pad1.SetBottomMargin(0.05)

    graph_ratio1.GetXaxis().SetRangeUser(0,1100)
    graph_ratio1.GetYaxis().SetRangeUser(0.5,1.1)
    graph_ratio1.Draw("AP")
    graph_ratio2.Draw("same P")

    legend.Draw("same") 
    name = "ratios_stddevs.pdf"
    canvas.SaveAs(oDir+name)



def plotBoth(roofile,hist1,hist2,hist3,hist4,massPoint,oDir):
    root_file = ROOT.TFile(roofile, "READ")
    histogram1 = root_file.Get(hist1)
    histogram1.SetStats(0)
    histogram1.SetMarkerStyle(20)
    histogram1.SetMarkerSize(0.7)
    histogram1.SetMarkerColor(9)
    histogram1.SetLineColor(9)

    histogram2 = root_file.Get(hist2)
    histogram2.SetMarkerStyle(20)
    histogram2.SetMarkerSize(0.7)
    histogram2.SetMarkerColor(46)
    histogram2.SetLineColor(46)

    histogram3 = root_file.Get(hist3)
    histogram3.SetMarkerStyle(20)
    histogram3.SetMarkerSize(0.7)
    histogram3.SetMarkerColor(ROOT.kBlack)
    histogram3.SetLineColor(ROOT.kBlack)

    histogram4 = root_file.Get(hist4)
    histogram4.SetMarkerStyle(20)
    histogram4.SetMarkerSize(0.7)
    histogram4.SetMarkerColor(8)
    histogram4.SetLineColor(8)

    histogram1.SaveAs(oDir+"M_"+str(massPoint)+"_DedxPower2.root") 
    histogram4.SaveAs(oDir+"M_"+str(massPoint)+"_DedxAtlas.root") 
    maxY = max([histogram1.GetMaximum(),histogram2.GetMaximum(),histogram3.GetMaximum(),histogram4.GetMaximum()])

    legend = ROOT.TLegend(0.65, 0.6, 0.9, 0.9)
    legend.SetTextFont(42)
    legend.SetTextSize(0.035)
    legend.SetMargin(0.15)
    legend.SetY1NDC(0.6)
    legend.SetY2NDC(0.3)
    #legend.AddEntry(histogram1, "Mass = p #sqrt{#frac{I_{h} - C_{MIP}}{K}}", "L")
    legend.AddEntry(histogram2, "Mass = #frac{p #sqrt{1 - #beta^{2}}}{#beta}", "L")
    #legend.AddEntry(histogram3, "Mass Combined", "L")
    legend.AddEntry(histogram4, "Mass Atlas", "L")
    canvas = ROOT.TCanvas("massDistribs_M"+str(massPoint), "massDistribs_M"+str(massPoint), 800, 600)

    pad1 = ROOT.TPad("histograms_M"+str(massPoint), "Histograms_M"+str(massPoint), 0, 0, 1, 1)
    pad1.Draw()
    pad1.cd()
    pad1.SetTopMargin(0.05)
    pad1.SetBottomMargin(0.05)
    histogram1.GetXaxis().SetRangeUser(massPoint*0.5,massPoint*1.5)
    histogram1.GetYaxis().SetRangeUser(0,maxY*1.1)

    histogram2.GetXaxis().SetRangeUser(massPoint*0.5,massPoint*1.5)
    histogram2.GetYaxis().SetRangeUser(0,maxY*1.1)

    histogram3.GetXaxis().SetRangeUser(massPoint*0.5,massPoint*1.5)
    histogram3.GetYaxis().SetRangeUser(0,maxY*1.1)

    histogram4.GetXaxis().SetRangeUser(massPoint*0.5,massPoint*1.5)
    histogram4.GetYaxis().SetRangeUser(0,maxY*1.1)

    #histogram1.Draw("SAME HIST")
    histogram2.Draw("SAME HIST")    
    #histogram3.Draw("SAME HIST")    
    histogram4.Draw("SAME HIST")    
    legend.Draw("same") 
   
    latex1 = GetLatexStddev(9) 
    latex2 = GetLatexStddev(8) 
    
    latex1.DrawLatex(0.76, 0.25, 'Std dev = {:.1f}'.format(histogram2.GetStdDev()))
    latex2.DrawLatex(0.76, 0.17, 'Std dev = {:.1f}'.format(histogram4.GetStdDev()))
    
    name = "massDistribs_M"+str(massPoint)+".pdf"
    canvas.SaveAs(oDir+name)

def main():
    xmax = 1100
    xmin = 100
    xbin = 40
    ybin = 50
    ymax = 200
    ymin = 0

    oDir = 'massIh_vs_Beta_vs_Atlas/'
    input_directory = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Signal_good/ATLAS_mass/"

    massIh = "testIhPt_hMass_SR1_9fp10"
    massBeta = "testIhPt_hMassBeta_SR1_9fp10"
    massCombined = "testIhPt_hMassCombined_SR1_9fp10"
    massAtlas = "testIhPt_hMassAtlas_SR1_9fp10"

    mpoint = []
    stdDevmassIh = []
    stdDevErrmassIh = []
    meanmassIh = []
    meanErrmassIh = []

    stdDevmassBeta = []
    stdDevErrmassBeta = []
    meanmassBeta = []
    meanErrmassBeta = []

    stdDevmassComb = []
    stdDevErrmassComb = []
    meanmassComb = []
    meanErrmassComb = []

    stdDevmassAtlas = []
    stdDevErrmassAtlas = []
    meanmassAtlas = []
    meanErrmassAtlas = []

    for root_file in os.listdir(input_directory):
        if root_file.endswith(".root"):
            # Extract the number from the file name
            number = int( (''.join(filter(str.isdigit, root_file)))[:-10])
            
            root_file_path = os.path.join(input_directory, root_file)
            std_dev_and_error_massIh = get_standard_deviation(root_file_path, massIh)
            std_dev_and_error_massBeta = get_standard_deviation(root_file_path, massBeta)
            std_dev_and_error_massComb = get_standard_deviation(root_file_path, massCombined)
            std_dev_and_error_massAtlas = get_standard_deviation(root_file_path, massAtlas)


            print("\n********** Mass hypothesis = {} **********\n".format(number))
            print("Mass using bethe-bloch parametrisation: Std dev = {} and error = {} | Mean = {} and mean error = {}\n".format(std_dev_and_error_massIh[0],std_dev_and_error_massIh[1],std_dev_and_error_massIh[2],std_dev_and_error_massIh[3]))
 
            print("Mass using beta information : Std dev = {} and error = {} | Mean = {} and mean error = {}\n".format(std_dev_and_error_massBeta[0],std_dev_and_error_massBeta[1],std_dev_and_error_massBeta[2],std_dev_and_error_massBeta[3]))

            mpoint.append(number)
            stdDevmassIh.append(std_dev_and_error_massIh[0])
            stdDevErrmassIh.append(std_dev_and_error_massIh[1])
            meanmassIh.append(std_dev_and_error_massIh[2])
            meanErrmassIh.append(std_dev_and_error_massIh[3])

            stdDevmassBeta.append(std_dev_and_error_massBeta[0])
            stdDevErrmassBeta.append(std_dev_and_error_massBeta[1])
            meanmassBeta.append(std_dev_and_error_massBeta[2])
            meanErrmassBeta.append(std_dev_and_error_massBeta[3])


            stdDevmassComb.append(std_dev_and_error_massComb[0])
            stdDevErrmassComb.append(std_dev_and_error_massComb[1])
            meanmassComb.append(std_dev_and_error_massComb[2])
            meanErrmassComb.append(std_dev_and_error_massComb[3])

            stdDevmassAtlas.append(std_dev_and_error_massAtlas[0])
            stdDevErrmassAtlas.append(std_dev_and_error_massAtlas[1])
            meanmassAtlas.append(std_dev_and_error_massAtlas[2])
            meanErrmassAtlas.append(std_dev_and_error_massAtlas[3])



            plotBoth(root_file_path,massIh,massBeta,massCombined,massAtlas,number,oDir)


    n = len(mpoint)
    print("Len mpoint = ",len(mpoint))
    x = np.array(mpoint,dtype=np.float64)
    y_stddev_massIh = np.array(stdDevmassIh,dtype=np.float64)
    y_stddev_massBeta = np.array(stdDevmassBeta,dtype=np.float64)
    y_stddev_massComb = np.array(stdDevmassComb,dtype=np.float64)
    y_stddev_massAtlas = np.array(stdDevmassAtlas,dtype=np.float64)
    y_mean_massIh = np.array(meanmassIh,dtype=np.float64)
    y_mean_massBeta = np.array(meanmassBeta,dtype=np.float64)
    y_mean_massComb = np.array(meanmassComb,dtype=np.float64)
    y_mean_massAtlas = np.array(meanmassAtlas,dtype=np.float64)

    xerr = np.array([0]*len(mpoint),dtype=np.float64)

    yerr_stddev_massIh = np.array(stdDevErrmassIh,dtype=np.float64)
    yerr_stddev_massBeta = np.array(stdDevErrmassBeta,dtype=np.float64)
    yerr_stddev_massComb = np.array(stdDevErrmassComb,dtype=np.float64)
    yerr_stddev_massAtlas = np.array(stdDevErrmassAtlas,dtype=np.float64)

    yerr_mean_massIh = np.array(meanErrmassIh,dtype=np.float64)
    yerr_mean_massBeta = np.array(meanErrmassBeta,dtype=np.float64)
    yerr_mean_massComb = np.array(meanErrmassComb,dtype=np.float64)
    yerr_mean_massAtlas = np.array(meanErrmassAtlas,dtype=np.float64)



    graph_std_dev_massIh = ROOT.TGraphErrors(n,x,y_stddev_massIh,xerr,yerr_stddev_massIh)
    graph_std_dev_massIh.SetTitle("Standard Deviation vs Number mass Ih")
    graph_std_dev_massIh.GetXaxis().SetTitle("Mass [GeV]")
    graph_std_dev_massIh.GetYaxis().SetTitle("Standard Deviation")
    graph_std_dev_massIh.SetMarkerStyle(20)
    graph_std_dev_massIh.SetMarkerSize(0.7)
    graph_std_dev_massIh.SetMarkerColor(9)

    

    graph_std_dev_massBeta = ROOT.TGraphErrors(n,x,y_stddev_massBeta,xerr,yerr_stddev_massBeta)
    graph_std_dev_massBeta.SetTitle("Standard Deviation vs Number mass Beta")
    graph_std_dev_massBeta.GetXaxis().SetTitle("Mass [GeV]")
    graph_std_dev_massBeta.GetYaxis().SetTitle("Standard Deviation")
    graph_std_dev_massBeta.SetMarkerStyle(20)
    graph_std_dev_massBeta.SetMarkerSize(0.7)
    graph_std_dev_massBeta.SetMarkerColor(46)


    graph_std_dev_massComb = ROOT.TGraphErrors(n,x,y_stddev_massComb,xerr,yerr_stddev_massComb)
    graph_std_dev_massComb.SetTitle("Standard Deviation vs Number mass Beta")
    graph_std_dev_massComb.GetXaxis().SetTitle("Mass [GeV]")
    graph_std_dev_massComb.GetYaxis().SetTitle("Standard Deviation")
    graph_std_dev_massComb.SetMarkerStyle(20)
    graph_std_dev_massComb.SetMarkerSize(0.7)
    graph_std_dev_massComb.SetMarkerColor(ROOT.kBlack)


    graph_std_dev_massAtlas = ROOT.TGraphErrors(n,x,y_stddev_massAtlas,xerr,yerr_stddev_massAtlas)
    graph_std_dev_massAtlas.SetTitle("Standard Deviation vs Number mass Atlas")
    graph_std_dev_massAtlas.GetXaxis().SetTitle("Mass [GeV]")
    graph_std_dev_massAtlas.GetYaxis().SetTitle("Standard Deviation")
    graph_std_dev_massAtlas.SetMarkerStyle(20)
    graph_std_dev_massAtlas.SetMarkerSize(0.7)
    graph_std_dev_massAtlas.SetMarkerColor(8)





    graph_mean_massIh = ROOT.TGraphErrors(n,x,y_mean_massIh,xerr,yerr_mean_massIh)
    graph_mean_massIh.SetTitle("Mean vs Number")
    graph_mean_massIh.GetXaxis().SetTitle("Mass [GeV]")
    graph_mean_massIh.GetYaxis().SetTitle("Mean")
    graph_mean_massIh.SetMarkerStyle(20)
    graph_mean_massIh.SetMarkerSize(0.7)
    graph_mean_massIh.SetMarkerColor(9)



    graph_mean_massBeta = ROOT.TGraphErrors(n,x,y_mean_massBeta,xerr,yerr_mean_massBeta)
    graph_mean_massBeta.SetTitle("Mean vs Number")
    graph_mean_massBeta.GetXaxis().SetTitle("Mass [GeV]")
    graph_mean_massBeta.GetYaxis().SetTitle("Mean")
    graph_mean_massBeta.SetMarkerStyle(20)
    graph_mean_massBeta.SetMarkerSize(0.7)
    graph_mean_massBeta.SetMarkerColor(46)


    graph_mean_massComb = ROOT.TGraphErrors(n,x,y_mean_massComb,xerr,yerr_mean_massComb)
    graph_mean_massComb.SetTitle("Mean vs Number")
    graph_mean_massComb.GetXaxis().SetTitle("Mass [GeV]")
    graph_mean_massComb.GetYaxis().SetTitle("Mean")
    graph_mean_massComb.SetMarkerStyle(20)
    graph_mean_massComb.SetMarkerSize(0.7)
    graph_mean_massComb.SetMarkerColor(ROOT.kBlack)

    graph_mean_massAtlas = ROOT.TGraphErrors(n,x,y_mean_massAtlas,xerr,yerr_mean_massAtlas)
    graph_mean_massAtlas.SetTitle("Mean vs Number")
    graph_mean_massAtlas.GetXaxis().SetTitle("Mass [GeV]")
    graph_mean_massAtlas.GetYaxis().SetTitle("Mean")
    graph_mean_massAtlas.SetMarkerStyle(20)
    graph_mean_massAtlas.SetMarkerSize(0.7)
    graph_mean_massAtlas.SetMarkerColor(8)

  
    ratio1stddev = []
    ratio2stddev = []
    for i in range(len(y_stddev_massIh)):
        ratio1stddev.append(y_stddev_massComb[i]/y_stddev_massIh[i]) 
        ratio2stddev.append(y_stddev_massComb[i]/y_stddev_massBeta[i])

    plotRatios(ratio1stddev,ratio2stddev,mpoint,"ratios_signal_stddevs/")


    legend = ROOT.TLegend(0.1, 0.5, 0.3, 0.7)
    legend.AddEntry(graph_mean_massBeta, "Mass using #beta", "P")
    legend.AddEntry(graph_mean_massIh, "Mass using Ih", "P")
    legend.AddEntry(graph_mean_massComb, "Mass combined", "P")
    legend.AddEntry(graph_mean_massAtlas, "Mass Atlas", "P")



    line_mean = ROOT.TLine(min(mpoint), min(mpoint), max(mpoint), max(mpoint))
    line_mean.SetLineColor(ROOT.kGreen)
    legend2 = ROOT.TLegend(0.1, 0.5, 0.3, 0.7)
    legend2.AddEntry(graph_mean_massBeta, "Mass using #beta", "P")
    legend2.AddEntry(graph_mean_massIh, "Mass using Ih", "P")
    legend2.AddEntry(graph_mean_massComb, "Mass combined", "P")
    legend2.AddEntry(graph_mean_massAtlas, "Mass Atlas", "P")
    legend2.AddEntry(line_mean,"f(x) = x","l")

    canvas_graphs = ROOT.TCanvas("canvas_graphs", "TGraphErrors", 800, 600)
    canvas_graphs.Divide(1, 2)
    canvas_graphs.cd(1)
    graph_std_dev_massIh.Draw("AP")
    graph_std_dev_massBeta.Draw("SAME P")
    graph_std_dev_massComb.Draw("SAME P")
    graph_std_dev_massAtlas.Draw("SAME P")
    legend.Draw("same")
    canvas_graphs.Update()
    

    canvas_graphs.cd(2)
    graph_mean_massIh.Draw("AP")
    graph_mean_massBeta.Draw("SAME P")
    graph_mean_massComb.Draw("SAME P")
    graph_mean_massAtlas.Draw("SAME P")
    line_mean.Draw("same")
    legend2.Draw("same")
    canvas_graphs.Update()
    canvas_graphs.SaveAs(oDir+"graph_errors_vs_number.pdf")
    canvas_graphs.SaveAs(oDir+"graph_errors_vs_number.root")
    
    canvas_graphs.Draw()
     

if __name__ == "__main__":
    ROOT.gROOT.SetBatch(True)  # Run ROOT in batch mode to prevent GUI window from popping up
    main()

