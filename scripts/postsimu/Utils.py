import sys, os, re, math
import numpy as npy
tools_path = '/bioinfo/users/gletort/Invade/PhysiCell/PhysiCell-MaBoSS/scripts/postsimu/'
sys.path.append( tools_path )

########## Files/Dirs functions 

def is_time( filename, basename='cells_' ):
	""" Check if given file is an output file
	And extract its time information if yes
	"""
	if os.path.isfile( filename ):
		ind = filename.find( basename )
		if ind >= 0:
			num = filename[ind+len(basename):]
			num = num[ 0 : (len(num)-4) ]
			return int(num)
	return -1

def get_times( datapath, name='/output/', filename='cells_' ):
	""" Return sorted times list """
	fold = datapath+name
	filelist = os.listdir( fold )
	times = []
	for f in filelist:
		res = is_time( os.path.join(fold, f), filename )
		if res != -1:
			times.append( res )
	return npy.sort(npy.unique(times))

def column_names( filein, delim=';' ):
	""" read first line to get the column names """
	try:
		reading = open( filein, 'r' )
		line = reading.readline().strip()
		reading.close()
		col_names = re.split(delim, line)
	except Exception as e:
		raise Exception( "Problem while loading file "+filein+" :"+str(e) )
	return col_names

def read_file( datapath, time, name="/output/cells_", what='float' ):
	""" Read the output file of given time
	And return array of values 
	"""
	filename = datapath+name+str(time).zfill(5)+'.txt'
	return read_column_file( filename, what=what )


def read_column_file( fily, delim=';', what='float' ):
	""" read a column file of given name and return content """
	cnames = column_names( fily, delim )
	#dt = zip( cnames, (what,)*len(cnames) )
	nc = len(cnames)
	dt = zip( cnames[:(nc-1)], ((what,)*(nc)))
	dt.append( (cnames[(nc-1)], 'S32',) )
	datas = npy.loadtxt( fily, dt, delimiter=delim, skiprows=1 )
	return datas

def read_micro_file( datapath, what, time, name="/microutput/" ):
	""" Read the output file of given time
	And return array of values 
	"""
	filename = datapath+name+what+'_t'+str(time).zfill(5)+'.txt'
	cnames = [ 'x', 'y', 'z', 'dens' ]
	dt = zip( cnames, ('float',)*len(cnames) )
	datas = npy.loadtxt( filename, dt, delimiter=';', skiprows=0 )
	return datas


def is_run( dirpath ):
	""" Check if given directory is a run$i folder
	If so, return its number $i
	Else return -1
	"""
	dirpath = os.path.abspath(dirpath)
	if os.path.isdir( dirpath ):
		last = os.path.basename( dirpath )
		if last.find('run') == 0:
			return int(last[3:])
	return -1

def contains_runs( dirpath ):
	""" Check if given directory contains the run$i folders """
	if os.path.isdir( dirpath ):
		for d in os.listdir( dirpath ):
			if is_run( os.path.join(dirpath,d) ) >= 0:
				return 1
	return -1


def has_file( tdir, endfile ):
	""" check if input directory contains file finishing as specified """
	for fname in os.listdir( tdir ):
		if fname.endswith( endfile ):
			return True
	print ' result file '+tdir+' *'+endfile+' not found'
	return False


########## Handle parameters file

def readline_parameter( line, which ):
	""" read current line and extract which parameter """
	cols = line.split('\t')
	if which == 'NULL':
		return [cols[0], 0]
	
	param = cols.index(which)
	return [cols[0], cols[param+1]]


def extract_parameters( anpath, which, which2='NULL' ):
	""" read the parameter which values in run$i folders 
		Optionnal look also for which2 parameter """
	infile = anpath+"parameters.txt"
	nums = []
	para = []
	para2 = []
	#if not os.path.isfile( infile ):
	#	print 'Warning: parameter file '+infile+' doesn t exist. Creating it..'
	cmd = 'python '+tools_path+'Parameter.py ./run* '+which+' '+which2
	os.system(cmd)
	
	f = open( infile, 'r' )
	for line in f.readlines():
		if line.find( which ) >= 0:
			[num, val] = readline_parameter( line, which )
			nums = nums + [int(num)]
			para = para + [float(val)]
			if which2 != 'NULL':
				[num, val2] = readline_parameter( line, which2 )
				para2 = para2 + [float(val2)]
	f.close()
	if which2 != 'NULL':
		return [ nums, para, para2 ]
	return [ nums, para ]




######### Calculation functions

def point_norm( x, y, z ):
	""" return the norm of the given point """
	return math.sqrt( x*x + y*y + z*z )

def vnorm( xs, ys, zs ):
	""" norm of each point of the list/array """
	return npy.vectorize( point_norm )(xs,ys,zs)

def norm( xs, ys, zs ):
	""" Norm of list of points """
	res = []
	for i in range(0,len(xs)):
		res.append( math.sqrt( xs[i]*xs[i] + ys[i]*ys[i] + zs[i]*zs[i] ) )
	return res

def distance( x1, y1, z1, x0, y0, z0 ):
	""" Distance between two 3D points """
	dx = x1 - x0
	dy = y1 - y0
	dz = z1 - z0
	return math.sqrt( dx*dx + dy*dy + dz*dz )


############ Special PhysiCell functions

def phase_color( phase ):
	""" associate a color with given phase number """
	if phase == 0:
		return 'green'
	if phase == 1:
		return 'darkgreen'
	if phase == 2:
		return 'skyblue'
	if phase == 3:
		return 'cornflowerblue'
	if phase == 100:
		return 'darkred'
	if phase == 101:
		return 'dimgrey'
	if phase == 102:
		return 'grey'
	if phase == 103:
		return 'darkgrey'
	return 'yellow'

def keep_lines( din, keep ):
	""" Keep lines of din of index keep """
	dout = []
	for k in keep:
		dout.append( din[k] )
	return dout

######### Previous
def mean_speed( df, tstep, dim ):
	""" Average speed of each cell """
	idx = np.unique(df['id'])

	mspeed = []
	for cellid in idx:
		mycell = df['id']==cellid
		if len( df[mycell]['t'] ) > 2:
			[t, speed] = cell_speed( df, mycell, tstep, dim )
			if not math.isnan(mean(speed)):
				mspeed = r_[ mspeed, mean(speed) ]

	return mspeed


def cell_speed( df, mycell, tstep, dim ):
	""" Calculate cells speed """
	x = df[mycell]['x']
	y = df[mycell]['y']
	if dim == 3:
		z = df[mycell]['z']
	fin = len(x)

	xshift = x[2:]
	x = x[1:fin-1]
	yshift = y[2:]
	y = y[1:fin-1]
	if dim == 3:
		zshift = z[2:]
		z = z[1:fin-1]
		speed = (xshift-x)*(xshift-x) + (yshift-y)*(yshift-y) + (zshift-z)*(zshift-z)
	else:
		speed = (xshift-x)*(xshift-x) + (yshift-y)*(yshift-y) 
	
	speed = sqrt(speed)
	speed = speed / tstep
		
	t = (df[mycell]['t'][2:] + df[mycell]['t'][1:fin-1])/2.0
	
	return [t, speed]



def analyse_invading( df, radlim ):
	""" Analyze the panda dataframe for invasion quantification:
	return percentage of cells further than given radius radlim
	return maximal radius reached
	"""
	# create a list of all cell id's
	idx = unique(df['id'])

	# calc n_out at final time 
	# max dist reach any time
	outs = [0.0, 0, 0, 0.0]     # nb cells out of subtypes
	ncells = [0.0, 0, 0, 0.0]   # total nb cells of subtypes
	maxRad = [0.0, 0, 0, 0.0]   # max dist reached of subtypes
	
	for cellid in idx:
		celldf = (df['id']==cellid)
		time = df[celldf]['t']
		tlast = max( time )
		last = time == tlast
		x = df[celldf][last]['x']
		y = df[celldf][last]['y']
		z = df[celldf][last]['z']
		rad = math.sqrt( x*x + y*y + z*z )
		out = rad > radlim
		subt = int( df[celldf][last]['subtype'] )
		if out:
			outs[subt] = outs[subt] + 1.0
		ncells[subt] = ncells[subt] + 1.0
		maxRad[subt] = max( maxRad[subt], rad )

	return [ maxRad, outs, ncells ]



def calculate_rank(vector):
	a={}
	rank=1
	for num in sorted(vector):
		if num not in a:
			a[num]=rank
			rank=rank+1
	return[a[i] for i in vector]


def inbetween( x, dmin, dmax ):
	""" return true if x in between [dmin, dmax} """
	return ( x >= dmin and x < dmax )

def hill_func( x, lamb, h ):
	""" Hill function """
	x = npy.asarray(x)
	return ( npy.power(x, lamb) / ( npy.power(h,lamb) + npy.power(x,lamb) ) )
