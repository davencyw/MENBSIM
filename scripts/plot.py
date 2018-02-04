import plotdensity as pd
import csv
import numpy as np
from os import listdir
from os.path import isfile, join


folder = "../data/out"
plotfolder = "../data/out/plots"
ending = ".posdat"

files = [f for f in listdir(folder) if f.endswith(ending)]
step = 0
for f in files:
    print("processing " + f)
    with open(join(folder,f), 'rb') as csvfile:
        step += 1
        csvreader = csv.reader(csvfile, delimiter=",")
        data = list(csvreader)
        particles = int(len(data)-1)
        pos = np.array(data[1:particles+1], dtype=np.float32)
        filename = "plot_"+str(step)+".png"
        filepath = join(plotfolder,filename)
        pd.plot3ddensity(pos[:,0], pos[:,1], pos[:,2], time=step, filepath =filepath)
