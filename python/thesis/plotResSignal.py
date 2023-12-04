import os
import ROOT

def get_standard_deviation(root_file, hist_name):
    root_file = ROOT.TFile(root_file, "READ")
    histogram = root_file.Get(hist_name)
    if histogram:
        std_deviation = (histogram.GetStdDev(),histogram.GetStdDevError())
        root_file.Close()
        return std_deviation
    else:
        root_file.Close()
        return None

def main():
    xmax = 1100
    xmin = 100
    xbin = 40
    ybin = 50
    ymax = 200
    ymin = 0

    input_directory = "/opt/sbg/cms/ui3_data1/rhaeberl/HSCP_prod/V7p0/HSCPpairStau_V7p0/Merged_HSCPpairStau/"
    hist_name = "HSCParticleAnalyzer/BaseName/PostS_SR1_pt100_Fpix_Mass"


    mpoint = []
    stdDev = []
    stdDevErr = []
    for root_file in os.listdir(input_directory):
        if root_file.endswith(".root"):
            # Extract the number from the file name
            number = int(''.join(filter(str.isdigit, root_file)))
            root_file_path = os.path.join(input_directory, root_file)
            std_dev_and_error = get_standard_deviation(root_file_path, hist_name)
            print("Stau at M = {} has std dev = {} and error = {}".format(number,std_dev_and_error[0],std_dev_and_error[1]))
            mpoint.append(number)
            stdDev.append(std_dev_and_error[0])
            stdDevErr.append(std_dev_and_error[1])



    
    massRes1Dtest = ROOT.TH1D("massRes1Dtest", "1D Histogram", 100, 100, 1100)

    massRes = ROOT.TH2D("massRes", "2D Histogram", xbin, xmin, xmax,ybin, ymin, ymax)

    for i in range(9):
        massRes.Fill(mpoint[i],stdDev[i])
        for p in range(int(stdDev[i])):
            massRes1Dtest.Fill(mpoint[i])
    count = 0
    for i in range(massRes1Dtest.GetNbinsX()+1):
        if massRes1Dtest.GetBinContent(i) != 0:
            massRes1Dtest.SetBinError(i,stdDevErr[count])
            print("Set Error for point in bin#{} as {}".format(i,stdDevErr[count]))
	    count+=1


    ROOT.gStyle.SetPalette(ROOT.kRainBow)



    canvas = ROOT.TCanvas("canvas", "TH2D Standard Deviation vs Number", 800, 600)
    massRes.SetStats(0)
    #massRes.GetYaxis().SetNdivisions(505)
    massRes.Draw("COLZ")
    #canvas.SetGrid()

    # Save the canvas as an image file (optional)
    canvas.SaveAs("std_deviation_vs_number.png")
    canvas.SaveAs("std_deviation_vs_number.root")

    # Display the canvas
    canvas.Draw()
    canvas1d = ROOT.TCanvas("canvas2", "Standard Deviation vs Number in 1D trick", 800, 600)
    massRes1Dtest.SetStats(0)
    #massRes.GetYaxis().SetNdivisions(505)
    massRes1Dtest.SetMarkerStyle(21)
    massRes1Dtest.SetMarkerColor(46)
    massRes1Dtest.SetMarkerSize(1)
    #canvas.SetGrid()
    massRes1Dtest.Draw("EP1")
    # Save the canvas as an image file (optional)
    canvas1d.SaveAs("std_deviation_vs_number_1dtrick.png")
    canvas1d.SaveAs("std_deviation_vs_number_1dtrick.root")

    canvas1d.Draw()

if __name__ == "__main__":
    ROOT.gROOT.SetBatch(True)  # Run ROOT in batch mode to prevent GUI window from popping up
    main()

