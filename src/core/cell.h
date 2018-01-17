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

#ifndef _Cell_h_
#define _Cell_h_

#include "../BioFVM/BioFVM.h"
#include "cell_container.h"
#include "PhysiCell_constants.h"

#include "cell_volume.h"
#include "cell_phenotype.h"
#include "cell_cycle.h"
#include "cell_line.h"
#include "maboss_network.h"
#include "../base/vector3d.h"
#include "../base/writeSVG.h"

using namespace BioFVM; 

class CellCycle;
class Cell_Container;
class CellLine;

/** \brief Dynamic (alive) cell (move, interact, divide, die...)
 *
 * A cell object must have pointer to its CellLine, defining its common properties.
 * Plus has its own properties (current volume, current phenotype index, current phase...).
 * And its mechanics information (position, velocity...).
 * Can do a lot of things
 *
 * From initial PhysiCell distribution
 * \author Adapted by G. Letort, Institut Curie
 * \date 25/03/2017.
 */
class Cell : public Basic_Agent 
{
friend class CellCycle;

	private:

		/** \brief Indicates if has already been flagged or not for removal */
		int removal_flagged;
		/** \brief Indicates if has already been flagged or not for division */
		int division_flagged;

		/** \brief Current polarity (Front-Rear) axis of the cell */	
		Vector3d polarization; 
		
		/** \brief Cell cycle functions, states.. */
		CellCycle* ccycle;	
		
		/** \brief Indicates level of cell freezing: 0 not, 1 cannot change volume, 3 (for bitwise operations) cannot change volume and move */
		int freezed;

		/** \brief Indicates if cell MMP are activated or not (degrading the matrix) */
		int mmped;

	protected:
		/** \brief Size properties of the cell (radius, volume) */	
		CellVolume volume; 
		
		/** \brief A number (p) between 1 and 0, showing how polarized the cell is */	
		double polarized; 

		/** \brief Percentage of current motility amplitude (evolve between 0 and 1) */
		double pmotility;
		
		/** \brief Percentage of current adhesion strength (evolve between 0 and 1) */
		double padhesion;
		
		/** \brief Percentage of current integrins level (evolve between 0 and 1) */
		double pintegrin;


	public:

		/** \brief Pointer to ommon properties object of cell type */
		CellLine* cell_line;
		/** \brief Vector containing reference to neihbors cells, temporary used each time step */	
		std::vector<Cell*> neighbors;
		/** \brief Active motility of the cell */
		Vector3d motility;  

		/** \brief Add repulsive and attractive forces between neighboring cells and ECM */ 	
		void add_potentials(Cell*); 
		/** \brief Put all the reactions (uptake and secretion) the current cell to 0 */
		void turn_off_reactions(double);


		/** \brief Default constructor */
		Cell();
		/** \brief Destructor */
		~Cell();

		/** \brief Initialize environement uptake, secretion and saturation rates vectors */
		void init_env_rates();

		/** \brief Assign cell line */
		void set_cell_line( CellLine* cl); 
		
		/** \brief Get cell line index */
		int get_cell_line();
		
		/** \brief Load default values from cell line */
		void load_default_values();

		/** \brief Flag cell to be divived */
		void to_divide();
		
		/** \brief One time step for the cell */
		void step(double dt);
		
		/**  \brief Divide current cell
		 * 
		 * Along current orientation axis if polarized:
		 *   d = (1-p)(r-(r.theta)theta) ~perpendicular + (r.theta) theta ~parallel; then normalised
		 *   (Orientation is front-rear polarity of cell, spindle axis is parallel to FR-polarity: see Reffay et al 2011, Thery et al 2006)
		 *   Kid = new cell as same properties of mother cell
		 * */
		void divide();
		
		/** \brief Flag cell to be removed */
		void to_remove();
		
		/** \brief Remove it from the cell container 
		 *
		 * Cell doesn't exist anymore after that function. Rest in peace.
		 * */
		void die();
		
		/** \brief Update the value of freezing of the cell with bitwise operation
		 *
		 * Do a bitwise-or comparison on freezed and input parameter:
		 * if freezed = 0, it will be the value of the parameter frozen
		 * if freezed = 1, it will be either 1 (frozen = 0) or 3 (frozen = 3) */
		void freezer( int frozen );
		
		/** \brief Set the value of freezed */
		inline void freezing( int frozen )
		{ freezed = frozen; };

		/** \brief Copy the attributes of the cell to current cell */
		void copy_cell(Cell *);
		/** \brief Copy the rates vector 
		 *
		 * @param copyme cell from which to copy rates */
		void copy_rates( Cell* copyme );
	
		/** \brief Return pointer to the cell_container elements in which the cell is */
		Cell_Container* cell_container();

		void intialize_functions( void ); 		

		/** \brief Change the secretion rate of given density
		 *
		 * Change to a given amount=fact*secretion 
		 * @param field name of the microenv density to change secretion rate of
		 * @param fact multiply current value of secretion rate by fact 
		 * @param dt current time step of cell uptaking */
		void secrete(std::string field, double fact, double dt);

		/** \brief Return level of protein given by index around the cell */
		double local_density(std::string field);
		
		/** \brief Return if level of protein given by index around the cell is high enough (compared to given threshold) 
		 *
		 * Return -1 if the asked field does not exists in the µenv. */
		int feel_enough(std::string field);

		/** \brief Return true if level of oxygen is lower than critical level */
		bool not_enough_oxygen();

		/* Return true if level of oxygen is lower than necrosis critical level */
		bool necrotic_oxygen();
		
		/** \brief Return oxygen mediated necrosis rate */
		double oxygen_necrosis();
		
		/** \brief Return oxygen mediated proliferation */
		double oxygen_prolif();

		///// Interactions with neighbors and BM

		/** \brief Return coefficient of repulsion strength with ecm */
		virtual double get_ecm_repulsion();
		
		virtual double get_repulsion();
		virtual double get_adhesion();

		/** \brief Gives current friction coefficient value */
		virtual double get_friction();
		virtual void add_cell_basement_membrane_interactions(double t, double dist);
		virtual double max_interaction();

		/** \brief Calculate adhesion coefficient with other cell */
		double adhesion( Cell* other_cell );
	
		/** \brief Return amount of contact with other cells */
		inline double contact_cell()
		{ return cell_contact / volume.get_radius() ; };

		/** \brief Return if cell has enough contact with other cells (compared to given threshold) 
		 *
		 * @param level: level of neighbors (0: lower threshold, 1: 2 times the threshold ) */	
		bool has_neighbor(int level);

		/** \brief Return amount of nucleus deformation */
		inline double nucleus_deformation()
		{ return nucleus_deform / volume.get_nuclear_radius() ; };
		
		/** \brief Return amount of contact with ECM */
		inline double contact_ecm()
		{ return ecm_contact / volume.get_radius() ; };
		
		/** \brief Return if cell has enough contact with ECM (compared to given threshold) */	
		bool touch_ECM();

		/** \brief Change the current value of cell cell adhesion percent coeff, increase or decrease according to up value */
		inline void evolve_cellcell_coef( int up, double dt )
		{ evolve_coef( up, &padhesion, dt ); };
		
		/** \brief Change the current value of integrin percent coeff, increase or decrease according to up value */
		inline void evolve_integrin_coef( int up, double dt )
		{ evolve_coef( up, &pintegrin, dt ); };
		
		/** \brief Return value of adhesion strength with ECM according to integrin level */
		double integrinStrength();

		//////////// Polarization functions //////////////////
		/** \brief Assign null polarization parameters */
		inline void null_polarization() { polarized = 0; polarization.set(0,0,0); };
		/** \brief Assigns a random orientation to the cell */
		inline void assign_polarization() { polarization.randomize_normed(); }
		/**  \brief Update orientation of cell, align it to velocity direction according to cell persistence
		 *
		 * p(t+dt) = (p(t) + dt*v(t)) / (1+dt/tau) */
		virtual void update_polarization( double dt);  
		
		/** \brief Try: Radius depending on aligment with current polarity */
		double polarized_radius( Vector3d dir ); 
	
		/** \brief Change the current value of polariity percent coeff, increase or decrease according to up value 
		 *
		 * Add limit to 0.9 to have small noise always even for high polarization
		 * */
		inline void evolve_polarity_coef( int up, double dt )
		{ evolve_coef( up, &polarized, dt ); if (polarized>=0.91) polarized=0.9; };
	

		///// Functions related to motion		
		
		void set_previous_velocity(double xV, double yV, double zV);
		
		/** \brief Update cell current velocity */
		void update_cell_motion( double time_since_last );

		/** \brief Calculate motility forces according to mode:
		 *
		 * 0, random; 1, along polarity axis; other: nothing
		 * */
		void set_motility(double ); 
	
		/** \brief Motility with random direction, and magnitude of motion given by customed coefficient */
		void set_3D_random_motility( double dt );
		
		/**
		 * Motility in the polarity axis migration
		 * Strength of alignement depends of the polarity parameter, as for division axis
		 * Persistence defined in the polarization direction updating.
		 * Polarity coefficient never reach 1 so there is some noise
		 * */
		void set_3D_polarized_motility( double dt );

		/** \brief Change the current value of motility percent coeff, increase or decrease according to up value */
		inline void evolve_motility_coef( int up, double dt )
		{ evolve_coef( up, &pmotility, dt); };

		////// Functions related to volume

		void set_total_volume(double);
		void set_new_volume(double vol);

		/** \brief Set the initial volumes values */
		inline void set_initial_volumes( std::vector<double> vols )
		{ volume.set_initial_volumes( vols ); };

		/** \brief Update volumes of the cell 
		 *
		 * Can choose between different mode of volume updating, with cell_line->mode_volume parameter.
		 * Currently, only one mode (update_volume_default) proposed */
		void update_volume(double); 
	
		/** \brief Update the volumes of the cell, through CellVolume object and in BioFVM
		 *
		 * Call the update of volume of its CellVolume object. Set also its current volume in BioFVM with its new value. */	
		void update_volume_default(double); 
		
		/** \brief update volume changes rates from given values */
		inline void update_volume_change_rating( double c, double n, double f)
		{ volume.update_volume_change_rates( c, n, f); };
		
		/** \brief update target fluid fraction value */
		inline void update_target_fluid( double frac )
		{ volume.target_fluid(frac); };
	
		/** \brief update target solid nuclear value */
		inline void update_target_nuclear( double fact )
		{ volume.multiply_target_nuclear( fact ); };

		/** \brief update rupture threshold to 2 times total volume */
		inline void double_rupture_threshold()
		{ volume.double_rupture_threshold(); };
	
		/** \brief Return true if total volume exceeds rupture threshold */
		inline bool rupture()
		{ return volume.rupture(); };

		/** \brief Return true if total volume exceeds given value */
		inline bool over(double val)
		{ return volume.over(val); };

		/** \brief Return true if the cell is big enough to divide (reached target sizes) */
		inline bool target_reached()
		{ return volume.targetted(); };

		void update_radius();
		virtual double current_radius() { return volume.get_radius(); };
		virtual double current_nuclear_radius() { return volume.get_nuclear_radius(); };
		
		/** \brief Degrade the surrounding ECM 
		 *
		 * @param dt time step of mechanics, to scale degradation amount
		 * Currently, handle only the case of ECM as a density */
		void degrade_ecm( double dt );		

		/** \brief (De)-Activate ECM degradation by the cell */
		inline void set_mmp( int activate )
		{ mmped = activate; };


		/////////// Functions related to cell cycle

		/** \brief Create the cell cycle object
		 *
		 * Cycle type is classic if pointer to network is NULL
		 * MaBoSS type if network is given
		 *
		 * Phase is phase code of initial cell phase
		 * */
		void set_cycle( MaBossNetwork* mboss, int phase, double elap );
		
		/** \brief Initialize values of cell cycle, check if do it in other way */
		void init_cell_cycle_classic( int mode_phase, double etime );
		
		/** \brief Update cell cycle state */
		void update_cycle( double cycle_dt, double time_since_last, double t );

		/** \brief Update target fluid and nuclear values */
		inline void update_target_fluid_nuclear( double fluid, double nuclear )
		{ update_target_fluid(fluid); update_target_nuclear(nuclear); };

		/////// Output/Input functions

		void save_all_cells_to_matlab( std::string filename ); 
		
		/** \brief Write phase current property in given file stream */
		virtual void output( std::string& delimeter, std::ofstream* file );
		
		/** \brief Get colors value according to color mode */
		void get_colors( std::vector<std::string>* cols, int mode );
		/**\brief Simple mode of coloring
		 *
		 * Fill values of colors to use: cyto_color, cyto_outline , nuclear_color, nuclear_outline */
		void simple_cell_coloring( std::vector<std::string>* cols );
		/**\brief Phase mode of coloring
		 *
		 * Fill values of colors to use according to cell phase: cyto_color, cyto_outline , nuclear_color, nuclear_outline */
		void phase_cell_coloring( std::vector<std::string>* cols );
		/** \brief Draw the current cell in SVG file */
		void drawSVG( WriteSVG* svg, std::ostream* os, int zslice, std::vector<double> lims, bool plot_nuclei, int mode_color );

};
#endif
