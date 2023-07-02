"""

2022/10/03
create datacard from the mass distributions of signal and data in region D
using distributions that are already normalized from Dylan

"""

import ROOT as rt
import csv
import re
import sys
import collections
import os

from collections import OrderedDict
import uproot

import scipy
import awkward
import numpy as np
import time



def make_datacard_hscp_merged(outDataCardsDir,  modelName, signal, bkg, observation, sig_unc, bkg_unc):

    text_file = open(outDataCardsDir+modelName+".txt", "w")

    text_file.write('imax {0} \n'.format(1))
    text_file.write('jmax {0} \n'.format(1))
    text_file.write('kmax * \n')
    text_file.write('shapes * * FAKE \n')



    text_file.write('--------------- \n')
    text_file.write('--------------- \n')
    text_file.write('bin \t  chD \n')
    text_file.write('observation \t {} \n'.format(observation))
    text_file.write('------------------------------ \n')
    text_file.write('bin \t chD \t chD \n')
    text_file.write('process \t signal \t bkg \n')
    text_file.write('process \t 0 \t 1 \n')
    text_file.write('rate \t {} \t {} \t \n'.format(signal, bkg))
    text_file.write('------------------------------ \n')
    text_file.write('errBckg \t lnN \t - \t {} \n'.format(bkg_unc))
    text_file.write('errSig  \t lnN \t {} \t 0 \n'.format(sig_unc))

    '''
    text_file.write('sig_Trigger  \t lnN \t 0.869112881723/0.996985742453 \t - \t 0.869112881723/0.996985742453 \t - \n')
    text_file.write('sig2017_Fpix \t lnN \t 1.0/0.999847562177 \t - \t - \t - \n')
    text_file.write('sig2017_C  \t lnN \t 1.00013111731/0.999848031204 \t - \t - \t - \n')
    text_file.write('sig2017_K  \t lnN \t 1.00012775366/0.999803189393 \t - \t - \t - \n')
    text_file.write('sig_pu  \t lnN \t 0.998502302438/1.00135130722 \t - \t 0.998502302438/1.00135130722 \t - \n')
    text_file.write('sig2018_C  \t lnN \t - \t - \t 1.00013111731/0.999848031204 \t - \n')
    text_file.write('sig2018_K  \t lnN \t - \t - \t 1.00012775366/0.999803189393 \t - \n')
    text_file.write('sig2018_Fpix \t lnN \t - \t - \t 1.0/0.999847562177 \t - \n')
    '''

    text_file.close()



def make_datacard_2channels_hscp(outDataCardsDir,  modelName, signal, bkg, observation, sig_unc, bkg_unc):

    text_file = open(outDataCardsDir+modelName+".txt", "w")

    text_file.write('imax {0} \n'.format(2))
    text_file.write('jmax {0} \n'.format(1))
    text_file.write('kmax * \n')
    text_file.write('shapes * * FAKE \n')



    text_file.write('--------------- \n')
    text_file.write('--------------- \n')
    text_file.write('bin \t  ch2017 \t ch2018 \n')
    text_file.write('observation \t {} \t {} \n'.format(observation,observation))
    text_file.write('------------------------------ \n')
    text_file.write('bin \t ch2017 \t ch2017 \t ch2018 \t ch2018 \n')
    text_file.write('process \t signal \t bkg \t signal \t bkg \n')
    text_file.write('process \t 0 \t 1 \t 0 \t 1 \n')
    text_file.write('rate \t {} \t {} \t \n'.format(signal, bkg))
    text_file.write('------------------------------ \n')
    text_file.write('errBckg \t lnN \t - \t {} \t - \t {} \n'.format(bkg_unc,bkg_unc))
    text_file.write('errSig  \t lnN \t {} \t 0 \t {} \t 0 \n'.format(sig_unc,sig_unc))

    '''     
    text_file.write('sig_Trigger  \t lnN \t 0.869112881723/0.996985742453 \t - \t 0.869112881723/0.996985742453 \t - \n')
    
    text_file.write('sig2017_C  \t lnN \t 1.00013111731/0.999848031204 \t - \t - \t - \n')
    text_file.write('sig2017_K  \t lnN \t 1.00012775366/0.999803189393 \t - \t - \t - \n')
    text_file.write('sig2018_C  \t lnN \t - \t - \t 1.00013111731/0.999848031204 \t - \n')
    text_file.write('sig2018_K  \t lnN \t - \t - \t 1.00012775366/0.999803189393 \t - \n')
   
     
    text_file.write('sig2017_Fpix \t lnN \t 1.0/0.999847562177 \t - \t - \t - \n')
    text_file.write('sig_pu  \t lnN \t 0.998502302438/1.00135130722 \t - \t 0.998502302438/1.00135130722 \t - \n')
    text_file.write('sig2018_Fpix \t lnN \t - \t - \t 1.0/0.999847562177 \t - \n')
    '''
    text_file.close()

def add_2018_to_datacards_2channels(outDataCardsDir,modelName,signal, bkg):
    if os.path.exists(outDataCardsDir+modelName+".txt"):
        print("The file exists.")
    else:
        print("The file does not exist, please first run on 2017 dataset")
        sys.exit()

    text_file = open(outDataCardsDir+modelName+".txt", "r")

    lines = text_file.readlines()
    if(len(lines))>13:
        line = lines[12].rstrip('\n')
        print("Will add signal and bckg for 2018 to datacard")
        line += "{} \t {} \n".format(signal,bkg)
        lines[12] = line

    with open(outDataCardsDir+modelName+".txt", 'w') as newfile:         
        newfile.writelines(lines)
