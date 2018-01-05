"""

Functions for basic plotting

"""
import matplotlib
matplotlib.use('Cairo')
import matplotlib.pyplot as plt
import pylab as pyl
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

def plot_one_xy( x, y, splot=111, col='b', mark='o', line='None', lab='' ):
	""" plot y = f(x) """

	ax = plt.subplot(splot)
	plt.plot( x, y, color=col, marker=mark, markersize=7.5, linestyle=line, linewidth=2.5, label=lab )
	ax.yaxis.tick_left()
	ax.xaxis.tick_bottom()
	plt.locator_params(axis='y', nbins=6)
	plt.locator_params(axis='x', nbins=8)
	if lab != '':
		plt.legend( loc='upper left' )

def plot_multi_y( x, y, splot=111, mark='o', line='None', lab='' ):
	""" plot ys = f(x) """

	ax = plt.subplot(splot)
	#cmap = plt.get_cmap('PuOr')
	cmap = [ '#FF9632', '#0064C8' ]
	cmap = [ '#FF9632', '#64BEFF' ]
	cmap = [ '#FF9632', '#6400C8' ]
	#cmap = [ '#FF9632', '#0000C8' ]
	colors = [cmap[i] for i in range( len(y[0])) ]
	ax.set_color_cycle(colors)

	plt.plot( x, y, marker=mark, markersize=7.5, linestyle=line, linewidth=2.5, label=lab )
	#for i in range(len(y)):
	#	plt.plot( x, y[i], marker=mark, linestyle=line, label=lab )
	ax.yaxis.tick_left()
	ax.xaxis.tick_bottom()
	plt.locator_params(axis='y', nbins=6)
	plt.locator_params(axis='x', nbins=7)
	if lab != '':
		plt.legend( loc='upper left' )

def plot_pie( x, splot=111, col='b', lab='' ):
	""" plot pie graphic of x """

	plt.subplot(splot)
	_, _, autotext = plt.pie( x, colors=col, shadow=True, labels=lab, autopct='%1.1f%%')
	for autot in autotext:
		autot.set_color('white')
	plt.axis('equal')

def plot_errorbar( x, y, ystd, splot=111, col='b', mark='o', line='-', lab='' ):
	""" plot y = f(x) with error bars """

	ax = plt.subplot(splot)
	plt.errorbar( x, y, ystd, color=col, marker=mark, linestyle=line, linewidth=2.5, label=lab )
	ax.yaxis.tick_left()
	ax.xaxis.tick_bottom()
	plt.locator_params(axis='y', nbins=6)
	plt.locator_params(axis='x', nbins=8)
	if lab != '':
		plt.legend( loc='upper left' )

def scatter_one_xy( x, y, splot=111, col='b', mark='o', lab='' ):
	""" plot y = f(x) """

	ax = plt.subplot(splot)
	plt.scatter( x, y, color=col, marker=mark, label=lab )
	ax.yaxis.tick_left()
	ax.xaxis.tick_bottom()
	if lab != '':
		plt.legend( loc='upper left' )

def plot_xyz( x, y, z, splot=111, col='b', mark='o', lab='' ):
	""" plot in 3D """
	pyl.plot( x, y, z, color=col, marker=mark, label=lab )

def log_scale( splot=111, x=False, y=False ):
	""" put axis to log scale if True """
	ax = plt.subplot( splot )
	if x:
		ax.set_xscale('log')
	if y:
		ax.set_yscale('log')

def plot_title( xname='', yname='', splot=111, intitle='' ):
	""" add axes names, plot title """
	ax = plt.subplot( splot )
	ax.set_xlabel( xname, fontsize=30 )
	ax.set_ylabel( yname, fontsize=30 )
	ax.set_title( intitle )
	for label in ax.get_xticklabels() + ax.get_yticklabels():
		label.set_fontsize(25)

def barplot_n_xy( x, ys, n, xname='', yname='', intitle='', splot=111, col='b' ):
	""" 
	barplot of ys sort by x 
		n : number of ys bar to plot
		x : names of the categories
		ys: list of ys values
	"""

	col = [ '#FF9632', '#0064C8', '#64BEFF', '#6400C8' ]
	plt.subplot(splot)
	index = np.arange(n)
	#barw = 1.0/(n+1)
	barw = 0.4
	#for i in range(0,n):
	#	plt.bar( index+i*barw, ys[i], barwidth=barw, color=col[i], label=x[i] )
	ax = plt.bar( index*4, ys, width=barw*4, color=col, label=x)
	plt.xticks([])
	#plt.set_xticklabels(x)
	
	rects = ax.patches
	for rect, label in zip(rects, x):
		height = rect.get_height()
		plt.text(rect.get_x() + rect.get_width()/2, height+5, label, ha='center', va='bottom')

	plt.xlabel( xname )
	plt.ylabel( yname )
	plt.title( intitle )


def multibar( x, ys, y2, n, xname='', yname='', intitle='', splot=111, col='b' ):
	""" 
	barplot of ys sort by x 
		n : number of ys bar to plot
		x : names of the categories
		ys: list of ys values
	"""

	col = [ '#FF9632', '#FF9632', '#FF9632', '#FF9632' ]
	col2 = [ '#FF9632', '#0064C8', '#64BEFF', '#6400C8' ]
	plt.subplot(splot)
	index = np.arange(n)
	#barw = 1.0/(n+1)
	barw = 0.4
	#for i in range(0,n
	#	plt.bar( index+i*barw, ys[i], barwidth=barw, color=col[i], label=x[i] )
	ax = plt.bar( index*3, ys, width=barw*2, color=col, label='None')
	print y2
	plt.bar( index*3+barw*2, y2, width=barw*2, color=col2, label='None')
	plt.xticks([])
	#plt.set_xticklabels(x)
	
	rects = ax.patches
	#for rect, label in zip(rects, x):
	#	height = rect.get_height()
	#	plt.text(rect.get_x() + rect.get_width()/2, height+5, label, ha='center', va='bottom')

	plt.xlabel( xname )
	plt.ylabel( yname )
	plt.title( intitle )

def boxstrip( x, ys, n, xname='', yname='', intitle='', splot=111, col='b' ):
	""" 
	boxplot + stripchart of ys sort by x 
		n : number of ys bar to plot
		x : names of the categories
		ys: list of ys values
	"""

	col = '#FF9632'
	plt.subplot(splot)
	bp = plt.boxplot(ys, whis='range', widths=[0.75, 0.75, 0.75, 0.75])
	plt.ylim(1.55,1.85)
	for element in bp['medians']:
		element.set_color('grey')
		element.set_linewidth(5)
	for element in bp['boxes']:
		element.set_color('grey')
		#element.set_facecolor('yellow')
		element.set_linewidth(3)
		element.set_linestyle('solid')
		#element.set_fill(True)
		#element.set_hatch('/')
	for element in bp['whiskers']:
		element.set_color('grey')
		element.set_linewidth(3)		
	for element in bp['caps']:	
		element.set_color('grey')

	for i in range(n):
		y = ys[i]
		x = np.random.normal(i+1, 0.2, len(y))
		plt.plot(x, y, col, ls='None', marker='o', markersize=9)

	plt.xticks([])
	plt.xlabel( xname )
	plt.ylabel( yname )
	plt.locator_params(axis='y', nbins=6)
	plt.title( intitle )


def plot_map( x, y, z, colmap='gist_rainbow' ):
	""" Plot a 2d color map """
	import scipy.interpolate
	import numpy as npy
	x = npy.asarray(x)
	y = npy.asarray(y)
	z = npy.asarray(z)
	xm, ym = npy.linspace( x.min(), x.max(), 500 ), npy.linspace( y.min(), y.max(), 500 )
	xm, ym = npy.meshgrid( xm, ym)
	zmesh = scipy.interpolate.griddata( (x, y), z, (xm, ym), method='cubic')
	plt.imshow( zmesh, vmin=0.026, vmax=0.0337, origin='lower', 
	#plt.imshow( zmesh, origin='lower', 
			#extent=[x.min(), x.max(), y.min(), y.max()])
			extent=[x.min(), x.max(), y.min(), y.max()], cmap=plt.get_cmap(colmap) )
	plt.colorbar()
	#plt.plot(x,y,'*', color='k')

def plot_background_map( x, y, colmap='gist_rainbow', ymax=1050 ):
	""" Plot a 2d color map as background """
	import numpy as npy
	x = npy.asarray(x, dtype=npy.float32)
	y = npy.asarray(y, dtype=npy.float32)
	yarr = npy.vstack((y,))
	plt.imshow( yarr, origin='lower', 
			#extent=[x.min(), x.max(), 0, 1050], vmin=0, vmax=0.1, cmap=plt.get_cmap(colmap), alpha=0.5 )
			extent=[0, x.max(), 0, ymax], vmin=0, vmax=25, cmap=plt.get_cmap(colmap), alpha=0.25, aspect='auto' )
	#plt.colorbar()


def generate_colors( n = 10, colmap="gist_rainbow" ):
	""" generate color map of given number """
	return plt.cm.get_cmap( colmap, n )

def add_hline( y=0, col='k', style='-' ):
	""" add an horizontal line at y """
	plt.axhline( y, color=col, linestyle=style )

def add_vline( x=0, col='k', style='-' ):
	""" add a vertical line at x """
	plt.axvline( x, color=col, linestyle=style )

def draw_circle( xcenter, ycenter, radius, col='k', filling=False ):
	""" add a circle on the current plot, of center, radius """
	fig = plt.gcf()
	ax = fig.gca()
	circ = plt.Circle( (xcenter,ycenter), radius, color='k', fill=False )
	ax.add_artist(circ)
	ax.yaxis.tick_left()
	ax.xaxis.tick_bottom()

def dim_plot( dim=2, splot=111 ):
	""" 3D or 2D plotting """
	if dim == 2:
		sfig = pyl.subplot(splot)
	else:
		sfig = pyl.subplot(splot,projection='3d')
		sfig.set_zlabel('z')
		sfig.set_xlabel('x')
		sfig.set_ylabel('y')


def bfig():
	""" Create figure """
	fig = plt.figure(dpi=300)

def efig( filename ):
	""" save figure """
	plt.margins(0.05)
	plt.autoscale(enable=True, axis='both', tight=False)
	plt.show()
	fig = plt.gcf()
	fig.subplots_adjust(left=0.05, right=0.95, bottom=0.1, top=0.9 )
	plt.tight_layout()
	fig.savefig( filename )
	print ''+filename+' saved'

def esplot( splot=111 ):
	""" scale and margined the subplot """
	plt.margins(0.05)
	plt.subplot(splot)
	plt.autoscale(enable=True, axis='both', tight=False)

def efig_all( filename ):
	""" save figure """
	plt.show()
	fig = plt.gcf()
	fig.subplots_adjust(left=0.1, right=0.95, bottom=0.1, top=0.9 )
	fig.tight_layout()
	fig.savefig( filename, bbox_inches="tight" )
	print ''+filename+' saved'
	
