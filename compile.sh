g++ -std=c++11 -fPIC `root-config --cflags --glibs` -c src/RegionMassPlot.cc 
g++ -std=c++11 -fPIC `root-config --cflags --glibs` -c src/PlotTools.cc 
g++ -std=c++11 -fPIC `root-config --cflags --glibs` -c src/MassTools.cc 
g++ -fPIC -shared MassTools.o PlotTools.o RegionMassPlot.o -o libTools.so
