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

#ifndef __cell_phenotype_h__
#define __cell_phenotype_h__

#include <vector>
#include <fstream>
#include <map>

#include "../base/utilities.h"
#include "../base/readXML.h"

/** 
 * \struct Phase
 * \brief Cell cycle phase (duration, rates...)
 *
 * Contains information of its main characteristics: name, phase duration, rates...*/
struct Phase
{
		int code; 
		std::string name; 

		double duration; 	
		double birth_rate; 
		double death_rate; 
		double necrosis_rate=0;
		double calcification_rate; 
		int death_type; 
		double arrest_density; 
		double volume_change_timescale_N;
		double volume_change_timescale_C;
		double volume_change_timescale_F;

		/** \brief Color of the phase for SVG drawing */
		std::string color;	
};

/**
 * \class CellPhenotype 
 * \brief Cell phenotype: set of cycle phases 
 * 
 * Contains a vector of phases, and methods to go from one phase to another
 * */
class CellPhenotype
{
	friend class CellCycle;

	private: 
		/** \brief Map of the phases to access index from its PhysiCell code */
		std::map<int, int> phase_map;

	protected:
		/** \brief All possible phases of the phenotype */
		std::vector<Phase> phases;

	public:

		/** \brief Default constructor */
		CellPhenotype();
		/** \brief Destructor */
		~CellPhenotype() {};

		/** \brief Read properties of phenotype from parameter file */
		void read_properties( ReadXML* reader, std::string what, std::string phenname );

		/** \brief Return the index of a given phase from its code */
		inline int get_phase_index(int phase_code) { return phase_map[phase_code]; };

		/** \brief Return pointer to the phase given by its code 
		 *
		 * @param phase_code code of the phase to get 
		 * @return pointer on the asked phase 
		 * */	
		inline Phase* get_phase( int phase_code )
		{ return &(phases[ get_phase_index(phase_code) ]); };

		/** \brief Initialize birth rate and duration */
		void set_phase_rate( int phase, double birth_rate, double add = 0 );

		/** \brief Set death rate and type 
		 *
		 * @param phase number of the phase to update 
		 * @param rate death rate value to put 
		 * @param type number of the death type to put 
		 * */
		void set_death( int phase, double rate, int type );

		/** \brief Create default phenotype: normoxic (21% O2), MCF7 cell line */
		void set_MCF7();
		
		/** \brief Create default phenotype: normoxic (21% O2), MCF7 cell line, with around 24h division time */
		void set_MCF7_24();

		/** \brief Phenotype of cells restricted to quiescent phase only 
		 *
		 * This phenotype has only one phase in which cells doesn't grow nor proliferate */
		void set_G0_cells();

		/** \brief create default phenotype: normoxic (21% O2), MCF7 cell line with very short dying phases */
		void set_MCF7_fastdeath();
		
		/** \brief create default phenotype: for fibroblast 3T3 cell line */
		void set_3T3();

		/** \brief Return the RGB code of given phase */
		inline std::string get_color( int phase )
		{ return phases[phase].color; };

		/** \brief Write current phenotype values to file */
		void output( int phase, std::string& delimeter, std::ofstream* file );

		/** \brief Write properties of the given phase number 
		 *
		 * @param i number of the phase of which to write properties 
		 * @param os write the phase propertie in the given stream 
		 * */
		void write_phase_properties( int i, std::ostream& os );

		/** \brief Write properties of the cell phenotype */
		void write_properties(std::ostream& os);
};


#endif
