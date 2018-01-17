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

#include "../BioFVM/BioFVM_agent_container.h"
#include "PhysiCell_constants.h"
#include "../BioFVM/BioFVM_vector.h"
#include "../BioFVM/BioFVM.h"
#include "cell.h"
#include "sphere.h"

using namespace BioFVM;

Cell_Container::Cell_Container()
{
	boundary_condition_for_pushed_out_agents = PhysiCell_constants::default_boundary_condition_for_pushed_out_agents;
	cells_ready_to_divide.clear();
	cells_ready_to_die.clear();
	num_divisions_in_current_step = 0;
	num_deaths_in_current_step = 0;
	initialized = false;
	write_all_cells = false;
	membrane_shape = "none";
	membrane_length = 0;
}	

Cell_Container::~Cell_Container()
{
}

void Cell_Container::initialize(double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double voxel_size)
{
	initialize(x_start, x_end, y_start, y_end, z_start, z_end , voxel_size, voxel_size, voxel_size);
}

void Cell_Container::initialize(double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double dx, double dy, double dz)
{
	//boundary_condition_for_pushed_out_agents = PhysiCell_constants::default_boundary_condition_for_pushed_out_agents;
	//cells_ready_to_divide.resize(0);
	//cells_ready_to_die.resize(0);

	underlying_mesh.resize(x_start, x_end, y_start, y_end, z_start, z_end , dx, dy, dz);
	agent_grid.resize(underlying_mesh.voxels.size());
	max_cell_interactive_distance_in_voxel.resize(underlying_mesh.voxels.size(), 0.0);
	agents_in_outer_voxels.resize(6);
}

int Cell_Container::nb_active_cells()
{
	int res = 0;
	for( int i=0; i < (int) all_basic_agents.size(); i++ )
	{
		if ( all_basic_agents[i]->active() )
			res ++;
	}
	return res;
}

void Cell_Container::update_all_cells(double t)
{
	update_all_cells(t, dt_settings.cell_cylce_dt_default, dt_settings.mechanics_dt_default);
}

void Cell_Container::update_all_cells(double t, double dt)
{
	update_all_cells(t, dt,dt);
}

/* Change the repulsion coefficient value of all passive cells */
void Cell_Container::set_passive_repulsion( double r )
{
		#pragma omp parallel for
		for( int i=0; i < (int) all_basic_agents.size(); i++ )
		{
			if ( all_basic_agents[i]->passive() )
				(static_cast<Sphere*>(all_basic_agents[i]))->set_repulsion(r);	
		}
}

void Cell_Container::update_all_cells_cycle( double cell_cycle_dt, double time_since_last_cycle, double t )
{
		// Reset the max_radius in each voxel. It will be filled in set_total0volume
		// It might be better if we calculate it before mechanics each time 
		std::fill(max_cell_interactive_distance_in_voxel.begin(), max_cell_interactive_distance_in_voxel.end(), 0.0);
		
		#pragma omp parallel for
		for( int i=0; i < (int) all_basic_agents.size(); i++ )
		{
			if ( !all_basic_agents[i]->passive() )
				(static_cast<Cell*>(all_basic_agents[i]))->update_cycle( cell_cycle_dt, time_since_last_cycle, t );	
		}

		// process divides / removes ( can/should be parallelized too ?) 
		#pragma omp parallel for
		for( int i=0; i < (int) cells_ready_to_divide.size(); i++ )
		{
			(cells_ready_to_divide[i])->divide();
		}
		
		for( auto cell : cells_ready_to_die )
		{	
			cell->die();	
		}

		num_divisions_in_current_step +=  cells_ready_to_divide.size();
		num_deaths_in_current_step +=  cells_ready_to_die.size();
		
		cells_ready_to_die.clear();
		cells_ready_to_divide.clear();
}

void Cell_Container::update_all_cells_mechanics( double mechanics_dt, double time_since_last_mechanics )
{
	// Compute velocities
	#pragma omp parallel for 
	for( int i=0; i < (int) all_basic_agents.size(); i++ )
	{
		all_basic_agents[i]->update_cell_motion( time_since_last_mechanics, membrane_length, membrane_shape );
		if ( !all_basic_agents[i]->passive() )
				(static_cast<Cell*>(all_basic_agents[i]))->degrade_ecm( mechanics_dt );	
	}
		
	// Calculate new positions
	#pragma omp parallel for 
	for( int i=0; i < (int) all_basic_agents.size(); i++ )
	{
		all_basic_agents[i]->update_position(time_since_last_mechanics);
	}
		
	// Update cell indices in the container
	for( auto cell: all_basic_agents )
		cell->update_cell_index_container();
}

void Cell_Container::update_all_cells(double t, double cell_cycle_dt, double mechanics_dt)
{
	//if it is the time for running cell cycle, do it!
	double time_since_last_cycle= t- last_cell_cycle_time;

	// Means dt has to be small enough that 0.0001 is good limit ???
	if( fabs(time_since_last_cycle- cell_cycle_dt)<0.0001 || !initialized)
	{
		if(!initialized)
			time_since_last_cycle= cell_cycle_dt;
		update_all_cells_cycle( cell_cycle_dt, time_since_last_cycle, t );
		last_cell_cycle_time = t;
	}

	double time_since_last_mechanics = t - last_mechanics_time;
	
	// if( time_since_last_mechanics>= mechanics_dt || !initialized)
	if( fabs(time_since_last_mechanics - mechanics_dt)<0.0001 || !initialized)
	{
		if(!initialized)
			time_since_last_mechanics= mechanics_dt;
		update_all_cells_mechanics( mechanics_dt, time_since_last_mechanics );
		last_mechanics_time=t;
	}

	initialized=true;
	return;
}

/* Check if point is inside BM  */
int Cell_Container::inside_BM(Vector3d* pos)
{
	if ( membrane_length > 0 )
	{
		if ( membrane_shape == "duct" )
			return inside_BM_duct(pos);
		else if ( membrane_shape == "sphere" )
			return inside_BM_sphere(pos);
		else if ( membrane_shape == "sheet" )
			return inside_BM_sheet(pos);
	}
	return 0;
}

/* If point is inside BM for sphere geom */
int Cell_Container::inside_BM_sphere(Vector3d* pos)
{
	double distance_to_origin = pos->norm();  // distance to the origin 
	return ( ( membrane_length - distance_to_origin) > 0 );
}

/* If point is inside BM for sheet geom (in between two planes) */
int Cell_Container::inside_BM_sheet(Vector3d* pos)
{
	return ( ( membrane_length - fabs( (*pos)[2])) > 0 );
}

/* If point is inside BM for duct geom */
int Cell_Container::inside_BM_duct(Vector3d* pos)
{
	//Note that this function assumes that duct cap center is located at <0, 0, 0>
	if ( (*pos)[0] >= 0 ) // Cell is within the cylinder part of the duct
	{
		double distance_to_x_axis= pos->distance_to_xaxis();
		distance_to_x_axis = std::max(distance_to_x_axis, EPSILON);		// prevents division by zero
		return ( (membrane_length - distance_to_x_axis) > 0 );
	}
	
	// Cell is inside the cap of the duct
	double distance_to_origin= pos->norm();  // distance to the origin 
	return ( (membrane_length - distance_to_origin) > 0 );
}
	

/** delete the cell of given index */
void Cell_Container::delete_cell( int index )
{
	// deregister agent in from the agent container
	remove_agent( all_basic_agents[index] );
	// de-allocate (delete) the cell; 
	delete all_basic_agents[index]; 

	// performance goal: don't delete in the middle -- very expensive reallocation
	// alternative: copy last element to index position, then shrink vector by 1 at the end O(constant)
	// move last item to index location 
	if ( all_basic_agents.size() > 1 ) 
	{
		all_basic_agents[ all_basic_agents.size()-1 ]->index=index;
		all_basic_agents[index] = all_basic_agents[ all_basic_agents.size()-1 ];
		// shrink the vector
		all_basic_agents.pop_back();	
	}
	else
		all_basic_agents.resize(0); // only deleted agent was in the vector
	
}

/** add the agent to the grid */
void Cell_Container::register_agent( Basic_Agent* agent )
{
	agent_grid[agent->get_current_mechanics_voxel_index()].push_back(agent);	
}

void Cell_Container::add_new_cell( Cell* cell )
{
	all_basic_agents.push_back( cell );
	cell->index = all_basic_agents.size() - 1;
	cell->set_container(this);
}

Cell* Cell_Container::create_cell()
{
	Cell* pNew = new Cell(); 
	all_basic_agents.push_back( pNew ); 
	pNew->index = all_basic_agents.size() - 1;
	pNew->set_container(this);
	return pNew; 
}

Sphere* Cell_Container::create_sphere()
{
	Sphere* pNew = new Sphere(); 
	all_basic_agents.push_back( pNew ); 
	pNew->index = all_basic_agents.size() - 1;
	pNew->set_container(this);
	return pNew; 
}

void Cell_Container::add_agent_to_outer_voxel(Cell* agent)
{
	int escaping_face = find_escaping_face_index(agent);
	agents_in_outer_voxels[escaping_face].push_back(agent);
	agent->put_out();
}

int Cell_Container::update_voxel_in_container( double x, double y, double z )
{
	Vector3d pos(x,y,z);
	if ( is_position_valid( pos ) )
		return nearest_voxel_index( pos );
	else
		return -1;

}

bool Cell_Container::contain_any_cell(int voxel_index)
{
	return agent_grid[voxel_index].size()==0?false:true;
}

int Cell_Container::find_escaping_face_index( Cell* agent )
{
	Vector3d pos = agent->get_position();
	if (pos[0] <= underlying_mesh.bounding_box[0])
		return PhysiCell_constants::mesh_lx_face_index;
	if (pos[0] >= underlying_mesh.bounding_box[3])
		return PhysiCell_constants::mesh_ux_face_index;
	if (pos[1] <= underlying_mesh.bounding_box[1])
		return PhysiCell_constants::mesh_ly_face_index;
	if (pos[1] >= underlying_mesh.bounding_box[4])
		return PhysiCell_constants::mesh_uy_face_index;
	if (pos[2] <= underlying_mesh.bounding_box[2])
		return PhysiCell_constants::mesh_lz_face_index;
	if (pos[2] >= underlying_mesh.bounding_box[5])
		return PhysiCell_constants::mesh_uz_face_index;
	return -1;
}

void Cell_Container::flag_cell_for_division( Cell* pCell )
{ 
#pragma omp critical 
{cells_ready_to_divide.push_back( pCell );} }

void Cell_Container::flag_cell_for_removal( Cell* pCell )
{ 
#pragma omp critical 
{cells_ready_to_die.push_back( pCell );} }

int Cell_Container::writePov(double timepoint, double scale)
{
	/** \todo to move part of it in basic_agent if need pov files */
/**	std::string filename; 
	filename.resize( 1024 ); 
	sprintf( (char*) filename.c_str() , "output//cells_%i.pov" , (int)round(timepoint) ); 
	std::ofstream povFile (filename.c_str(), std::ofstream::out);
	povFile<<"#include \"colors.inc\" \n";
	povFile<<"#include \"header.inc\" \n";
	
	for( auto agent: (all_basic_agents) )
	{
		if ( !agent->passive() )
		{
		Cell* cell = static_cast<Cell*>(agent);
		std::string _nameCore;
		
		if (cell->phase_code() > 0)
		{
			int code = cell->phase_code();
			if (code ==PhysiCell_constants::Ki67_positive_premitotic || code==PhysiCell_constants::Ki67_positive_postmitotic || code==PhysiCell_constants::Ki67_positive || code==PhysiCell_constants::Ki67_negative || code==PhysiCell_constants::live)
				_nameCore="LIVE";
			else if (code==PhysiCell_constants::apoptotic)
				_nameCore="APOP";
			else if (code==PhysiCell_constants::necrotic_swelling || code==PhysiCell_constants::necrotic_lysed || code==PhysiCell_constants::necrotic)
				_nameCore="NEC";
			else if (code==PhysiCell_constants::debris)
				_nameCore="DEBR";
			else
				_nameCore="MISC";
		}
		else if( cell->type==PhysiCell_constants::TUMOR_TYPE)
			_nameCore="LIVE";
		else if( cell->type==PhysiCell_constants::VESSEL_TYPE)
			_nameCore="ENDO";
		else
			_nameCore="MISC";
	
		Vector3d pos = cell->get_position();
		std::string center= "<" + std::to_string( pos[0]/scale) + "," + std::to_string(pos[1]/scale) +","+ std::to_string(pos[2]/scale) +">";
		std::string core = "sphere {\n\t" + center + "\n\t " + std::to_string( cell->volume.radius/scale) + "\n\t FinishMacro ( " + center +","+ _nameCore+ "Finish,"+ _nameCore + "*1)\n}\n";
		povFile<< core;		
		}
	}
	
	povFile<<"#include \"footer.inc\" \n";
	povFile.close();
*/
	return 0;
}

int Cell_Container::writeCellReport(double timepoint, int cyclemode)
{
	std::string filename; 
	filename.resize( 1024 );
    std::string delimeter = ";"; // for paraview reading, doesn't like \t	
	sprintf( (char*) filename.c_str() , "output//cells_%05d.txt" , (int)round(timepoint) ); 
	std::ofstream povFile (filename.c_str(), std::ofstream::out);
	povFile<<"Time" << delimeter; 
	povFile<<"ID" << delimeter;
	povFile << "x" << delimeter << "y" << delimeter << "z" << delimeter;
	povFile << "radius" << delimeter << "volume_total" << delimeter << "radius_nuclear" << delimeter << "contact_ECM" << delimeter << "freezer" << delimeter << "polarized_fraction" << delimeter << "motility" << delimeter;
   povFile	<< "cell_line" << delimeter;
	if ( cyclemode == 0 )
	   povFile << "phenotype" << delimeter << "phase" << delimeter << "elapsed_time";
	else
		povFile << "Cell_cell" << delimeter << "phase" << delimeter << "Cycle";
	//povFile << delimeter << "angle";
	povFile << delimeter << "NFkB";
	povFile << std::endl;

   std::ofstream passFile;
   // write passive file if option is ON
	if ( write_all_cells )
	{
		std::string passfilename; 
	passfilename.resize( 1024 );
		sprintf( (char*) passfilename.c_str() , "output//passive_cells_%05d.txt" , (int)round(timepoint) ); 
		passFile.open(passfilename.c_str(), std::ofstream::out);
		passFile << "Time" << delimeter; 
		passFile <<"ID" << delimeter;
		passFile << "x" << delimeter << "y" << delimeter << "z" << delimeter;
		passFile << "radius" << delimeter << "volume_total" << delimeter << "volume_nuclear_fluid" << delimeter << "volume_nuclear_solid" << delimeter << "volume_cytoplasmic_fluid" << delimeter << "volume_cytoplasmic_solid" << delimeter << "volume_calcified_fraction" << delimeter;
		passFile	<< "cell_line" << delimeter << "phenotype" << delimeter << "phase" << delimeter << "elapsed_time\n";
	}

	for( auto cell : all_basic_agents )
	{
		if ( !cell->passive() )
		{
			povFile << timepoint << delimeter;
			cell->output( delimeter, &povFile );
		}
		else if ( write_all_cells )
		{
			passFile << timepoint << delimeter;
			cell->output( delimeter, &passFile );
		}
	}
	if ( write_all_cells )
		passFile.close();
	povFile.close();

	return 0;
}

void Cell_Container::log_output(double t, int output_index, Microenvironment* microenvironment, std::ofstream& report_file, int cyclemode)
{
	int num_new_cells= 0;
	int num_deaths=0;
	std::cout << "current simulated time: " << t   << " minutes " << std::endl; 
	std::cout << "interval wall time: ";
	BioFVM::TOC();
	BioFVM::display_stopwatch_value( std::cout , BioFVM::stopwatch_value() ); 
	std::cout << std::endl; 
	std::cout << "total wall time: "; 
	BioFVM::RUNTIME_TOC();
	BioFVM::display_stopwatch_value( std::cout , BioFVM::runtime_stopwatch_value() ); 
	std::cout << std::endl;
	
	std::cout << "time: "<<t<<std::endl;
	num_new_cells=t==0 ? all_basic_agents.size() : num_divisions_in_current_step;
	num_deaths = num_deaths_in_current_step;
	std::cout << "total number of agents (newly born, deaths): " << all_basic_agents.size() << "(" << num_new_cells << ", " << num_deaths << ")" << std::endl; 
	report_file << t << "\t" << all_basic_agents.size() << "\t" << num_new_cells << "\t" << num_deaths << "\t" << BioFVM::stopwatch_value();
   	
	int ndens = microenvironment->number_of_densities();
	for ( int i = 0; i < ndens; i++ )
	{
		report_file << "\t" << microenvironment->total_density(i);
	}
	report_file << std::endl; 
	BioFVM::TIC();
	
	num_divisions_in_current_step = 0;
	num_deaths_in_current_step = 0;
	//writePov(t, scale);
	writeCellReport(t, cyclemode);
	// Writing microenvironment state, don't for now
/**	std::string filename; 
	filename.resize( 1024 , '\0' ); 
	sprintf( (char*) filename.c_str() , "microutput/output%08d.mat" , output_index ); 
	filename.resize( strlen( filename.c_str() ) ); 
	std::cout << "\tWriting to file " << filename << " ... " << std::endl; 
	microenvironment.write_to_matlab( filename ); */
}

void Cell_Container::draw_cells_SVG( WriteSVG* svg, std::ostream* os, double zslice, std::vector<double> lims, bool plot_nuclei, int mode_color )
{
	int nagents = (int) all_basic_agents.size();
	for( int i=0; i < nagents; i++ )
	{
		if ( all_basic_agents[i]->active() )
			(static_cast<Cell*>(all_basic_agents[i]))->drawSVG( svg, os, zslice, lims, plot_nuclei, mode_color );	
	}
}
