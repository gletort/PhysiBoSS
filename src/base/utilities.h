
/**
 * \brief Common fonctions like random generators, convert radius from volume and vice-versa... used in the code
 *
 * \author Gaëlle Letort, Institut Curie
 * \date March 2017
 * */

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

#ifndef __PhysiCell_utilities_h__
#define __PhysiCell_utilities_h__

#include <iostream>
#include <ctime>
#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <limits>
#include <algorithm>

long SeedRandom( long input );
long SeedRandom( void );
double UniformRandom( void );
double UniformRandom11( void );
int UniformInt();

/** \brief Generate random number following normal distribution 
 *
 * @param mu mean of normal distribution
 * @param sigma standard deviation of the normal distribution
 */
double NormRandom(double mu, double sigma);

std::string formatted_minutes_to_DDHHMM( double minutes );

static const std::string delim = "\t";

static const double EPSILON = std::numeric_limits<double>::epsilon();
inline double sign( double x ) { return (x > 0) ? 1 : -1; };
static const double PI4 = 4.0 * M_PI;
static const double PI4_3 = 4.0 / 3.0 * M_PI;


/** \brief Calculate volume from radius 
 *
 * Return 4/3*pi*rad^3*/
inline double get_volume( double rad ) { return PI4_3 * rad * rad * rad; }; 
	
/** \brief Calculate surface from radius */
inline double get_surface( double rad ) { return PI4 * rad * rad; }; 

/** \brief Calculate radius from volume */
inline double get_radius( double vol ) { return pow( 1.0/PI4_3 * vol, 1.0/3.0 ); };

/** \brief Relative difference between two numbers */
inline double relative_diff( double a, double b )
{ if ( b < EPSILON ) return 0; return ( fabs(a-b)/b ); };

inline void strip( std::string* str ) 
{ (*str).erase(remove(str->begin(), (*str).end(), ' '), (*str).end()); };

inline double current_value( double min, double max, double percent )
{ return (min + (max-min) * percent); };

/** \brief Change the current value of the input coefficient, increase or decrease according to up value 
 *
 * @param up if should increase current value
 * @param coef pointer on coefficient to change */
void evolve_coef( int up, double* coef, double dt );

#endif
