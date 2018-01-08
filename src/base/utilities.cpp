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

#include "utilities.h"
#include "../BioFVM/BioFVM.h"
#include "../core/PhysiCell_constants.h"


std::random_device rd;
std::mt19937 gen(rd());

long SeedRandom( long input )
{
	gen.seed(input);
	return input;
}


long SeedRandom( void )
{ 
	unsigned seed = std::time(0);
	seed *= std::random_device()();
	gen.seed(seed);
	return seed;
}

int UniformInt()
{
	std::uniform_int_distribution<int> int_dis;
	return int_dis(gen);
}

double UniformRandom()
{
	return std::generate_canonical<double, 10>(gen);
}

double UniformRandom11()
{
	double res = UniformRandom();
	return ( 2.0 * ( res - 0.5 ) ); 
}

double NormRandom(double mu, double sigma)
{
	double U1, U2, W, mult;
	static double X1, X2;
	static int call = 0;

	if (call == 1)
	{
		call = !call;
		return (mu + sigma * (double) X2);
	}

	do
	{
		U1 = UniformRandom11();
		U2 = UniformRandom11();
		W = pow(U1, 2) + pow(U2, 2);
	}
	while (W >= 1 || W == 0);

	mult = sqrt((-2 * log (W)) / W);
	X1 = U1 * mult;
	X2 = U2 * mult;

	call = !call;

	return (mu + sigma * (double) X1);
}

std::string formatted_minutes_to_DDHHMM( double minutes )
{
	static std::string output; 
	output.resize( 1024 ); 

	int nMinutes = round( minutes ); 
	int nDays = minutes / 1440; 
	nMinutes -= nDays*1440; 

	int nHours = nMinutes / 60;
	double dMinutes = minutes - 60*( nDays*24 + nHours ); 
	if( dMinutes < 0 )
	{ dMinutes = 0.0; }
	sprintf( (char*) output.c_str(),"%d days, %d hours, and %2.2f minutes", nDays,nHours,dMinutes);

	return output ;
}

/* Change the current value of the input coefficient, increase or decrease according to up value */
void evolve_coef( int up, double* coef, double dt )
/**{ 
	// increase exponentially
	if ( up )
	{
		if ( (*coef) < EPSILON ) 
			(*coef) = EPSILON; 	
		(*coef) = std::sqrt( (*coef) );
		(*coef) = (*coef) > 1 ? (1-EPSILON) : (*coef);
	}
	else
	{
		// decrease exponentially
		if ( (*coef) >= 1 )
			(*coef) = 1 - EPSILON;
		(*coef) *= (*coef);	
		(*coef) = (*coef) < 0 ? EPSILON : (*coef);
	}
}*/
{ 
	// if up, increase, else decrease
	if ( !up )
		dt = -dt;

	(*coef) +=  (*coef) * (1 - (*coef)) * dt/10.0 ;

	(*coef) = (*coef) > 1 ? (1-EPSILON) : (*coef);
	(*coef) = (*coef) < 0 ? (EPSILON) : (*coef);
}
