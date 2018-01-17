#ifndef __cell_volume_h__
#define __cell_volume_h__

#include <fstream>
#include "../base/utilities.h"

/**
 * \brief Volumes of a cell: total, nuclear, cytoplasmic... 
 *
 * Keep track of current volumes of the cell, and take care of evolving them towards target values. 
 * Each cell has a CellVolume attribute to handle everything related to its volume.
 * Cell has a nuclear radius, cytoplasm radius, and fluid/solid parts for each.
 *
 * Can convert radius to volume and inversely with base/utilities functions.
 * */

class CellVolume
{
	private:

		/** \brief Total volume of the cell */
		double total;
		/** \brief Total solid volume of the cell */
		double solid;
		/** \brief Total fluid volume of the cell */
		double fluid;
		/** \brief Fraction of fluid volume compared to total volume */
		double fluid_fraction; 

		double nuclear;
		double nuclear_fluid;
		double nuclear_solid; 

		double cytoplasmic;
		double cytoplasmic_fluid; 
		double cytoplasmic_solid; 

		double cytoplasmic_biomass_change_rate; 
		double nuclear_biomass_change_rate; 
		double fluid_change_rate;

		double target_solid_cytoplasmic;
		double target_solid_nuclear;
		double target_fluid_fraction;
		double target_total;
		
		double cytoplasmic_to_nuclear_fraction;

		double rupture_threshold;
		double calcified_fraction;

		// Geometry
		/** \brief Current radius of the cell */	
		double radius; 
		/** \brief Current nucleus radius */
		double nuclear_radius; 
		/** \brief Area of the surface of the cell */
		double surface_area; 

	public:

		/** \brief Default constructor */
		CellVolume();
		/** \brief Destructor, nothing special to do */
		~CellVolume(){};

		/** \brief Get cell radius */
		inline double get_radius()
		{ return radius; };
		
		/** \brief Get cell total volume */
		inline double get_total()
		{ return total; };
		
		/** \brief Get cell nucleus radius */
		inline double get_nuclear_radius()
		{ return nuclear_radius; };

		/** \brief Get sums of radii and nuclear radii */
		void sum_radii( CellVolume* other_volume, double* rad, double* nrad );

		/** \brief Multiply current volumes value by given ratio, return total volume */
		double multiply_by_ratio(double);

		/** \brief Divide current volumes by 2, return total value */
		inline double divide() 
		{ return multiply_by_ratio(0.5); };

		/** \brief Update volume changing rates from current cell cycle phase */
		void update_volume_change_rates( double change_timescale_C, double change_timescale_N, double change_timescale_F);

		/** \brief Update target solid nuclear value by given factor */
		inline void multiply_target_nuclear( double fact )
		{ target_solid_nuclear *= fact; };

		/** \brief Set value of target fluid fraction */
		inline void target_fluid( double frac )
		{ target_fluid_fraction = frac; };
		
		/** \brief update rupture threshold to 2 times total volume */
		inline void double_rupture_threshold()
		{ rupture_threshold = total * 2.0; };
		
		/** \brief Return true if total volume exceeds rupture threshold */
		inline bool rupture()
		{ return (total > rupture_threshold); };

		/** \brief Return true if radius have reached targetted values */
		inline bool targetted()
		{	return relative_diff( total, target_total ) <= UniformRandom() * 0.1; };

		/** \brief Return true if total volume exceeds given value */
		inline bool over(double val)
		{ return ( total > val ); };

		/** \brief Return current value of fluid fraction */
	   inline double fluid_frac()
	   { return fluid_fraction; };	   

		/** \brief Update current volume by current rates for time step */
		void update_volumes_timestep( double dt, double calrate );

		/** \brief Update radius values with given values
		 *
		 * If set to -1, then update according to current cell volumes */
		void update_radius( double rad = -1, double nrad = -1);

		/** \brief Update volumes from given volume fraction */
		void update_volume_from_fraction( double fraction );

		/** \brief Set current volumes so that total = given value */
		void set_volume_total( double vol );
		
		/** \brief Set new volumes so that total = given value 
		 *
		 * @param vol target total volume
		 * @param frac target fluid to solid volume fraction */
		void set_new_volume( double vol, double frac );

		/** \brief Set default init volumes from given values  
		 * \n 04/24: Added noise in the initial volumes of the cell (+-10% all volumes)
		 * */
		void set_initial_volumes( std::vector<double> vals );

		/** \brief Set target values and add small noise to targets value, usefull when copy the cell */
		void noise_target(double target_rad, double target_fraction, double target_solid_nuc);

		/** \brief Write current volume values to file */
		void output( std::string& delimeter, std::ofstream* file );
};

#endif
