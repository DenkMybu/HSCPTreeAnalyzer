g++ -fPIC `root-config --cflags --glibs` -c src/RegionMassPlot.cc 
g++ -fPIC `root-config --cflags --glibs` -c src/PlotTools.cc 
g++ -fPIC `root-config --cflags --glibs` -c src/MassTools.cc 
g++ -fPIC `root-config --cflags --glibs` -c src/MassTools.cc 
g++ -fPIC -shared MassTools.o PlotTools.o RegionMassPlot.o -o libTools.so
