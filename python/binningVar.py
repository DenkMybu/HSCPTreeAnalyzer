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
#rebinning=array.array('d',[0.,20.,40.,60.,80.,100.,120.,140.,160.,180.,200.,220.,240.,260.,280.,300.,340.,380.,420.,460.,520.,580.,660.,760.,860.,1060.,1260.])


#rebinning=array.array('d',[0.,20.,40.,60.,80.,100.,120.,140.,160.,180.,200.,220.,240.,260.,280.,300.,340.,380.,420.,480.,580.,750.,1400.])

#SIGMA 1.2 
#rebinning=array.array('d', [0.0, 20.0, 45.0, 70.0, 100.0, 130.0, 165.0, 205.0, 250.0, 300.0, 355.0, 420.0, 515.0, 715.0, 4000.0])

#SIGMA 1.0
#rebinning=array.array('d', [0.0, 20.0, 40.0, 60.0, 85.0, 110.0, 135.0, 165.0, 195.0, 230.0, 270.0, 315.0, 365.0, 420.0, 490.0, 570.0, 4000.0])

#SIGMA 0.8
#rebinning=array.array('d', [0.0, 15.0, 30.0, 45.0, 65.0, 85.0, 105.0, 125.0, 145.0, 170.0, 195.0, 220.0, 250.0, 280.0, 315.0, 355.0, 395.0, 440.0, 515.0, 715.0, 4000.0])

#SIGMA 0.85
#rebinning=array.array('d', [0.0, 15.0, 30.0, 50.0, 70.0, 90.0, 110.0, 135.0, 160.0, 185.0, 215.0, 245.0, 280.0, 315.0, 355.0, 400.0, 455.0, 515.0, 715.0, 1400.0])

#SIGMA 0.9
#rebinning=array.array('d', [0.0, 15.0, 35.0, 55.0, 75.0, 95.0, 120.0, 145.0, 170.0, 200.0, 230.0, 265.0, 305.0, 345.0, 390.0, 445.0, 515.0, 715.0, 1400.0])

#SIGMA 0p5
#rebinning=array.array('d', [0.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 85.0, 100.0, 115.0, 130.0, 145.0, 160.0, 175.0, 190.0, 205.0, 225.0, 245.0, 265.0, 285.0, 305.0, 330.0, 355.0, 380.0, 410.0, 440.0, 470.0, 515.0, 715.0, 4000.0])


#SIGMA 0.7
#rebinning=array.array('d', [0.0, 15.0, 30.0, 45.0, 60.0, 75.0, 90.0, 110.0, 130.0, 150.0, 170.0, 190.0, 215.0, 240.0, 265.0, 295.0, 325.0, 360.0, 395.0, 435.0, 480.0, 530.0, 585.0, 755.0, 1400.0])

#SIGMA 0.7 Ih > 4.05
#rebinning=array.array('d', [0.0, 15.0, 30.0, 45.0, 60.0,75.0, 90.0, 110.0, 130.0, 150.0, 170.0, 190.0, 215.0, 240.0, 265.0, 295.0, 325.0, 355.0, 385.0, 420.0, 470.0, 560.0, 1400.0])

#SIGMA 0.7 Ih > 4.15 
#rebinning=array.array('d', [0.0, 60.0, 70.0, 85.0, 105.0, 125.0, 145.0, 165.0, 185.0, 210.0, 235.0, 260.0, 290.0, 320.0, 350.0, 380.0, 520.0, 1400.0])

#SIGMA 0.7 Ih > 4.35 
rebinning=array.array('d', [0.0, 60.0, 70.0, 85.0, 105.0, 125.0, 145.0, 165.0, 185.0, 210.0, 235.0, 260.0, 295.0, 410.0, 1400.0])


#SIGMA 0.7 3.95 < Ih < 4.05
#rebinning=array.array('d', [0.0, 60.0, 70.0, 85.0, 105.0, 125.0, 145.0, 165.0, 185.0, 210.0, 235.0, 260.0, 290.0, 320.0, 355.0,400.0, 545.0, 1400.0])

#SIGMA 1.0
#rebinning=array.array('d', [0.0, 20.0, 40.0, 60.0, 85.0, 110.0, 135.0, 165.0, 195.0, 230.0, 270.0, 315.0, 365.0, 420.0, 485.0, 560.0, 650.0, 1400.0])

#SIGMA 1.3
#rebinning=array.array('d', [0.0, 25.0, 50.0, 80.0, 110.0, 145.0, 185.0, 230.0, 280.0, 340.0, 410.0, 495.0, 600.0, 740.0, 1400.0])

sizeRebinning=len(rebinning)-1
max_rebin = max(rebinning)


cfgFile = "../macros/Weights_regB_P.txt"

cfg2D = "../macros/Weights_2D_eta_p.txt"
