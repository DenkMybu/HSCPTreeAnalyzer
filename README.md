# HSCPTreeAnalyzer

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


## To do :
 - change the way histos are save in RegionMassControl and in the TSelector
 - change usage of references
 - test with TProof to produce few plots on Raph's root file
 - add python scripts

More documentation about TProof can be found here https://root.cern/download/proof.pdf
