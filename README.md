# HSCPTreeAnalyzer

## Setup
 - ROOT vesion >= 6.14
 - python3

## Goals of the package:
 - running over the TTree produced by Analyzer.cc in [SUSYBSMAnalysis-HSCP](https://github.com/CMS-HSCP/SUSYBSMAnalysis-HSCP)
 - parallelize the processing through the use of TProof-lite
 - produce the bunch of plots used to predict & valide the background prediction
 - ease the implementation of additional plots through the use of python scripts

## Tools/methods used:
 - TProof
 - TTreeReader (Value or Array) to simply the syntax (single line)
 - Use config files where the selections/variables are stored. See cfg directory
 - Use python script to generate the code to be compiled/executed

Classes are defined in inc/ src/ directory and compiled into a library libTools.so.
This library needs to be loaded in root macros.

## Usage
> root -l -b -q macro.cc

## To-do for Eric:
 - remove useless TTreeReader Value/Array
 - add patterns to add variables
 - add HSCP selection
 - add patterns to change the selection
 - add a selection of a single HSCP candidate per event (most ionizing)
 - write the associated python scripts
 - fix the storage of TCanvas (duplicate name)
 - compile.sh changed into a Makefile
 - fix issue with K & C global variable
 - create a macros directory and move code

## To-do for Raphael:
 - Extend MassRegionPlots for all quantiles
 - Port all required pieces of code from Dylan's code
 - Check the code compatibility by running background prediction macros
 - clean the code to avoid hard-coded values and becoming configurable
 - check the usage of references in classes/functions


More documentation about TProof can be found [here](https://root.cern/download/proof.pdf).
Ideas about other usage of parallelization with root can be found [here](https://iopscience.iop.org/article/10.1088/1742-6596/898/7/072022/pdf)
