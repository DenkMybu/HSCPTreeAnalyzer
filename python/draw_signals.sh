#!/bin/bash


param2="100"
param1="SR1"


echo "Making signals files for search region = $param1 with pT cut = $param2"
python display_signal.py "$param2" "$param1"
