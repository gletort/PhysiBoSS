/*
#############################################################################
# If you use BioFVM in your project, please cite BioFVM and the version     #
# number, such as below:                                                    #
#                                                                           #
# We solved the diffusion equations using BioFVM (Version 1.1.5) [1]        #
#                                                                           #
# [1] A. Ghaffarizadeh, S.H. Friedman, and P. Macklin, BioFVM: an efficient #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2016, Paul Macklin and the BioFVM Project              #
# All rights reserved.                                                      #
#                                                                           #
# Redistribution and use in source and binary forms, with or without        #
# modification, are permitted provided that the following conditions are    #
# met:                                                                      #
#                                                                           #
# 1. Redistributions of source code must retain the above copyright notice, #
# this list of conditions and the following disclaimer.                     #
#                                                                           #
# 2. Redistributions in binary form must reproduce the above copyright      #
# notice, this list of conditions and the following disclaimer in the       #
# documentation and/or other materials provided with the distribution.      #
#                                                                           #
# 3. Neither the name of the copyright holder nor the names of its          #
# contributors may be used to endorse or promote products derived from this #
# software without specific prior written permission.                       #
#                                                                           #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           #
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER #
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    #
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      #
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              #
#                                                                           #
#############################################################################
*/

#ifndef __BioFVM_agent_container_h__
#define __BioFVM_agent_container_h__

#include <vector>
#include "../base/vector3d.h"

namespace BioFVM{

class Basic_Agent; 
class Microenvironment;

class Agent_Container
{
 protected:	

 public:
	/** Grid of agents by nodes */
	std::vector<std::vector<Basic_Agent*> > agent_grid;
	/** Vector containing all the agents in the simulation */
	std::vector<Basic_Agent*> all_basic_agents; 
	
	std::vector<double> max_cell_interactive_distance_in_voxel;

	Agent_Container();	
	virtual ~Agent_Container();	
	/** \brief Return if given position is valid in the mesh or not */
	virtual bool is_position_valid( Vector3d pos ) = 0;
	/** \brief Return index of nearest voxel from position */
	virtual int nearest_voxel_index( Vector3d pos ) = 0;
	virtual void register_agent( Basic_Agent* agent ) {};
	virtual void initialize( int num_voxels ) {};
	virtual void remove_agent( Basic_Agent* agent );
	virtual void add_agent_to_outer_voxel(Basic_Agent* agent) {};
	/** Remove agent from mechanics grid */
	virtual void remove_agent_from_voxel(Basic_Agent* agent, int voxel_index);
	virtual void add_agent_to_voxel(Basic_Agent* agent, int voxel_index) { agent_grid[voxel_index].push_back(agent); };
	virtual void update_all_cells(double dt) {};
	virtual int find_escaping_face_index(Basic_Agent* agent) {return -1;};

	/** \brief Simulate secretion and uptake of cells at nearest voxel in microenvironment */
	void simulate_cell_sources_and_sinks( Microenvironment* m, double dt );

	/** \brief Erase and remove agent of given index */
	void delete_basic_agent( int index );
	/** \brief Erase and remove given agent */
	void delete_basic_agent( Basic_Agent* pDelete );
	/** \brief When initializing cells, set the uptake constants value */
	void set_internal_uptake( double dt );

	/** \brief update position in container if valid */
	virtual int update_voxel_in_container( double x, double y, double z) = 0;

	/** \brief Return voxel index connected in moore neighnorhood */
	virtual std::vector<int> moore_voxels( int index ) = 0;
	/** \brief Return center of voxel at given index */
	virtual std::vector<double> voxel_center( int index ) = 0;
};


};
#endif
