/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.0.0) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2016 (in preparation).                         #
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.0.0) [1],  #
# with BioFVM [2] to solve the transport equations.                         #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2016 (in preparation).                         #
#                                                                           #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient     #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2016, Paul Macklin and the PhysiCell Project           #
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

#include "cell.h"
#include "cell_cycle_classic.h"
#include "cell_cycle_network.h"
#include "cell_container.h"
#include "../base/utilities.h"
#include "PhysiCell_constants.h"
#include "../base/vector3d.h"
#include<limits.h>
#include<math.h>
#include <random>
#include <unordered_map>

Cell::Cell() : Basic_Agent()
{
	container = NULL;
	cell_line = NULL;
	ccycle = NULL;
	polarized = 0;
	freezed = 0;
	pmotility = 0.5;
	padhesion = 0.5;
	pintegrin = 0.5;
	assign_polarization();
	set_previous_velocity(0, 0, 0);
	division_flagged = 0;
	removal_flagged = 0;
	mmped = 0;
}

Cell::~Cell()
{
	if ( ccycle )
		delete ccycle;
	cell_line = NULL; // cell_line is deleted somewhere else, here just unreference the pointer
	if ( container )
		container = NULL;
}

/* Create the cell cycle object, classic or network if pointer not null */
void Cell::set_cycle( MaBossNetwork* mboss, int phase, double elap )
{
	if ( mboss != NULL )
	{
		ccycle = new CellCycleNetwork(mboss);
	}
	else
	{
		ccycle = new CellCycleClassic();
	}
	ccycle->set_cell( this );
	ccycle->set_current_phase( phase );
	ccycle->set_elapsed_time( elap );
}


/* Initialize values of cell cycle, check if do it in other way */
void Cell::init_cell_cycle_classic( int mode_phase, double etime )
{
	static_cast<CellCycleClassic*>(ccycle)->init_phase( mode_phase, etime );	
}

/* Return pointer to the cell_container elements in which the cell is */
Cell_Container* Cell::cell_container() 
{ 
	return static_cast<Cell_Container*>(container); 
}

/* copy all the cell attributes */
void Cell::copy_cell( Cell* copy_me )
{
	cell_line = copy_me->cell_line;
	ccycle = copy_me->ccycle->copy();
	ccycle->set_cell( this );
	type = copy_me->type;
	freezed = copy_me->freezed;
	velocity = -1*copy_me->velocity; 
	previous_velocity = -2*copy_me->previous_velocity; 
	microenvironment = copy_me->get_microenvironment();
	copy_rates( copy_me );	
	polarized = copy_me->polarized;
	polarization = -1*copy_me->polarization;
	pmotility = copy_me->pmotility;
	padhesion = copy_me->padhesion;
	volume = copy_me->volume;
	volume.noise_target( copy_me->cell_line->get_init_radius(), copy_me->cell_line->get_default_volume(2), copy_me->cell_line->get_default_volume(3) );
	mmped = copy_me->mmped;
}

/* Copy the rates vector */
void Cell::copy_rates( Cell* copyme )
{
	secretion_rates = std::vector<double>( copyme->secretion_rates );
	uptake_rates = std::vector<double>( copyme->uptake_rates );
	saturation_densities = std::vector<double>( copyme->saturation_densities );
	cell_source_sink_solver_temp1 = std::vector<double>(copyme->cell_source_sink_solver_temp1);
	cell_source_sink_solver_temp2 = std::vector<double>(copyme->cell_source_sink_solver_temp2);
}

/**  Divide current cell
 * Along current orientation axis if polarized:
 *   d = (1-p)(r-(r.theta)theta) ~perpendicular + (r.theta) theta ~parallel; then normalised
 *   (Orientation is front-rear polarity of cell, spindle axis is parallel to FR-polarity: see Reffay et al 2011, Thery et al 2006, and parallel to AP axis in epithelial sheet Wang et al 2017)
 *   Kid = new cell as same properties of mother cell
 * */
void Cell::divide()
{
	if ( is_out_of_domain )
		return;
	// functionalisé
	Cell* kid = NULL;
	division_flagged = 0;

	#pragma omp critical
	{ kid = cell_container()->create_cell(); }
	kid->copy_cell( this );
	volume.noise_target( cell_line->get_init_radius(), cell_line->get_default_volume(2), cell_line->get_default_volume(3) );

	// check the following operations to make sure it is the right way to do it
	double radius = volume.get_radius();

	// randomly place the new agent close to me
	// with overlap so repulse each other
	if (! kid->assign_position( position - 0.05 * radius * polarization.dir() ) )
	{
		std::cout << "Could not assign position to daughter cell" << std::endl;
		#pragma omp critical
		{cell_container()->delete_cell( kid->index );}
		return;
	}
	
	//change my position to keep the center of mass intact and then see if I need to update my voxel index
	position += 0.05 * radius * polarization.dir();
	update_voxel_in_container();
	volume.divide(); 
	double total = kid->volume.divide();
	kid->set_total_volume(total);
	set_total_volume(total);
}

void Cell::set_previous_velocity(double xV, double yV, double zV)
{
	previous_velocity[0]=xV; 
	previous_velocity[1]=yV; 
	previous_velocity[2]=zV; 
}

/* Update polarization direction with velocity-alignment model */
void Cell::update_polarization( double dt )
{
	// p(n+1) = (p(n)+dt*v(n)) / (1+dt/tau)
	polarization += dt * velocity;
	polarization /= (1 + dt/cell_line->persistence);
}

/* Assign cell line */
void Cell::set_cell_line( CellLine* cl) 
{ 
	cell_line = cl; 		
	volume.set_initial_volumes( cl->def_volumes );
}

/* Get cell line index */
int Cell::get_cell_line()
{ 
	return cell_line->getLineIndex(); 
}	

void Cell::set_new_volume(double vol)
{
	Basic_Agent::set_total_volume(vol);
	volume.set_new_volume(vol, cell_line->get_default_volume(2));

	double max_interaction = volume.get_radius() * cell_line->max_interaction_distance_factor;
	if(cell_container()->max_cell_interactive_distance_in_voxel[current_mechanics_voxel_index] < max_interaction )
		cell_container()->max_cell_interactive_distance_in_voxel[current_mechanics_voxel_index] = max_interaction;
}

void Cell::set_total_volume(double vol)
{
	Basic_Agent::set_total_volume(vol);
	volume.set_volume_total(vol);

	double max_interaction = volume.get_radius() * cell_line->max_interaction_distance_factor;
	if(cell_container()->max_cell_interactive_distance_in_voxel[current_mechanics_voxel_index] < max_interaction )
		cell_container()->max_cell_interactive_distance_in_voxel[current_mechanics_voxel_index] = max_interaction;
}

/* Initialize environement uptake, secretion and saturation rates vectors */
void Cell::init_env_rates()
{
	int ndens = number_of_microdensities();
	secretion_rates.resize( ndens, 0.0 );
	uptake_rates.resize( ndens, 0.0 ) ;
	saturation_densities.resize( ndens, 50.0 );
	int index = density_index("oxygen");
	if ( index >= 0 )
	{
		uptake_rates[index] = 10000*cell_line->init_uptake_rate;
	}
	index = density_index("tnf");
	if ( index >= 0 )	
	{
		saturation_densities[index] = 1;
		uptake_rates[index] = cell_line->init_uptake_rate;
	}
	index = density_index("tgfb");
	if ( index >= 0 )	
	{
		saturation_densities[index] = 30; // 4
		uptake_rates[index] = cell_line->init_uptake_rate;
		secretion_rates[index] = 0.04;
	}
}

/* Load default values from cell line */
void Cell::load_default_values()
{
	polarized = cell_line->def_polarized;
}

/* Return level of protein given by index around the cell */
double Cell::local_density(std::string field)
{ 
	int ind = density_index(field);
	if ( ind >= 0 )
		return (nearest_density_vector())[ind]; 
	return -1;
}

/* Change the secretion rate of given specie to given amount*factor  */
void Cell::secrete(std::string field, double fact, double dt)
{
	int index = density_index(field);
	if ( index >= 0 )
	{
		secretion_rates[index] = fact*cell_line->secretion/microvoxel_volume();
		set_internal_uptake_constants(dt);
	}
}

void Cell::turn_off_reactions(double dt)
{	
	// is_active=false;  /* uncomment this line if you want to completely turn off all the reactions*/
	
	// Reduce all the uptake and secretion rates by a factor of 10
	for( int i = 0; i < (int) uptake_rates.size(); i++ )
		uptake_rates[i] = 0.0;  
	for( int i = 0; i < (int) secretion_rates.size(); i++ )
		secretion_rates[i] = 0.0;  
	set_internal_uptake_constants(dt);
}

void Cell::die()
{
	cell_container()->delete_cell( index );
} 

double Cell::oxygen_necrosis()
{
   double ox = local_density("oxygen");	
   if ( ox == -1 ) return -1;
   return cell_line->necrosis_rate_O2( ox ); 
}

double Cell::oxygen_prolif()
{ 
   double ox = local_density("oxygen");	
   // oxygen density absent, do as if unlimited concentration
   if ( ox == -1 ) 
		return cell_line->proliferation_O2_based(INT_MAX); 
	return cell_line->proliferation_O2_based(ox); 
}

void Cell::update_cycle( double cycle_dt, double time_since_last, double t )
{
	if ( is_out_of_domain )
		return;
	ccycle->do_one_cycle_step( time_since_last, t );
	if ( freezed == 0 )
		update_volume( time_since_last );
}

/* Flag the cell to be divided */
void Cell::to_divide()
{
	if ( !division_flagged )
		cell_container()->flag_cell_for_division(this);
	division_flagged = 1;
}	

/* Flag the cell to be removed */
void Cell::to_remove()
{
	if ( !removal_flagged )
		cell_container()->flag_cell_for_removal(this);
	removal_flagged = 1;
}	

/**
 * Calculate motility forces according to mode:
 * 0, random; 1, along polarity axis; other: nothing
 * */
void Cell::set_motility( double dt )
{
	// Cell frozen, cannot actively move
	if ( freezed > 2 )
		return;
	switch( cell_line->mode_motility )
	{
		case 0:
			set_3D_random_motility(dt);
			break;
		case 1:
			set_3D_polarized_motility(dt);
			break;
		default:
			return;
			break;
	}
	velocity += motility;
}

void Cell::update_volume( double dt ) 
{ 
	switch( cell_line->mode_volume )
	{
		case 0:
			update_volume_default(dt);
			break;
		default:
			return;
			break;
	}
}

void Cell::add_cell_basement_membrane_interactions( double dt, double distance ) 
{
	//Note that the distance_to_membrane function must set displacement values (as a normal vector)
	double max_interactive_distance = cell_line->max_interaction_distance_factor * volume.get_radius();
		
	double temp_a=0;
	// Adhesion to basement membrane
	if(distance< max_interactive_distance)
	{
		temp_a= (1- distance/max_interactive_distance);
		temp_a*=temp_a;
		temp_a*=- cell_line->Ccba;
	}
	// Repulsion from basement membrane
	double temp_r=0;
	if ( distance < volume.get_radius() )
	{
		temp_r= (1- distance/volume.get_radius());
		temp_r*=temp_r;
		temp_r*= cell_line->Ccbr;
	}
	temp_r+=temp_a;
	if(temp_r==0)
		return;

	velocity += temp_r * displacement;	
	return;	
}

/* Update the value of freezing of the cell with bitwise operation
* Do a bitwise-or comparison on freezed and input parameter:
* if freezed = 0, it will be the value of the parameter frozen
* if freezed = 1, it will be either 1 (frozen = 0) or 3 (frozen = 3) */
void Cell::freezer( int frozen )
{
	freezed = freezed | frozen;
}

double Cell::get_adhesion()
{
	return 1;
}

double Cell::get_ecm_repulsion() 
{
	return cell_line->Cecmr; 
}

/* Gives current friction coefficient value */
double Cell::get_friction()
{
	//return ccycle->phase_coef(1)  * get_surface(volume.get_radius()) * friction_coeff;
	return get_surface(volume.get_radius()) * friction_coeff;
}

double Cell::get_repulsion() 
{
	return cell_line->Cccr; 
}
		
double Cell::max_interaction() 
{ 
	return cell_line->max_interaction_distance_factor * volume.get_radius(); 
}

/* (To simulate ellipsitic cells - radius in a given direction axis dir of the cell according to its polarity 
 * Max radius is in polarity axis direction, and smaller in perpendicular direction ) */
double Cell::polarized_radius( Vector3d dir ) 
{ 
	double cosP = fabs( dir * polarization.dir() );
	double rA = 1 + polarized * (cell_line->max_interaction_distance_factor - 1);
	double maxR = rA * volume.get_radius(); 
	double minR = volume.get_radius() * sqrt( 1.0 / rA  );
	return cosP * maxR + (1-cosP) * minR;
}

/* Calculate adhesion coefficient with other cell */
double Cell::adhesion( Cell* other_cell )
{
	double adh = 0;
	if ( cell_line == other_cell->cell_line )
		adh = std::min( cell_line->get_homotypic_strength(padhesion), other_cell->cell_line->get_homotypic_strength(padhesion) );
	else
		adh = std::min( cell_line->get_heterotypic_strength(padhesion), other_cell->cell_line->get_heterotypic_strength(padhesion) );

	return adh;
}

/* Update time step and total volume */
void Cell::update_volume_default( double dt )
{
	volume.update_volumes_timestep( dt, ccycle->get_calcification_rate() );
	// The following line is needed in every volume regulation method (it sets BioFVM total_volume)
	set_total_volume(volume.get_total());	
}

/* Return true if level of oxygen is lower than critical level */
bool Cell::not_enough_oxygen()
{
	double ox = local_density("oxygen");
	if ( ox >= 0 )	
		return ( UniformRandom()/2.0 < (cell_line->o2_no_proliferation - ox)) ;
   return false;	
}

/* Return if level of protein given by index around the cell is high enough (compared to given threshold) */
int Cell::feel_enough(std::string field)
{	
	//return local_density(field) > cell_line->prot_threshold; 
	int ind = density_index(field);
	if ( ind >= 0 )	
		return bounded[ind] > cell_line->get_threshold(field); 
	return -1;
}

/* Return if cell has enough contact with other cells (compared to given threshold determined by the given level) */	
bool Cell::has_neighbor(int level)
{ 
	if ( level == 0 )
		return contact_cell() > cell_line->contact_cc_threshold; 
	else
		return contact_cell() > (2 * cell_line->contact_cc_threshold); 
}
		
/* Return if cell has enough contact with ECM (compared to given threshold) */	
bool Cell::touch_ECM()
{ 
	return contact_ecm() > cell_line->contact_ecm_threshold; 
}


/* Return true if level of oxygen is lower than necrosis critical level */
bool Cell::necrotic_oxygen()
{
	double ox = local_density("oxygen");
	//std::cout << ox << " " << (cell_line->o2_necrotic) - ox << std::endl;
	if ( ox >= 0 )	
		return ( UniformRandom() * 0.005 < (cell_line->o2_necrotic - ox) );
   return false;	
}
		
/* Return value of adhesion strength with ECM according to integrin level */
double Cell::integrinStrength()
{ 
	return cell_line->get_integrin_strength( pintegrin ); 
}

/* Degrade the surrounding ECM 
 *
 * param dt time step */
void Cell::degrade_ecm( double dt )
{
	if ( is_out_of_domain )
		return;
	if ( !mmped ) 
		return;

	// Check if there is ECM material in given voxel
	int ecm_index = density_index("ecm");
	int current_index = get_current_mechanics_voxel_index();
	#pragma omp critical
	{
		double dens = microenvironment->nearest_density_vector(current_index)[ecm_index];
		if ( dens > EPSILON )
		{
			dens -= cell_line->get_degradation( pintegrin ) * dt; // to change by a rate
			dens = dens > 0 ? dens : 0;
			microenvironment->nearest_density_vector(current_index)[ecm_index] = dens;
		}
	}
}	

/* Motility with random direction, and magnitude of motion given by customed coefficient */
void Cell::set_3D_random_motility( double dt )
{
	double probability = UniformRandom();
	
	if ( probability < dt / cell_line->persistence )
	{
		Vector3d tmp;
		tmp.randomize_normed();
		motility = cell_line->get_motility_amplitude(pmotility) * tmp;
	}
}

/*
 * Motility in the polarity axis migration + little noise
 * Persistence in the update polarization
 * */
void Cell::set_3D_polarized_motility( double dt )
{
	// mot = (1-p) * r + p * pol
	motility.randomize_normed();
	motility *= (1 - polarized);
	motility += polarized * polarization.dir();
	// Normalized it
	motility.normalize();
	// mot = mot_coef * mot_dir
	motility *= cell_line->get_motility_amplitude(pmotility);
}

void Cell::output( std::string& delimeter, std::ofstream* file )
{
	(*file) << ID << delimeter << position[0] << delimeter << position[1] << delimeter << position[2] << delimeter;
	volume.output( delimeter, file );
	(*file) << contact_ecm() << delimeter;
	//(*file) << integrinStrength() << delimeter;
	(*file) << freezed << delimeter;
	(*file) << polarized << delimeter;
	(*file) << cell_line->get_motility_amplitude(pmotility) << delimeter;
	cell_line->output( delimeter, file );
	ccycle->output( delimeter, file );
	//double angle = ( polarization.dir() * position.dir() );
	//(*file) << angle;
	(*file) << std::endl;
}	

/* Get colors value according to color mode */
void Cell::get_colors( std::vector<std::string>* cols, int mode )
{
	for ( int i=0; i < (int) cols->size(); i++ )
	{
		(*cols)[i] = "rgb(0,0,0)";
	}

	switch ( mode )
	{
		case 0:
			simple_cell_coloring( cols );
			break;
		case 1:
			phase_cell_coloring( cols );
			break;
		default:
			break;
	}
}

/* Fill values of colors to use: cyto_color, cyto_outline , nuclear_color, nuclear_outline */
void Cell::simple_cell_coloring( std::vector<std::string>* cols )
{
	if ( cell_line->getLineIndex() == 0 )
	{
		(*cols)[0] = "rgb(255,0,0)";
		(*cols)[2] = "rgb(200,0,0)";
	}
	if ( cell_line->getLineIndex() == 1 )
	{
		(*cols)[0] = "rgb(0,255,0)";
		(*cols)[2] = "rgb(0,200,0)";
	}
}

/* Fill values of colors to use according to cell current phase: cyto_color, cyto_outline , nuclear_color, nuclear_outline */
void Cell::phase_cell_coloring( std::vector<std::string>* cols )
{
	if ( cell_line->getLineIndex() == 0 )
	{
		(*cols)[0] = ccycle->get_phase_color();
		//(*cols)[2] = "rgb(200,0,0)";
	}
}

/* Draw the current cell in SVG file */
void Cell::drawSVG( WriteSVG* svg, std::ostream* os, int zslice, std::vector<double> lims, bool plot_nuclei, int mode_color )
{
   // Has an intersection with the Z slice to draw	
	if( fabs( position[2] - zslice ) < volume.get_radius() )
	{
		double r = volume.get_radius() ; 
		double rn = volume.get_nuclear_radius() ; 
	    double z = fabs(position[2] - zslice) ; 
   
		std::vector<std::string> colors(4);
		get_colors( &colors, mode_color );

		(*os) << "  <g id=\"cell" << ID << "\">" << std::endl; 
  
		// figure out how much of the cell intersects with z = 0 
		double plot_radius = sqrt( r*r - z*z ); 
		(*svg).add_circle( position[0]-lims[0], position[1]+lims[4]-lims[2], plot_radius, 0.5, colors[1], colors[0], 1 ); 

		// plot the nucleus if it, too intersects z = 0;
		if( z < rn && plot_nuclei )
		{   
			plot_radius = sqrt( rn*rn - z*z ); 
			(*svg).add_circle( position[0]-lims[0], position[1]+lims[4]-lims[2], plot_radius, 0.1, colors[3], colors[2], 0.3); 
		}					  
		(*os) << "  </g>" << std::endl;
	}
}
