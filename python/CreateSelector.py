import os
from Functions import *
import pandas as pd


#######################################
# Take care of the HSCP preselection
#######################################
# Read config files
#######################################

df_HSCPsel = pd.read_csv("../cfg/HSCPpreSelections.csv",skipinitialspace=True)
print(df_HSCPsel)
df_plots = pd.read_csv("../cfg/Variables.csv",skipinitialspace=True)

# header files: template and new one
h_temp = open("../macros/HSCPTemplate.h",'r')
h_out = open("../macros/HSCPSelector.h",'w')

for line in h_temp:
    #copy the line of the code
    h_out.write(line)
    #add code to declare data members corresponding to the HSCP preselections
    if line.find("ADD-HSCP-SELECTION")>0:
        content = Code_HSCPSelectorProto(df_HSCPsel)
        h_out.write(content)

h_out.close()
h_temp.close()

#  C files: template and new one
c_temp = open("../macros/HSCPTemplate.C",'r')
c_out = open("../macros/HSCPSelector.C",'w')

for line in c_temp:
    #copy the line of the code
    c_out.write(line)
    #add code for the HSCP preselection methods
    if line.find("ADD-SELECTION-METHODS")>0:
        content = Code_HSCPSelectorImpl(df_HSCPsel)
        c_out.write(content)
    #add code to add labels & members for HSCP preselection into vectors
    if line.find("FILL-SELECTION-VECTOR")>0:
        content = Code_HSCPSelectorAddLabelsAndPointers2Vector(df_HSCPsel)
        c_out.write(content)
    #add code to add a plot
    if line.find("ADD-CPLOTS")>0:
        content = Code_AddPlot(df_plots)
        c_out.write(content)
    #add code to fill a plot
    if line.find("FILL-CPLOTS")>0:
        content = Code_FillPlot(df_plots)
        c_out.write(content)

c_out.close()
c_temp.close()

#
#
