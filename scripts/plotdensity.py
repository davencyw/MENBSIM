import seaborn as sb
import numpy as np
import scipy as sp
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# https://python-graph-gallery.com/370-3d-scatterplot/
cmap = sb.cubehelix_palette(as_cmap=True, dark=0, light=1, reverse=True)


def plot2ddensity(datax, datay, ax1='Y', ax2='Z', filepath='', time=0, xmin=0, xmax=0, ymin=0, ymax=0):

    data = np.vstack((datax, datay))
    data = np.transpose(data)
    df = pd.DataFrame(data, columns=["ax1", "ax2"])

    fig, ax = sb.plt.subplots()
    sb.kdeplot(df.ax1, df.ax2, cmap=cmap, n_levels=60, shade=True, ax=ax)

    if(xmin == 0 and xmax == 0 and ymin == 0 and ymax == 0):
        xmin, xmax = ax.get_xlim()
        ymin, ymax = ax.get_ylim()

    df.plot.scatter(x='ax1', y='ax2', ax=ax, c='w', alpha=0.4, s=22)

    sb.plt.xlim(xmin, xmax)
    sb.plt.ylim(ymin, ymax)
    ax.set_xlabel(ax1)

    #title = "evolution of star formation at t=" + str(time)
    #sb.plt.title(title)
    sb.plt.ylabel(ax2).set_rotation(0)

    if(filepath!=''):
        fig.savefig(filepath,bbox_inches='tight',dpi=200);
    else:
        sb.plt.show()

    return (xmin, xmax, ymin, ymax)


def plot3ddensity(datax, datay, dataz, filepath='', time=0, xmin=0, xmax=0, ymin=0, ymax=0, zmin=0, zmax=0, rota=35, rotb=45):

    data = np.vstack((datax, datay, dataz))
    data = np.transpose(data)

    kde = sp.stats.gaussian_kde(data.T)
    density = kde(data.T)

    df = pd.DataFrame(data, columns=["x", "y", "z"])
    fig = sb.plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(df['x'], df['y'], df['z'],c=density, cmap=cmap, s=22)
    ax.view_init(rota, rotb)

    if(xmin == 0 and xmax == 0 and ymin == 0 and ymax == 0 and zmin == 0 and zmax == 0):
        xmin, xmax = ax.get_xlim()
        ymin, ymax = ax.get_ylim()
        zmin, zmax = ax.get_zlim()

    ax.set_xlim(xmin, xmax)
    ax.set_ylim(ymin, ymax)
    ax.set_zlim(zmin, zmax)
    ax.set_xticklabels([])
    ax.set_yticklabels([])
    ax.set_zticklabels([])
    #ax.set_axis_off()

    if(filepath!=''):
        fig.savefig(filepath,bbox_inches='tight',dpi=200)
    else:
        plt.show()


    return (xmin, xmax, ymin, ymax, zmin, zmax)


def plotsimdata(filepath):
    return

if __name__ == "__main__":

    #EXAMPLE PLOTS
    datax = np.random.uniform(0, 10000, 5000)
    datay = np.random.uniform(0, 10000, 5000)
    dataz = np.random.uniform(0, 10000, 5000)

    plot3ddensity(datax=datax, datay=datay, dataz=dataz)
    #plot2ddensity(datax=datax, datay=datay)
