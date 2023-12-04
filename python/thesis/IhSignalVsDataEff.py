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

def create_and_return_graph(data, output_filename,threshold,prt_data,color):

    graph = ROOT.TGraph(len(data))
    graph.GetXaxis().SetRangeUser(100,1250)

    graphData = ROOT.TGraph(1)
    graphData.SetPoint(0,1200, prt_data)
    graphData.SetMarkerStyle(20)
    graphData.SetMarkerColor(color)
    graphData.SetMarkerSize(0.9) 
    
    for i, (file_number, percentage) in enumerate(data):
        graph.SetPoint(i,int(file_number), int(percentage))
    
    graph.GetXaxis().SetTitle("Signal Mass [GeV]            Data (SingleMuon)")
    graph.GetXaxis().SetTitleOffset(-1.2)
    graph.GetYaxis().SetTitle("Percentage (%)")
    graph.SetMarkerStyle(20)
    graph.SetMarkerColor(color)
    graph.SetMarkerSize(0.9) 

    return (graph,graphData)



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
    Ih_cut = [3.9,3.95,4.0,4.05]
    Ih_colors = [1,6,3,2]
    histogram_name = 'mass_ih_regionAll_testIhPt'

    data_directory = '/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/4_october_data_validation_pt100'
    data_rootfile = "Mu2018_massCut_0_pT100_V7p0_Fpix.root"
    data_file = ROOT.TFile(os.path.join(data_directory, data_rootfile), 'read')
    data_hist = data_file.Get(histogram_name)

    data_CR = 'mass_ih_regionAll_testIhPt'    
    data_CR_hist = data_file.Get(data_CR)
    data_CR_hist.GetYaxis().SetRangeUser(0,1000)

    signal_directory = '/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Signal_good/pStau_V7p0'

    all_duos = []
    for p in range(len(Ih_cut)):
        prc_sig = []
        prc_data = []
        tmp_prc_data= percentage_above(data_CR_hist,Ih_cut[p]) * 20
        prc_data.append(tmp_prc_data)
        print("SingleMuon 2018 has {}% of events with Ih_StripOnly > {}".format(percentage_above(data_CR_hist,Ih_cut[p]),Ih_cut[p]))
        for root_file_name in os.listdir(signal_directory):
            if root_file_name.endswith('.root') and 'massCut_0_pT100_V7p0_Fpix' in root_file_name:
                mass_point = root_file_name.split('_')[0][5:]
                root_file = ROOT.TFile(os.path.join(signal_directory, root_file_name), 'read')    
                histogram = root_file.Get(histogram_name)
                if histogram:
                    prc_sig.append((mass_point,percentage_above(histogram,Ih_cut[p])))
                    print("PairStau M = {} has {}% of events with Ih_StripOnly > {}".format(mass_point,percentage_above(histogram,Ih_cut[p]),Ih_cut[p]))
                     

                root_file.Close()
          
        name_prt_all = str(Ih_cut[p]).replace('.','p')
        duo = create_and_return_graph(prc_sig, outdir+"Stau_percentage_above_Ih_"+name_prt_all,Ih_cut[p],tmp_prc_data,Ih_colors[p])
        all_duos.append(duo)
    
    canvas = ROOT.TCanvas('canvas', 'Canvas', 800, 600)
    frame = ROOT.TH1F("frame","frame", 1250,0,1250)
    frame.SetTitle("Percentage of Entries for different Ih cuts")
    frame.GetXaxis().SetRangeUser(100,1250)
    frame.GetXaxis().SetTitle("Signal Mass [GeV]                  Data")
    frame.GetXaxis().SetNdivisions(510)

    frame.GetYaxis().SetRangeUser(0,100)
    frame.GetYaxis().SetTitle("Percentage (%)")
    frame.GetYaxis().SetNdivisions(505)


    yaxis_secondary = ROOT.TGaxis(1250, 0, 1250, 100, 0, 5, 510, "+L")
    yaxis_secondary.SetTitle("Percentage (%)")
    yaxis_secondary.SetLabelSize(0.03)
    yaxis_secondary.SetTitleSize(0.03)
    yaxis_secondary.SetLineColor(ROOT.kBlue)
    yaxis_secondary.SetLabelColor(ROOT.kBlue)
    yaxis_secondary.SetTitleColor(ROOT.kBlue)



    frame.SetStats(0)
    frame.Draw() 
    legend = ROOT.TLegend(0.5, 0.3, 0.65, 0.6)
 
    yaxis_secondary.Draw("SAME")
    line = ROOT.TLine(1090, 0, 1090, 100)
    line.SetLineColor(ROOT.kBlack)
    line.SetLineStyle(2)
    line.SetLineWidth(1)
    line2 = ROOT.TLine(1100, 0, 1100, 100)
    line2.SetLineColor(ROOT.kBlack)
    line2.SetLineStyle(2)
    line2.SetLineWidth(1)
    line.Draw("SAME")
    line2.Draw("SAME")
    for p in range(len(Ih_cut)):
        all_duos[p][0].Draw("SAME P")
        all_duos[p][1].Draw("SAME P")
        legend.AddEntry(all_duos[p][0],"I_{h} > "+str(Ih_cut[p]), "P")

    legend.Draw("SAME")
    canvas_name = 'Mass_vs_Ih_all_data'
    canvas.SetName(canvas_name)
    canvas.SaveAs(outdir+canvas_name + '.png')
        

if __name__ == "__main__":
    main()
