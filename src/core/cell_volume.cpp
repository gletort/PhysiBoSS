#include <cmath>
#include "cell_volume.h"

CellVolume::CellVolume()
{
	total = 0;
	solid = 0;
	fluid = 0;
	fluid_fraction = 0;
	nuclear = 0;
	nuclear_fluid = 0;
	nuclear_solid = 0;
	cytoplasmic = 0;
	cytoplasmic_fluid = 0;
	cytoplasmic_solid = 0;
	cytoplasmic_biomass_change_rate = 0; 
    nuclear_biomass_change_rate = 0; 
	fluid_change_rate = 0;
	target_solid_cytoplasmic = 0;
	target_solid_nuclear = 0;
	target_fluid_fraction = 0;
	target_total = 0;
	cytoplasmic_to_nuclear_fraction = 0;
	rupture_threshold = 0;
	calcified_fraction = 0;
	radius = 0;
	nuclear_radius = 0;
	surface_area = 0;
}

/* Get sums of radii and nuclear radii */
void CellVolume::sum_radii( CellVolume* other_volume, double* rad, double* nrad )
{
	(*rad) = radius + other_volume->radius;
	(*nrad) = nuclear_radius + other_volume->nuclear_radius;
}

/* Update radius values with given values
* If set to -1, then update according to current cell volumes */
void CellVolume::update_radius( double rad, double nrad )
{
	if ( rad < 0 )
		radius = pow( 0.238732414638 * total , 0.33333333333333333333333333333333 );
	else 
		radius = rad;	
	if ( nrad < 0 )
		nuclear_radius = pow( 0.238732414638 * nuclear , 0.33333333333333333333333333333333 );  
	else 
		nuclear_radius = nrad;	
	surface_area = get_surface( radius );
}

/* Update current volume by current rates for time step, calcification rate given */
void CellVolume::update_volumes_timestep( double dt, double calrate )
{
	fluid += dt * fluid_change_rate *  ( target_fluid_fraction * total - fluid );
	fluid = fluid < 0 ? 0 : fluid;
	nuclear_fluid = (nuclear / total) * fluid;
	cytoplasmic_fluid = fluid - nuclear_fluid; 
	
	nuclear_solid  += dt * nuclear_biomass_change_rate * (target_solid_nuclear - nuclear_solid );    
	nuclear_solid = nuclear_solid < 0 ? 0 : nuclear_solid ;
	
	target_solid_cytoplasmic = cytoplasmic_to_nuclear_fraction * target_solid_nuclear;

	cytoplasmic_solid += dt * cytoplasmic_biomass_change_rate * ( target_solid_cytoplasmic - cytoplasmic_solid );	
	cytoplasmic_solid = cytoplasmic_solid < 0 ? 0 : cytoplasmic_solid;
	
	solid = nuclear_solid + cytoplasmic_solid;
	nuclear = nuclear_solid + nuclear_fluid; 
	cytoplasmic = cytoplasmic_solid + cytoplasmic_fluid; 
	
	calcified_fraction = dt * calrate * ( 1 - calcified_fraction );
	//std::cout  << cytoplasmic+nuclear << "\t" << total << std::endl;
	total = cytoplasmic + nuclear; 
	//std::cout << relative_diff (total, target_total ) << "\t" << total << "\t" << target_total << std::endl; 
	update_radius();
}

/* Update volumes from given volume fraction and current radius */
void CellVolume::update_volume_from_fraction( double fraction )
{
	fluid_fraction = fraction;
	total = get_volume( radius );
	fluid = fluid_fraction * total;
	solid = total - fluid;

	nuclear = get_volume( nuclear_radius );
	nuclear_fluid = fluid_fraction * nuclear;
	nuclear_solid = nuclear - nuclear_fluid;

	cytoplasmic = total - nuclear;
	cytoplasmic_fluid = fluid_fraction * cytoplasmic;
	cytoplasmic_solid = cytoplasmic - cytoplasmic_fluid;
}

/* Update volume changing rates from current cell cycle phase */
void CellVolume::update_volume_change_rates( double change_timescale_C, double change_timescale_N, double change_timescale_F )
{
	// Add some noise
	change_timescale_C += change_timescale_C * NormRandom(0,0.025);
	change_timescale_N += change_timescale_N * NormRandom(0,0.025);
	change_timescale_F += change_timescale_F * NormRandom(0,0.025);

	double divider= -log(.05);	
	cytoplasmic_biomass_change_rate = divider / change_timescale_C;	
	nuclear_biomass_change_rate = divider / change_timescale_N;	
	fluid_change_rate = divider / change_timescale_F;
}

/* Multiply current volumes value by given ratio, return total value */
double CellVolume::multiply_by_ratio( double ratio )
{
	total *= ratio;
	solid *= ratio;
	fluid *= ratio;
	nuclear *= ratio;
	nuclear_fluid *= ratio;
	nuclear_solid *= ratio;
	cytoplasmic *= ratio;
	cytoplasmic_fluid *= ratio;
	cytoplasmic_solid *= ratio;
	return total;
}
	
/* Set current volumes so that total = given value */
void CellVolume::set_volume_total( double vol )
{
	multiply_by_ratio( (vol/total) );
	update_radius();
}

/* Set current volumes so that total = given value */
void CellVolume::set_new_volume( double vol, double frac )
{
	//while ( relative_diff(total,vol) > 0.05 )
	//{
	//
	//multiply_by_ratio( (vol/total) );
	total = vol;
	nuclear = total / (1 + cytoplasmic_to_nuclear_fraction);
	update_radius();
	update_volume_from_fraction(frac);
	update_radius();
}
		
/* Set default init volumes from given values */ 
void CellVolume::set_initial_volumes( std::vector<double> vals )
{
	// add some noise
	//for ( int i = 0; i < 6; i++ )
	//{
//		vals[i] += vals[i] * NormRandom(0,0.01);
//	}
	update_radius( vals[0], vals[1] );
	update_volume_from_fraction( vals[2] );
	target_fluid_fraction = vals[2];
	target_solid_nuclear = vals[3];
	target_solid_cytoplasmic = vals[4];
	target_total = get_volume( vals[0] ) * 2;
	cytoplasmic_to_nuclear_fraction = vals[5];
}

/* Add small noise to targets value, usefull when copy the cell */
void CellVolume::noise_target( double target_rad, double target_fraction, double target_solid_nuc  )
{
	target_fluid_fraction = target_fraction;
	//target_fluid_fraction += target_fluid_fraction * NormRandom(0,0.01);
	target_total = get_volume( target_rad ) * 2;
	//target_total += target_total * NormRandom(0,0.01);
	target_solid_nuclear = target_solid_nuc;
	//target_solid_nuclear += target_solid_nuclear * NormRandom(0,0.01);
}

/* Write current volume values to file */
void CellVolume::output( std::string& delimeter, std::ofstream* file )
{
	(*file) << radius << delimeter 
		    << total << delimeter << nuclear_radius << delimeter; 
}
