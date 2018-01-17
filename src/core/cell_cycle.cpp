#include "cell_cycle.h"
#include "cell_cycle_classic.h"
#include "cell_cycle_network.h"

CellCycle::CellCycle()
{ 
	elapsed_time = 0;
	cycle_mode = 0;
	mycell = NULL;
	phase = NULL;
}

CellCycle::~CellCycle()
{
	if ( mycell )
		mycell = NULL;
	if ( phase )
		phase = NULL;
}

/* Copy all parameters */
CellCycle* CellCycle::copy()
{
	CellCycle* res = NULL;
	if ( cycle_mode == 0 )
	{
		res = new CellCycleClassic();
		(static_cast<CellCycleClassic*>(res))->copy_cycle( static_cast<CellCycleClassic*>(this) );
	}
	else
	{
		res = new CellCycleNetwork();
		(static_cast<CellCycleNetwork*>(res))->copy_cycle( static_cast<CellCycleNetwork*>(this) );
	}
	res->set_phase( phase );
	res->elapsed_time = elapsed_time;
	return res;
}

/* update volume changes rates from current phase*/
void CellCycle::update_volume_change_rating()
{
	mycell->update_volume_change_rating( 
			phase->volume_change_timescale_C,
			phase->volume_change_timescale_N,
			phase->volume_change_timescale_F );
}

/* Set cell cycle phase */
void CellCycle::set_current_phase( int phase_code )
{
	// initial state is cycling, update directly nucleus target size
	if ( phase == NULL && phase_code == PhysiCell_constants::Ki67_positive_premitotic )
		mycell->update_target_nuclear(2.0);
		
	phase = mycell->cell_line->get_phase(phase_code);
	elapsed_time = 0;
	update_volume_change_rating();
}

/* Return coefficient according to cell curent phase */
double CellCycle::phase_coef( int what )
{
	// Coeff stronger if in mitose
   if ( what == 0 )
   {	   
		if ( (phase->name == "Ki67_postmitotic" ) )
		   return 2;
		else
			return 1;	
   }
   // Coef smaller for necrosis cells
   if ( what == 1 )
   {	   
		if ( (phase->name == "necrotic_swelling" ) || ( phase->name == "necrotic_lysed" ) || (phase->name == "necrotic") )
		   return 0.1;
		else
			return 1;
   }
   return 1;
}

/* Initiate apoptosis cycle phase */
void CellCycle::start_apoptosis()
{
	mycell->turn_off_reactions(0.1);
	mycell->update_target_fluid_nuclear(0, 0);
}

/* Initiate necrosis lysis phase */
void CellCycle::start_necrosis_lysis()
{
	mycell->update_target_fluid(0);
	mycell->update_target_nuclear(0);
}

/* Initiate necrosis swelling phase */
void CellCycle::start_necrosis_swelling(double dt)
{
	mycell->turn_off_reactions(dt);
	mycell->update_target_fluid_nuclear(1, 0);
	mycell->double_rupture_threshold();
}

double CellCycle::get_calcification_rate()
{
	return phase->calcification_rate;
}
