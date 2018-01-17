/**
 * \class Sphere 
 * \brief Passive spheres, can be used to simulate dextran, ECM...
 *
 * Don't do cycle, dividing... No active migration but can be pushed and repulse cells \n
 * Stronger repulsion, and difficult to move it (strong friction)
 * Created in the initial file and don't change from that. Or could add possibility of cells to degrade it.
 * 
 * \date 04/25/2017
 * \author G. Letort, Institut Curie
 */

#ifndef _Sphere_h_
#define _Sphere_h_

#include "../BioFVM/BioFVM.h"
#include "../base/utilities.h"

using namespace BioFVM;
class Cell_Container;

class Sphere : public Basic_Agent
{

	private:

		/*** \brief Maximal interaction distance factor with sphere and cells */
		double max_interaction_distance_f;
		
		/** \brief Repulsion coefficient parameter */
		double rep;

	public:

		/** \brief Default Constructor */
		Sphere(); 	
		/** \brief Destructor */
		~Sphere(){};
	
		// Virtual functions defined in basic agent
		
		/** \brief Current value of sphere radius */
		virtual double current_radius() { return get_radius(vol); };
		/** \brief Try radius depending on alignment with polarity */
		virtual double polarized_radius(Vector3d dir) { return get_radius(vol); };
		/** \brief Passive spheres doesn't have nucleus, thus return all cell radius */
		virtual double current_nuclear_radius() { return get_radius(vol); };
		/** \brief Coefficient of repulsion for passive sphere */
		virtual double get_repulsion() { return rep; };
		/** \brief Return coefficient of repulsion strength with ecm */
		virtual double get_ecm_repulsion() { return rep; };
		inline void set_repulsion(double r) { rep = r; };
		/** \brief Friction of passive cells: how easy to move them 
		 *
		 * To set the motility of passive cells */
		inline void set_friction(double f) { friction_coeff = f; };
		/** \brief Adhesive strength of passive spheres, here non-adhesives */
		virtual double get_adhesion() { return 0; };
		virtual double get_friction() { return get_surface(current_radius()) * friction_coeff; };
		/** \brief How far passive sphere can reach (be deformed) */
		virtual double max_interaction() { return max_interaction_distance_f * get_radius(vol); };
		/** \brief can constrain sphere inside the BM by adding repulsion */
		virtual void add_cell_basement_membrane_interactions( double dt, double distance );
		/** \brief don't do anything: sphere are immobiles */
		virtual void set_motility( double dt ) { return; } ;
		/** \brief don't do anything: sphere are not polarized */
		virtual void update_polarization(double dt) { return; };

		/** \brief Write current cell state, same columns as an active cells
		 *
		 * Put -1 for column were passive cell doesn't have value */
		virtual void output( std::string& delimeter, std::ofstream* file );
};

#endif
