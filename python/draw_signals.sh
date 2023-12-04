#!/bin/bash

#param2_array=("70" "100" "200" "300")
param2_array=("100")
param1_array=("SR0" "SR1" "SR2")


#param2_array=( "100")
#param1_array=("SR1")

for param1 in "${param1_array[@]}"; do
    for param2 in "${param2_array[@]}"; do
        echo "Making signals files for search region = $param1 with pT cut = $param2"
        python display_signal.py "$param2" "$param1"
    done
done
