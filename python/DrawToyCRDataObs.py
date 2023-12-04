import os
import ROOT
from binningVar import *
import sys
from USE_DATE import USED_DATE,VERSION


def poissonHisto(h, RNG):
    hres = h.Clone()
    for i in range(h.GetNbinsX()+1):
        hres.SetBinContent(i, RNG.Poisson(h.GetBinContent(i)))
        
    return hres



# Define the input and output directories
input_base_path = "V"+VERSION+ '/' +USED_DATE  + "pt"
output_base_path = "V"+VERSION+"/dataV"+VERSION + "_pt"

# List of pT values
#pt_values = ["70", "100", "200", "300"]
pt_values = ["100"]

# List of years and SR values
years = ["2017", "2018"]
sr_values = ["SR0", "SR1", "SR2"]

# Loop through directories and process files
for pt in pt_values:
    input_dir = input_base_path + pt
    output_dir = output_base_path + pt

    # Create output directory if it doesn't exist
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for year in years:
        for sr in sr_values:
            # Input file name
            input_file_name = '{}_{}_predMass.root'.format(year, sr)
            input_file_path = os.path.join(input_dir, input_file_name)

            # Output file name
            output_file_name = '{}_{}_ObsMassPoisson.root'.format(year, sr)
            output_file_path = os.path.join(output_dir, output_file_name)

            # Output hist name
            output_file_name_rebin = '{}_{}_ObsMass_poissonHisto_rebin'.format(year, sr)
            output_file_name_raw = '{}_{}_ObsMass_poissonHisto_raw'.format(year, sr)
             

            # Read input histogram
            input_file = ROOT.TFile(input_file_path, "READ")

             
            input_hist_name = os.path.splitext(input_file_name)[0]+'_raw'
            print("Retrieving histo {} from file  {}".format(input_hist_name,input_file_path))

            input_hist = input_file.Get(input_hist_name)
            if not isinstance(input_hist, ROOT.TH1F): 
                continue
            #if isinstance(input_hist, ROOT.TObject):
                #continue

            print("Input hist ={}".format(input_hist))        
            input_hist = input_hist.Rebin(sizeRebinning,"",rebinning)      

            # Write the output histogram to the output file
            output_file = ROOT.TFile(output_file_path, "RECREATE")

            # Create a random number generator for Poisson distribution
            rng = ROOT.TRandom3()

            # Apply PoissonHisto function
            output_hist = poissonHisto(input_hist, rng)
            output_hist.Write(output_file_name_raw)

            #output_hist = output_hist.Rebin(sizeRebinning,"",rebinning)            
            output_hist.Write(output_file_name_rebin)
            output_file.Close()

            # Clean up
            input_file.Close()
