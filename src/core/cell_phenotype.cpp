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

#include "cell_phenotype.h"
#include "PhysiCell_constants.h"

#include <map>


CellPhenotype::CellPhenotype()
{
	phases.resize(0);
}

/* Read properties of phenotype from parameter file */
void CellPhenotype::read_properties( ReadXML* reader, std::string what, std::string phenname )
{
}

/* Set birth rate and duration */
void CellPhenotype::set_phase_rate( int phase, double birth_rate, double add )
{
	phases[phase].birth_rate = birth_rate;
	phases[phase].duration = 1.0 / (phases[phase].birth_rate + add );
}

/* Set death rate and type */
void CellPhenotype::set_death( int phase, double rate, int type )
{
	phases[phase].death_rate = rate;
	phases[phase].death_type = type;
}

/* create default phenotype: normoxic (21% O2), MCF7 cell line */
void CellPhenotype::set_MCF7()
{
	phases.resize(7);
	
	phases[0].code = PhysiCell_constants::Ki67_positive_premitotic; 
	phases[0].name = "Ki67_positive_premitotic"; 
	phases[0].duration = 13 * 60.0; // minutes
	phases[0].birth_rate = 1.0 / phases[0].duration; 
	phases[0].death_rate = 0.00105 / 60.0; // minutes 
	phases[0].death_type = PhysiCell_constants::apoptotic; 
	phases[0].arrest_density = pow( 0.006012 , 1.5 ); // cells per cubic micron  
	phases[0].volume_change_timescale_N = 9.1*60.0; 
	phases[0].volume_change_timescale_C = 11.1*60.0; 
	phases[0].volume_change_timescale_F = 1*60.0;
	phases[0].color = "rgb(0,200,100)";	
	phase_map[(int)PhysiCell_constants::Ki67_positive_premitotic] = 0;
	
	phases[1].code = PhysiCell_constants::Ki67_positive_postmitotic; 
	phases[1].name = "Ki67_positive_postmitotic"; 
	phases[1].duration = 2.5 * 60.0;
	phases[1].birth_rate = 0; 
	phases[1].death_rate = 0.00105 / 60; // arbitrary 
	phases[1].arrest_density = 9e99; // cells per cubic micron  
	phases[1].volume_change_timescale_N = 9.1*60.0; 
	phases[1].volume_change_timescale_C = 11.1*60.0; 
	phases[1].volume_change_timescale_F = 1*60.0; 	
	phases[1].color = "rgb(0,100,50)";	
	phase_map[(int)PhysiCell_constants::Ki67_positive_postmitotic] = 1;

	phases[2].code = PhysiCell_constants::Ki67_negative; 
	phases[2].name = "Ki67_negative"; 
	phases[2].duration = 1 * 60.0; //74.35 * 60.0;
	phases[2].birth_rate = 0; 
	phases[2].death_rate = 0.00105 / 60; // arbitrary 
	phases[2].arrest_density = 9e99; // cells per cubic micron  
	phases[2].volume_change_timescale_N = 9.1*60.0; 
	phases[2].volume_change_timescale_C = 11.1*60.0; 
	phases[2].volume_change_timescale_F = 1*60.0; 	
	phases[2].color = "rgb(0,100,200)";	
	phase_map[(int)PhysiCell_constants::Ki67_negative] = 2;
	
	phases[3].code = PhysiCell_constants::apoptotic; 
	phases[3].name = "apoptotic"; 
	phases[3].duration = 8.6 * 60.0;
	phases[3].birth_rate = 0; 
	phases[3].death_rate = 0; 
	phases[3].arrest_density = 9e99; // cells per cubic micron  
	phases[3].volume_change_timescale_N = 8.6*60.0; 
	phases[3].volume_change_timescale_C = 3*60.0; 
	phases[3].volume_change_timescale_F = 1*60.0; 	
	phases[3].color = "rgb(150,0,0)";	
	phase_map[(int)PhysiCell_constants::apoptotic] = 3;

	phases[4].code = PhysiCell_constants::necrotic_swelling; 
	phases[4].name = "necrotic_swelling"; 
	phases[4].duration = 3.0 * 60.0;
	phases[4].volume_change_timescale_N = 230*60.0; 
	phases[4].volume_change_timescale_C = 936.2*60.0; 
	phases[4].volume_change_timescale_F = 4.47*60.0; 
	phases[4].calcification_rate = 0.0042/60.0;	
	phases[4].color = "rgb(50,50,50)";	
	phase_map[(int)PhysiCell_constants::necrotic_swelling] = 4;

	phases[5].code = PhysiCell_constants::necrotic_lysed; 
	phases[5].name = "necrotic_lysed"; 
	phases[5].duration = 45.0 * 24 * 60.0;
	phases[5].volume_change_timescale_N = 230*60.0; 
	phases[5].volume_change_timescale_C = 936.2*60.0; 
	phases[5].volume_change_timescale_F = 59*60.0; 
	phases[5].calcification_rate = 0.0042/60.0;	
	phases[5].color = "rgb(100,100,100)";	
	phase_map[(int)PhysiCell_constants::necrotic_lysed] = 5;
	
	phases[6].code = PhysiCell_constants::live; 
	phases[6].name = "live"; 
	phases[6].duration = 17.1 * 60.0;
	phases[6].birth_rate = 1.0/phases[6].duration; 
	phases[6].death_rate = 0.00105/60.0; 
	phases[6].arrest_density = pow( 0.006012, 1.5); // cells per cubic micron  
	phases[6].volume_change_timescale_N = 9.1*60.0; 
	phases[6].volume_change_timescale_C = 11.1*60.0; 
	phases[6].volume_change_timescale_F = 1*60.0; 	
	phases[6].color = "rgb(0,0,0)";	
	phase_map[(int)PhysiCell_constants::live] = 6;
	
}

/* create default phenotype: normoxic (21% O2), MCF7 cell line, division around 24h */
void CellPhenotype::set_MCF7_24()
{
	phases.resize(7);
	
	phases[0].code = PhysiCell_constants::Ki67_positive_premitotic; 
	phases[0].name = "Ki67_positive_premitotic"; 
	phases[0].duration = 13 * 60.0; // minutes
	phases[0].birth_rate = 1.0 / phases[0].duration; 
	phases[0].death_rate = 0.00105 / 60.0; // minutes 
	phases[0].death_type = PhysiCell_constants::apoptotic; 
	phases[0].arrest_density = pow( 0.006012 , 1.5 ); // cells per cubic micron  
	phases[0].volume_change_timescale_N = 18*60.0; // double all
	phases[0].volume_change_timescale_C = 22*60.0; // around 24h
	phases[0].volume_change_timescale_F = 2*60.0;
	phases[0].color = "rgb(0,200,100)";	
	phase_map[(int)PhysiCell_constants::Ki67_positive_premitotic] = 0;
	
	phases[1].code = PhysiCell_constants::Ki67_positive_postmitotic; 
	phases[1].name = "Ki67_positive_postmitotic"; 
	phases[1].duration = 2.5 * 60.0;
	phases[1].birth_rate = 0; 
	phases[1].death_rate = 0.00105 / 60; // arbitrary 
	phases[1].arrest_density = 9e99; // cells per cubic micron  
	phases[1].volume_change_timescale_N = 9.1*60.0; 
	phases[1].volume_change_timescale_C = 11.1*60.0; 
	phases[1].volume_change_timescale_F = 1*60.0; 	
	phases[1].color = "rgb(0,100,50)";	
	phase_map[(int)PhysiCell_constants::Ki67_positive_postmitotic] = 1;

	phases[2].code = PhysiCell_constants::Ki67_negative; 
	phases[2].name = "Ki67_negative"; 
	phases[2].duration = 1 * 60.0; //74.35 * 60.0;
	phases[2].birth_rate = 0; 
	phases[2].death_rate = 0.00105 / 60; // arbitrary 
	phases[2].arrest_density = 9e99; // cells per cubic micron  
	phases[2].volume_change_timescale_N = 9.1*60.0; 
	phases[2].volume_change_timescale_C = 11.1*60.0; 
	phases[2].volume_change_timescale_F = 1*60.0; 	
	phases[2].color = "rgb(0,100,200)";	
	phase_map[(int)PhysiCell_constants::Ki67_negative] = 2;
	
	phases[3].code = PhysiCell_constants::apoptotic; 
	phases[3].name = "apoptotic"; 
	phases[3].duration = 8.6 * 60.0;
	phases[3].birth_rate = 0; 
	phases[3].death_rate = 0; 
	phases[3].arrest_density = 9e99; // cells per cubic micron  
	phases[3].volume_change_timescale_N = 8.6*60.0; 
	phases[3].volume_change_timescale_C = 3*60.0; 
	phases[3].volume_change_timescale_F = 1*60.0; 	
	phases[3].color = "rgb(150,0,0)";	
	phase_map[(int)PhysiCell_constants::apoptotic] = 3;

	phases[4].code = PhysiCell_constants::necrotic_swelling; 
	phases[4].name = "necrotic_swelling"; 
	phases[4].duration = 3.0 * 60.0;
	phases[4].volume_change_timescale_N = 230*60.0; 
	phases[4].volume_change_timescale_C = 936.2*60.0; 
	phases[4].volume_change_timescale_F = 4.47*60.0; 
	phases[4].calcification_rate = 0.0042/60.0;	
	phases[4].color = "rgb(50,50,50)";	
	phase_map[(int)PhysiCell_constants::necrotic_swelling] = 4;

	phases[5].code = PhysiCell_constants::necrotic_lysed; 
	phases[5].name = "necrotic_lysed"; 
	phases[5].duration = 45.0 * 24 * 60.0;
	phases[5].volume_change_timescale_N = 230*60.0; 
	phases[5].volume_change_timescale_C = 936.2*60.0; 
	phases[5].volume_change_timescale_F = 59*60.0; 
	phases[5].calcification_rate = 0.0042/60.0;	
	phases[5].color = "rgb(100,100,100)";	
	phase_map[(int)PhysiCell_constants::necrotic_lysed] = 5;
	
	phases[6].code = PhysiCell_constants::live; 
	phases[6].name = "live"; 
	phases[6].duration = 17.1 * 60.0;
	phases[6].birth_rate = 1.0/phases[6].duration; 
	phases[6].death_rate = 0.00105/60.0; 
	phases[6].arrest_density = pow( 0.006012, 1.5); // cells per cubic micron  
	phases[6].volume_change_timescale_N = 9.1*60.0; 
	phases[6].volume_change_timescale_C = 11.1*60.0; 
	phases[6].volume_change_timescale_F = 1*60.0; 	
	phases[6].color = "rgb(0,0,0)";	
	phase_map[(int)PhysiCell_constants::live] = 6;
	
}

/* create default phenotype: normoxic (21% O2), MCF7 cell line with very short dying phases */
void CellPhenotype::set_MCF7_fastdeath()
{
	phases.resize(7);
	
	phases[0].code = PhysiCell_constants::Ki67_positive_premitotic; 
	phases[0].name = "Ki67_positive_premitotic"; 
	phases[0].duration = 13 * 60.0; // minutes
	phases[0].birth_rate = 1.0 / phases[0].duration; 
	phases[0].death_rate = 0.00105 / 60.0; // minutes 
	phases[0].death_type = PhysiCell_constants::apoptotic; 
	phases[0].arrest_density = pow( 0.006012 , 1.5 ); // cells per cubic micron  
	phases[0].volume_change_timescale_N = 9.1*60.0; 
	phases[0].volume_change_timescale_C = 11.1*60.0; 
	phases[0].volume_change_timescale_F = 1*60.0;
	phases[0].color = "rgb(0,200,100)";	
	phase_map[(int)PhysiCell_constants::Ki67_positive_premitotic] = 0;
	
	phases[1].code = PhysiCell_constants::Ki67_positive_postmitotic; 
	phases[1].name = "Ki67_positive_postmitotic"; 
	phases[1].duration = 2.5 * 60.0;
	phases[1].birth_rate = 0; 
	phases[1].death_rate = 0.00105 / 60; // arbitrary 
	phases[1].arrest_density = 9e99; // cells per cubic micron  
	phases[1].volume_change_timescale_N = 9.1*60.0; 
	phases[1].volume_change_timescale_C = 11.1*60.0; 
	phases[1].volume_change_timescale_F = 1*60.0; 	
	phases[1].color = "rgb(0,100,50)";	
	phase_map[(int)PhysiCell_constants::Ki67_positive_postmitotic] = 1;

	phases[2].code = PhysiCell_constants::Ki67_negative; 
	phases[2].name = "Ki67_negative"; 
	phases[2].duration = 3 * 60.0; //74.35 * 60.0;
	phases[2].birth_rate = 0; 
	phases[2].death_rate = 0.00105 / 60; // arbitrary 
	phases[2].arrest_density = 9e99; // cells per cubic micron  
	phases[2].volume_change_timescale_N = 9.1*60.0; 
	phases[2].volume_change_timescale_C = 11.1*60.0; 
	phases[2].volume_change_timescale_F = 1*60.0; 	
	phases[2].color = "rgb(0,100,200)";	
	phase_map[(int)PhysiCell_constants::Ki67_negative] = 2;
	
	phases[3].code = PhysiCell_constants::apoptotic; 
	phases[3].name = "apoptotic"; 
	phases[3].duration = 1 * 60.0;
	phases[3].birth_rate = 0; 
	phases[3].death_rate = 0; 
	phases[3].arrest_density = 9e99; // cells per cubic micron  
	phases[3].volume_change_timescale_N = 1*60.0; 
	phases[3].volume_change_timescale_C = 0.3*60.0; 
	phases[3].volume_change_timescale_F = 0.1*60.0; 	
	phases[3].color = "rgb(150,0,0)";	
	phase_map[(int)PhysiCell_constants::apoptotic] = 3;

	phases[4].code = PhysiCell_constants::necrotic_swelling; 
	phases[4].name = "necrotic_swelling"; 
	phases[4].duration = 0.2 * 60.0;
	phases[4].volume_change_timescale_N = 0.4*60.0; 
	phases[4].volume_change_timescale_C = 0.8*60.0; 
	phases[4].volume_change_timescale_F = 0.1*60.0; 
	phases[4].calcification_rate = 0.0042/60.0;	
	phases[4].color = "rgb(50,50,50)";	
	phase_map[(int)PhysiCell_constants::necrotic_swelling] = 4;

	phases[5].code = PhysiCell_constants::necrotic_lysed; 
	phases[5].name = "necrotic_lysed"; 
	phases[5].duration = 0.8 * 60.0;
	phases[5].volume_change_timescale_N = 0.4*60.0; 
	phases[5].volume_change_timescale_C = 0.8*60.0; 
	phases[5].volume_change_timescale_F = 0.2*60.0; 
	phases[5].calcification_rate = 0.0042/60.0;	
	phases[5].color = "rgb(100,100,100)";	
	phase_map[(int)PhysiCell_constants::necrotic_lysed] = 5;
	
	phases[6].code = PhysiCell_constants::live; 
	phases[6].name = "live"; 
	phases[6].duration = 17.1 * 60.0;
	phases[6].birth_rate = 1.0/phases[6].duration; 
	phases[6].death_rate = 0.00105/60.0; 
	phases[6].arrest_density = pow( 0.006012, 1.5); // cells per cubic micron  
	phases[6].volume_change_timescale_N = 9.1*60.0; 
	phases[6].volume_change_timescale_C = 11.1*60.0; 
	phases[6].volume_change_timescale_F = 1*60.0; 	
	phases[6].color = "rgb(0,0,0)";	
	phase_map[(int)PhysiCell_constants::live] = 6;
	
}

/* Phenotype of cells restricted to quiescent phase only */
void CellPhenotype::set_G0_cells()
{
	phases.resize(1);
	
	phases[0].code = PhysiCell_constants::Ki67_negative; 
	phases[0].name = "Ki67_negative"; 
	phases[0].duration = 130000 * 60.0; // minutes
	phases[0].birth_rate = 0.0 / phases[0].duration; 
	phases[0].death_rate = 0.0 / 60.0; // minutes 
	phases[0].death_type = PhysiCell_constants::apoptotic; 
	phases[0].arrest_density = pow( 0.006012 , 1.5 ); // cells per cubic micron  
	phases[0].volume_change_timescale_N = 999.1*60.0; // not growing
	phases[0].volume_change_timescale_C = 111.1*60.0; 
	phases[0].volume_change_timescale_F = 111*60.0;
	phases[0].color = "rgb(0,200,100)";	
	phase_map[(int)PhysiCell_constants::Ki67_negative] = 0;
}

/* create default phenotype: for fibroblast 3T3 cell line */
void CellPhenotype::set_3T3()
{
	phases.resize(7);
	
	phases[0].code = PhysiCell_constants::Ki67_positive_premitotic; 
	phases[0].name = "Ki67_positive_premitotic";    // G1 + S 
	phases[0].duration = 14 * 60.0; // minutes
	phases[0].birth_rate = 0.0 / phases[0].duration; 
	phases[0].death_rate = 0.00007;       // minutes, =10% in 24h 
	phases[0].death_type = PhysiCell_constants::apoptotic; 
	phases[0].arrest_density = pow( 0.006012 , 1.5 ); // cells per cubic micron  
	phases[0].volume_change_timescale_N = 10*60.0; 
	phases[0].volume_change_timescale_C = 14*60.0;  // time to reach 95% of new volume
	phases[0].volume_change_timescale_F = 1.1*60.0;
	phases[0].color = "rgb(0,200,100)";	
	phase_map[(int)PhysiCell_constants::Ki67_positive_premitotic] = 0;

	// Duration counts here	
	phases[1].code = PhysiCell_constants::Ki67_positive_postmitotic; 
	phases[1].name = "Ki67_positive_postmitotic"; 
	phases[1].duration = 2.7 * 60.0;  // after M = M
	phases[1].birth_rate = 0; 
	phases[1].death_rate = 0.00007; // arbitrary 
	phases[1].arrest_density = 9e99; // cells per cubic micron  
	phases[1].volume_change_timescale_N = 9.1*60.0; 
	phases[1].volume_change_timescale_C = 11.1*60.0; 
	phases[1].volume_change_timescale_F = 1*60.0; 	
	phases[1].color = "rgb(0,100,50)";	
	phase_map[(int)PhysiCell_constants::Ki67_positive_postmitotic] = 1;

	phases[2].code = PhysiCell_constants::Ki67_negative; 
	phases[2].name = "Ki67_negative";   // G0 
	phases[2].duration = 0 * 60.0; // recycle nearly immediatly
	phases[2].birth_rate = 0; 
	phases[2].death_rate = 0.00007;       // minutes 
	phases[2].arrest_density = 9e99; // cells per cubic micron  
	phases[2].volume_change_timescale_N = 9.1*60.0; 
	phases[2].volume_change_timescale_C = 11.1*60.0; 
	phases[2].volume_change_timescale_F = 1*60.0; 	
	phases[2].color = "rgb(0,100,200)";	
	phase_map[(int)PhysiCell_constants::Ki67_negative] = 2;
	
	phases[3].code = PhysiCell_constants::apoptotic; 
	phases[3].name = "apoptotic"; 
	phases[3].duration = 8.6 * 60.0;
	phases[3].birth_rate = 0; 
	phases[3].death_rate = 0; 
	phases[3].arrest_density = 9e99; // cells per cubic micron  
	phases[3].volume_change_timescale_N = 8.6*60.0;   // from PhysiCell publi
	phases[3].volume_change_timescale_C = 3*60.0; 
	phases[3].volume_change_timescale_F = 1*60.0; 	
	phases[3].color = "rgb(150,0,0)";	
	phase_map[(int)PhysiCell_constants::apoptotic] = 3;

	phases[4].code = PhysiCell_constants::necrotic_swelling; 
	phases[4].name = "necrotic_swelling"; 
	phases[4].duration = 3.0 * 60.0;
	phases[4].volume_change_timescale_N = 230*60.0; 
	phases[4].volume_change_timescale_C = 936.2*60.0; 
	phases[4].volume_change_timescale_F = 4.47*60.0; 
	phases[4].calcification_rate = 0.0042/60.0;	
	phases[4].color = "rgb(50,50,50)";	
	phase_map[(int)PhysiCell_constants::necrotic_swelling] = 4;

	phases[5].code = PhysiCell_constants::necrotic_lysed; 
	phases[5].name = "necrotic_lysed"; 
	phases[5].duration = 45.0 * 24 * 60.0;
	phases[5].volume_change_timescale_N = 230*60.0; 
	phases[5].volume_change_timescale_C = 936.2*60.0; 
	phases[5].volume_change_timescale_F = 59*60.0; 
	phases[5].calcification_rate = 0.0042/60.0;	
	phases[5].color = "rgb(100,100,100)";	
	phase_map[(int)PhysiCell_constants::necrotic_lysed] = 5;
	
	phases[6].code = PhysiCell_constants::live; 
	phases[6].name = "live"; 
	phases[6].duration = 17.1 * 60.0;
	phases[6].birth_rate = 1.0/phases[6].duration; 
	phases[6].death_rate = 0.00105/60.0; 
	phases[6].arrest_density = pow( 0.006012, 1.5); // cells per cubic micron  
	phases[6].volume_change_timescale_N = 9.1*60.0; 
	phases[6].volume_change_timescale_C = 11.1*60.0; 
	phases[6].volume_change_timescale_F = 1*60.0; 	
	phases[6].color = "rgb(0,0,0)";	
	phase_map[(int)PhysiCell_constants::live] = 6;
	
}

/** Write current phenotype values to file */
void CellPhenotype::output( int phase, std::string& delimeter, std::ofstream* file )
{
	int code = phases.size() > 0 ? phases[phase].code : -1;
	(*file) << code << delimeter;		
}	

/* Write properties of the given phase number */
void CellPhenotype::write_phase_properties( int i, std::ostream& os )
{
	os << "\t\t<phase_" << i << "> " << std::endl;
	os << "\t\t\t <code> " << phases[i].code << " </code>" << std::endl; 
	os << "\t\t\t <name> " << phases[i].name << " </name>" << std::endl; 
	os << "\t\t\t <duration> " << phases[i].duration << " </duration>" << std::endl; 
	os << "\t\t\t <timescale_nucleus> " << phases[i].volume_change_timescale_N << " </timescale_nucleus>" << std::endl; 
	os << "\t\t\t <timescale_cytoplasm> " << phases[i].volume_change_timescale_C << " </timescale_cytoplasm>" << std::endl; 
	os << "\t\t\t <timescale_fluid> " << phases[i].volume_change_timescale_F << " </timescale_fluid>" << std::endl; 
	os << "\t\t</phase_" << i << "> " << std::endl;
}

/* Write properties of the cell phenotype */
void CellPhenotype::write_properties(std::ostream& os)
{
	os << "\t<cell_phenotype_" << 0 << ">" << std::endl;
	for ( int i = 0; i < (int) phases.size(); i++ )
		write_phase_properties( i, os );
	os << "\t</cell_phenotype_" << 0 << ">" << std::endl;
	os << std::endl;
}
