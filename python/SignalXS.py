nbEventsSig = [96122.0,96660.0,96823.0,96638.0,96454.0,96930.0,96755.0,96645.0,96940.0,98603.0,99336.0,99647.0,99736.0,99812.0,99850.0,99897.0]



sampleNames = ["Gluino800","Gluino1000","Gluino1400","Gluino1600","Gluino1800","Gluino2000","Gluino2200","Gluino2400","Gluino2600","pairStau308","pairStau432","pairStau557","pairStau651","pairStau745","pairStau871","pairStau1029"]

repo = "data_validation_pt300"
ptCut = "_pT300"
sampleArray = [
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/'+repo + '/Glu800_massCut_0'+ ptCut +'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu1000_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu1400_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu1600_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu1800_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu2000_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu2200_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu2400_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu2600_massCut_0'+ptCut  + '_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau308_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau432_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau557_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau651_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau745_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau871_massCut_0'+ptCut+'_Fpix.root',
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau1029_massCut_0'+ptCut+'_Fpix.root',
]

crossSectionArray = {
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu800_massCut_0'+ptCut+'_Fpix.root' : 1.81,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu1000_massCut_0'+ptCut+'_Fpix.root' : 0.385,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu1400_massCut_0'+ptCut+'_Fpix.root' : 0.0284,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu1600_massCut_0'+ptCut+'_Fpix.root' : 0.00887,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu1800_massCut_0'+ptCut+'_Fpix.root' : 0.00293,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu2000_massCut_0'+ptCut+'_Fpix.root' : 0.00101,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu2200_massCut_0'+ptCut+'_Fpix.root' : 0.000356,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu2400_massCut_0'+ptCut+'_Fpix.root' : 0.000128,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/Glu2600_massCut_0'+ptCut+'_Fpix.root' : 4.62e-5,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau308_massCut_0'+ptCut+'_Fpix.root' : 0.005617969677491303,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau432_massCut_0'+ptCut+'_Fpix.root' : 0.0013205993021946,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau557_massCut_0'+ptCut+'_Fpix.root' : 0.0003982919391389629,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau651_massCut_0'+ptCut+'_Fpix.root' : 0.00018170273864904894,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau745_massCut_0'+ptCut+'_Fpix.root' : 8.760649473515753e-5,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau871_massCut_0'+ptCut+'_Fpix.root' : 3.548748280460506e-5,
'/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_10_6_30/src/HSCPTreeAnalyzer/macros/Data_good/' + repo + '/pStau1029_massCut_0'+ptCut+'_Fpix.root' : 1.1684554271190975e-5,
}


DCnames = ["Gluino800_2018","Gluino1000_2018","Gluino1400_2018","Gluino1600_2018","Gluino1800_2018","Gluino2000_2018","Gluino2200_2018","Gluino2400_2018","Gluino2600_2018","pairStau308_2018","pairStau432_2018","pairStau557_2018","pairStau651_2018","pairStau745_2018","pairStau871_2018","pairStau1029_2018"]

pathDC = "/opt/sbg/cms/ui4_data1/rhaeberl/CMSSW_11_3_4/src/HSCPLimits/scripts/"
