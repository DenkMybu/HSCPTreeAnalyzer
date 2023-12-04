import ROOT
import os

ROOT.gROOT.SetBatch(ROOT.kTRUE)

def percentage_above(hist,low_x,high_x):
    total_entries = hist.GetEntries()
    entries_in = 0
    for bin_x in range(1, hist.GetNbinsX() + 1):
        for bin_y in range(1, hist.GetNbinsY() + 1):
            if hist.GetXaxis().GetBinCenter(bin_x) > low_x and hist.GetXaxis().GetBinCenter(bin_x) < high_x:
                entries_in += hist.GetBinContent(bin_x, bin_y)
    if total_entries == 0:
        percentage_in = 0
    else:
        percentage_in = (entries_in / total_entries) * 100
    return percentage_in

    

def main():
    outdir = "2D_plots/"
    Ih_cut = [3.9,3.95,4.0,4.05,4.1,4.2]
    histogram_name = 'mass_ih_regionAll_testIhPt'

    data_directory = '/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/4_october_data_validation_pt100'
    data_rootfile = "Mu2018_massCut_0_pT100_V7p0_Fpix.root"

    data_file = ROOT.TFile(os.path.join(data_directory, data_rootfile), 'read')
    data_hist = data_file.Get(histogram_name)
    lowval = 4.25
    highval = 8
    prc_data = percentage_above(data_hist,lowval,highval)
    print("SingleMuon 2018 has {}% of events with Ih_StripOnly c [ {} - {} ] ".format(prc_data,lowval,highval))


if __name__ == "__main__":
    main()
