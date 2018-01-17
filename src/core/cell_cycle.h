#ifndef __CELL_CYCLE_H
#define __CELL_CYCLE_H

#include "cell.h"
#include "cell_phenotype.h"

class Cell;

/**
 * \class CellCycle
 * \brief Manage cell cycling (going from one phase to another)
 *
 * Manage current cell cycle phase of the cell, and update it according to its cycling properties. 
 * Can have different type of cycling methods: 
 *
 * 1 - \a classic (see CellCycleClassic class), similar to initial PhysiCell cycle implementation: deterministic or stochastic passages from one cell to another.
 *
 * 2 - \a network (see CellCycleNetwork class), handle the cycling and state of the cell with a boolean network instance (MaBoSS).
 *
 * This class is the mother class, contains common attributes/methods of the different cycle modes. Try to put as many common prop as possible.
 *
 * \date 05/30/2017
 * \author G. Letort, Institut Curie
 */
class CellCycle
{
	private:

	protected:
		/** \brief Pointer to its cell */
	   Cell* mycell;
	
	   /** \brief Pointer to current phase */
	   Phase* phase;

		/** \brief Time spent so far in the current phase 
		 *
		 * Set back to 0 when changing phase */
		double elapsed_time; 

		/** \brief Mode of cell cycle: 0: classic, 1: BN */
		int cycle_mode;

	public:

		CellCycle();
		virtual ~CellCycle();

		/** \brief Copy all parameters */
		CellCycle* copy();
	
		/** \brief Set current elapsed time value */
		inline void set_elapsed_time( double t ) 
		{ elapsed_time = t; } ;
	
		/** \brief set pointer to current cell */
		inline void set_cell( Cell* thecell )
		{ mycell = thecell; };
		
		/** \brief Set cell cycle phase from its code */
		void set_current_phase( int phase_code );
		
		/** \brief Set cell cycle phase from phase pointer */
		inline void set_phase( Phase* ph )
		{ phase = ph; };
		
		/** \brief Return coefficient according to cell curent phase 
		 *
		 * @param what which mode: if 0, return 2 if the cell is in mitose, 1 otherwise. If 1, return smaller coefficient for necrotic cells */
		double phase_coef( int what );

		/** \brief Return the RGB code of current phase color */
		inline std::string get_phase_color()
		{ return phase->color; };

		/** \brief Initiate apoptosis cycle phase */
		void start_apoptosis();

		/** \brief Initiate necrosis lysis phase */
		void start_necrosis_lysis();
		
		/** \brief Initiate necrosis swelling phase 
		 *
		 * @param dt current cycling time step 
		 * */
		void start_necrosis_swelling(double dt);
		
		/** \brief update volume changes rates from current phase*/
		void update_volume_change_rating();
		
		/** \brief Return calcification rate according to cycle 
		 *
		 * @return current calcification rate, determined by the current cell phase 
		 * */
		double get_calcification_rate();

		/** \brief Do one cell cycle step 
		 * Implemented and specific in daughter classes 
		 *
		 * @param dt current cycling time step 
		 * @param t current time */
		virtual void do_one_cycle_step(double dt, double t) = 0;

		/** \brief Output current phase of the cell 
		 *
		 * @param delimeter character used to separate the columns in the output file (use ";" for paraview reading )
		 * @param file stream of output of cell current states in which to write */
		virtual void output( std::string& delimeter, std::ofstream* file ) = 0;
};

#endif
