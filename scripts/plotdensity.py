import seaborn as sb
import numpy as np
import scipy as sp
import pandas as pd
from mpl_toolkits.mplot3d import Axes3D

#https://python-graph-gallery.com/370-3d-scatterplot/

cmap = sb.cubehelix_palette(as_cmap=True, dark=0, light=1, reverse=True);

def plot2ddensity(datax, datay, filepath, time=0, xmin=0, xmax=0, ymin=0, ymax=0):

	data=np.vstack((datax,datay));
	data=np.transpose(data);
	df = pd.DataFrame(data,columns=["x","y"]);

	fig,ax = sb.plt.subplots();
	sb.kdeplot(df.x,df.y, cmap=cmap, n_levels=60, shade=True, ax=ax);

	if(xmin==0 and xmax==0 and ymin==0 and ymax==0) :
		xmin, xmax = ax.get_xlim();
		ymin, ymax = ax.get_ylim();

	df.plot.scatter(x='x',y='y',ax=ax, c='w', alpha=0.4, s=22);

	sb.plt.xlim(xmin,xmax);
	sb.plt.ylim(ymin,ymax);

	title="evolution of star formation at t=" + str(time);
	sb.plt.title(title)
	sb.plt.ylabel('y').set_rotation(0);

	#fig.savefig(filepath,bbox_inches='tight',dpi=200);
	sb.plt.show();


	return;



if __name__=="__main__":
   	datax = np.random.uniform(0,10000,5000);
	datay = np.random.uniform(0,10000,5000);
	dataz = np.random.uniform(0,10000,5000);

	plot2ddensity(datax=datax, datay=datay, filepath='test.png');
