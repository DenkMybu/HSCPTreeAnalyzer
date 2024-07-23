#!/bin/bash

version=$(python -c 'from USE_DATE import VERSION; print("V"+VERSION)') 

argB=$1
argC=$2

day=$(date +%d)
month=$(date +%B | tr '[:upper:]' '[:lower:]')
combined="$day$month"

dt=$(python -c "from USE_DATE import USED_DATE; print(USED_DATE)")
echo "Successfully imported the USE DATE as ${dt}"

#output_types=("pt70" "pt100" "pt200" "pt300")
output_types=("pt100")

mu2017_base="/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/combined_massed/${dt}data_validation"

mu2018_base="/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/combined_masses/${dt}data_validation"

for ot in "${output_types[@]}"; do
    output_dir="${version}/${dt}${ot}"
    numeric_ot="${ot#pt}"  # Remove "pt" prefix to get the numeric part
    #mu2017_file="${mu2017_base}_${ot}/Mu2017_massCut_0_pT${numeric_ot}_${version}_Fpix_Ih4p05.root"
    #mu2017_file="${mu2017_base}_${ot}/Mu2017_massCut_0_pT${numeric_ot}_${version}_Fpix_Ih4p15.root"
    #mu2017_file="${mu2017_base}_${ot}/Mu2017_massCut_0_pT${numeric_ot}_${version}_Fpix_Ih4p35.root"
    #mu2017_file="${mu2017_base}_${ot}/Mu2017_massCut_0_pT${numeric_ot}_${version}_Fpix_IhMid.root"

    mu2017_file="${mu2017_base}_${ot}/Mu2017_massCut_0_pT${numeric_ot}_${version}_Fpix_beta.root"

    #mu2017_file="${mu2017_base}_${ot}/Mu2017_massCut_0_pT${numeric_ot}_${version}_Fpix_SubstractedIh.root"
    #mu2017_file="${mu2017_base}_${ot}/Mu2017_massCut_0_pT${numeric_ot}_${version}_Fpix.root"

    #mu2018_file="${mu2018_base}_${ot}/Mu2018_massCut_0_pT${numeric_ot}_${version}_Fpix_Ih4p05.root"
    #mu2018_file="${mu2018_base}_${ot}/Mu2018_massCut_0_pT${numeric_ot}_${version}_Fpix_Ih4p15.root"
    #mu2018_file="${mu2018_base}_${ot}/Mu2018_massCut_0_pT${numeric_ot}_${version}_Fpix_Ih4p35.root"
    #mu2018_file="${mu2018_base}_${ot}/Mu2018_massCut_0_pT${numeric_ot}_${version}_Fpix_IhMid.root"
    mu2018_file="${mu2018_base}_${ot}/Mu2018_massCut_0_pT${numeric_ot}_${version}_Fpix_beta.root"


    #mu2018_file="${mu2018_base}_${ot}/Mu2018_massCut_0_pT${numeric_ot}_${version}_Fpix_SubstractedIh.root"
    #mu2018_file="${mu2018_base}_${ot}/Mu2018_massCut_0_pT${numeric_ot}_${version}_Fpix.root"
    echo "Running on singleMu 2017 and then 2018 sample, saving outputs to $output_dir, showing $argB integrals and max mass is $argC, with PT cut = ${numeric_ot}"

    python NoSigDataValidation.py "$mu2017_file" "$output_dir" "$argB" "$argC" "$numeric_ot"
    python NoSigDataValidation.py "$mu2018_file" "$output_dir" "$argB" "$argC" "$numeric_ot"
done
