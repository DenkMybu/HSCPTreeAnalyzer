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


def return_sigeff_givenbkg(hist_signal, hist_background, variable_name,massPoint):
    if not hist_signal or not hist_background:
        print("Error: Invalid histograms. Please check the variable names.")
        return None

    total_signal = hist_signal.Integral()
    total_background = hist_background.Integral()
    
    signal_efficiency_points = []
    background_rejection_points = []
    print("For signal with M = {}\n".format(massPoint))
    for bin in range(1, hist_signal.GetNbinsX() + 1):
        signal_above_threshold = hist_signal.Integral(bin, hist_signal.GetNbinsX())
        background_above_threshold = hist_background.Integral(bin, hist_background.GetNbinsX())

        signal_efficiency = signal_above_threshold / total_signal
        background_rejection = 1 - (background_above_threshold / total_background)

        signal_efficiency_points.append(signal_efficiency)
        background_rejection_points.append(background_rejection)
         
        print("For {} = {}, Signal eff = {} | Bkg rejection = {}".format(variable_name,hist_signal.GetXaxis().GetBinCenter(bin),signal_efficiency,background_rejection))
        #if bin % 5 == 0:
            #print("For {} = {}, Signal eff = {} | Bkg rejection = {}".format(variable_name,hist_signal.GetXaxis().GetBinCenter(bin),signal_efficiency,background_rejection))
        
        if int(background_rejection*1000) >= 995:
            print("Returning sig eff {}  with bkg rejec = {} , for mass point {}".format(signal_efficiency,background_rejection,massPoint))
            return signal_efficiency 

    return -1


def createPlot(eff_1,variable_name_1,eff_2,variable_name_2,massPoints):
    canvas = ROOT.TCanvas("SigEff_ih_tof", "SigEff_ih_tof", 800, 600)
    graph1 = ROOT.TGraph(len(massPoints), array.array('d', massPoints), array.array('d', eff_1))
    graph2 = ROOT.TGraph(len(massPoints), array.array('d', massPoints), array.array('d', eff_2))

    graph1.SetMarkerStyle(20)
    graph1.SetMarkerColor(ROOT.kBlue)
    graph2.SetMarkerStyle(20)
    graph2.SetMarkerColor(ROOT.kRed)

    pad1 = ROOT.TPad("pad1", "pad1", 0.0, 0.0, 1.0, 1.0)
    pad1.Draw()
    pad1.cd()
    graph1.GetYaxis().SetRangeUser(0,1)
    graph1.Draw("AP")
    graph1.GetXaxis().SetTitle("Signal Masspoints")
    graph1.GetYaxis().SetTitle("Signal Efficiency")

    graph2.Draw("P same")
    legend = ROOT.TLegend(0.2, 0.8, 0.4, 0.9)
    legend.AddEntry(graph1, variable_name_1, "p")
    legend.AddEntry(graph2, variable_name_2, "p")
    legend.Draw()

    oName = "SigEff_bkgRejec99p9_"+variable_name_1+"_vs_"+variable_name_2+".png"
    canvas.SaveAs(oName)



def main():
    if len(sys.argv) != 3:
        print("Usage: python script.py <var_name_1> <var_name_2>")
        sys.exit(1)
   
    presel = "testIhPt_"
    variable_name_1 = sys.argv[1]
    variable_name_2 = sys.argv[2]

    signalMasspoints = ["200","247","308","432","557","651","745","871","1029"]
    signalMasspointsFloat = array.array('d',[200,247,308,432,557,651,745,871,1029])
 
    iDir = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/test/"
    fileBkg = ROOT.TFile.Open(iDir+"Mu2017_massCut_0_pT100_Vtest_Gstrip.root","READ")

    fileSignals = [ROOT.TFile.Open(iDir+"pStau"+i+"_massCut_0_pT100_Vtest_Gstrip.root","READ") for i in signalMasspoints]
    hist_signals_1= [fileSignals[i].Get(presel+"h" + variable_name_1) for i in range(len(fileSignals))]
    hist_signals_2= [fileSignals[i].Get(presel+"h" + variable_name_2) for i in range(len(fileSignals))]


    hist_background_1 = fileBkg.Get(presel+"h" + variable_name_1)
    hist_background_2 = fileBkg.Get(presel+"h" + variable_name_2)
        
    sigEff_1 = []
    sigEff_2 = []

    for i in range(len(fileSignals)):
        sigEff_1.append(return_sigeff_givenbkg(hist_signals_1[i], hist_background_1, variable_name_1,signalMasspoints[i]))
        sigEff_2.append(return_sigeff_givenbkg(hist_signals_2[i], hist_background_2, variable_name_2,signalMasspoints[i]))
        
    createPlot(sigEff_1,variable_name_1,sigEff_2,variable_name_2,signalMasspointsFloat)


if __name__=="__main__":
    main()
