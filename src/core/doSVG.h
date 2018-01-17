/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.1.0) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2017 (in revision).                            #
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.1.0) [1],  #
# with BioFVM [2] to solve the transport equations.                         #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, 2017 (in revision).                            #
#                                                                           #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient     #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2017, Paul Macklin and the PhysiCell Project           #
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

#include <vector>
#include <string>

#ifndef __Do_SVG__
#define __Do_SVG__

#include "../base/writeSVG.h"
#include "../base/utilities.h"
#include "../core/cell_container.h"
#include "../BioFVM/BioFVM_utilities.h"

class DoSVG
{
	private:

		// SVG options 
		
		bool plot_nuclei; 
		std::string simulation_time_units;
		std::string mu;
		std::string simulation_space_units;
	
		std::string label_time_units; 

		// Text options

		double font_size; 
		std::string font_color;
		std::string font;

		double bar_width; 
		double plot_height;
		double plot_width;
		double top_margin;
		double bar_margin;

	public:
		
		DoSVG();
		~DoSVG(){};

// done 
std::vector<double> transmission( std::vector<double>& incoming_light, std::vector<double>& absorb_color, double thickness , double stain );

/**std::vector<std::string> false_cell_coloring_Ki67( Cell* pCell ); // done 
std::vector<std::string> false_cell_coloring_live_dead( Cell* pCell ); // done 

std::vector<std::string> hematoxylin_and_eosin_cell_coloring( Cell* pCell ); // done 
std::vector<std::string> hematoxylin_and_eosin_stroma_coloring( double& ECM_fraction , double& blood_vessel_fraction); // planned 
*/

/** \brief Print the current time to the top of the plot */
void print_time( WriteSVG* svg, Microenvironment* M, double time, int total_cell_count, int zslice );
/** \brief Add a scale bar to the plot */
void draw_scale_bar( WriteSVG* svg );
/** \brief Add runtime information to the plot */
void print_run_time( WriteSVG* svg );

/** \brief Do the plot */
void SVG_plot( std::string filename, Microenvironment* M, Cell_Container* container, double z_slice, double time, int mode_cell_color );

};

#endif
