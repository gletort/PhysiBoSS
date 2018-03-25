/** 
 * Class for cell properties (parameters) common to cells of the same cell type
 * Contains the phenotypes and phases definition.
 * Each cell of class Cell points to a CellLine object, and has information of which phenotype index is current, which phase index.
 * Has to be initialised correctly
 *
 * \brief Cell common properties for each cell types
 *
 * Adapted on 04/04/2017, Letort G., Institut Curie
 */

#ifndef __cell_line_h__
#define __cell_line_h__

#include <vector>
#include <fstream>
#include <unordered_map>

#include "../base/utilities.h"
#include "../base/readXML.h"
#include "cell.h"

class CellLine
{
	friend class Cell;
	
	private:

		/** \brief Name of the current cell line */	
		std::string name; 
		/** \brief Index of the current cell line */
		int line_index;

		/** \brief Coefficient of active migration strength, min and max */
		double motility_magnitude[2];
		
		/** \brief Strength of adhesion between cell and ECM */
		double Cecm[2]; 
			
		/** \brief Rate of ECM degradation */
		double mmp;

		/** \brief Cell-ECM repulsion coefficient */
		double Cecmr;

		/** \brief Default initial volumes values */
		std::vector<double> def_volumes;
		
		/** \brief Threshold value of concentration to be ON 
		 *
		 * First value is the name of the field to consider.
		 * Second value is the corresponding threshold. */
		std::map<std::string, double> prot_threshold;

		/** \brief Cell line phenotype: set of cycle phases */
		CellPhenotype line_phenotype;

	protected:

		double o2_necrotic; // mmHg
		double o2_critical; // mmHg
	
		double o2_no_proliferation; // mmHg
		double o2_hypoxic; // mmHg
		double o2_ref; // mmHg
		double o2_saturation; // mmHg
	
		double max_necrosis_rate; // 1 day^-1
		double init_uptake_rate; // initial uptake rate
		
		/** \brief Amount of densities secretion when activated */
		double secretion;

		double glucose_necrosis_threshold;  // dimensionless 
		//double maximum_stretch; 
		double max_interaction_distance_factor;

		double Cccr; //used in calculating cell-cell repulsion			
		double Ccba; //used in calculating cell-basement membrane repulsion	
		double Ccbr; //used in calculating cell-basement membrane repulsion	
		

		/** \brief Threshold of cell_cell contact to be ON, relative to radius */
	   double contact_cc_threshold;	
		
	   /** \brief Threshold of cell_ECM contact to be ON, relative to radius */
	   double contact_ecm_threshold;	

		/** \ brief Persistence coefficient of motility */
		double persistence;	
	
		/** \brief Default value of polarization of the cell */
		double def_polarized;

	public:

		/// \todo Public attributes for now, to change as protected/private later ?
		/** Choose which function to use: 0 default */
		int mode_motility; 
		int mode_volume;	
		int mode_velocity;	
		
		/// previously in custom data
		//double motility_t_persistence;
		
		/** \brief Strength of adhesion between cells of same types */
		double Ccca_homotypic[2]; 
		/** \brief Strength of adhesion between cells of different types */
		double Ccca_heterotypic[2]; //used in calculating cell-cell adhesion

		/** \brief default constructor */
		CellLine();
		/** \brief Destructor */
		~CellLine() {};

		/** \brief Set the cell line index */
		void setLineIndex( int ind )
		{ line_index = ind; };

		/** \brief Return value of line index */
		int getLineIndex() 
		{ return line_index; };

		/** \brief Read the properties of the cell line from the parameter input file */
		void read_properties( ReadXML* reader, std::string what );

		/** \brief Scale the oxygen parameters by given factor (voxel size) */
		void scaleOxygenLevels( double fac );
		
		/** \brief Return address of the cell line phenotype */
		inline CellPhenotype* get_phenotype()
		{ return (&line_phenotype); };

		/** \brief Return default initial radius */
		inline double get_init_radius()
		{ return def_volumes[0]; };
		
		/** \brief Return default volume value */
		inline double get_default_volume(int which)
		{ return def_volumes[which]; };
		
		/** \brief Return address of cell phase fiven its code */
		inline Phase* get_phase( int phase_code )
		{ return line_phenotype.get_phase(phase_code); };

		/** \brief Return duration from oxygen level */
		double proliferation_O2_based( double oxygen );
		
		/** \brief Return current necrosis rate of the cell depending of O2 */
		double necrosis_rate_O2( double oxygen );
		
		/** \brief Output current phenotype phase values to file */
		void output( std::string& delimeter, std::ofstream* file );

		/** \brief Set the migration strength value */
		inline void set_migration_magnitude( double migr )
		{ motility_magnitude[0] = migr; };	
	
		/** \brief Get the current value of motility coefficient */
		inline double get_motility_amplitude( double percent )
		{ return current_value( motility_magnitude[0], motility_magnitude[1], percent ); };

		/** \brief Get the current value of homotypic adhesion strength */
		inline double get_homotypic_strength( double percent )
		{ return current_value( Ccca_homotypic[0], Ccca_homotypic[1], percent ); };
		
		/** \brief Get the current value of heterotypic adhesion strength */
		inline double get_heterotypic_strength( double percent )
		{ return current_value( Ccca_heterotypic[0], Ccca_heterotypic[1], percent ); };
		
		/** \brief Get the current value of integrin strength */
		inline double get_integrin_strength( double percent )
		{ return current_value( Cecm[0], Cecm[1], percent ); };
	
		/** \brief Get the current value of ECM degradation coefficient */
		inline double get_degradation( double percent )
		{ return (mmp * percent); };

		/** \brief Get the value of persistence coefficient */
		inline double get_persistence()
		{ return persistence; };
		
		/** \brief Return the protein threshold defined for the given field
		*
		 * If no value has been set for this field, use default value */
		double get_threshold( std::string field );
	
		/** \brief Read the value of threshold for given field 
		 *
		* Use the default value if not in the parameter file */
		void read_threshold( ReadXML* reader, std::string what, std::string field );

		/** \brief Write properties of the cell line */
		void write_properties( std::ostream& os );

		void display_information( std::ostream& os ); 
		void display_information_xml( std::ostream& os ); 
};

#endif
