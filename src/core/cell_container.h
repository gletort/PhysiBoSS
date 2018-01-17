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

#ifndef __PhysiCell_cell_container_h__
#define __PhysiCell_cell_container_h__

#include <vector>
#include "../BioFVM/BioFVM_agent_container.h"
#include "../BioFVM/BioFVM_mesh.h"
#include "../BioFVM/BioFVM_microenvironment.h"
#include "../base/writeSVG.h"
#include "cell.h"
#include "sphere.h"


typedef struct 
{
	double cell_cylce_dt_default=6;
	double mechanics_dt_default=0.1;
} Time_Settings;


class Cell;
class Sphere; 

class Cell_Container : public BioFVM::Agent_Container
{
 private:	
	std::vector<Cell*> cells_ready_to_divide; // the index of agents ready to divide
	std::vector<Cell*> cells_ready_to_die;
	int boundary_condition_for_pushed_out_agents; 	// what to do with pushed out cells
	bool initialized;
	/** \brief Contains limits of the space */
	BioFVM::Cartesian_Mesh underlying_mesh;  
	/** \brief Write all cells (active and passive)
	 *
	 * Write passive cells in other output files */
	bool write_all_cells;

	/** \brief If has BM, shape (so far duct or spheroid implemented) */
	std::string membrane_shape;
	/** \brief If has BM, size of it (radius of the spheroid or of duct */
	double membrane_length;

 public:

	Time_Settings dt_settings;
	int num_divisions_in_current_step;
	int num_deaths_in_current_step;
	
	double last_cell_cycle_time= 0;
	double last_mechanics_time= 0;

	/** Default constructor */
	Cell_Container();
	/** Destructor */
	~Cell_Container();
 	void initialize(double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double voxel_size);
	void initialize(double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double dx, double dy, double dz);
	std::vector<std::vector<Cell*> > agents_in_outer_voxels;
	
	/** \brief Get the number of active cells in the container */ 
	int nb_active_cells();
	
	/** \brief delete the cell of given index */
	void delete_cell( int index );

	/** \brief Set the shape of the BM */
	inline void set_membrane_shape( std::string shape )
	{ membrane_shape = shape; strip( &membrane_shape ); } ;
	
	/** \brief Set the size of the BM */
	inline void set_membrane_length( double l )
	{ membrane_length = l; } ;

	/** \brief Check if point is inside BM  */
	int inside_BM(Vector3d* pos);
	/** \brief If point is inside BM for duct geom */
	int inside_BM_duct(Vector3d* pos);
	/** \brief If point inside spheroid geometry */
	int inside_BM_sphere(Vector3d* pos);
	/** \brief If point is inside BM for sheet geom (in between two planes) */
	int inside_BM_sheet(Vector3d* pos);
	
	void update_all_cells(double t);
	void update_all_cells(double t, double dt);
	void update_all_cells(double t, double cell_cycle_dt, double mechanics_dt);
	/** Update all cells in vector current cycle state */
	void update_all_cells_cycle( double cell_cycle_dt, double time_since_last_cycle, double t );
	/** Update all cells in vector mechanics (velocity and position) */
	void update_all_cells_mechanics( double mechanics_dt, double time_since_last_mechanics );

	void register_agent( Basic_Agent* agent );
	/** Create and return a new cell, added in the container */
	Cell* create_cell();  
	/** Create and return a new sphere, added in the container */
	Sphere* create_sphere();  
	/** Add a cell in the container */
	void add_new_cell(Cell* cell);
	
	void add_agent_to_outer_voxel(Cell* agent);
	
	void flag_cell_for_division( Cell* pCell ); 
	void flag_cell_for_removal( Cell* pCell ); 
	bool contain_any_cell(int voxel_index);

	/** \brief Change the repulsion coefficient value of all passive cells */
	void set_passive_repulsion( double r );

	/** \brief Return if given position is valid in the mesh or not */
	virtual bool is_position_valid( Vector3d pos )
	{ return underlying_mesh.is_position_valid( pos[0], pos[1], pos[2] ); };
	
	/** \brief Return index of nearest voxel from position */
	virtual int nearest_voxel_index( Vector3d pos )
	{ return underlying_mesh.nearest_voxel_index( pos ); };
	
	/** \brief Return voxel index connected in moore neighnorhood */
	virtual std::vector<int> moore_voxels( int index )
	{ return underlying_mesh.moore_connected_voxel_indices[index]; };

	/** \brief Return center of voxel at given index */
	virtual std::vector<double> voxel_center( int index )
	{ return underlying_mesh.voxels[index].center; };

	/** \brief Return mesh size */
	inline int mesh_size()
	{ return underlying_mesh.size(); };

	
	/** \brief put option of writing all cells to true */
	inline void write_all() { write_all_cells = true; } ;

	virtual int update_voxel_in_container( double x, double y, double z );

	int find_escaping_face_index(Cell* agent);

	/** \brief Outputs */
	void log_output(double t, int output_index, BioFVM::Microenvironment* microenvironment, std::ofstream& report_file, int cyclemode);
	/** \brief Write summary report of events during the output times */
	int writeCellReport(double timepoint, int cyclemode);
	int writePov(double timepoint, double scale);

	/** \brief Draw active cells of the container in SVG file */
	void draw_cells_SVG( WriteSVG* svg, std::ostream* os, double zslice, std::vector<double> lims, bool plot_nuclei, int mode_color );

};

	
#endif
