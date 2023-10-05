#!/bin/bash

param2_array=("2017" "2018")
param1_array=("SR0" "SR1" "SR2")


for param1 in "${param1_array[@]}"; do
    for param2 in "${param2_array[@]}"; do
        echo "Making pred files for search region = $param1 and year = $param2"
        python display_pred.py "$param1" "$param2"
    done
done
