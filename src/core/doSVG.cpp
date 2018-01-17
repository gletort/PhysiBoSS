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

#include "./doSVG.h"

DoSVG::DoSVG()
{
		plot_nuclei = true; 
		simulation_time_units = "min";
		mu = "&#956;";
		simulation_space_units = "&#956;m";
	
		label_time_units = "days"; 
	
		font_size = 20; 
		font_color = "black";
		font = "Arial";

		bar_width = 10; 
}

// works for any Ki67-based cell cycle model 
/**std::vector<std::string> DoSVG::false_cell_coloring_Ki67( Cell* pCell )
{
	static std::vector< std::string > output( 4 , "rgb(0,0,0)" );
    
    // output[0] = cyto_color, output[1] = cyto_outline , output[2] = nuclear_color, output[3] = nuclear_outline

	// positive_premitotic - Green
	if (pCell->phenotype.get_current_phase_code() == PhysiCell_constants::Ki67_positive_premitotic || 
		pCell->phenotype.get_current_phase_code() == PhysiCell_constants::Ki67_positive )  
	{
		output[0] = "rgb(0,255,0)";
		output[2] = "rgb(0,125,0)";
	}

    if (pCell->phenotype.get_current_phase_code() == PhysiCell_constants::Ki67_positive_postmitotic )  // postive_postmitotic - Magenta
    {
        output[0] = "rgb(255,0,255)";
        output[2] = "rgb(125,0,125)";
    }

    if (pCell->phenotype.get_current_phase_code() == PhysiCell_constants::Ki67_negative )  // Ki-67 negative/Quiescent - Blue
    {
        output[0] = "rgb(40,200,255)";
        output[2] = "rgb(20,100,255)";
    }

    if (pCell->phenotype.get_current_phase_code() == PhysiCell_constants::apoptotic )  // Apoptotic - Red
    {
        output[0] = "rgb(255,0,0)";
        output[2] = "rgb(125,0,0)";
    }
	
	// Necrotic - Brown
	if( pCell->phenotype.get_current_phase_code() == PhysiCell_constants::necrotic_swelling || 
		pCell->phenotype.get_current_phase_code() == PhysiCell_constants::necrotic_lysed || 
		pCell->phenotype.get_current_phase_code() == PhysiCell_constants::necrotic )
	{
		output[0] = "rgb(250,138,38)";
		output[2] = "rgb(139,69,19)";
    }
    
    return output;
}
*/
/**std::vector<std::string> DoSVG::false_cell_coloring_live_dead( Cell* pCell )
{
	static std::vector< std::string > output( 4 , "rgb(0,0,0)" );
    
  // output[0] = cyto_color, output[1] = cyto_outline , output[2] = nuclear_color, output[3] = nuclear_outline

	// live cell - Green
	if (pCell->phenotype.get_current_phase_code() == PhysiCell_constants::live )  
	{
		output[0] = "rgb(0,255,0)";
		output[2] = "rgb(0,125,0)";
		return output; 
	}
	
	// if not, dead colors 
	
	if (pCell->phenotype.get_current_phase_code() == PhysiCell_constants::apoptotic )  // Apoptotic - Red
	{
		output[0] = "rgb(255,0,0)";
		output[2] = "rgb(125,0,0)";
	}
	
	// Necrotic - Brown
	if( pCell->phenotype.get_current_phase_code() == PhysiCell_constants::necrotic_swelling || 
		pCell->phenotype.get_current_phase_code() == PhysiCell_constants::necrotic_lysed || 
		pCell->phenotype.get_current_phase_code() == PhysiCell_constants::necrotic )
	{
		output[0] = "rgb(250,138,38)";
		output[2] = "rgb(139,69,19)";
  }	
	
	return output; 
}
*/

std::vector<double> DoSVG::transmission( std::vector<double>& incoming_light, std::vector<double>& absorb_color, double thickness , double stain )
{
	double param = thickness * stain / 255.0; 

	static std::vector<double> output( 3, 0.0 );
 
	for( int i=0; i < 3 ; i++ )
//	{ output[i] = incoming_light[i] * exp( -param * absorb_color[i] / 255.0 ); }
	{ output[i] = incoming_light[i] * exp( -param * absorb_color[i] ); }
 
	return output;
}

/**std::vector<std::string> DoSVG::hematoxylin_and_eosin_cell_coloring( Cell* pCell )
{
	static std::vector<std::string> out( 4, "rgb(255,255,255)" );
	// cyto_color, cyto_outline , nuclear_color, nuclear_outline

	// cytoplasm colors 
  
	double fluid_fraction = pCell->phenotype.volume.cytoplasmic_fluid / (pCell->phenotype.volume.cytoplasmic + 1e-10);
	double solid_fraction = pCell->phenotype.volume.cytoplasmic_solid / (pCell->phenotype.volume.cytoplasmic + 1e-10);
	double calc_fraction  = pCell->phenotype.volume.calcified_fraction; 
 
	static double thickness = 20;
 
	static std::vector<double> light( 3, 255.0 ); 
 
	static std::vector<double> eosin_absorb( 3 , 3.0 ); // (3,33,3)
	static std::vector<double> hematoxylin_absorb( 3, 45.0 ); // (45,51,20)
	static bool setup_done = false; 
	if( !setup_done )
	{
		eosin_absorb[1] = 33.0;
		hematoxylin_absorb[1] = 51.0; 
		hematoxylin_absorb[2] = 20.0; 
		setup_done = true; 
	}

	static std::vector<double> temp( 3, 0.0 );
 
	temp = transmission( light, eosin_absorb , thickness , solid_fraction );
	temp = transmission( temp , hematoxylin_absorb ,thickness, calc_fraction );
 
	static char szTempString [128]; 
	sprintf( szTempString , "rgb(%u,%u,%u)", (int) round( temp[0] ) , (int) round( temp[1] ) , (int) round( temp[2]) ); 
	out[0].assign( szTempString ); 
	out[1] = out[0]; 
 
	// nuclear colors 
 
	// fluid_fraction = pCell->phenotype.volume.nuclear_fluid / (pCell->phenotype.volume.nuclear + 1e-10); // pCell->phenotype.volume.nuclear_fluid_volume / ( pCell->State.nuclear_volume + 1e-10 ); 
	solid_fraction = pCell->phenotype.volume.nuclear_solid / (pCell->phenotype.volume.nuclear + 1e-10); // pCell->State.nuclear_solid_volume / ( pCell->State.nuclear_volume + 1e-10 ); 
 
	temp = transmission( light , hematoxylin_absorb , thickness , solid_fraction );

	sprintf( szTempString , "rgb(%u,%u,%u)", (int) round( temp[0] ) , (int) round( temp[1] ) , (int) round( temp[2]) ); 
	out[2].assign( szTempString ); 
	out[3] = out[2]; 

	return out;
}*/

void DoSVG::print_time( WriteSVG* svg, Microenvironment* M, double time, int total_cell_count, int zslice )
{
	char* szString; 
	szString = new char [1024]; 
 
	std::string time_label = formatted_minutes_to_DDHHMM( time ); 
 
	sprintf( szString , "Current time: %s, z = %3.2f %s", time_label.c_str(), zslice, simulation_space_units.c_str() ); 
	(*svg).add_text( szString, font_size*0.5, font_size*(.2+1), font.c_str(), font_color.c_str(), font_size );
	sprintf( szString , "%u agents" , total_cell_count ); 
	(*svg).add_text( szString, font_size*0.5, font_size*(.2+1+.2+.9), font.c_str(), font_color.c_str(), 0.95*font_size );
	
	delete [] szString; 
}

void DoSVG::draw_scale_bar( WriteSVG* svg )
{
	double bar_height = 0.01 * plot_height; 
	//double bar_stroke_width = 0.001 * plot_height; 
	
	std::string bar_units = simulation_space_units; 
	
	double temp = bar_width;  
	// convert from micron to mm
	if( temp > 999 && std::strstr( bar_units.c_str(), mu.c_str() ) )
	{
		temp /= 1000;
		bar_units = "mm";
	}
	// convert from mm to cm 
	if( temp > 9 && std::strcmp( bar_units.c_str() , "mm" ) == 0 )
	{
		temp /= 10; 
		bar_units = "cm";
	}
	
	char* szString; 
	szString = new char [1024];
	sprintf( szString, "%u %s", (int) round( temp ), bar_units.c_str() );
 
	(*svg).rect( plot_width - bar_margin - bar_width, plot_height + top_margin - bar_margin - bar_height, bar_width, bar_height, 0.002 * plot_height, "rgb(255,255,255)", "rgb(0,0,0)" );
	(*svg).add_text( szString, plot_width - bar_margin - bar_width + 0.25*font_size, plot_height+top_margin-bar_margin-bar_height-0.25*font_size, font.c_str(), font_color.c_str(), font_size ); 
	
	delete [] szString; 
}

void DoSVG::print_run_time( WriteSVG* svg )
{
	char* szString; 
	szString = new char [1024]; 
	RUNTIME_TOC(); 
	std::string formatted_stopwatch_value = format_stopwatch_value( runtime_stopwatch_value() );
	svg->add_text( formatted_stopwatch_value.c_str(), bar_margin, top_margin + plot_height - bar_margin, font.c_str(), font_color.c_str(), 0.75*font_size );
	delete [] szString; 
}
	
void DoSVG::SVG_plot( std::string filename , Microenvironment* M, Cell_Container* container, double z_slice, double time, int mode_cell_color )
{
	std::vector<double> lims (5, 0);
	lims[0] = M->mesh.bounding_box[0];  // X_min
	lims[1] = M->mesh.bounding_box[3]; // X_max
	lims[2] = M->mesh.bounding_box[1];  // Y_min
	lims[3] = M->mesh.bounding_box[4];  // Y_max

	plot_width = lims[1] - lims[0]; 
	plot_height = lims[3] - lims[2]; 
	top_margin = font_size * (.2+1+.2+.9+.5 ); 
	bar_margin = 0.025 * plot_height; 
	lims[4] = top_margin;

	// open the file, write a basic "header"
	std::ofstream os( filename , std::ios::out );
	WriteSVG svg;
	svg.set_stream( &os );
	svg.start_file( plot_width , plot_height + top_margin );

	// draw the background 
	svg.rect( 0 , 0 , plot_width, plot_height + top_margin , 0.002 * plot_height , "white", "white" );

	double dx_stroma = M->mesh.dx; 
	//double dy_stroma = M->mesh.dy; 
	
	os << " <g id=\"ECM\">" << std::endl; 
  
	int ratio = 1; 
	double voxel_size = dx_stroma / (double) ratio ; 
  
	//double half_voxel_size = voxel_size / 2.0; 
	//double normalizer = 78.539816339744831 / (voxel_size*voxel_size*voxel_size); 
 
 // color in the background ECM
/* 
 if( ECM.TellRows() > 0 )
 {
  // find the k corresponding to z_slice
  
  
  
  Vector position; 
  *position(2) = z_slice; 
  

  // 25*pi* 5 microns^2 * length (in source) / voxelsize^3
  
  for( int j=0; j < ratio*ECM.TellCols() ; j++ )
  {
   // *position(1) = *Y_environment(j); 
   *position(1) = *Y_environment(0) - dy_stroma/2.0 + j*voxel_size + half_voxel_size; 
   
   for( int i=0; i < ratio*ECM.TellRows() ; i++ )
   {
    // *position(0) = *X_environment(i); 
    *position(0) = *X_environment(0) - dx_stroma/2.0 + i*voxel_size + half_voxel_size; 
	
    double E = evaluate_Matrix3( ECM, X_environment , Y_environment, Z_environment , position );	
	double BV = normalizer * evaluate_Matrix3( OxygenSourceHD, X_environment , Y_environment, Z_environment , position );
	if( isnan( BV ) )
	{ BV = 0.0; }

	vector<string> Colors;
	Colors = hematoxylin_and_eosin_stroma_coloring( E , BV );
	Write_SVG_rect( os , *position(0)-half_voxel_size-X_lower , *position(1)-half_voxel_size+top_margin-Y_lower, 
	voxel_size , voxel_size , 1 , Colors[0], Colors[0] );
   
   }
  }
 
 }
*/
	os << " </g>" << std::endl; 
 
	// Now draw vessels
	/*
	 std::vector<std::string> VesselColors = hematoxylin_and_eosin_stroma_coloring( 0,1 );

	 os << " <g id=\"BloodVessels\">" << endl; 
	 extern vector<BloodVesselSegment*> BloodVesselSegments; 
	 Vector Offset; 
	 *Offset(0) = X_lower; 
	 *Offset(1) = Y_lower-top_margin;
	*/
 
	// write the simulation time to the top of the plot
	int total_cell_count = container->nb_active_cells();
	print_time( &svg, M, time, total_cell_count, z_slice );

	// plot intersecting cells 
	os << " <g id=\"cells\">" << std::endl;
   container->draw_cells_SVG( &svg, &os, z_slice, lims, plot_nuclei, mode_cell_color );	
	os << "</g>" << std::endl; 
	
	// plot intersecting BM points
	/* 
	 for( int i=0 ; i < BasementMembraneNodes.size() ; i++ )
	 {
		// vector<string> Colors = false_cell_coloring( pC ); 
		BasementMembraneNode* pBMN = BasementMembraneNodes[i]; 
		double thickness =0.1; 
		
		if( fabs( *(pBMN->Position)(2) - z_slice ) < thickness/2.0 ) 
		{
		 string bm_color ( "rgb(0,0,0)" );
		 double r = thickness/2.0; 
		 double z = fabs( *(pBMN->Position)(2) - z_slice) ; 

		 os << " <g id=\"BMN" << pBMN->ID << "\">" << std::endl; 
		 Write_SVG_circle( os,*(pBMN->Position)(0)-X_lower, *(pBMN->Position)(1)+top_margin-Y_lower, 10*thickness/2.0 , 0.5 , bm_color , bm_color ); 
		 os << " </g>" << std::endl;
		}
		// pC = pC->pNextCell;
	 }
	*/ 
 
	// draw a scale bar
	draw_scale_bar( &svg );

	// plot runtime
	// print_run_time( svg ); 

	// draw a box around the plot window
	svg.rect( 0 , top_margin, plot_width, plot_height , 0.002 * plot_height , "rgb(0,0,0)", "none" );
	
	// close the svg tag, close the file
	svg.end_file(); 
	os.close();

	return; 
}

