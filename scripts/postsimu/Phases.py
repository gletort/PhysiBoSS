import sys, os
import numpy as npy
tools_path = '/bioinfo/users/gletort/Invade/PhysiCell/PhysiCell/scripts/postsimu/'
sys.path.append( tools_path )
import Utils as uts
from Functions_Plot import *

def get_one_phases( basedir ):
	""" Calculate the number of cell in each phases at all time points """
	times = uts.get_times( basedir )
	nsurv = []
	napop = []
	nnecr = []
	naive = []

	## to calculate nb of unique cell in each state
	listapop = []
	listnecr = []
	listsurv = []
	listnaiv = []
	for t in times:
		tdata = uts.read_file( basedir, t )
		phase = tdata['phase']
		ind = npy.where(phase<=2)
		ind = ind[0]
		nsurv.append( len(ind) )
		if len(ind) > 1:
			cells = tdata['ID'][ind]
		else:
			cells = []
		listsurv = npy.r_[listsurv,cells]
		listsurv = npy.unique(listsurv)
		ind = npy.where(phase==100)
		ind = ind[0]
		napop.append( len(ind) )
		if len(ind) > 1:
			cells = tdata['ID'][ind]
		else:
			cells = []
		listapop = npy.r_[listapop,cells]
		listapop = npy.unique(listapop)
		ind = npy.where(phase>100)
		ind = ind[0]
		nnecr.append( len(ind) )
		if len(ind) > 1:
			cells = tdata['ID'][ind]
		else:
			cells = []
		listnecr = npy.r_[listnecr,cells]
		listnecr = npy.unique(listnecr)
		ind = npy.where(phase==3)
		ind = ind[0]
		naive.append( len(ind) )
		if len(ind) > 1:
			cells = tdata['ID'][ind]
		else:
			cells = []
		listnaiv = npy.r_[listnaiv,cells]
		listnaiv = npy.unique(listnaiv)
	
	return [times, nsurv, naive, napop, nnecr, listsurv, listapop, listnecr, listnaiv]
	
def do_one_phase( basedir, anapath ):
	""" Calculate and plot n of cells in each phase of one run folder """
	[times, surv, naive, apop, necr, lists, lista, listn, listnaive] = get_one_phases( basedir )
	
	## Plot results
	bfig()
	outname = anapath+'/cells_phases.png'
	plot_one_xy( times, surv, 111, col='g', mark='o' ) 
	plot_one_xy( times, naive, 111, col='b', mark='v' ) 
	plot_one_xy( times, apop, 111, col='r', mark='+' ) 
	plot_one_xy( times, necr, 111, col='k', mark='*' ) 
	plot_title( 'Time (min)', 'Nb of cells' )
	efig_all( outname )

	bfig()
	outname = anapath+'/cells_nphases.png'
	plot_pie( [len(lists), len(lista), len(listn)], 111, col=['g','r','k'], lab=['Survival', 'Apoptosis', 'Necrosis'] )
	efig_all( outname )

def multi_runs_phase( basedir, anapath, nums, para, paraName, grouping=0, vert=-1 ):
	""" Plot together different simulations in run$i folders 
	nums contain the run$i value associated with the parameter value 
	grouping: if plot results by same parameter value
	"""
	sorting = npy.argsort(para)
	sort_para = npy.argsort(sorting)
	colsg = generate_colors(len(nums), 'Greens')
	colsr = generate_colors(len(nums), 'Reds')
	colsb = generate_colors(len(nums), 'Blues')
	colsk = generate_colors(len(nums), 'Greys')
	

	ns = []
	na = []
	nn = [] 
	nnaive = []

	bfig()
	for i in range(len(nums)):
		curdir = basedir+'run'+str(nums[i])
		sorted_num = sort_para[i]
		if not os.path.isdir( curdir ):
			print 'Simulation folder '+curdir+' not found'
		else:
			print 'Processing folder '+curdir+'...'
			[times, surv, naive, apop, necr, lists, lista, listn, listnaive] = get_one_phases( curdir )
			#ns.append(len(lists))
			last = len(surv)
			ns.append(surv[last-1])
			#na.append(len(lista))
			na.append(apop[last-1])
			#nn.append(len(listn))
			nn.append(necr[last-1])
			nnaive.append(naive[len(times)-1])
			if len(npy.unique(para)) > 1:
				plot_one_xy( times, surv, 221, col=colsg(sorted_num), line='-', mark='None' ) 
				plot_one_xy( times, naive, 222, col=colsb(sorted_num), line='-', mark='None' ) 
				plot_one_xy( times, apop, 223, col=colsr(sorted_num), line='-', mark='None' ) 
				plot_one_xy( times, necr, 224, col=colsk(sorted_num), line='-', mark='None' ) 
			else:
				surv = npy.asarray(surv, dtype=npy.float32)
				apop = npy.asarray(apop, dtype=npy.float32)
				naive = npy.asarray(naive, dtype=npy.float32)
				necr = npy.asarray(necr, dtype=npy.float32)
				plot_one_xy( times, surv/surv[0], 111, col='g', line='-', mark='None' ) 
				plot_one_xy( times, naive/surv[0], 111, col='b', line='-', mark='None' ) 
				plot_one_xy( times, apop/surv[0], 111, col='r', line='-', mark='None' ) 
				plot_one_xy( times, necr/surv[0], 111, col='k', line='-', mark='None' )
	
	outname = anapath+'/phases.png'
	if len(npy.unique(para)) > 1:
		plot_title( 'Time (min)', 'Nb of survival cells', 221 )
		plot_title( 'Time (min)', 'Nb of naive cells', 222 )
		plot_title( 'Time (min)', 'Nb of apoptosis cells', 223 )
		plot_title( 'Time (min)', 'Nb of necroptic cells', 224 )
	else:
		if vert > 0:
			v = 0
			while v < max(times):
				add_vline(v)
				v = v + vert
		plot_title( 'Time (min)', '% of cells', 111 )
	efig_all(outname)

	bfig()
	if len(npy.unique(para)) > 1:
		para = npy.asarray(para)
		ns = npy.asarray(ns)
		na = npy.asarray(na)
		nn = npy.asarray(nn)
		nnaive = npy.asarray(nnaive)
		plot_one_xy( para[sorting], ns[sorting], col='g', line='-' )
		plot_one_xy( para[sorting], na[sorting], col='r', line='-' )
		plot_one_xy( para[sorting], nn[sorting], col='k', line='-' )
		plot_one_xy( para[sorting], nnaive[sorting], col='b', line='-' )
		plot_title( ''+paraName, 'Final cell number' )
		outname = anapath+'/para_nphases.png'
	else:
		plot_pie( [npy.mean(ns), npy.mean(na), npy.mean(nn), npy.mean(nnaive)], 111, col=['g','r','k', 'b'], lab=['Survival', 'Apoptosis', 'Necrosis', 'Naive'] )
		outname = anapath+'/mean_nphases.png'
	efig_all(outname)

	# group by same parameter values
	if grouping > 0:
		for curpara in npy.unique(para):
			bfig()
			keep = (para == curpara)
			plot_pie( [ npy.mean(ns[keep]), npy.mean(na[keep]), npy.mean(nn[keep]), npy.mean(nnaive[keep]) ], 111, col=['g','r','k','b'], lab=['Survival', 'Apoptosis', 'Necrosis', 'Naive'] )
			outname = anapath+'/pie_phase_'+str(curpara)+'.png'
			efig_all(outname)
		


def do_phase( basedir, anapath, paraName='NULL', group=0, vert=-1 ):
	""" Calculate nb of cells for each phase of current folder"""
	if uts.is_run( basedir ) >= 0:
		do_one_phase( basedir, anapath )
	elif uts.contains_runs( basedir ) >=0:
		[nums, para] = uts.extract_parameters( anapath, paraName )
		multi_runs_phase( basedir, anapath, nums, para, paraName, group, vert )
	else:
		print 'Given directory is not a run directory nor contains run$i subfolders'
