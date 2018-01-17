/**
 * \brief Main class for simulation
 *
 * Init environment, cells and launch simu
 * Time parameters of simulation are in minutes
 * \todo possibility to set time scale (seconds, minutes, hours)?
 * \todo continue parameters input file construction and reading
 * \todo check memory leaks and classes destructors
 *
 * \author Gaelle Letort, Institut Curie
 * \date 20 March 2017
 * */
#include <iostream>
#include <fstream>

#ifndef __SIMUL_H__
#define __SIMUL_H__

#include "cell.h"
#include "cell_line.h"
#include "cell_container.h"
#include "../base/utilities.h"
#include "PhysiCell_constants.h"

#include "../BioFVM/BioFVM.h"  
#include "../base/readXML.h"  
#include "doSVG.h"  
#include "maboss_network.h"

class Simul
{
	private:
	   /** \brief Reaction-diffusion time step */	
		double dt;
		/** \brief Maximum (final) time */
		double t_max;
		/** \brief Output times intereval */
		double t_output_interval;
		/** \brief Output densities (microenvironment) times intereval */
		double t_output_densities;
		/** \brief Mechanics time step */
		double mechanics_dt;
		/** \brief Cell cycle time step */
		double cell_cycle_dt;

		/** \brief Which cell cycle mode is used: 0 classic, 1 boolean network*/
		int mode_cycle;

		/// Space parameters
		/** \brief Radius of cell */
		double cell_radius;
		/** \brief Membrane shape */
		std::string membrane_shape;
		/** \brief Membrane radius */
		double membrane_rad;

		/// Environment parameters
		/** \brief Microenvironment object */
		BioFVM::Microenvironment microenvironment;

		/** \brief to read XML parameter file */
		ReadXML reader;

		/** \brief Cell line properties 
		 *
		 * Different cell lines possibles: so can do co-cultures 
		 * */
		std::vector<CellLine*> cell_types;
		/** \brief proportion of each cell type at creation */
		std::vector<double> type_prop;

		Cell_Container cell_container;

		/** \brief Size of BM radius, 
		 *
		 * \todo to see where to place it */
		double duct_radius;
		/** \brief If load cells from an initial file */
		std::string init_file;
		/** \brief If file exists create ecm distribution from it */
		std::string ecm_file;

		/** \brief Minimum voxel precision */
		double min_voxel_size;

		/** \brief max height for injection */
		double max_z;

		/** \brief Number of diffusing entities to simulate */
		int ndensities;
		/** \brief Densities concentration */ 
		std::vector<double> concentrations;
		/** \brief Names of the different diffusing entities */
		std::vector<std::string> conc_names;

		/** \brief If add passive cells, number of cells */
		double npassive;
		/** \brief Friction of passive cells: how easy to move them 
		 *
		 * To set the motility of passive cells */
		double fric_passive;

		/** \brief Time from which passive cells have repulsion coefficient */
		double tpassive;
		/** \brief Time from which EGF density is added */
		double tegf;
		/** \brief Time from which TNF density is added */
		double ttnf;
		/** \brief Duration of TNF injection (keep constant concentration at first voxel) */
		double ttnf_pulse;
		/** \brief When next time TNF density will be added */
		double ttnf_next;
		/** \brief Time from which TNF density is removed */
		double tremtnf;
		/** \brief Mode of densities injection (from sides, everywhere, Dirichlet...) 
		 *
		 * Possibles modes:
		 * 0 : everywhere
		 * 1 : From all sides (boundary)
		 */
		int mode_injection;

		/** \brief Ratio of (not empty) voxels to write to output file for microenv densities */
		double ratio_write;
		/** \brief If write properties file */
		bool write_prop;

		/** \brief Boolean Network if cell cycle mode is BN */
		MaBossNetwork* maboss;

	public:

		/** \brief Default constructor */
		Simul();
		/** \brief Destructor, frees memory */
		~Simul();

		/** \brief Put default values in all attributes */
		void clear();

		/** \brief Set parameter file name value */
		inline void set_parameter_file( std::string name ) 
		{ reader.set_file(name); }; 

		/** \brief Set init file name value */
		inline void set_init_file( std::string name ) 
		{ init_file = name; }; 
		
        /** \brief Create the microenvironement object */
		void initEnvironment();
		
		/** \brief Add concentration of the given density at the extremities (same as initiation) */
		void inject_density( int density_index );

		/** \brief Remove a given density everywhere */
		void remove_density( int density_index );
		
		/** \brief Initialize simulation parameters, load xml parameter file */
		void initSimul() throw();
		
		/** \brief Initialize a cell line
		 *
		 * \todo make it more parameterizable
		 * */
		void initCellLine( CellLine* cl, int type );

		/** \brief Initialize cell cycle Boolean network style if cycle_mode==1 */ 
		void initNetwork();
		
		/** \brief Initialize all the cells */
		void initCells(std::string input_file);
		
		/** \brief run one time step */
		void run_time_step(double t);

		/** \brief Run simul for all times */
		void run();

		/** \brief Add one cell with given properties */
		void addCell( double x, double y, double z, double rad, int cl, int pha, double elap );

		/** \brief Add one cell to the all_cells set, of given position */	
		void add_one_cell( Vector3d position );
		
		/** \brief Add one passive cell (sphere) to the all_cells set, of given position and radius and friction (motility) */	
		void add_one_passive_cell( Vector3d position, double r, double fric );

		/** \brief Create a sphere of given radius full of cells of given radius */
		void create_sphere( double sphere_radius);

		/* Load ecm density values from given file */
		void load_ecm_file();
		
		/** \brief Load cells from given input file*/
		void load_txt_file();
        
		/** \brief Write Density values to output file */
		void writeDensity( int index, double t );
        
		/** \brief Load output file and plot it as SVG */ 
        void plotFile( double t, std::string input_file );

        /** \brief Initialize all the simulation (parameters, environment, cells...) */
        void initialize( std::string input_file );
		
		/** \brief Write properties of the simulation */
		void write_properties(std::ostream& os);
};

#endif

