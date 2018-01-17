#ifndef __CELL_CYCLE_CLASSIC_H
#define __CELL_CYCLE_CLASSIC_H

#include "maboss_network.h"
#include "cell.h"


class CellCycleClassic : public CellCycle 
{
	private:

		/** \brief Mode of cell cycle */
		int mode_cell_current_phase; 
		/** \brief Phase already initialized */
		bool phase_model_initialized; 
		/** \brief Mode of cell cycle updating */
		int mode_cell_and_death_parameters;

	public:

		CellCycleClassic();
		~CellCycleClassic();

		/** \brief Initialize phase values */
		void init_phase( int mode_phase, double etime );

		/** \brief Copy cell cycle classic attributes */
		void copy_cycle( CellCycleClassic* copy_me);
		
		/** \brief Do one cell cycle step */
		virtual void do_one_cycle_step( double dt, double t );
	
		/** \brief Actually do one cycle step */
		void advance_cell_current_phase( double dt );
		
		/** \brief Return current phase-dependant duration time */
		double get_duration_O2( double o2_duration );
		
		void update_cell_and_death_parameters(double);
		void update_cell_and_death_parameters_O2_based(double); 
		void update_cell_and_death_parameters_O2_based_density_arrest(double); 
		void update_cell_and_death_parameters_O2_based_volume_arrest(double); 
		
		
		////// Functions: models standards
		bool check_necrosis( double dt ); 
		/** \brief Return current necrosis rate */
		double get_necrosis_rate( double nrate );
		
		bool check_apoptosis( double dt, bool stoc_A); 
		void ki67_advanced_cycle_model( double dt, bool stoc_K1, bool stoc_K2, bool stoc_Q, bool stoc_A); 
		void ki67_advanced_cycle_model_deterministic( double dt); 
		void ki67_advanced_cycle_model_legacy( double dt); 
		void ki67_advanced_cycle_model_stochastic( double dt); 
		void ki67_basic_cycle_model( double dt, bool stoc_K, bool stoc_Q, bool stoc_A);
		void ki67_basic_cycle_model_deterministic( double dt);
		void ki67_basic_cycle_model_stochastic( double dt);
		void ki67_basic_cycle_model_legacy( double dt);
		void live_apoptotic_cycle_model( double dt);
		void total_cells_cycle_model( double dt );
		 void death_apoptosis_model( double dt, bool stochastic);
		void death_apoptosis_model_deterministic( double dt);
		void death_apoptosis_model_stochastic( double dt);
		void death_necrosis_swelling_model( double dt );
		void death_necrosis_lysed_model( double dt );
		
		/** \brief Output current phase of the cell */
		virtual void output( std::string& delimeter, std::ofstream* file );
};

#endif
