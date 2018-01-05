"""
All analyzes: read result datas and analyze them

Usage: Analyze [-w What] [-i input_folder] [-p parameter] [-t time] [-o other_parameter]

Parameters:
	what: choose analyze to do. Possible choices (radius, cellcell, traj, phase, density, density_proj, BN)
		  default: radius
	input_folder: path to folder containing the runs folders
				  default: './'
	time:		precise time to analyse if not all
	para:         parameter name that is varied between the different run folders
	other:		additionnal parameter when needed

	
		* analyse density: plot the given density (precised by para argument, e.g. para=tnf) along the radius (distance to the point (0,0)).
		* analyse density_proj: plot the 2d color map of the density values, all z projetted
		* analyse phase: plot the number of cells in each phase (naive, apoptosis, necrosis, proliferating) according to time
		* analyse cellline: plot the number of cells in each phase cell line according to time
		* analyse biphase: plot the number of cells in each phase according to 2 given parameters values
		* analyse report: read the report files and plot the mean density value according to time (density precised in 'other' argument)
		* analyse overlay: overlay the tnf average concentration as grey background and phases numbers as lines
		* analyse overline: overlay the tnf average concentration as grey background and number of cell in each cell lines
		* analyse BN: analyse BN state of survival cells at last time points (or other adapatation)


Creation: May 2017, G. Letort, Institute Curie
"""
import sys, getopt, os

# read input parameters
basedir = './'
what = 'radius'
param = 'NULL'
other = 'NULL'
time = 0
vert = -1

try:
	opts, args = getopt.getopt( sys.argv[1:], "h:i:w:p:t:o:v:", ["help", "input=", "what=", "para=", "time=", "other=", "vert="] )
except getopt.GetoptError:
	#usage()
	sys.exit(2)
for opt, arg in opts:
	if opt in ("-h", "--help"):
		usage()
		sys.exit()
	elif opt in ("-i", "--input"):
		basedir = arg
	elif opt in ("-w", "--what"):
		what = arg
	elif opt in ("-p", "--para"):
		param = arg
	elif opt in ("-t", "--time"):
		time = int(arg)
	elif opt in ("-o", "--other"):
		other = arg
	elif opt in ("-v", "--vert"):
		vert = int(arg)


anapath = basedir+'analyses/'
if not os.path.isdir( anapath ):
	os.makedirs( anapath )

if what == 'radius':
	import Distances as dst
	dst.do_radius( basedir, anapath )
elif what == 'cellcell':
	import Compression as comp
	comp.do_cell_cell( basedir, anapath, time )
elif what == 'traj':
	import Trajectories as traj
	traj.do_trajectory( basedir, anapath )
elif what == 'phase':
	import Phases as ph
	group = 0
	if other == 'group':
		group = 1
	ph.do_phase( basedir, anapath, param, group, vert )
elif what == 'BN':
	import BNState as bn
	bn.survivalState( basedir, anapath, param )
elif what == 'cellline':
	import CellLine as cl
	group = 0
	if other == 'group':
		group = 1
	cl.do_cellline( basedir, anapath, param, group )
elif what == 'biphase':
	import BiPhases as bph
	bph.do_phase( basedir, anapath, param, other )
elif what == 'density':
	import Microenv as micro
	micro.do_density( basedir, param, anapath, time, other )
elif what == 'density_proj':
	import Microenv as micro
	micro.proj_density( basedir, param, anapath, time )
elif what == 'report':
	import Report as rep
	rep.report_density( basedir, anapath, param, other )
elif what == 'overlay':
	import OverlayPhaseEnv as over
	over.phase_env( basedir, anapath, param, other )
elif what == 'overline':
	import OverlayCellLine as ovline
	ovline.cellline_env( basedir, anapath, param, other )
else:
	print 'Argument not known'


