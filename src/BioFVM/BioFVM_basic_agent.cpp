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

#include "BioFVM_basic_agent.h"
#include "../core/cell.h"
#include "../core/sphere.h"
#include "BioFVM_agent_container.h"
#include "BioFVM_vector.h" 

namespace BioFVM
{


	Basic_Agent::Basic_Agent()
	{
		//give the agent a unique ID  
		static int max_basic_agent_ID = 0; 
		ID = max_basic_agent_ID; // 
		max_basic_agent_ID++; 
		// initialize position and velocity
		is_active=true;
		is_moving=true;
		is_out_of_domain=false;

		vol = 1.0; 
		friction_coeff = 0.001;
		type = 0;

		// link into the microenvironment, if one is defined 
		secretion_rates.resize(0);
		uptake_rates.resize(0);
		saturation_densities.resize(0);
		bounded.resize(0);
		extern Microenvironment* default_microenvironment;

		register_microenvironment( default_microenvironment ); 

		current_mechanics_voxel_index = -1;
		return;	
	}

	/* Update agent postion */
	//make sure to update current_voxel_index if you are implementing this function
	void Basic_Agent::update_position(double dt)
	{ 
		if ( is_out_of_domain || !is_moving )
			return;

		// use Adams-Bashforth 
		//static double d1; 
		//static double d2; 
		//static bool constants_defined = false; 
		//if( constants_defined == false )
		//{
		//	d1 = dt; 
		//	d1 *= 1.5; 
		//	d2 = dt; 
		//	d2 *= -0.5; 
		//	constants_defined = true; 
		//}

		// Add friction value
		/** \todo: is friction_coeff just like that ? */
		velocity *= 1.0 / get_friction();

		position += dt*1.5 * velocity;	
		position += -0.5*dt * previous_velocity;

		// update orientation (polarity) vector, active cell only
		if ( !passive() )
			update_polarization(dt);

		// overwrite previous_velocity for future use 
		previous_velocity = velocity; 
		velocity[0]=0; velocity[1]=0; velocity[2]=0;

		// #pragma omp critical
		//{update_voxel_in_container();}
		updated_current_mechanics_voxel_index = container->update_voxel_in_container( position[0], position[1], position[2] );
		return; 
	}


	bool Basic_Agent::assign_position(std::vector<double> new_position)
	{
		return assign_position( Vector3d(new_position[0], new_position[1], new_position[2]) );
	}

	bool Basic_Agent::assign_position( Vector3d pos )
	{
		if( !container->is_position_valid(pos))
		{	
			return false;
		}
		position = pos;

		// update microenvironment current voxel index
		update_voxel_index();
		// update current_mechanics_voxel_index
		current_mechanics_voxel_index = container->nearest_voxel_index( position );
		container->register_agent(this);
		return true;
	}

	void Basic_Agent::update_voxel_index()
	{
		if( !get_microenvironment()->mesh.is_position_valid(position[0],position[1],position[2]))
		{	
			current_voxel_index=-1;
			is_active=false;
			return;
		}

		// @ To Change
		Vector3d pos( position[0], position[1], position[2] );
		current_voxel_index = microenvironment->nearest_voxel_index( pos );
	}

	/** Update cell indices in the container */
	void Basic_Agent::update_cell_index_container()
	{
		if(!is_out_of_domain && is_moving)
			update_voxel_in_container();
	}

	/* Update voxel in the container */
	void Basic_Agent::update_voxel_in_container()
	{
		// call the method from BioFVM_basic_agent to update microenvironment's voxel index
		update_voxel_index();

		if( updated_current_mechanics_voxel_index == -1 )// updated_current_mechanics_voxel_index is updated in update_position
		{
			// check if this agent has a valid voxel index, if so, remove it from previous voxel
			if( current_mechanics_voxel_index >= 0 )
			{
				// #pragma omp critical
				{container->remove_agent_from_voxel(this, current_mechanics_voxel_index);}
			}
			// #pragma omp critical
			{container->add_agent_to_outer_voxel(this);}
			// std::cout<<"cell out of boundary..."<< __LINE__<<" "<<ID<<std::endl;
			current_mechanics_voxel_index=-1;
			is_out_of_domain=true;
			is_active=false;
			return;
		}

		// update mesh indices (if needed)
		if( updated_current_mechanics_voxel_index != current_mechanics_voxel_index )
		{
			// #pragma omp critical
			{
				container->remove_agent_from_voxel(this, current_mechanics_voxel_index);
				container->add_agent_to_voxel(this, updated_current_mechanics_voxel_index);
			}
			current_mechanics_voxel_index = updated_current_mechanics_voxel_index;
		}
	}

	void Basic_Agent::set_internal_uptake_constants( double dt )
	{
		// overall form: dp/dt = S*(T-p) - U*p 
		//   p(n+1) - p(n) = dt*S(n)*T(n) - dt*( S(n) + U(n))*p(n+1)
		//   p(n+1)*temp2 =  p(n) + temp1
		//   p(n+1) = (  p(n) + temp1 )/temp2
		//int nearest_voxel= current_voxel_index;
		double internal_constant_to_discretize_the_delta_approximation = dt * vol / ( (microenvironment->voxels(current_voxel_index)).volume ) ; // needs a fix 
		
		// temp1 = dt*(V_cell/V_voxel)*S*T 
		cell_source_sink_solver_temp1.assign( secretion_rates.size() , 0.0 ); 
		cell_source_sink_solver_temp1 += secretion_rates; 
		cell_source_sink_solver_temp1 *= saturation_densities; 
		cell_source_sink_solver_temp1 *= internal_constant_to_discretize_the_delta_approximation; 
		
		// temp2 = 1 + dt*(V_cell/V_voxel)*( S + U )
		cell_source_sink_solver_temp2.assign( secretion_rates.size() , 1.0 ); 
		axpy( &(cell_source_sink_solver_temp2) , internal_constant_to_discretize_the_delta_approximation , secretion_rates );
		axpy( &(cell_source_sink_solver_temp2) , internal_constant_to_discretize_the_delta_approximation , uptake_rates );	
	}

	void Basic_Agent::register_microenvironment( Microenvironment* microenvironment_in )
	{
		microenvironment = microenvironment_in; 
		if ( !passive() )
		{
			int ndens = microenvironment->density_vector(0).size();	
			secretion_rates.resize( ndens, 0.0 );
			saturation_densities.resize( ndens, 50.0 );
			uptake_rates.resize( ndens, 0.0 );	
			bounded.resize( ndens, 0.0 );	

			// some solver temporary variables 
			cell_source_sink_solver_temp1.resize( ndens , 0.0 );
			cell_source_sink_solver_temp2.resize( ndens , 1.0 );
		}
		return; 
	}

	Microenvironment* Basic_Agent::get_microenvironment( void )
	{ return microenvironment; }

	Basic_Agent::~Basic_Agent()
	{
		// unreferences the pointers
		microenvironment = NULL;
		container = NULL;
	}


	int Basic_Agent::get_current_voxel_index( void )
	{
		return current_voxel_index;
	}

	std::vector<double>& Basic_Agent::nearest_density_vector( void ) 
	{  
		return microenvironment->nearest_density_vector( current_voxel_index ); 
	}

	void Basic_Agent::set_total_volume(double volume)
	{
		this->vol=volume;
		volume_is_changed=true;
	}

	double Basic_Agent::get_total_volume()
	{
		return vol;
	}
	void Basic_Agent::simulate_secretion_and_uptake( Microenvironment* pS, double dt )
	{
		if(!is_active)
		{ return; }

		if(volume_is_changed)
		{
			set_internal_uptake_constants(dt);
			volume_is_changed=false;
		}
		
		// decrease 1 percent per min
		double fact = dt * vol / ( (microenvironment->voxels(current_voxel_index)).volume ) ; 
		bounded -= 0.01 * dt * bounded;
		for ( int i = 0; i < bounded.size(); i++ )
			bounded[i] = bounded[i] > 0 ? bounded[i] : 0;
		bounded += fact * uptake_rates * nearest_density_vector();   

		#pragma omp critical
		{
			(*pS)(current_voxel_index) += cell_source_sink_solver_temp1; 
			(*pS)(current_voxel_index) /= cell_source_sink_solver_temp2; 
		}

		return; 
	}

	/* Add repulsion/adhesion strength with given agen (Sphere or Cell) */
	void Basic_Agent::add_potentials(Basic_Agent* other_agent) 
		/**{
		  if( this->ID == other_agent->ID )
		  return;	

		  displacement = position - other_agent->position;
		  double distance = displacement.norm();   
		// Make sure that the distance is not zero
		distance = std::max(distance, 0.00001);
		Vector3d direction = displacement.dir();	

		//Repulsive
		double R = polarized_radius(direction) + other_agent->polarized_radius(direction);
		double RN = current_nuclear_radius() + other_agent->current_nuclear_radius();

		double temp_r;
		if( distance > R ) 
		temp_r = 0; 
		else
		{
		// repulsion stronger if nucleii overlap, see Macklin et al. 2012, 2.3.1
		if ( distance < RN )
		{
		double M = 1.0;
		double c = 1.0 - RN/R;
		c *= c;
		c -= M;
		temp_r = c*distance/RN + M;
		nucleus_deform += (RN-distance);
		}
		else
		{
		temp_r = 1 - distance/R;
	temp_r *= temp_r;
}
// hyp: repulsion strength is defined by strongest cell (deformability)
temp_r *= std::max( get_repulsion(), (*other_agent).get_repulsion() );
}

//////////////////////////////////////////////////////////////////

R += 1.5; // attraction at longer distance (fillopodes...)
// add 1.5 µm as filopode length 1-2 µm, Mogilner and Rubinstein paper

// Adhesive
if ( distance < R ) 
{	
	double temp_a = 1 - distance/R; 
	temp_a *= temp_a; 

	// hyp: junction strength is limited by weakest cell
	// \todo tmp version: passive cells (sphere) doesn't form adhesion nor cells adhere to them, to change
	double adh = get_adhesion();
	double other_adh = other_agent->get_adhesion();
	if ( adh == 0 || other_adh == 0 )
		adh = 0;
	else
	{
		cell_contact += (R-distance);
		adh = ( static_cast<Cell*>(this) )->adhesion( static_cast<Cell*>(other_agent) );

		temp_a *= adh;
		temp_r -= temp_a;
	}

}
/////////////////////////////////////////////////////////////////
if(temp_r==0)
	return;
	temp_r/=distance;

	velocity += temp_r * displacement;
	return;
	}*/
{
	if( this->ID == other_agent->ID )
		return;	

	displacement = position - other_agent->position;
	double distance = displacement.norm();   
	// Make sure that the distance is not zero
	distance = std::max(distance, 0.00001); 

	//Repulsive
	double R = current_radius() + other_agent->current_radius();
	double RN = current_nuclear_radius() + other_agent->current_nuclear_radius();

	double temp_r;
	if( distance > R ) 
		temp_r = 0; 
	else
	{
		// repulsion stronger if nucleii overlap, see Macklin et al. 2012, 2.3.1
		if ( distance < RN )
		{
			double M = 1.0;
			double c = 1.0 - RN/R;
			c *= c;
			c -= M;
			temp_r = c*distance/RN + M;
			nucleus_deform += (RN-distance);
		}
		else
		{
			temp_r = 1 - distance/R;
			temp_r *= temp_r;
		}
		// hyp: repulsion strength is defined by strongest cell (deformability)
		temp_r *= std::max( get_repulsion(), (*other_agent).get_repulsion() );
	}

	//////////////////////////////////////////////////////////////////

	// Adhesive
	double max_interactive_distance = max_interaction() + other_agent->max_interaction();
	if ( distance < max_interactive_distance ) 
	{	
		double temp_a = 1 - distance/max_interactive_distance; 
		temp_a *= temp_a; 
		// hyp: junction strength is limited by weakest cell
		double adh;
		double thisadh = get_adhesion();
		double otadh = other_agent->get_adhesion();

		// first case, passive cell with active cell
		if ( thisadh == 0 && otadh == 1 )
		{
			ecm_contact += (max_interactive_distance-distance);
			adh = static_cast<Cell*>(other_agent)->integrinStrength();
		}
		else
		{
			// second case, active cell with passive cell
			if ( thisadh == 1 && otadh == 0 )
			{
				ecm_contact += (max_interactive_distance-distance);
				adh = static_cast<Cell*>(this)->integrinStrength();
			}
			else
			{
				// passive, passive
				if ( thisadh == 0 && otadh == 0 )
				{
					adh = 0;
				}
					// active, active
					else
					{
						cell_contact += (max_interactive_distance-distance);
						adh = ( static_cast<Cell*>(this) )->adhesion( static_cast<Cell*>(other_agent) );
					}
				}
		}

		temp_a *= adh;
		temp_r -= temp_a;
	}
	/////////////////////////////////////////////////////////////////
	if(temp_r==0)
		return;
	temp_r/=distance;
	
	velocity += temp_r * displacement;
	return;
}


/* Update cell velocity */
void Basic_Agent::update_cell_motion( double time_since_last, double l, std::string shape )
{
	cell_contact = 0;
	ecm_contact = 0;
	nucleus_deform = 0;
	if( !is_out_of_domain && is_moving)
		update_velocity( time_since_last, l, shape );
}

/* Calculate agent distance to BM if defined */
double Basic_Agent::distance_to_membrane(double l, std::string shape)
{
	if ( l > 0 )
	{
		if ( shape == "duct" )
			return distance_to_membrane_duct(l);
		else if ( shape == "sphere" )
			return distance_to_membrane_sphere(l);
		else if ( shape == "sheet" )
			return distance_to_membrane_sheet(l);
	}
	return 0;
}

/* Distance to membrane Sphere 
 * Basement membrane is a sphere of radius BM_radius 
 * Sphere center is (0,0,0)
 * */
double Basic_Agent::distance_to_membrane_sphere(double length)
{
	double distance_to_origin = position.norm();  // distance to the origin 
	distance_to_origin = std::max(distance_to_origin, EPSILON);	  // prevents division by zero
	displacement = -1 / distance_to_origin * position;
	if ( (length - distance_to_origin) < 0 )
		displacement *= 2.0; // penalize more outside of the sphere cells, stronger rappel
	return fabs(length - distance_to_origin);
}

/* Distance to membrane Sheet
 * Basement membrane is a sheet of height 2*BM_radius 
 * Z value is in between -BM_radius and +BM_radius
 * */
double Basic_Agent::distance_to_membrane_sheet(double length)
{
	double distance = fabs(position[2]);  // |z| position
	distance = std::max(distance, EPSILON);	  // prevents division by zero
	displacement[0] = 0;
	displacement[1] = 0;
	displacement[2] = -1 / distance * position[2];
	if ( (length - distance) < 0 )
		displacement *= 2.0; // penalize more outside of the sphere cells, stronger rappel
	return fabs(length - distance);
}

/// Distance to membrane functions
double Basic_Agent::distance_to_membrane_duct(double length)
{
	//Note that this function assumes that duct cap center is located at <0, 0, 0>
	if ( position[0] >= 0 ) // Cell is within the cylinder part of the duct
	{
		double distance_to_x_axis= position.distance_to_xaxis();
		distance_to_x_axis = std::max(distance_to_x_axis, EPSILON);		// prevents division by zero
		displacement[0]=0; 
		displacement[1]= -position[1]/ distance_to_x_axis; 
		displacement[2]= -position[2]/ distance_to_x_axis; 
		return fabs(length - distance_to_x_axis);
	}

	// Cell is inside the cap of the duct
	double distance_to_origin= position.norm();  // distance to the origin 
	distance_to_origin = std::max(distance_to_origin, EPSILON);			  // prevents division by zero
	displacement = -1 / distance_to_origin * position;
	return fabs(length - distance_to_origin);
}

void Basic_Agent::update_velocity( double dt, double l, std::string shape ) 
{
	double dist = distance_to_membrane(l, shape);
	if ( dist > 0 )
		add_cell_basement_membrane_interactions(dt, dist);


	//First check the neighbors in my current voxel
	for( auto neighbor : container->agent_grid[get_current_mechanics_voxel_index()] )
	{
		add_potentials( neighbor );
	}
	
	int ecm_index = density_index("ecm");
	if ( ecm_index >= 0 )
		add_ecm_interaction( ecm_index, get_current_mechanics_voxel_index() );

	for ( auto neighbor_voxel_index : container->moore_voxels(get_current_mechanics_voxel_index()) )
	{
		if ( !is_neighbor_voxel(container->voxel_center(get_current_mechanics_voxel_index()), container->voxel_center(neighbor_voxel_index), neighbor_voxel_index) )
			continue;
		if ( ecm_index >= 0 )
			add_ecm_interaction( ecm_index, neighbor_voxel_index );
		for( auto other_neighbor : container->agent_grid[neighbor_voxel_index] )
		{
			add_potentials( other_neighbor );
		}
	}

	// Add active motility term
	if ( !passive() )
		set_motility(dt);
}

/* Return current voxel volume */
double Basic_Agent::microvoxel_volume()
{
	return microenvironment->voxel_volume( get_current_voxel_index());
}

/* Calculate repulsion/adhesion between agent and ecm according to its local density */
void Basic_Agent::add_ecm_interaction( int index_ecm, int index_voxel )
{
	// Check if there is ECM material in given voxel
	double dens = microenvironment->nearest_density_vector(index_voxel)[index_ecm];
	// if voxel is "full", density is 1
	dens = std::min( dens, 1.0 ); 
	if ( dens > EPSILON )
	{
		// Distance between agent center and ECM voxel center
		displacement = position - microenvironment->voxel_center(index_voxel);
		double distance = displacement.norm();
		// Make sure that the distance is not zero
		distance = std::max(distance, EPSILON);
		
		double ecmrad = microenvironment->voxel_rad(index_voxel);
		double dd = current_radius() + ecmrad;  
		double dnuc = current_nuclear_radius() + ecmrad;  

		double tmp_r = 0;
		// Cell overlap with ECM node, add a repulsion term
		if ( distance < dd )
		{
			// repulsion stronger if nucleii overlap, see Macklin et al. 2012, 2.3.1
			if ( distance < dnuc )
			{
				double M = 1.0;
				double c = 1.0 - dnuc/dd;
				c *= c;
				c -= M;
				tmp_r = c*distance/dnuc + M;
				nucleus_deform += (dnuc-distance);
			}
			else
			{
				tmp_r = ( 1 - distance / dd );
				tmp_r *= tmp_r;
			}
			tmp_r *= dens * get_ecm_repulsion();
		}

		// Cell adherence to ECM through integrins
		double max_interactive_distance = max_interaction() + ecmrad;
		if ( distance < max_interactive_distance ) 
		{	
			double temp_a = 1 - distance/max_interactive_distance; 
			temp_a *= temp_a; 
			/* \todo change dens with a maximal density ratio ? */
			ecm_contact += dens * (max_interactive_distance-distance);
			temp_a *= dens * ( static_cast<Cell*>(this) )->integrinStrength();
			tmp_r -= temp_a;
		}
		
		/////////////////////////////////////////////////////////////////
		if(tmp_r==0)
			return;
		tmp_r/=distance;

		velocity += tmp_r * displacement;
	}
}

bool Basic_Agent::is_neighbor_voxel(std::vector<double> my_voxel_center, std::vector<double> other_voxel_center, int other_voxel_index)
{
	double max_interactive_distance = max_interaction() + container->max_cell_interactive_distance_in_voxel[other_voxel_index];

	int comparing_dimension=-1, comparing_dimension2=-1;
	if(my_voxel_center[0]== other_voxel_center[0] && my_voxel_center[1]== other_voxel_center[1])
		comparing_dimension=2;
	else if(my_voxel_center[0]== other_voxel_center[0] && my_voxel_center[2]== other_voxel_center[2])
		comparing_dimension=1;
	else if(my_voxel_center[1]== other_voxel_center[1] && my_voxel_center[2]== other_voxel_center[2])
		comparing_dimension=0;

	if(comparing_dimension!=-1) 
	{ //then it is an immediate neighbor (through side faces)
		double surface_coord= 0.5*(my_voxel_center[comparing_dimension] + other_voxel_center[comparing_dimension]);
		if(std::fabs(position[comparing_dimension] - surface_coord) > max_interactive_distance)
			return false;
		return true;
	}
	comparing_dimension=-1;

	if(my_voxel_center[0]== other_voxel_center[0])
	{
		comparing_dimension=1; comparing_dimension2=2;
	}
	else if(my_voxel_center[1]== other_voxel_center[1])
	{
		comparing_dimension=0; comparing_dimension2=2;
	}
	else if(my_voxel_center[2]== other_voxel_center[2])
	{
		comparing_dimension=0; comparing_dimension2=1;
	}
	if(comparing_dimension!=-1)
	{
		double line_coord1= 0.5*(my_voxel_center[comparing_dimension] + other_voxel_center[comparing_dimension]);
		double line_coord2= 0.5*(my_voxel_center[comparing_dimension2] + other_voxel_center[comparing_dimension2]);
		double distance_squared= std::pow( position[comparing_dimension] - line_coord1,2)+ std::pow( position[comparing_dimension2] - line_coord2,2);
		if(distance_squared > max_interactive_distance * max_interactive_distance)
			return false;
		return true;
	}

	// @ToChange: my_voxel_center en Vector3d
	std::vector<double> cp= 0.5*(my_voxel_center+other_voxel_center);
	Vector3d corner_point( cp[0], cp[1], cp[2] );
	double distance_squared = (corner_point - position).normSqr();
	if(distance_squared > max_interactive_distance * max_interactive_distance)
		return false;
	return true;
}

};
