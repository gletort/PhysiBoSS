# coding: utf-8
# MaBoSS (Markov Boolean Stochastic Simulator)
# Copyright (C) 2011-2018 Institut Curie, 26 rue d'Ulm, Paris, France
   
# MaBoSS is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
   
# MaBoSS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
   
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 

# Module: maboss/comm.py
# Authors: Eric Viara <viara@sysra.com>
# Date: May-December 2018

#
# to add in environment before running this script:
# export MABOSS_SERVER=../../engine/src/MaBoSS-server
#

from __future__ import print_function
import maboss.comm, maboss.simul, maboss.result, sys

# MaBoSS client instantiation
# - the MaBoSS client forks a MaBoSS-server (defined as default as "MaBoSS-server" or in MABOSS_SERVER environment variable)
# - this MaBoSS client can be used for multiple simulations
mbcli = maboss.comm.MaBoSSClient()

# create a simulation (network + config)
TESTLOC = "../../engine/tests/"
simulation = maboss.simul.Simulation(bndfile = TESTLOC + "/cellcycle-bad.bnd", cfgfiles = [TESTLOC + "/cellcycle_runcfg.cfg", TESTLOC + "/cellcycle_runcfg-thread_1-simple.cfg"])

# run the simulation, the forked MaBoSS-server will be used
check = True
check = False
verbose = False
augment = False
override = False
augment = True
#override = True
result = mbcli.run(simulation, {"check" : check, "hexfloat" : True, "augment" : augment, "override" : override, "verbose" : verbose}) # will call Result(mbcli, simulation)

# get the returned data (notice the data is not checkd)
result_data = result.getResultData()

# prints the returned data
if result_data.getStatus():
    print("result_data status=", result_data.getStatus(), "errmsg=", result_data.getErrorMessage(), file=sys.stderr)
if result_data.getStatus() == 0: # means Success
    print("FP", result_data.getFP())
    print("Runlog", result_data.getRunLog())
    print("ProbTraj", result_data.getProbTraj())
    print("StatDist", result_data.getStatDist())


