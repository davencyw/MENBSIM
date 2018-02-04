import plotdensity as pd
import csv
import numpy as np
from os import listdir
from os.path import isfile, join
import seaborn as sb
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt


folder = "../data/out/logs"
plotfolder = "../data/out/plots"
ending = ".log"
starting = "verify"

softening = [0.01,0.1,1,5]

files = [f for f in listdir(folder) if f.endswith(ending) and f.startswith(starting) ]
step = 0
for f in files:
    print("processing " + f)
    with open(join(folder,f), 'rb') as csvfile:
        csvreader = csv.reader(csvfile, delimiter=",")
        data = list(csvreader)
        shells = int(len(data))
        shelldata = np.array(data[0:shells+1], dtype=np.float32)
        plt.semilogy( shelldata[:,2],shelldata[:,0], label="averaged")
        plt.semilogy( shelldata[:,2], shelldata[:,1], label="analytical")
        plt.legend()
        plt.title('comparing to newtons second therom for spherical potentials, softening='+str(softening[step]))
        plt.xlabel('shell distance r')
        plt.ylabel('forcemagnitude')
        plt.savefig('force_verification'+str(softening[step])+'.png', bbox_inches='tight', dpi=200)
        plt.clf()
        step += 1
