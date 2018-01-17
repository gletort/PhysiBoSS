#include "cell_cycle_classic.h"

CellCycleClassic::CellCycleClassic() : CellCycle()
{
	cycle_mode = 0;
	mode_cell_current_phase = 0;
	mode_cell_and_death_parameters = 0;
	phase_model_initialized = false;
}

CellCycleClassic::~CellCycleClassic()
{
}

void CellCycleClassic::copy_cycle( CellCycleClassic* copy_me)
{
	mode_cell_current_phase = copy_me->mode_cell_current_phase;
	phase_model_initialized = false;
}

/* Initialize phase */
void CellCycleClassic::init_phase( int mode_phase, double etime )
{
	mode_cell_current_phase = mode_phase;
	phase_model_initialized = false;
	elapsed_time = etime;
}

void CellCycleClassic::do_one_cycle_step( double dt, double t )
{
	advance_cell_current_phase(dt);
}

void CellCycleClassic::advance_cell_current_phase(double dt) 
{ 
	switch( mode_cell_current_phase )
	{
		case 0:
			return;
			break;
		case 1:
			death_necrosis_swelling_model(dt);
			break;
		case 2:
			death_necrosis_lysed_model(dt);
			break;
		case 3:
			death_apoptosis_model_stochastic(dt);
			break;
		case 4:
			death_apoptosis_model_deterministic(dt);
			break;
		case 5:
			ki67_advanced_cycle_model_legacy(dt);
			break;
		case 6:
			ki67_advanced_cycle_model_stochastic(dt);
			break;
		default:
			return;
			break;
	}
}

bool CellCycleClassic::check_necrosis(double dt)
{
	double necrosis_rate = get_necrosis_rate( mycell->oxygen_necrosis() );
	double probability_necrosis = dt * necrosis_rate; 
	
	if(uniform_random() < probability_necrosis)
	{
		set_current_phase(PhysiCell_constants::necrotic_swelling); 
		phase_model_initialized = false;
		mode_cell_current_phase = 1;  // death_necrosis_swelling_model 
		return true;
	}
	return false;
}

/* Return current necrosis rate */
double CellCycleClassic::get_necrosis_rate( double nrate )
{
	double res;
	switch( mode_cell_and_death_parameters )
	{
		case 0:
			res = nrate;
			break;
		default:
			res = phase->necrosis_rate;
			break;
	}
	return res;
}

bool CellCycleClassic::check_apoptosis( double dt, bool stoc_A )
{
	double probability_apoptosis = dt * phase->death_rate; 
	if( uniform_random() < probability_apoptosis )
	{
		// set the current phase to apoptotic 
		set_current_phase(PhysiCell_constants::apoptotic); 
		phase_model_initialized = false;
		mode_cell_current_phase = stoc_A ? 3 : 4; // death_apoptosis_model_stochastic: death_apoptosis_model_deterministic; 
		return true; 
	}	
	return false;
}

/////////////////////////// Standard models  ////////////////////////////////////////

/* ki67 model, O2 dependant for ki67_negative phase */
void CellCycleClassic::ki67_basic_cycle_model( double dt, bool stoc_K, bool stoc_Q, bool stoc_A)
{
	// necrotic death? 
	bool is_necrotic = check_necrosis(dt);
	if (is_necrotic)
		return;
	
	// apoptotic death? 
	bool is_apoptotic = check_apoptosis(dt, stoc_A);
	if (is_apoptotic)
		return;
	
	// K phase
	if( phase->code == PhysiCell_constants::Ki67_positive )
	{
		if( phase_model_initialized == false )
		{
			// set phase-specific parameters 
			mycell->update_target_nuclear(2.0);
			phase_model_initialized = true; 
		}
	
		double duration = phase->duration;
		double probability_K_Q = dt / duration;		
		// advance to Q phase? 
		if( (!stoc_K && elapsed_time >= duration-0.001 ) ||  (stoc_K && uniform_random() < probability_K_Q ) )
		{
			// flag the cell for division
			mycell->to_divide(); 
			set_current_phase(PhysiCell_constants::Ki67_negative); 
			phase_model_initialized = false;
			return; 
		}
	}
	
	// Q phase 
	if( phase->code == PhysiCell_constants::Ki67_negative )
	{
		if( !phase_model_initialized )
		{
			// set phase-specific parameters 
			mycell->update_target_nuclear(0.5);
			phase_model_initialized = true; 
		}

		// model Ki67_negative is oxygen level dependant for duration time	
		double duration = get_duration_O2( mycell->oxygen_prolif() );	

		// advance to K phase? 
		double probability_Q_K = (dt / duration); 
		if( (!stoc_Q && elapsed_time >= duration-0.001 ) ||  (stoc_Q && uniform_random() < probability_Q_K ) )
		{
			set_current_phase(PhysiCell_constants::Ki67_positive_premitotic); 
			phase_model_initialized = false;
			return; 
		}
	}

	elapsed_time += dt;  
}

void CellCycleClassic::ki67_basic_cycle_model_deterministic(double dt)
{
	ki67_basic_cycle_model(dt, false, false, false);
}

void CellCycleClassic::ki67_basic_cycle_model_stochastic(double dt)
{
	ki67_basic_cycle_model(dt, true, true, true);
}

void CellCycleClassic::ki67_basic_cycle_model_legacy(double dt)
{
	ki67_basic_cycle_model(dt, false, true, false);
}

void CellCycleClassic::ki67_advanced_cycle_model(double dt, bool stoc_K1, bool stoc_K2, bool stoc_Q, bool stoc_A)
{
	// necrotic death? 
	bool is_necrotic = check_necrosis(dt);
	if (is_necrotic)
		return;
	
	// apoptotic death? 
	bool is_apoptotic = check_apoptosis(dt, stoc_A);
	if (is_apoptotic)
		return;
	
	// K1 phase
	if( phase->code == PhysiCell_constants::Ki67_positive_premitotic )
	{
		if( !phase_model_initialized )
		{
			// set phase-specific parameters 
			mycell->update_target_nuclear(2.0);
			phase_model_initialized = true; 
		}
		
		double duration = phase->duration;
		double probability_K1_K2 = dt / duration;		
		// advance to K2 phase? 
		if( (!stoc_K1 && elapsed_time >= duration-0.001 ) ||  (stoc_K1 && uniform_random() < probability_K1_K2 ) )
		{
			// return true: flag the cell for division 
		    mycell->to_divide();	
			set_current_phase(PhysiCell_constants::Ki67_positive_postmitotic); 
			phase_model_initialized = false;
			return ; 
		}
	}
	
	// K2 phase
	if( phase->code == PhysiCell_constants::Ki67_positive_postmitotic )
	{
		if( !phase_model_initialized )
		{
			// set phase-specific parameters 			
			mycell->update_target_nuclear(0.5);
			phase_model_initialized = true; 
		}
		
		double duration = phase->duration;
		double probability_K2_Q = dt / duration;
			
		// advance to Q phase? 
		if( (!stoc_K2 && elapsed_time >= duration -0.001 ) || (stoc_K2 && uniform_random() < probability_K2_Q))
		{
			set_current_phase(PhysiCell_constants::Ki67_negative); 
			phase_model_initialized = false;
			return; 
		}
	}	
	
	// Q phase 
	if( phase->code == PhysiCell_constants::Ki67_negative )
	{
		if( !phase_model_initialized )
			phase_model_initialized = true;

		// model Ki67_negative is oxygen level dependant for duration time	
		double duration = get_duration_O2( mycell->oxygen_prolif() );	
		
		// advance to K1 phase? 
		double probability_Q_K1 = (dt / duration); 
		
		if( (!stoc_Q && elapsed_time >= duration -0.001 ) || (stoc_Q && uniform_random() < probability_Q_K1))
		{
			set_current_phase(PhysiCell_constants::Ki67_positive_premitotic); 
			phase_model_initialized = false;
			return; 
		}
	}

	elapsed_time += dt; 
	return ; 
}

void CellCycleClassic::ki67_advanced_cycle_model_deterministic(double dt)
{
	ki67_advanced_cycle_model(dt, false, false, false, false);
}

void CellCycleClassic::ki67_advanced_cycle_model_legacy(double dt)
{
	ki67_advanced_cycle_model(dt, false, false, true, false);
}

void CellCycleClassic::ki67_advanced_cycle_model_stochastic(double dt)
{
	ki67_advanced_cycle_model(dt, true, true, true, true);
}

void CellCycleClassic::live_apoptotic_cycle_model( double dt)
{
	// necrotic death? 
	bool is_necrotic = check_necrosis(dt);
	if (is_necrotic)
		return;
	
	// apoptotic death? 
	bool is_apoptotic = check_apoptosis(dt, false);
	if (is_apoptotic)
		return;
	
	// live phase 
	if( phase->code == PhysiCell_constants::live )
	{
		// parameters set? 
		if( !phase_model_initialized )
		{
			// set phase-specific parameters 
			update_volume_change_rating();
			phase_model_initialized = true; 
		}		
		
		// divide? 
		double probability_division = dt / phase->duration; 
		if( uniform_random() < probability_division )
		{
			// flag the cell for division ;
			mycell->to_divide(); 
			elapsed_time = 0.0; 
			return; 
		}
	}
	elapsed_time += dt; 
}

void CellCycleClassic::total_cells_cycle_model(double dt )
{	
	elapsed_time += dt; 
}

void CellCycleClassic::death_apoptosis_model(double dt, bool stochastic)
{
	if( !phase_model_initialized )
	{
		// set phase-specific parameters 
		start_apoptosis(); 
		phase_model_initialized = true;
	}
		
	double duration = phase->duration;
	double probability_removal = dt / duration ; 
		
	if((!stochastic && elapsed_time >= duration - 0.001 ) || (stochastic && uniform_random() < probability_removal))
	{
		mycell->to_remove();
		return; 
	}
	elapsed_time += dt; 
	return;
}

void CellCycleClassic::death_apoptosis_model_stochastic(double dt)
{
	death_apoptosis_model(dt, true);
}

void CellCycleClassic::death_apoptosis_model_deterministic(double dt)
{
	death_apoptosis_model(dt, false);
}

void CellCycleClassic::death_necrosis_swelling_model(double dt )
{
	if( !phase_model_initialized )
	{
		// set phase-specific parameters 
		start_necrosis_swelling(dt);
		phase_model_initialized = true; 
	}
	elapsed_time += dt; 
	if( mycell->rupture() )
	{
		set_current_phase(PhysiCell_constants::necrotic_lysed); 
		phase_model_initialized = false;
		mode_cell_current_phase = 2; // death_necrosis_lysed_model; 			
		mycell->update_target_fluid_nuclear(0,0); 
		return; 
	}		
}

void CellCycleClassic::death_necrosis_lysed_model(double dt )
{
	if( !phase_model_initialized )
	{
		// set phase-specific parameters 
		start_necrosis_lysis(); 
		phase_model_initialized = true; 
	}
	elapsed_time += dt; 
	if( ! mycell->over(PhysiCell_constants::cell_removal_threshold_volume) )
		mycell->to_remove();	
}

/* Return current duration time of the phase, according to model and env condition*/
double CellCycleClassic::get_duration_O2( double o2_duration )
{
	double res;
	switch( mode_cell_and_death_parameters )
	{
		case 0:
			if ( phase->code == PhysiCell_constants::Ki67_negative )
			{
				double dres = o2_duration;
				if ( dres == INT_MAX ) res = INT_MAX;
				else res = dres * phase->duration;
			}
			else
				res = phase->duration;
			break;
		default:
			break;
	}
	return res;
}


/* Output current phase of the cell */
void CellCycleClassic::output( std::string& delimeter, std::ofstream* file )
{
	(*file) << 0 << delimeter;
	(*file) << phase->code << delimeter;
	(*file) << elapsed_time << delimeter;
}
