/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.0.0) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2016 (in preparation).                         #
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.0.0) [1],  #
# with BioFVM [2] to solve the transport equations.                         #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2016 (in preparation).                         #
#                                                                           #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient     #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2016, Paul Macklin and the PhysiCell Project           #
# All rights reserved.                                                      #
#                                                                           #
# Redistribution and use in source and binary forms, with or without        #
# modification, are permitted provided that the following conditions are    #
# met:                                                                      #
#                                                                           #
# 1. Redistributions of source code must retain the above copyright notice, #
# this list of conditions and the following disclaimer.                     #
#                                                                           #
# 2. Redistributions in binary form must reproduce the above copyright      #
# notice, this list of conditions and the following disclaimer in the       #
# documentation and/or other materials provided with the distribution.      #
#                                                                           #
# 3. Neither the name of the copyright holder nor the names of its          #
# contributors may be used to endorse or promote products derived from this #
# software without specific prior written permission.                       #
#                                                                           #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           #
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER #
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    #
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      #
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              #
#                                                                           #
#############################################################################
*/

#include "cell_line.h"
#include "PhysiCell_constants.h"

#include <map>
#include <climits>

CellLine::CellLine() : line_phenotype()
{
	name = "MCF7";
	line_index = 0;
	
	o2_necrotic = 5; // mmHg
	o2_critical = 2.5; // mmHg
	o2_no_proliferation = 5.7; // 5, mmHg
	o2_hypoxic = 15; // mmHg
	o2_ref = 6.06; // mmHg
	o2_saturation = 38; // mmHg
	
	max_necrosis_rate = 1 / (24*60.0); // 1 day^-1
	init_uptake_rate = 10;
	secretion = 1;	
	
	glucose_necrosis_threshold = 0.1;  // dimensionless 
	max_interaction_distance_factor = 1.4;

	Ccca_homotypic[0] = 0.170577155519015; //used in calculating cell-cell adhesion
	Ccca_homotypic[1] = 0.170577155519015; //used in calculating cell-cell adhesion
	Ccca_heterotypic[0]= 0.170577155519015; //used in calculating cell-cell adhesion
	Ccca_heterotypic[1]= 0.170577155519015; //used in calculating cell-cell adhesion
	Cecm[0] = 0; 
	Cecm[1] = 0;	
	Cccr= 10.0; //used in calculating cell-cell repulsion			
	Ccba= 1.70577155519015; //used in calculating cell-basement membrane adhesion	
	Ccbr= 10.0; //used in calculating cell-basement membrane repulsion	
	Cecmr = Ccbr;

	def_polarized = 0; // default polarity coefficient
	persistence = 0.5; 

	// Modes initialisation: default 0
	mode_motility = 0;
	mode_volume = 0;
	mode_velocity = 0;
	
	motility_magnitude[0] = 5.0;      	// check this value tmp 1
	motility_magnitude[1] = 5.0;      	// check this value tmp 1
	
	contact_cc_threshold = 1;
	contact_ecm_threshold = 1;
	prot_threshold.clear();
	mmp = 0;

	line_index = 0;
	line_phenotype.set_MCF7(); // default phases parameters
}

/* Read the properties of the cell line from the parameter input file */
void CellLine::read_properties( ReadXML* reader, std::string what )
{
	// biophysical prop
   	reader->getDoubleValue( what, "motility_amplitude_min", &motility_magnitude[0] );
   	reader->getDoubleValue( what, "motility_amplitude_max", &motility_magnitude[1] );
   	reader->getDoubleValue( what, "polarity_coefficient", &def_polarized );
   	reader->getDoubleValue( what, "persistence", &persistence );
   	reader->getIntValue( what, "mode_motility", &mode_motility );
	reader->getDoubleValue( what, "homotypic_adhesion_min", &Ccca_homotypic[0] );
	reader->getDoubleValue( what, "homotypic_adhesion_max", &Ccca_homotypic[1] );
	reader->getDoubleValue( what, "heterotypic_adhesion_min", &Ccca_heterotypic[0] );
	reader->getDoubleValue( what, "heterotypic_adhesion_max", &Ccca_heterotypic[1] );
	reader->getDoubleValue( what, "ecm_adhesion_min", &Cecm[0] );
	reader->getDoubleValue( what, "ecm_adhesion_max", &Cecm[1] );
	reader->getDoubleValue( what, "ecm_degradation", &mmp );
	reader->getDoubleValue( what, "cell_ecm_repulsion", &Cecmr );
	reader->getDoubleValue( what, "cell_cell_repulsion", &Cccr );
	reader->getDoubleValue( what, "cell_basement_membrane_repulsion", &Ccbr );
	reader->getDoubleValue( what, "max_interaction_factor", &max_interaction_distance_factor );
	
	// O2 levels
	reader->getDoubleValue( what, "oxygen_necrotic", &o2_necrotic );
	reader->getDoubleValue( what, "oxygen_critical", &o2_critical );
	reader->getDoubleValue( what, "oxygen_no_proliferation", &o2_no_proliferation );
	reader->getDoubleValue( what, "oxygen_hypoxic", &o2_hypoxic );
	reader->getDoubleValue( what, "oxygen_reference", &o2_ref );
	reader->getDoubleValue( what, "oxygen_saturation", &o2_saturation );
	reader->getDoubleValue( what, "initial_uptake_rate", &init_uptake_rate );
	reader->getDoubleValue( what, "secretion_rate", &secretion );

	// threshold for network inputs	
	reader->getDoubleValue( what, "contact_cell_cell_threshold", &contact_cc_threshold );
	reader->getDoubleValue( what, "contact_cell_ECM_threshold", &contact_ecm_threshold );
	double pthreshold = 1;
	reader->getDoubleValue( what, "protein_threshold", &pthreshold );
	prot_threshold["default"] = pthreshold;

	def_volumes.resize(6);
	reader->getDoubleValue( what, "cell_radius", &(def_volumes[0]) );
	reader->getDoubleValue( what, "nucleus_radius", &(def_volumes[1]) );
	reader->getDoubleValue( what, "fluid_fraction", &(def_volumes[2]) );
	reader->getDoubleValue( what, "solid_nuclear", &(def_volumes[3]) );
	reader->getDoubleValue( what, "solid_cytoplasmic", &(def_volumes[4]) );
	reader->getDoubleValue( what, "cytoplasmic_nuclear_fraction", &(def_volumes[5]) );

	// Mode of phase parameters	
	int pheno = 0;   //default MCF7
	reader->getIntValue( what, "phenotype_number", &pheno );
	if ( pheno == 1 )
		line_phenotype.set_MCF7_fastdeath(); // default phases parameters
	if ( pheno == 2 )
		line_phenotype.set_MCF7_24(); 
	if ( pheno == 3 )
		line_phenotype.set_3T3(); 
	if ( pheno == 4 )
		line_phenotype.set_G0_cells(); 
}

/* Read the value of threshold for given field 
 *
 * Use the default value if not in the parameter file */
void CellLine::read_threshold( ReadXML* reader, std::string what, std::string field )
{
	double pth = prot_threshold["default"];
	reader->getDoubleValue( what, field+"_threshold", &pth );
	prot_threshold[field] = pth;
}

/* Return the protein threshold defined for the given field
 *
 * If no value has been set for this field, use default value */
double CellLine::get_threshold( std::string field)
{
	auto it = prot_threshold.find( field );
	if ( it != prot_threshold.end() )
		return it->second;
	return prot_threshold["default"];
}


/* Scale the oxygen parameters by given factor (voxel size) */
void CellLine::scaleOxygenLevels( double fac )
{
	o2_necrotic *= fac;
	o2_critical *= fac;
	o2_no_proliferation *= fac;
	o2_hypoxic *= fac;
	o2_ref *= fac;
	o2_saturation *= fac;
}

/* Return duration from oxygen level */
double CellLine::proliferation_O2_based( double oxygen )
{
	// Update T_Q
	if( oxygen > o2_no_proliferation )
	{
		if( oxygen <= o2_saturation )
			return (o2_ref - o2_no_proliferation) / (oxygen - o2_no_proliferation);
		
		return (o2_ref - o2_no_proliferation) / (o2_saturation - o2_no_proliferation);
	}
	return INT_MAX;
}

/* return necrosis rate according to O2 level */
double CellLine::necrosis_rate_O2( double oxygen )
{
	// Update necrosis rate
	if ( oxygen <= o2_critical )
		return max_necrosis_rate;
	else if ( oxygen < o2_necrotic )
	{
		double factor = (o2_necrotic - oxygen) / (o2_necrotic - o2_critical);
		return max_necrosis_rate * factor;
	}
	else
		return 0;
}


void CellLine::display_information( std::ostream& os )
{
	os << "Cell line = " << name << ", index: " << line_index << std::endl; 
	return;
} 

void CellLine::display_information_xml( std::ostream& os )
{
	os << "<!-- cell line " << name << " -->" << std::endl;
	os << "<!-- index " << line_index << " -->" << std::endl; 
	return;
} 

/* Write properties of the cell line */
void CellLine::write_properties( std::ostream& os )
{
	os << "<!-- ------------------------------------------------------- -->" << std::endl;
	os << "<!-- -------------- Cell line properties ------------------- -->" << std::endl;
	os << "<cell_properties>" << std::endl;
	os << "\t<oxygen_necrotic> " << o2_necrotic << " </oxygen_necrotic>" << std::endl;
	os << "\t<oxygen_critical> " << o2_critical << " </oxygen_critical>" << std::endl;
	os << "\t<oxygen_no_proliferation> " << o2_no_proliferation << " </oxygen_no_proliferation>" << std::endl;
	os << "\t<oxygen_hypoxic> " << o2_hypoxic << " </oxygen_hypoxic>" << std::endl;
	os << "\t<oxygen_reference> " << o2_ref << " </oxygen_reference>" << std::endl;
	os << "\t<oxygen_saturation> " << o2_saturation << " </oxygen_saturation>" << std::endl;
	os << "\t<max_necrosis_rate> " << max_necrosis_rate << " </max_necrosis_rate>" << std::endl;
	os << "\t<glucose_necrosis_threshold> " << glucose_necrosis_threshold << " </glucose_necrosis_threshold>" << std::endl;
	//os << "\t<maximum_stretch> " << maximum_stretch << " </maximum_stretch>" << std::endl;
	os << "\t<max_interaction_factor> " << max_interaction_distance_factor << " </max_interaction_factor>" << std::endl;
	os << "\t<cell_cell_repulsion> " << Cccr << " </cell_cell_repulsion>" << std::endl;
	os << "\t<homotypic_adhesion_min> " << Ccca_homotypic[0] << " </homotypic_adhesion_min>" << std::endl;
	os << "\t<homotypic_adhesion_max> " << Ccca_homotypic[1] << " </homotypic_adhesion_max>" << std::endl;
	os << "\t<heterotypic_adhesion_min> " << Ccca_heterotypic[0] << " </heterotypic_adhesion_min>" << std::endl;
	os << "\t<heterotypic_adhesion_max> " << Ccca_heterotypic[1] << " </heterotypic_adhesion_max>" << std::endl;
	os << "\t<ecm_adhesion_min> " << Cecm[0] << " </ecm_adhesion_min>" << std::endl;
	os << "\t<ecm_adhesion_max> " << Cecm[1] << " </ecm_adhesion_max>" << std::endl;
	os << "\t<ecm_degradation> " << mmp << " </ecm_degradation>" << std::endl;
	os << "\t<cell_ecm_repulsion> " << Cecmr << " </cell_ecm_repulsion>" << std::endl;
	os << "\t<cell_basement_membrane_adhesion> " << Ccba << " </cell_basement_membrane_adhesion>" << std::endl;
	os << "\t<cell_basement_membrane_repulsion> " << Ccbr << " </cell_basement_membrane_repulsion>" << std::endl;
	os << "\t<motility_magnitude_min> " << motility_magnitude[0] << " </motility_magnitude_min>" << std::endl;
	os << "\t<motility_magnitude_max> " << motility_magnitude[1] << " </motility_magnitude_max>" << std::endl;
	os << "\t<mode_motility> " << mode_motility << " </mode_motility>" << std::endl;
	os << "\t<persistence> " << persistence << " </persistence>" << std::endl;
	os << "\t<mode_volume> " << mode_volume << " </mode_volume>" << std::endl;
	os << "\t<contact_cell_cell_threshold> " << contact_cc_threshold << " </contact_cell_cell_threshold>" << std::endl;
	os << "\t<contact_cell_ECM_threshold> " << contact_ecm_threshold << " </contact_cell_ECM_threshold>" << std::endl;
	os << "\t<protein_threshold> " << prot_threshold["default"] << " </protein_threshold>" << std::endl;
	for ( std::map<std::string, double>::iterator fieldy = prot_threshold.begin(); fieldy != prot_threshold.end(); fieldy++ )
	{
		os << "\t<"+(fieldy->first)+"_threshold> " << fieldy->second << " </"+fieldy->first+"_threshold>" << std::endl;
	}
	os << "\t<secretion_rate> " << secretion << " </secretion_rate>" << std::endl;
	os << "\t<initial_uptake_rate> " << init_uptake_rate << " </initial_uptake_rate>" << std::endl;

	// write phenotype properties as subchilds
	line_phenotype.write_properties(os); 
}

/* Write cell infos (line ) into file */
void CellLine::output( std::string& delimeter, std::ofstream* file )
{ 
	(*file) << line_index << delimeter;
}
