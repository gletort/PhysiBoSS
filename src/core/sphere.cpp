#include "sphere.h"

Sphere::Sphere() : Basic_Agent()
{
	is_active = false;
	max_interaction_distance_f = 1.5;
	rep = 20;
	friction_coeff = 0.0001; //10 hard to move it
}

void Sphere::output( std::string& delimeter, std::ofstream* file )
{
	// position properties
	(*file) << ID << delimeter << position[0] << delimeter << position[1] << delimeter << position[2] << delimeter;
	// volume properties: only radius here
	(*file) << get_radius(vol) << delimeter 
		    << -1 << delimeter << -1 << delimeter 
			<< -1 << delimeter << -1 << delimeter 
			<< -1 << delimeter << -1 << delimeter;  
	// cell line, cycle phase properties, none
	(*file) << -1 << delimeter << -1 << delimeter << -1 << delimeter;
	// elapsed time
	(*file) << -1 << std::endl;
	
}	
		
void Sphere::add_cell_basement_membrane_interactions( double dt, double distance )
{
  //Note that the distance_to_membrane function must set displacement values (as a normal vector)
		
	// Repulsion from basement membrane
	double temp_r=0;
	if ( distance < get_radius(vol) )
	{
		temp_r = (1- distance/get_radius(vol));
		temp_r *=temp_r;
		temp_r *= rep;  //same repulsion coef as with cells, see if to change
	}
	if(temp_r==0)
		return;

	velocity += temp_r * displacement;	
	return;	
}
