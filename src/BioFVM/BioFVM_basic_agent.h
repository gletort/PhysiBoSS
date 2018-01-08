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

#ifndef __BioFVM_basic_agent_h__
#define __BioFVM_basic_agent_h__

#include <vector>
#include "BioFVM_microenvironment.h"
#include "BioFVM_matlab.h"
#include "BioFVM_vector.h"
#include "../base/vector3d.h"
#include "BioFVM_agent_container.h"
#include "../core/PhysiCell_constants.h"

namespace BioFVM
{

class Basic_Agent
{
 private:
	int selected_microenvironment; 
	
	int current_microenvironment_voxel_index;
	bool volume_is_changed;
	int current_voxel_index;	

 protected:
	Microenvironment* microenvironment; 
	/** \brief Reference to all cells container, must be set */
	Agent_Container* container;
	
	double vol;
	
	/** \brief Type of the agent (tumor, vessel or passive) */
	int type;
	
	/** \brief Friction coefficient of moving cell */
	double friction_coeff;

	std::vector<double> secretion_rates; 
	std::vector<double> saturation_densities; 
	std::vector<double> uptake_rates;  
	/** \brief amount of density bounded to the cell*/
	std::vector<double> bounded;  
	
	int current_mechanics_voxel_index;
	int updated_current_mechanics_voxel_index; // keeps the updated voxel index for later adjusting of current voxel index
	std::vector<double> cell_source_sink_solver_temp1;
	std::vector<double> cell_source_sink_solver_temp2;
	Vector3d previous_velocity; 
	bool is_active;
	bool is_moving;
	bool is_out_of_domain;
	int ID; 
	Vector3d position;  
	Vector3d velocity; 
	Vector3d displacement;

	/** \brief Amount of contact with other cells */
	double cell_contact;	
	/** \brief Amount of contact with ECM */
	double ecm_contact;	
	/** \brief Amount of nucleus deformation (contact with other cells nucleus) */
	double nucleus_deform;	

 public:
	int index; 
	
	Basic_Agent(); 
	virtual ~Basic_Agent(); 

	inline bool active() { return is_active; };
	inline bool passive() { return type == PhysiCell_constants::PASSIVE_TYPE; };
	
	inline void set_type(int t) { type = t; } ;

	double get_total_volume();
	void set_total_volume(double);
	void update_voxel_index();
	void update_voxel_in_container();
	/** Update cell indices in the container */
	void update_cell_index_container();
	
	void set_internal_uptake_constants( double dt ); // any time you update the cell volume or rates, should call this function. 

	void register_microenvironment( Microenvironment* );
	Microenvironment* get_microenvironment();
	/** \brief Return number of densities in µenv */
	inline int number_of_microdensities() const { return microenvironment->number_of_densities(); }	
	/** \brief Return the common container of all the cells */
	inline Agent_Container * get_container() { return container; };
	/** \brief Refers the current container to the given container */
	inline void set_container( Agent_Container* cont ) { container = cont; };

	
	//bool assign_position(double x, double y, double z);
	bool assign_position(std::vector<double> new_position);
	bool assign_position(Vector3d new_position);
	/** \brief Return vector of current agent position */
	inline Vector3d get_position() const { return position; };
	/** \brief Register info that agent is out of domain */
	inline void put_out() { is_out_of_domain = true; };

		/** \brief Update agent current position vector */
	void update_position( double dt );
	
	void update_cell_motion( double dt, double l, std::string shape );

	/** \brief Update agent velocity (active or passive) */
	void update_velocity( double dt, double l, std::string shape ); 
	
	/** \brief Calculate agent distance to BM if defined */
	double distance_to_membrane( double l, std::string shape);
	/** \brief Distance of agent to BA for duct geometry */
	double distance_to_membrane_duct( double l);
	/** \brief Distance of agent to BA for sphere geometry */
	double distance_to_membrane_sphere( double l);
	/** \brief Distance to membrane Sheet
	 * Basement membrane is a sheet of height 2*BM_radius 
	 * Z value is in between -BM_radius and +BM_radius
	 */
	double distance_to_membrane_sheet(double length);
	
	/** \brief Return current value of mechanics voxel index */
	inline int get_current_mechanics_voxel_index() { return current_mechanics_voxel_index; };

	// simulate secretion and uptake at the nearest voxel at the indicated microenvironment.
	// if no microenvironment indicated, use the currently selected microenvironment. 
	void simulate_secretion_and_uptake( Microenvironment* M, double dt ); 

	int get_current_voxel_index( void );
    /** \brief Return current voxel volume */
	double microvoxel_volume();

	// directly access the substrate vector at the nearest voxel at the indicated microenvironment 
	std::vector<double>& nearest_density_vector( int microenvironment_index ); 
	std::vector<double>& nearest_density_vector( void );

	/** \brief return value of sphere or cell radius */
	virtual double current_radius() = 0;
	/** \brief Try: Radius depending on aligment with current polarity */
	virtual double polarized_radius( Vector3d dir ) = 0; 
	/** \brief return value of sphere or cell nuclear radius */
	virtual double current_nuclear_radius() = 0;
	virtual double get_repulsion() = 0;
	virtual double get_adhesion() = 0;
	/** \brief Return current friction coefficient of the agent */
	virtual double get_friction() = 0;
	virtual void add_cell_basement_membrane_interactions( double dt, double distance ) = 0;
	virtual void set_motility( double dt ) = 0;
	virtual void update_polarization(double dt) = 0;
	virtual double max_interaction() = 0;
	virtual void output( std::string& delimeter, std::ofstream* file ) = 0;

	/** \brief Return index of given field if it's in the microenv */
	inline int density_index( std::string field )
	{ return microenvironment->get_index(field); };

	/** \brief Add repulsion/adhesion strength with given agen (Sphere or Cell) */
	void add_potentials(Basic_Agent* other_agent);

	/** \brief Calculate repulsion and adhesion between agent and ecm at given voxel index
	 *
	 * @param index_ecm index of the ECM density in the microenv vector of densities
	 * @param index_voxel index of the current ECM voxel  */
	void add_ecm_interaction( int index_ecm, int index_voxel );

	/** \brief Return coefficient of repulsion strength with ecm */
	virtual double get_ecm_repulsion() = 0;

	/** \brief function to check if a neighbor voxel contains any cell that can interact with me */
	bool is_neighbor_voxel(std::vector<double> my_voxel_center, std::vector<double> other_voxel_center, int other_voxel_index);

};

};

#endif

