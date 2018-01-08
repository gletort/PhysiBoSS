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

#include "BioFVM_agent_container.h"
#include "BioFVM_basic_agent.h"
#include "BioFVM_vector.h"


namespace BioFVM{

Agent_Container::Agent_Container() : agent_grid(), all_basic_agents()
{}

Agent_Container::~Agent_Container()
{
	while ( all_basic_agents.size() > 0 )
	{
		Basic_Agent* agent = all_basic_agents.back();
		all_basic_agents.pop_back();
		remove_agent( agent ); 
		delete agent;
	}
}

/**\todo don't remove in outer voxel list, check if emptied somewhere else ?*/
void Agent_Container::remove_agent_from_voxel( Basic_Agent* agent, int voxel_index )
{
	// check if on the grid
	if ( voxel_index == -1 )
	{
		return;
	}
	int delete_index = 0;

	while( agent_grid[voxel_index][ delete_index ] != agent )
	{
		delete_index++;
		if ( delete_index == (int) agent_grid[voxel_index].size() )
			return;
	}

	// move last item to index location  
	agent_grid[voxel_index][delete_index] = agent_grid[voxel_index][agent_grid[voxel_index].size()-1 ]; 
	// shrink the vector
	agent_grid[voxel_index].pop_back(); 
}		

/** Remove agent from mechanics grid */
void Agent_Container::remove_agent( Basic_Agent* agent )
{
	remove_agent_from_voxel( agent, agent->get_current_mechanics_voxel_index() );
}

/** Erase and remove agent of given index */
void Agent_Container::delete_basic_agent( int index )
{
	// deregister agent in microenvironment
	remove_agent(all_basic_agents[index]);
	// de-allocate (delete) the Basic_Agent; 
	delete all_basic_agents[index]; 

	// next goal: remove this memory address. 

	// performance goal: don't delete in the middle -- very expensive reallocation
	// alternative: copy last element to index position, then shrink vector by 1 at the end O(constant)

	// move last item to index location  
	all_basic_agents[ all_basic_agents.size()-1 ]->index = index;
	all_basic_agents[index] = all_basic_agents[ all_basic_agents.size()-1 ];

	// shrink the vector
	all_basic_agents.pop_back();
}

/** Erase and remove given agent */
void Agent_Container::delete_basic_agent( Basic_Agent* pDelete )
{
	delete_basic_agent(pDelete->index);
}

/** When initializing cells, set the uptake constants value */
void Agent_Container::set_internal_uptake( double dt )
{
	for ( auto agent: all_basic_agents )
	{
		if ( agent->active() )
			agent->set_internal_uptake_constants( dt );
	}
	std::cout << all_basic_agents.size() << " agents created succesfully" << std::endl;
}

/** Simulate secretion and uptake of cells at nearest voxel in microenvironment */
void Agent_Container::simulate_cell_sources_and_sinks( Microenvironment* m, double dt )
{
	#pragma omp parallel for
	for( int i=0 ; i < (int) all_basic_agents.size() ; i++ )
	{
		if ( all_basic_agents[i]->active() )		
			all_basic_agents[i]->simulate_secretion_and_uptake( m, dt ); 
	}
}


};
