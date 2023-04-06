# The goal is to evaluate the optimal number of workers
# and check the performance

import matplotlib.pyplot as plt
import subprocess
from time import time
import os

command = ["time","root","-l","-b","-q", "macro-test.cc"]
#change directory
os.chdir("../macros/")


nworkers = range(1,15,2)
exectime = []

def createTest(nw, ifilename, ofilename):
   ifile = open(ifilename,'r')
   ofile = open(ofilename,'w')
   for line in ifile:
       if line.find("TProof::Open")>=0:
          ofile.write("TProof::Open(\"workers="+str(nw)+"\");\n")
       else:
          ofile.write(line) 
   ifile.close()
   ofile.close()
   
# run the macro several times with  different number of workers
for nw in nworkers:
    print("#workers = ",nw)
    # change macro
    createTest(nw,"macro.cc","macro-test.cc")
    # launch the macro
    start = time()
    useless_cat_call = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, errors = useless_cat_call.communicate()
    useless_cat_call.wait()
    stop = time()
    exectime.append(stop-start)
    print("time = ",stop-start)

# do the plot

plt.plot(nworkers,exectime)
plt.title("Times [s] vs Nof workers")
plt.xlabel("#workers")
plt.ylabel("Times [s]")
plt.savefig('../img/Perf.png')
#plt.show()
