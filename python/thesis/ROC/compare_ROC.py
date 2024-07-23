import ROOT
import sys
import array
def renorm(hist):
    clone = hist.Clone()
    clone.Scale(1./clone.Integral())
    return clone

def list_possible_variables(file):
    print("You entered a variable for which there is not histogram; \nHere is the list of working variables:")
    possible_variables = set()
    for key in file.GetListOfKeys():
        obj = key.ReadObj()
        if isinstance(obj, ROOT.TH1) and key.GetName().startswith("testIhPt_"):
            variable_name = key.GetName().split('_')[1][1:]
            possible_variables.add(variable_name)
    print("\n".join(sorted(possible_variables)))


def create_roc_curve(hist_signal, hist_background, variable_name):
    if not hist_signal or not hist_background:
        print("Error: Invalid histograms. Please check the variable names.")
        return None

    total_signal = hist_signal.Integral()
    total_background = hist_background.Integral()
    
    signal_efficiency_points = []
    background_rejection_points = []

    for bin in range(1, hist_signal.GetNbinsX() + 1):
        signal_above_threshold = hist_signal.Integral(bin, hist_signal.GetNbinsX())
        background_above_threshold = hist_background.Integral(bin, hist_background.GetNbinsX())

        signal_efficiency = signal_above_threshold / total_signal
        background_rejection = 1 - (background_above_threshold / total_background)

        signal_efficiency_points.append(signal_efficiency)
        background_rejection_points.append(background_rejection)
        if bin % 5 == 0:
            print("For {} = {}, Signal eff = {} | Bkg rejection = {}".format(variable_name,hist_signal.GetXaxis().GetBinCenter(bin),signal_efficiency,background_rejection))

    roc_curve = ROOT.TGraph(len(signal_efficiency_points), array.array("d", signal_efficiency_points), array.array("d", background_rejection_points))
    return roc_curve

def compare_and_save_roc_curves(roc_curve_1, roc_curve_2, variable_name_1, variable_name_2, output_filename):
    if not roc_curve_1 or not roc_curve_2:
        print("Error: Invalid ROC curves. Please check the variable names.")
        return

    canvas = ROOT.TCanvas("canvas", "ROC Curve Comparison", 800, 600)
    pad1 = ROOT.TPad("padROC", "padROC", 0.0, 0.0, 1.0, 1.0)
    pad1.Draw()
    pad1.cd()
    pad1.SetLogy(1) 
    #pad1.SetLogx(1) 
    roc_curve_1.SetTitle("ROC Curve Comparison for Variables")
    roc_curve_1.GetXaxis().SetTitle("Signal Efficiency")
    roc_curve_1.GetYaxis().SetTitle("Background Rejection")
    roc_curve_1.GetXaxis().SetRangeUser(0, 1)
    roc_curve_1.GetYaxis().SetRangeUser(0, 1)
    roc_curve_1.SetLineColor(ROOT.kRed)
    roc_curve_1.SetLineWidth(2)
    roc_curve_1.Draw("SAME AL")

    roc_curve_2.SetLineColor(ROOT.kBlue)
    roc_curve_2.SetLineWidth(2)
    roc_curve_2.Draw("SAME L")

    legend = ROOT.TLegend(0.1, 0.3, 0.4, 0.5)
    legend.AddEntry(roc_curve_1, variable_name_1, "l")
    legend.AddEntry(roc_curve_2, variable_name_2, "l")
    legend.Draw()

    canvas.SaveAs(output_filename)

def plot_raw_distributions(hist_signal, hist_background, variable_name, output_filename):
    if not hist_signal or not hist_background:
        print("Error: Invalid histograms. Please check the variable names.")
        return

    cloneSig = renorm(hist_signal)
    cloneBkg = renorm(hist_background)
    maxVal = max(cloneSig.GetMaximum(),cloneBkg.GetMaximum())

    canvas = ROOT.TCanvas("canvas_raw", "Raw Distributions", 800, 600)
    cloneSig.SetLineColor(ROOT.kRed)
    cloneSig.SetMarkerStyle(20)
    cloneSig.SetMarkerSize(0.7)
    cloneSig.SetMarkerColor(ROOT.kRed)
    cloneSig.SetLineWidth(2)
    cloneSig.GetYaxis().SetRangeUser(0,1.2*maxVal)
    cloneSig.Draw("L")

    cloneBkg.SetLineColor(ROOT.kBlue)
    cloneBkg.SetMarkerColor(ROOT.kBlue)
    cloneBkg.SetLineWidth(2)
    cloneBkg.SetMarkerStyle(20)
    cloneBkg.SetMarkerSize(0.7)
    cloneBkg.Draw("SAME L")

    legend = ROOT.TLegend(0.45, 0.7, 0.75, 0.85)
    legend.AddEntry(cloneSig, variable_name + " Signal", "l")
    legend.AddEntry(cloneBkg, variable_name + " Background", "l")
    legend.Draw()

    canvas.SaveAs(output_filename)

def NicePlot(variable_name,hist_signal,hist_background,roc_curve_1):

    cloneSig = renorm(hist_signal)
    cloneBkg = renorm(hist_background)
    maxVal = max(cloneSig.GetMaximum(),cloneBkg.GetMaximum())

    canvas = ROOT.TCanvas("canvas_roc_distrib_"+variable_name, "Raw | ROC "+variable_name, 800, 600)
 
    pad1 = ROOT.TPad("padleft"+variable_name, "padleft"+variable_name, 0.0, 0.0, 0.5, 1.0)
    #pad1.SetBottomMargin(0.02)
    pad1.Draw()
    pad1.cd()
    cloneSig.SetLineColor(ROOT.kRed)
    cloneSig.SetLineWidth(2)
    cloneSig.GetYaxis().SetRangeUser(0,1.2*maxVal)
    cloneSig.Draw("SAME L")

    cloneBkg.SetLineColor(ROOT.kBlue)
    cloneBkg.SetLineWidth(2)
    cloneBkg.Draw("SAME L")

    '''
    legend = ROOT.TLegend(0.45, 0.7, 0.75, 0.85)
    legend.AddEntry(cloneSig, variable_name + " Signal", "l")
    legend.AddEntry(cloneBkg, variable_name + " Background", "l")
    legend.Draw("SAME")
    '''

    canvas.cd()
    pad2 = ROOT.TPad("padright"+variable_name, "padright"+variable_name, 0.5, 0.0, 1.0, 1.0)
    #pad2.SetBottomMargin(0.02)
    pad2.Draw()
    pad2.cd()

    roc_curve_1.SetTitle("ROC Curve Comparison for Variables")
    roc_curve_1.GetXaxis().SetTitle("Signal Efficiency")
    roc_curve_1.GetYaxis().SetTitle("Background Rejection")
    roc_curve_1.GetXaxis().SetRangeUser(0, 1)
    roc_curve_1.GetYaxis().SetRangeUser(0, 1)
    roc_curve_1.SetLineColor(ROOT.kRed)
    roc_curve_1.SetLineWidth(2)
    roc_curve_1.Draw("SAME L")
    '''
    legend2 = ROOT.TLegend(0.1, 0.3, 0.4, 0.5)
    legend2.AddEntry(roc_curve_1, variable_name, "l")
    legend2.Draw("SAME")
    '''
    oName = "ROC_vs_raw_"+variable_name+".pdf"
    canvas.SaveAs(oName)



def main():
    if len(sys.argv) != 3:
        print("Usage: python script.py <var_name_1> <var_name_2>")
        sys.exit(1)
   
    presel = "testIhPt_"
    variable_name_1 = sys.argv[1]
    variable_name_2 = sys.argv[2]

    signalMasspoints = ["200","247","308","432","557","651","745","871","1029"]
 
    iDir = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/test/"
    fileBkg = ROOT.TFile.Open(iDir+"Mu2017_massCut_0_pT100_Vtest_Gstrip.root","READ")

    fileSignals = [ROOT.TFile.Open(iDir+"pStau"+i+"_massCut_0_pT100_Vtest_Gstrip.root","READ") for i in signalMasspoints]

    fileSignal = ROOT.TFile.Open(iDir+"pStau432_massCut_0_pT100_Vtest_Gstrip.root","READ")
    
    
    hist_signal_1 = fileSignal.Get(presel+"h" + variable_name_1)
    hist_background_1 = fileBkg.Get(presel+"h" + variable_name_1)
    
    hist_signal_2 = fileSignal.Get(presel+"h" + variable_name_2)
    hist_background_2 = fileBkg.Get(presel+"h" + variable_name_2)
    
    if not hist_signal_1 or not hist_signal_2:
        list_possible_variables(fileBkg)
        sys.exit(1)
    
    roc_curve_1 = create_roc_curve(hist_signal_1, hist_background_1, variable_name_1)
    roc_curve_2 = create_roc_curve(hist_signal_2, hist_background_2, variable_name_2)
    
    oName = "test1_roc_"+variable_name_1+"_vs_"+variable_name_2+".pdf"
    compare_and_save_roc_curves(roc_curve_1, roc_curve_2, variable_name_1, variable_name_2, oName)
    output_raw_distribution_1_filename = "raw_"+variable_name_1+"_signal_vs_background.pdf"
    plot_raw_distributions(hist_signal_1, hist_background_1, variable_name_1, output_raw_distribution_1_filename)
    output_raw_distribution_2_filename = "raw_"+variable_name_2+"_signal_vs_background.pdf"
    plot_raw_distributions(hist_signal_2, hist_background_2, variable_name_2, output_raw_distribution_2_filename)

    #NicePlot(variable_name_1,hist_signal_1,hist_background_1,roc_curve_1)
    #NicePlot(variable_name_2,hist_signal_2,hist_background_2,roc_curve_2)
if __name__=="__main__":
    main()
