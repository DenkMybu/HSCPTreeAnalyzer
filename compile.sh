g++ -std=c++11 -fPIC `root-config --cflags --glibs` -I inc -c src/RegionMassPlot.cc 
g++ -std=c++11 -fPIC `root-config --cflags --glibs` -I inc -c src/PlotTools.cc 
g++ -std=c++11 -fPIC `root-config --cflags --glibs` -I inc -c src/MassTools.cc 
g++ -std=c++11 -fPIC `root-config --cflags --glibs` -I inc -c src/CPlots.cc 
g++ -fPIC -shared CPlots.o MassTools.o PlotTools.o RegionMassPlot.o -o libTools.so
rm *.o
