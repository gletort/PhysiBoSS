#include "cell_cycle_network.h"

CellCycleNetwork::CellCycleNetwork()
{
	cycle_mode = 1;
	time_to_update = 0;
	got_activated = 0;
}

CellCycleNetwork::CellCycleNetwork( MaBossNetwork* mboss )
{
	cycle_mode = 1;
	set_maboss( mboss );
	time_to_update = ( 1 + 0.5*UniformRandom11() ) * maboss->update_time_step();
	got_activated = 0;
}

CellCycleNetwork::~CellCycleNetwork()
{
	// network will be freed from Simul class, here just pointer
	if ( maboss )
		maboss = NULL; 
}


/* Copy all paramters specific to cell cycle network object */
void CellCycleNetwork::copy_cycle( CellCycleNetwork* copy_me)
{
	maboss = copy_me->maboss;
	set_time_to_update();
	nodes = copy_me->nodes;
	networkState = copy_me->networkState;
	got_activated = 0;
}

/* Initialization: set network */
void CellCycleNetwork::set_maboss( MaBossNetwork* mboss )
{
	maboss = mboss;
	set_time_to_update();
	// initialize all nodes to 0
	nodes.resize( maboss->number_of_nodes() );
	maboss->initNetworkState();
	maboss->set_default( &nodes );
}

/* random update time, to asynchronize it between all cells */
void CellCycleNetwork::set_time_to_update()
{
	time_to_update = (1 + 0.5*UniformRandom11()) * maboss->update_time_step();
}

void CellCycleNetwork::randomize_nodes()
{
	int which;
	for ( int i = 0; i < (int) nodes.size()/10; i++ )
	{
		 which = (int) (UniformRandom() * nodes.size());
		nodes[which] = (UniformRandom() < 0.5);
	}
}

/* For a given node name, test if there are enough protein of given field 
 *
 * If the field is not present on the microenv, don't set anthing, then initial value from BN conf files is used */
void CellCycleNetwork::enough_to_node( std::string nody, std::string field )
{
	int ind = maboss->get_node_index( nody );
	if ( ind >= 0 )
	{
		int felt = mycell->feel_enough(field) ;
		if ( felt != -1 )
			nodes[ind] = felt;
	}
}

/* Set inputs of the network from current cell state */
void CellCycleNetwork::set_input_nodes()
{
	int ind;

	// Oxygen input node O2; Oxygen or Oxy
	ind = maboss->get_node_index( "Oxygen" );
	if ( ind < 0 )
		ind = maboss->get_node_index( "Oxy" );
	if ( ind < 0 )
		ind = maboss->get_node_index( "O2" );
	if ( ind >= 0 )
		nodes[ind] = ( !mycell->necrotic_oxygen() );
	

	ind = maboss->get_node_index( "Neighbours" );
	if ( ind >= 0 )
		nodes[ind] = ( mycell->has_neighbor(0) );
	
	ind = maboss->get_node_index( "Nei2" );
	if ( ind >= 0 )
		nodes[ind] = ( mycell->has_neighbor(1) );

	enough_to_node( "Glucose", "glucose" );
	enough_to_node( "EGF", "egf" );
	enough_to_node( "IGF", "igf" );
	enough_to_node( "TNF", "tnf" );
	enough_to_node( "GF", "gf" );
	enough_to_node( "TGFbeta", "tgfb" );
	enough_to_node( "IL2", "il2" );
	enough_to_node( "TCR", "tcr" );

	// If has enough contact with ecm or not
	ind = maboss->get_node_index( "ECM_sensing" );
	if ( ind >= 0 )
		nodes[ind] = ( mycell->touch_ECM() );
	// If has enough contact with ecm or not
	ind = maboss->get_node_index( "ECM" );
	if ( ind >= 0 )
		nodes[ind] = ( mycell->touch_ECM() );
	// If has enough contact with ecm or not
	ind = maboss->get_node_index( "ECMicroenv" );
	if ( ind >= 0 )
		nodes[ind] = ( mycell->touch_ECM() );
	
	// If nucleus is deformed, probability of damage
	// Change to increase proba with deformation ? + put as parameter
	ind = maboss->get_node_index( "DNAdamage" );
	//std::cout << mycell->nucleus_deformation() << std::endl;
	if ( ind >= 0 )
		nodes[ind] = ( mycell->nucleus_deformation() > 0.5 ) ? (2*UniformRandom() < mycell->nucleus_deformation()) : 0;

}

/* Dying cell step */
void CellCycleNetwork::dying_apoptosis(double dt)
{
	if ( elapsed_time >= phase->duration )
		mycell->to_remove();
	else 
	{
		if( ! mycell->over(PhysiCell_constants::cell_removal_threshold_volume) )
			mycell->to_remove();
	}
	elapsed_time += dt;
	demobilize(dt);	
}

/* Necrosis_swelling dying step */
void CellCycleNetwork::dying_necrosis_swell(double dt)
{
	if ( mycell->rupture() )
	{
		set_current_phase(PhysiCell_constants::necrotic_lysed); 
		mycell->update_target_fluid_nuclear(0,0); 
		start_necrosis_lysis(); 
	}
	elapsed_time += dt;
	demobilize(dt);	
}

/* Necrosis lysis dying step */
void CellCycleNetwork::dying_necrosis_lysed(double dt )
{
	elapsed_time += dt; 
	if( ! mycell->over(PhysiCell_constants::cell_removal_threshold_volume) )
		mycell->to_remove();	
	demobilize(dt);	
}

/* Post-mitotic cell step */
void CellCycleNetwork::after_division(double dt)
{
	if ( elapsed_time >= phase->duration )
	{
		set_current_phase( PhysiCell_constants::Ki67_negative );
		// if no G0 phase, skip directly to cycling phase
		if ( phase->duration == 0 )
		{
			set_current_phase( PhysiCell_constants::Ki67_positive_premitotic );
			mycell->update_target_nuclear(2.0);
		}

	}
	else
		elapsed_time += dt;
}

/* Update MaboSS network states */
void CellCycleNetwork::run_maboss(double dt, double t)
{
	set_input_nodes();
	bool converged;
	#pragma omp critical
	{
		converged =	maboss->run( &networkState, &nodes, mycell->get_cell_line() );
	}
	//if ( !converged )
	//	randomize_nodes();
	from_nodes_to_cell(dt, t);
	
	set_time_to_update();
}

/* Do one cell cycle step */
void CellCycleNetwork::do_one_cycle_step( double dt, double t )
{
	// Specific action for each cell phase
	switch( phase->code )
	{
		// Dying cells: we don't need to update their network
		case PhysiCell_constants::apoptotic:
			dying_apoptosis(dt);
			return;
			break;
	
		case PhysiCell_constants::necrotic_swelling:
			dying_necrosis_swell(dt);
			return;
			break;
	
		case PhysiCell_constants::necrotic_lysed:
			dying_necrosis_lysed(dt);
			return;
			break;
				
		// go to cycling if resting time passed 
		case PhysiCell_constants::Ki67_negative:
			if ( elapsed_time > phase->duration )
				do_proliferation(dt);
			break;

			// Just after division
		case PhysiCell_constants::Ki67_positive_postmitotic:
			after_division(dt);
			break;

			// Just before division
		case PhysiCell_constants::Ki67_positive_premitotic:
			// Reached size to divide
			if ( mycell->target_reached() )
			{
				set_current_phase( PhysiCell_constants::Ki67_positive_postmitotic );
				mycell->to_divide();	
				mycell->update_target_nuclear(0.5);
				return;
			}
			break;	
		default:
			break;
	}
	
	// Random chance of dying
	if ( UniformRandom() <= phase->death_rate*dt )
	{
		set_current_phase( PhysiCell_constants::apoptotic );
		start_apoptosis();
		return;
	}	
	
	// If has oxygen density, check if lacks of oxygen or not
	if ( mycell->necrotic_oxygen() )
	{
		set_current_phase( PhysiCell_constants::necrotic_swelling );
		start_necrosis_swelling(dt);
		return;
	}


	// update network every cycle_time steps only
	if ( time_to_update <= 0 )
		run_maboss(dt, t);
	else
		time_to_update -= dt;
	
	// update elapsed time in between updates
	elapsed_time += dt;
}

/* Go to proliferative if needed */
void CellCycleNetwork::do_proliferation( double dt )
{
	// If cells is in G0 (quiescent)
	if ( phase->code == PhysiCell_constants::Ki67_negative )
	{
		// switch to pre-mitotic phase 
		set_current_phase( PhysiCell_constants::Ki67_positive_premitotic );
		mycell->update_target_nuclear(2.0);
	}
}

/* Read the value of output BN nodes and transfer action to the cell */
void CellCycleNetwork::from_nodes_to_cell(double dt, double t)
{
	int ind;

	//maboss->printNodes( &networkState );

	////// Nodes related to cell death
	// Die if not enough oxygen, BN output Hypoxia or Necrosis or Autophagy (not enough glucose)
	// Necrosis
	int necr = 0;
	ind = maboss->get_node_index( "Hypoxia" );
	if ( ind >= 0 ) 
		necr += nodes[ind];
	ind = maboss->get_node_index( "Necrosis" );
	if ( ind >= 0 ) 
		necr += nodes[ind];
	ind = maboss->get_node_index( "Autophagy" );
	if ( ind >= 0 ) 
		necr += nodes[ind];
	if ( necr )
	{
		set_current_phase( PhysiCell_constants::necrotic_swelling );
		start_necrosis_swelling(dt);
		return;
	}
	
	ind = maboss->get_node_index( "NonACD" );
	if ( ind >= 0 && nodes[ind] ) 
	{
		set_current_phase( PhysiCell_constants::necrotic_swelling );
		start_necrosis_swelling(dt);
		return;
	}
	
	// Apoptosis
	ind = maboss->get_node_index( "Apoptosis" );
	if ( ind >= 0 && nodes[ind] )
	{
		set_current_phase( PhysiCell_constants::apoptotic );
		start_apoptosis();
	}
	
	ind = maboss->get_node_index( "Migration" );
	if ( ind >= 0 )
		mycell->evolve_motility_coef( nodes[ind], dt );
	
	ind = maboss->get_node_index( "Polarization" );
	if ( ind >= 0 )
		mycell->evolve_polarity_coef( nodes[ind], dt );
	
	ind = maboss->get_node_index( "Cell_cell" );
	if ( ind >= 0 )
		mycell->evolve_cellcell_coef( nodes[ind], dt );
	
	ind = maboss->get_node_index( "Matrix_adhesion" );
	if ( ind >= 0 )
		mycell->evolve_integrin_coef( nodes[ind], dt );
	
	ind = maboss->get_node_index( "Matrix_modif" );
	if ( ind >= 0 )
	{
		mycell->set_mmp( nodes[ind] );
	}	
	
	ind = maboss->get_node_index( "EMT" );
	if ( ind >= 0 )
	{
		mycell->set_mmp( nodes[ind] );
	}	
	//	std::cout << "Degradation of matrix feedback not yet implemented." << std::endl;

	
	// Proliferation: cell going into cycling and growing
	int pro = 0;	
	ind = maboss->get_node_index( "Proliferation" );
	if ( ind >= 0 )
		pro += nodes[ind];
	ind = maboss->get_node_index( "Cell_growth" );
	if ( ind >= 0 )
		pro += nodes[ind];
	ind = maboss->get_node_index( "Survival" );
	if ( ind >= 0 )
		pro += nodes[ind];
	if ( pro > 0 )
		do_proliferation( dt );
	
	mycell->freezing( 0 );
	ind = maboss->get_node_index( "Quiescence" );
	if ( ind >= 0 && nodes[ind] )
		mycell->freezing(1);
	ind = maboss->get_node_index( "CCA" );
	if ( ind >= 0 && nodes[ind] )
		mycell->freezing(1);
	
	ind = maboss->get_node_index( "Cell_freeze" );
	if ( ind >= 0 )
	{
		mycell->freezer( 3*nodes[ind] ); // 3 for bitwise operation
	}


	//// Production of a density as feedback of active node
	// For model with TNF production, keep in memory time of first activation	
	ind = maboss->get_node_index( "NFkB" );
	if ( ind >= 0 )
	{
		// produce some TNF
		if ( nodes[ind] )
		{
			mycell->secrete("tnf", 1, dt);
			if ( got_activated == 0 )
			{
				//std::cout << t << std::endl;
				got_activated = t;
			}
		}
		else 
			mycell->secrete("tnf", 0, dt);
	}
	
	// others, active or not field secretion
	int nf = 2;
	std::string fieldsNodes[nf] = {"TGFbeta", "IL2"};
	std::string fields[nf] = {"tgfb", "il2"};
	for ( int f = 0; f < nf; f ++ )
	{
		ind = maboss->get_node_index( fieldsNodes[f] );
		if ( ind >= 0 )
			mycell->secrete(fields[f], nodes[ind], dt);
	}	
}

/* De-mobilize the cell components (dying cells): actin, cadherins, integrins */
void CellCycleNetwork::demobilize(double dt)
{
		mycell->evolve_motility_coef( 0, dt );
		mycell->evolve_polarity_coef( 0, dt );
		mycell->evolve_cellcell_coef( 0, dt );
		mycell->evolve_integrin_coef( 0, dt );
}

/* Output current phase of the cell */
/** \todo Create configuratble output (e.g. clickables options from Jupyter notebook ?) */
void CellCycleNetwork::output( std::string& delimeter, std::ofstream* file )
{
	int ind;
	(*file) << mycell->contact_cell() << delimeter;
	(*file) << phase->code << delimeter;
	ind = maboss->get_node_index("Matrix_adhesion");
	if ( ind >= 0 )
			(*file) << mycell->local_density("tgfb") << delimeter << -1 ;
	//	(*file) << (nodes[ind] ? 1:0) << delimeter;
	else
	{
		ind = maboss->get_node_index( "Survival" );
		if ( ind >= 0 )
		{
			(*file) << got_activated << delimeter;
			(*file) << -1;
			// tmp solution to print current network state 
			//for ( int val : nodes )
			//	(*file)	<< val;
		}	
	   else 
			(*file) << -1 << delimeter << -1;

	}
}

