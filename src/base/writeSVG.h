/*
#############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the ver-  #
# sion number, such as below:                                               #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.1.0) [1].  #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, PloS Comput. Biol. 2017 (in revision).         #
#     preprint DOI: 10.1101/088773                                          #
#                                                                           #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite       #
#     BioFVM as below:                                                      #
#                                                                           #
# We implemented and solved the model using PhysiCell (Version 1.1.0) [1],  #
# with BioFVM [2] to solve the transport equations.                         #
#                                                                           #
# [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for            #
#     Multicellular Systems, PloS Comput. Biol. 2017 (in revision).         #
#     preprint DOI: 10.1101/088773                                          #
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

/**
 * \brief Class WriteSVG: utilities to write a svg file from current cell states or .txt output file
 *
 * Either read output .txt file and plot it as svg format
 * Or directly plot as svg format current state during the simulation
 *
 * \todo add options to customized more the plotting
 *
 * \author Gaelle Letort, Institut Curie, adapted from PhysiCell source code
 * \date June 2017
 * */

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>

#ifndef _PhysiCell_SVG_h_
#define _PhysiCell_SVG_h_

class WriteSVG
{
	private:
		/** \brief Current output stream (file..) */
		std::ostream* os;  

	public:
		/** \brief Default constructor */
		WriteSVG()
		{ os = NULL; };
		/** \brief Destructor */
		~WriteSVG()
		{ os = NULL; };

		inline void set_stream( std::ostream* inos )
		{ os = inos; }

		/** \brief Create the file with given dimensions */
		void start_file( double width, double height );
		/** \brief Write end of file part of svg file */
		void end_file();

		/** \brief Add given text at given location in the file */
		void add_text( const char* str , double position_x, double position_y, const char* text_font, const char* text_color, double text_size );

		/** \brief Draw a circle */
		void add_circle( double center_x, double center_y, double radius, double stroke_size, std::string stroke_color , std::string fill_color, double opacity );

		/** \brief Draw a rectangle with given properties 
		 *
		 * @param UL_corner_x Upper left corner horizontal position
		 * @param UL_corner_y Upper left corner vertical position 
		 * @param width width of the rectangle to draw (from UL_corner_x to UL_corner_x + width then)
		 * @param height height of the rectangle to draw (from UL_corner_y to UL_corner_y + height then)
		 * @param stroke size thickness of the drawing of the rectangle outlines 
		 * @param stroke_color color of the rectangle outline
		 * @param fill_color color of the inside of the rectangle */
		void rect( double UL_corner_x, double UL_corner_y, double width, double height, double stroke_size, std::string stroke_color , std::string fill_color );

		/** \brief Draw a line with given properties 
		 *
		 * @param start_x horizontal position of first point 
		 * @param start_y vertical position of first point 
		 * @param end_x horizontal position of end point
		 * @param end_y vertical position of end point
		 * @param thickness width of the drawing of the line
		 * @param stroke_color name of the color of the line 
		 * */
		void add_line( double start_x, double start_y, double end_x , double end_y, double thickness, std::string stroke_color );  

};
#endif
