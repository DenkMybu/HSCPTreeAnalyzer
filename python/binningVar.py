import array
import ROOT


plusbinStartFit = 2
max_fit = 4000
max_CR_fit = 980
lowFitV = 70


xbinp = array.array('d',[ 16+(i*0.4) for i in range(1,311)])


xbinP = array.array('d',[ (i*0.2) for i in range(0,1001)])
xbinIh = array.array('d',[ 3.+(i*0.0125) for i in range(1,401)])
xbinIh.insert(0,3.)



#xbinp = array.array('d',[ 12+(i*0.2) for i in range(1,651)])

#Insert first bins with variable size
xbinp.insert(0,0.0)
#xbinp.insert(1,7.0)
#xbinp.insert(1,10.0)
xbinp.insert(1,12.0)
xbinp.insert(2,15.0)
xbinp.insert(3,16.0)

#Adding the rest
xbinp.append(141.0) 
xbinp.append(143.0)
xbinp.append(147.0)
xbinp.append(152.0)
xbinp.append(160.0)
xbinp.append(180.0)
xbinp.append(200.0)




'''
xbinMass = array.array('d',[(i*20) for i in range(1,21)])
xbinMass.insert(0,0.0)
xbinMass.append(420.0)
xbinMass.append(460.0)
xbinMass.append(500.0)
xbinMass.append(540.0)
xbinMass.append(580.0)
xbinMass.append(660.0)
xbinMass.append(740.0)
xbinMass.append(880.0)
xbinMass.append(1000.0)
xbinMass.append(1400.0)
xbinMass.append(2000.0)
xbinMass.append(2500.0)
xbinMass.append(3000.0)
'''

'''
xbinMass.append(2800.0)
xbinMass.append(3200.0)
xbinMass.append(3600.0)
xbinMass.append(4000.0)
'''

#xbinMass = array.array('d',[(i*10) for i in range(0,401)])
xbinMass = array.array('d',[(i*20) for i in range(0,201)])

#xbinMass = array.array('d',[(i*40) for i in range(0,101)])

'''
xbinMass = array.array('d',[(i*20) for i in range(1,21)])
xbinMass.insert(0,0.0)
xbinMass.append(450.0)
xbinMass.append(500.0)
xbinMass.append(550.0)
xbinMass.append(600.0)
xbinMass.append(650.0)
xbinMass.append(700.0)
xbinMass.append(750.0)
xbinMass.append(800.0)
xbinMass.append(850.0)
xbinMass.append(900.0)
xbinMass.append(1000.0)
xbinMass.append(1500.0)
xbinMass.append(2000.0)
xbinMass.append(2500.0)
xbinMass.append(3000.0)
'''



nxmass = len(xbinMass)-1


#rebinning=array.array('d',[0.,20.,40.,60.,80.,100.,120.,140.,160.,180.,200.,220.,240.,260.,280.,300.,320.,340.,360.,380.,405.,435.,475.,525.,585.,660.,755.,875.,1025.,1210.,1440.,1730.,2000.])

#rebinning=array.array('d',[0.,20.,40.,60.,80.,100.,120.,140.,160.,180.,200.,220.,240.,260.,280.,300.,320.,340.,360.,380.,400.,440.,480.,540.,580.,660.,760.,880.,1020.,1220.,1440.,1740.,2000.])
#IS OK
#rebinning=array.array('d',[0.,20.,40.,60.,80.,100.,120.,140.,160.,180.,200.,220.,240.,260.,280.,300.,320.,340.,360.,380.,400.,460.,520.,580.,640.,740.,840.,900.,1400.,2000.])
#rebinning=array.array('d',[0.,20.,40.,60.,80.,100.,120.,140.,160.,180.,200.,220.,240.,260.,280.,300.,320.,340.,360.,380.,420.,440.,460.,480.,540.,580.,660.,740.,860.,1200.,1600.,2000.])

#BINNING DYLAN 
#rebinning=array.array('d',[0.,20.,40.,60.,80.,100.,120.,140.,160.,180.,200.,220.,240.,260.,280.,300.,320.,340.,360.,380.,420.,440.,480.,520.,580.,660.,760.,880.,1000.,1200.,1400.,1700.,2000.])

#TEST STAU 
rebinning=array.array('d',[0.,20.,40.,60.,80.,100.,120.,140.,160.,180.,200.,220.,240.,260.,280.,300.,340.,380.,420.,460.,520.,580.,660.,760.,860.,1060.,1260.])

#rebinning=array.array('d',[0.,20.,40.,60.,80.,100.,120.,140.,160.,180.,200.,220.,240.,260.,280.,300.,320.,340.,380.,420.,480.,540.,600.,700.,800.,900.,1000.,1200.,1400.,1700.,2000.])



sizeRebinning=len(rebinning)-1
max_rebin = max(rebinning)


cfgFile = "../macros/Weights_regB_P.txt"

cfg2D = "../macros/Weights_2D_eta_p.txt"
