import ROOT
import os

ROOT.gROOT.SetBatch(ROOT.kTRUE)

def percentage_above(hist,threshold):
    total_entries = hist.GetEntries()
    entries_above_threshold = 0
    for bin_x in range(1, hist.GetNbinsX() + 1):
        for bin_y in range(1, hist.GetNbinsY() + 1):
            if hist.GetXaxis().GetBinCenter(bin_x) > threshold:
                entries_above_threshold += hist.GetBinContent(bin_x, bin_y)
    if total_entries ==0:
        percentage_above_threshold = 0
    else:
        percentage_above_threshold = (entries_above_threshold / total_entries) * 100
    return percentage_above_threshold


def create_and_save_graph(data, output_filename,threshold,prt_data):
    
    canvas = ROOT.TCanvas("Percentage_signal_above_Ih", "Graph Canvas", 800, 600)
    frame = ROOT.TH1F("frame","frame", 1250,0,1250)
    frame.SetTitle("Percentage of Entries with Ih > {}".format(threshold))
    frame.GetXaxis().SetRangeUser(100,1250)
    frame.GetXaxis().SetTitle("Signal Mass [GeV]                  Data")
    frame.GetXaxis().SetNdivisions(510)

    frame.GetYaxis().SetRangeUser(0,80)
    frame.GetYaxis().SetTitle("Percentage (%)")
    frame.GetYaxis().SetNdivisions(505)

    
    graph = ROOT.TGraph(len(data))
    graph.GetXaxis().SetRangeUser(100,1250)

    graphData = ROOT.TGraph(1)
    graphData.SetPoint(0,1200, prt_data)
    graphData.SetMarkerStyle(20)
    graphData.SetMarkerColor(ROOT.kBlue)
    graphData.SetMarkerSize(1.2) 

    
    for i, (file_number, percentage) in enumerate(data):
        graph.SetPoint(i,int(file_number), int(percentage))
    
    graph.GetXaxis().SetTitle("Signal Mass [GeV]            Data (SingleMuon)")
    graph.GetXaxis().SetTitleOffset(-1.2)
    graph.GetYaxis().SetTitle("Percentage (%)")
    graph.SetMarkerStyle(20)
    graph.SetMarkerColor(ROOT.kRed)
    graph.SetMarkerSize(1.2) 
    frame.SetStats(0)
    frame.Draw()
    graph.Draw("SAME P")

    line = ROOT.TLine(1090, 0, 1090, 80)
    line.SetLineColor(ROOT.kBlack)
    line.SetLineStyle(2)
    line.SetLineWidth(1)
    line2 = ROOT.TLine(1100, 0, 1100, 80)
    line2.SetLineColor(ROOT.kBlack)
    line2.SetLineStyle(2)
    line2.SetLineWidth(1)
    line.Draw("SAME")
    line2.Draw("SAME")
    graphData.Draw("SAME P") 
     
    canvas.SaveAs(output_filename+".png")


def MakeCanvas(hist,outdir):
    canvas = ROOT.TCanvas('canvas', 'Canvas', 800, 600)
    canvas.SetLogz(1)
    hist.Draw("colz")
    hist.SetStats(0)
    canvas_name = 'Mass_vs_Ih_CR_Fpix_3fp9_data'
    canvas.SetName(canvas_name)
    canvas.SaveAs(outdir+canvas_name + '.png')
    

def main():
    outdir = "2D_plots/"
    Ih_cut = 4.05
    histogram_name = 'mass_ih_regionAll_testIhPt'

    data_directory = '/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/4_october_data_validation_pt100'
    data_rootfile = "Mu2018_massCut_0_pT100_V7p0_Fpix.root"
    data_file = ROOT.TFile(os.path.join(data_directory, data_rootfile), 'read')
    data_hist = data_file.Get(histogram_name)
    prc_data = percentage_above(data_hist,Ih_cut)
    print("SingleMuon 2018 has {}% of events with Ih_StripOnly > {}".format(prc_data,Ih_cut))

    data_CR = 'mass_ih_regionC_3fp9_testIhPt'    
    data_CR_hist = data_file.Get(data_CR)
    data_CR_hist.GetYaxis().SetRangeUser(0,1000)
    MakeCanvas(data_CR_hist,outdir)

    signal_directory = '/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Signal_good/pStau_V7p0'
    percentage_sig = []
    for root_file_name in os.listdir(signal_directory):
        if root_file_name.endswith('.root') and 'massCut_0_pT100_V7p0_Fpix' in root_file_name:
            mass_point = root_file_name.split('_')[0][5:]
            
            root_file = ROOT.TFile(os.path.join(signal_directory, root_file_name), 'read')
    
            histogram = root_file.Get(histogram_name)
     
            
            if histogram:
                prc = percentage_above(histogram,Ih_cut)
                percentage_sig.append((mass_point, prc))
                print("Pair Stau M = {} has {}% of events with Ih_StripOnly > {}".format(mass_point,prc,Ih_cut))
                histogram.GetYaxis().SetRangeUser(0,1000)
                
                canvas = ROOT.TCanvas('canvas', 'Canvas', 800, 600)
                canvas.SetLogz(1)
                histogram.Draw("colz")
                histogram.SetStats(0)

                canvas_name = 'pStau_Mass_vs_Ih_M{}_'.format(mass_point)
                canvas.SetName(canvas_name)
                
                canvas.SaveAs(outdir+canvas_name + '.png')
                root_file.Close()

    name_prt_all = str(Ih_cut).replace('.','p')
    create_and_save_graph(percentage_sig, outdir+"Stau_percentage_above_Ih_"+name_prt_all,Ih_cut,prc_data) 

if __name__ == "__main__":
    main()
