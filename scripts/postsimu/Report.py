'''

Extract and plot informations from the report.txt files
	option 1: print density: tnf/oxygen... according to time 

'''

import sys, os
import numpy as npy
sys.path.append( './' )
import Utils as uts
from Functions_Plot import *


def get_one_density( basedir, densname ):
	""" Read mean density at all time points from report file """

	filename = basedir+'/report.txt'
	if os.path.isfile( filename ):
		datas = uts.read_column_file( filename, '\t' )
		columns = datas['simulated time']
		columns = [ columns, datas[densname] ]
		return columns
	else:
		print 'File '+filename+' not found'

	
def do_one_density( basedir, anapath, densName ):
    [times, dens] = get_one_density( basedir, densName )
    bfig()
    plot_one_xy( times, dens, 111, line='-', mark='None' ) 
    outname = anapath+'/time_'+densName+'.png'
    plot_title( 'Time (min)', 'Mean '+densName, 111 )
    efig_all(outname)

def multi_runs_density( basedir, anapath, nums, para, paraName, densName ):
	""" Plot together different simulations in run$i folders 
	nums contain the run$i value associated with the parameter value 
	"""
	sorting = npy.argsort(para)
	sort_para = npy.argsort(sorting)
	colsp = generate_colors(len(nums), 'Purples')

	dens = [] 

	bfig()
	for i in range(len(nums)):
		curdir = basedir+'run'+str(nums[i])
		sorted_num = sort_para[i]
		if not os.path.isdir( curdir ):
			print 'Simulation folder '+curdir+' not found'
		else:
			print 'Processing folder '+curdir+'...'
			[times, dens] = get_one_density( curdir, densName )
			if len(npy.unique(para)) > 1:
				plot_one_xy( times, dens, 111, col=colsp(sorted_num), line='-', mark='None' ) 
			else:
				plot_one_xy( times, dens, 111, col=colsp(len(nums)-1), line='-', mark='None' ) 
	
	outname = anapath+'/time_'+densName+'.png'
	plot_title( 'Time (min)', 'Mean '+densName, 111 )
	efig_all(outname)



def report_density( basedir, anapath, paraName='NULL', densName='tnf' ):
	""" Plot the mean density according to time from the report files """
	if uts.is_run( basedir ) >= 0:
		do_one_density( basedir, anapath, densName )
	elif uts.contains_runs( basedir ) >=0:
		[nums, para] = uts.extract_parameters( anapath, paraName )
		multi_runs_density( basedir, anapath, nums, para, paraName, densName )
	else:
		print 'Given directory is not a run directory nor contains run$i subfolders'
