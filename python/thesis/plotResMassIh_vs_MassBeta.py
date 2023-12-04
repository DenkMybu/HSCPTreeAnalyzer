import os
import ROOT
import numpy as np

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

def plotBoth(roofile,hist1,hist2,massPoint,oDir):
    root_file = ROOT.TFile(roofile, "READ")
    histogram1 = root_file.Get(hist1)
    histogram1.SetStats(0)
    histogram1.SetMarkerStyle(20)
    histogram1.SetMarkerSize(0.7)
    histogram1.SetMarkerColor(ROOT.kBlue)

    histogram2 = root_file.Get(hist2)
    histogram2.SetMarkerStyle(20)
    histogram2.SetMarkerSize(0.7)
    histogram2.SetMarkerColor(ROOT.kRed)

    legend = ROOT.TLegend(0.7, 0.6, 0.9, 0.9)
    legend.AddEntry(histogram1, "Mass using Ih", "P")
    legend.AddEntry(histogram2, "Mass using #beta", "P")
    canvas = ROOT.TCanvas("massDistribs_M"+str(massPoint), "massDistribs_M"+str(massPoint), 800, 600)

    pad1 = ROOT.TPad("histograms_M"+str(massPoint), "Histograms_M"+str(massPoint), 0, 0, 1, 1)
    pad1.Draw()
    pad1.cd()
    pad1.SetTopMargin(0.05)
    pad1.SetBottomMargin(0.05)
    histogram1.Draw("SAME P")
    histogram2.Draw("SAME P")    
    legend.Draw("same") 
    name = "massDistribs_M"+str(massPoint)+".pdf"
    canvas.SaveAs(oDir+name)
def main():
    xmax = 1100
    xmin = 100
    xbin = 40
    ybin = 50
    ymax = 200
    ymin = 0

    oDir = 'massIh_vs_Beta/'
    input_directory = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Signal_good/pStau_MassBeta/"
    massIh = "testIhPt_hMass_SR1_9fp10"
    massBeta = "testIhPt_hMassBeta_SR1_9fp10"

    mpoint = []
    stdDevmassIh = []
    stdDevErrmassIh = []
    meanmassIh = []
    meanErrmassIh = []

    stdDevmassBeta = []
    stdDevErrmassBeta = []
    meanmassBeta = []
    meanErrmassBeta = []
    for root_file in os.listdir(input_directory):
        if root_file.endswith(".root"):
            # Extract the number from the file name
            number = int( (''.join(filter(str.isdigit, root_file)))[:-6])
            root_file_path = os.path.join(input_directory, root_file)
            std_dev_and_error_massIh = get_standard_deviation(root_file_path, massIh)
            std_dev_and_error_massBeta = get_standard_deviation(root_file_path, massBeta)
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

            plotBoth(root_file_path,massIh,massBeta,number,oDir)


    n = len(mpoint)
    print("Len mpoint = ",len(mpoint))
    x = np.array(mpoint,dtype=np.float64)
    y_stddev_massIh = np.array(stdDevmassIh,dtype=np.float64)
    y_stddev_massBeta = np.array(stdDevmassBeta,dtype=np.float64)
    y_mean_massIh = np.array(meanmassIh,dtype=np.float64)
    y_mean_massBeta = np.array(meanmassBeta,dtype=np.float64)
    xerr = np.array([0]*len(mpoint),dtype=np.float64)
    yerr_stddev_massIh = np.array(stdDevErrmassIh,dtype=np.float64)
    yerr_stddev_massBeta = np.array(stdDevErrmassBeta,dtype=np.float64)
    yerr_mean_massIh = np.array(meanErrmassIh,dtype=np.float64)
    yerr_mean_massBeta = np.array(meanErrmassBeta,dtype=np.float64)



    graph_std_dev_massIh = ROOT.TGraphErrors(n,x,y_stddev_massIh,xerr,yerr_stddev_massIh)
    graph_std_dev_massIh.SetTitle("Standard Deviation vs Number mass Ih")
    graph_std_dev_massIh.GetXaxis().SetTitle("Mass [GeV]")
    graph_std_dev_massIh.GetYaxis().SetTitle("Standard Deviation")
    graph_std_dev_massIh.SetMarkerStyle(20)
    graph_std_dev_massIh.SetMarkerSize(0.7)
    graph_std_dev_massIh.SetMarkerColor(ROOT.kBlue)

    

    graph_std_dev_massBeta = ROOT.TGraphErrors(n,x,y_stddev_massBeta,xerr,yerr_stddev_massBeta)
    graph_std_dev_massBeta.SetTitle("Standard Deviation vs Number mass Beta")
    graph_std_dev_massBeta.GetXaxis().SetTitle("Mass [GeV]")
    graph_std_dev_massBeta.GetYaxis().SetTitle("Standard Deviation")
    graph_std_dev_massBeta.SetMarkerStyle(20)
    graph_std_dev_massBeta.SetMarkerSize(0.7)
    graph_std_dev_massBeta.SetMarkerColor(ROOT.kRed)


    graph_mean_massIh = ROOT.TGraphErrors(n,x,y_mean_massIh,xerr,yerr_mean_massIh)
    graph_mean_massIh.SetTitle("Mean vs Number")
    graph_mean_massIh.GetXaxis().SetTitle("Mass [GeV]")
    graph_mean_massIh.GetYaxis().SetTitle("Mean")
    graph_mean_massIh.SetMarkerStyle(20)
    graph_mean_massIh.SetMarkerSize(0.7)
    graph_mean_massIh.SetMarkerColor(ROOT.kBlue)



    graph_mean_massBeta = ROOT.TGraphErrors(n,x,y_mean_massBeta,xerr,yerr_mean_massBeta)
    graph_mean_massBeta.SetTitle("Mean vs Number")
    graph_mean_massBeta.GetXaxis().SetTitle("Mass [GeV]")
    graph_mean_massBeta.GetYaxis().SetTitle("Mean")
    graph_mean_massBeta.SetMarkerStyle(20)
    graph_mean_massBeta.SetMarkerSize(0.7)
    graph_mean_massBeta.SetMarkerColor(ROOT.kRed)
   

    legend = ROOT.TLegend(0.1, 0.5, 0.3, 0.7)
    legend.AddEntry(graph_mean_massBeta, "Mass using #beta", "P")
    legend.AddEntry(graph_mean_massIh, "Mass using Ih", "P")



    line_mean = ROOT.TLine(min(mpoint), min(mpoint), max(mpoint), max(mpoint))
    line_mean.SetLineColor(ROOT.kGreen)
    legend2 = ROOT.TLegend(0.1, 0.5, 0.3, 0.7)
    legend2.AddEntry(graph_mean_massBeta, "Mass using #beta", "P")
    legend2.AddEntry(graph_mean_massIh, "Mass using Ih", "P")
    legend2.AddEntry(line_mean,"f(x) = x","l")

    canvas_graphs = ROOT.TCanvas("canvas_graphs", "TGraphErrors", 800, 600)
    canvas_graphs.Divide(1, 2)
    canvas_graphs.cd(1)
    graph_std_dev_massIh.Draw("AP")
    graph_std_dev_massBeta.Draw("SAME P")
    legend.Draw("same")
    canvas_graphs.Update()
    

    canvas_graphs.cd(2)
    graph_mean_massIh.Draw("AP")
    graph_mean_massBeta.Draw("SAME P")
    line_mean.Draw("same")
    legend2.Draw("same")
    canvas_graphs.Update()
    canvas_graphs.SaveAs(oDir+"graph_errors_vs_number.pdf")
    canvas_graphs.SaveAs(oDir+"graph_errors_vs_number.root")
    
    canvas_graphs.Draw()
     

if __name__ == "__main__":
    ROOT.gROOT.SetBatch(True)  # Run ROOT in batch mode to prevent GUI window from popping up
    main()

