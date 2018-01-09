
/** \page init PhysiBoSS_CreateInitTxtFile executable 
 *
 * \brief Executable to create a txt file containing cells positions and sizes, ready to be used as initial condition of a simulation.
 *
 * This executable is created in the \p bin folder after compilation.
 * It allows the user to directly create an initial configuration file correctly formatted to be used in a simulation. It reads an input XML file (init.xml by default) containing the configuration choices (sphere, rectangle, cylinder...) and create the \p init.txt file in output. Thus it can be easily automatised different initial conditions. 
 *
 *\section usage Usage
 * To create the initial file, call PhysiBoSS_CreateInitTxtFile executable from a Terminal window.
 * The XML file to read (\p init.xml) containing the user choices can be precised as argument of the command. 
 * 
 * The file \p init.xml (or spheroid.xml here in the example below) contained two parts: the description of active cells initial geometry and sizes, and the description of eventual passive cells.
 * An example of such configuration file is given in the release and also shown in the example below.
 *
 * \section e Current Possibilities
 * In the current version, 3 geometries are proposed: spheric (mode=0), cylindric (mode=1) or rectangles (mode=2). 
 * The created shape is centered around (0,0,0) but this can be very easily modified in the source code.
 * Active and/or passive cells can be placed in those geometries.
 * The mean radius of cells can be precised, as well as the dimensions of the geometries (sphere radius, cylinder length and radius, rectangle width and length).
 * If 2 cell types are present the proportion to which the first cell type should be initialised can be precised with \p cell_proportion parameter. 
 *  
 * \section ex Example
 * Command: \n
 * \code path_to_physiboss/bin/PhysiBoSS_CreateInitTxtFile file spheroid.xml \endcode
 *
 * Example of the init configuration file \p spheroid.xml:
 * \verbatim
<?xml version="1.0" encoding="UTF-8" ?>

<active_cells>
	<mode> 0 </mode>
	<cell_radius> 9.5 </cell_radius>
	<length> 100 </length>
	<cell_spacing> 0.95 </cell_spacing>
</active_cells>

<passive_cells>
	<mode> 0 </mode>
	<cell_radius> 10 </cell_radius>
	<length> 150 </length>
	<min_length> 100 </min_length>
	<cell_spacing> 1 </cell_spacing>
</passive_cells>
 \endverbatim
 * With this file, we created a spheroid of radius 100 µm, composed of cells with a mean radius of 9.5 µm, and spaced with 0.95% of their size (small overlap). 
 * The spheroid is surrounded by a spheric extra-cellular matrix, with a radius of 150 µm (but empty in the 100 µm internal sphere) represented by passive cells of mean radius 10 µm. 
 *
 * \todo Add more options of configurations (more geometries, distributions of cell size and phases...). 
 *
 * \author G. Letort, Institut Curie 
 * \date June 2017
 * */
#ifndef _CREATETXT_
#define _CREATETXT_

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "../base/vector3d.h"
#include "../base/utilities.h"
#include "../base/readXML.h"


std::vector<Vector3d*> cells; // position of already placed cells
std::vector<double> rads;  // radius of already placed cells
std::string del = ";"; // delimiter: used ; because of paraview
double postm = 0.17;   // proportion of cells in post-mitotic phase initially
double proprad = 0.23; // variation of the radius, from minimal value to maximal (min+min*proprad), to get all the possible ranges of radius initially
int ncelllines = 2;   // number of distinct cell lines


/** \brief Add the cell 
 *
 * @param delta position
 * @param rad radius of the cell
 * @param type cell line number (-1 for passive)
 * @param out file in which to write it
 * @param minrad radius if cell is in post-mitotic phase 
 * @param phase phase in which the cell should be */
void addOneCell( Vector3d delta, double rad, int type, std::ostream& out, double minrad, int phase )
{
	// time, id
	out << 0 << del << 0 << del; 
	double elaps = 0;
	cells.push_back( new Vector3d(delta) );
	if ( UniformRandom() <= postm )
	{ 
		rad = minrad;
		elaps = UniformRandom()*2.7*60.0;
		phase = 1;
	}
	rads.push_back( rad );
	// x, y, z, rad
	out << delta[0] << del << delta[1] << del << delta[2] << del;
	out << rad << del;
	// volumes
	out << -1 << del << -1 << del << -1 << del << -1 << del;
	out << -1 << del << -1 << del;
	// cell line, phase, elapsed time
	out << type << del << phase << del << elaps;
	// angle
    out << del << -1 ;
	out << std::endl;
}

/** \brief Check if there is place to add a new cell
 * And add it if yes
 */
bool hasPlace( Vector3d delta, double rad, int type, std::ostream& out )
{
double spacing = 0.85; // min distance between spheres
	for (unsigned int idx = 0; idx < cells.size(); idx++)
	{
		if (( (*cells[idx]) - delta).norm() < spacing * (( rads[idx]) + rad) )
			return false;
	}

	// add cell if it is not too close to others
	addOneCell( delta, rad, type, out, rad, 0 );
	return true;
}

/** \brief Try to place one cell in the sphere of radius maxrad, out of sphere of radious minrad 
 * Cell has radius cellradius and cell line number type */
bool placeInSphere( double maxrad, double minrad, double cellradius, int type, std::ostream& out )
{
	// get random cell position
	Vector3d delta(0,0,0);
	delta.randomize_normed();
	delta *= (minrad + UniformRandom() * (maxrad-minrad) );

	// check if cells are not too close
	return hasPlace( delta, cellradius, type, out );
}

/** Sphere between radius minrad and radius maxrad, with cell of mean radius meancellrad and spacing between cells */
void createSphere( double maxrad, double minrad, double meancellrad, double spacing, double type, std::ostream& out, int phase )
{
	int xc=0,yc=0,zc=0;
	double x_spacing = meancellrad * sqrt(3) * spacing;
	double y_spacing = meancellrad * 2 * spacing;
	double z_spacing = meancellrad * sqrt(3) * spacing;

	Vector3d tempPoint;

	double sphere_radius = maxrad;
	for(double z=-sphere_radius;z<sphere_radius;z+=z_spacing, zc++)
		for(double x=-sphere_radius;x<sphere_radius;x+=x_spacing, xc++)
			for(double y=-sphere_radius;y<sphere_radius;y+=y_spacing, yc++)
			{
				tempPoint[0] = x + (zc)%2 * 0.5 * meancellrad ;
				tempPoint[0] += x_spacing * UniformRandom11()*0.15; // add some noise
				tempPoint[1] = y + (xc)%2 * meancellrad;
				tempPoint[1] += y_spacing * UniformRandom11()*0.15; // add some noise
				tempPoint[2] = z;
				tempPoint[2] += z_spacing * UniformRandom11()*0.15; // add some noise

				if( (tempPoint.norm() < sphere_radius) && (tempPoint.norm() > minrad) )
				{
					double rad = meancellrad + meancellrad * UniformRandom() * proprad;
					double typ = type;
				if ( type >= 0 )
				{
					if ( ncelllines <= 2 )
						typ = (UniformRandom() <= type) ? 0 : 1;
					else
					{
						// in current version, cannot create more than 3 cell lines, change to propose more active_cells_parameters in main
						double uran = UniformRandom();
						typ = 0;
						if ( uran <= type ) 
							typ = 1;
						else
						{
							if ( uran <= 2*type ) 
								typ = 2;
						}
					}
				}
					
					addOneCell( tempPoint, rad, typ, out, meancellrad, phase );
				}
			}
}
			
void createDisk( double length, double radius, double spacing, double type, std::ostream& out, int phase )
{
	int xc=0,yc=0,zc=0;
	double x_spacing = radius * sqrt(3) * spacing;
	double y_spacing = radius * 2 * spacing;

	Vector3d tempPoint;

	for(double x=-length; x<length; x+=x_spacing, xc++)
	{
		for(double y=-length; y<length; y+=y_spacing, yc++)
		{
			tempPoint[0] = x + (zc)%2 * 0.5 * radius ;
			tempPoint[0] += x_spacing * UniformRandom11()*0.15; // add some noise
			tempPoint[1] = y + (xc)%2 * radius;
			tempPoint[1] += y_spacing * UniformRandom11()*0.15; // add some noise
			tempPoint[2] = 0;

			if( (tempPoint.norm() < length) )
			{
				tempPoint[2] = UniformRandom11(); // with small noise
				double rad = radius + radius * UniformRandom() * proprad;
				double typ = type;
				if ( type >= 0 )
				{
					typ = (UniformRandom() <= type) ? 0 : 1;
				}
				addOneCell( tempPoint, rad, typ, out, radius, phase );
			}
		}
	}
}

/** \brief Place cells inside a rectangle of length length and width width */
void createRectangle( double length, double width, double radius, double spacing, double type, std::ostream& out, int phase )
{
	int xc=0,yc=0,zc=0;
	double x_spacing = radius * sqrt(3) * spacing;
	double y_spacing = radius * 2 * spacing;

	Vector3d tempPoint;

	for(double x=-length; x<length; x+=x_spacing, xc++)
	{
		for(double y=-width; y<width; y+=y_spacing, yc++)
		{
			tempPoint[0] = x;
			tempPoint[0] += x_spacing * UniformRandom11()*0.15; // add some noise
			tempPoint[1] = y + (xc)%2 * radius;
			tempPoint[1] += y_spacing * UniformRandom11()*0.15; // add some noise
			tempPoint[2] = 0;

			tempPoint[2] = UniformRandom11(); // with small noise
			double rad = radius + radius * UniformRandom() * proprad;
			double typ = type;
			if ( type >= 0 )
			{
				typ = (UniformRandom() <= type ) ? 0 : 1;
			}
			addOneCell( tempPoint, rad, typ, out, radius, phase );
		}
	}
}


/** Print help, to update to options */
void printHelp()
{
	std::cout << "Initial configuration creation" << std::endl;
	std::cout << "Nb: length dimension are in e-6m (µm)" << std::endl;
	std::cout << "ncells nb_of_cells: determine initial number of active cells" << std::endl;
	std::cout << "radius rad: mean initial radius of cells" << std::endl;
	std::cout << "mode which_mode: different initial configurations for active cells:" << std::endl;
	std::cout << "				   0: sphere" << std::endl;
	std::cout << "				   1: disc" << std::endl;
	std::cout << "				   2: rectangle" << std::endl;
	std::cout << "passncells nb_of_cells: determine initial number of passive cells" << std::endl;
	std::cout << "passradius rad: mean initial radius of passive cells" << std::endl;
	std::cout << "passmode which_mode: different initial configurations for passive cells:" << std::endl;
	std::cout << "				   0: sphere around active cells" << std::endl;
	return;
}

/** Read one argument and its value */
void readArg( std::string* inarg, int* i, int narg, char** allarg )
{
	(*i)++;
	if ( (*i) > narg )
	{
		std::cout << "Warning: Missing parameter value of " << (*inarg) << std::endl;
		(*inarg) = "Missing";
		return;
	}
	(*inarg) = allarg[(*i)];
}

/** MAIN function, create initial state */
int main(int argc, char **argv)
{
	std::string infile = "init.xml";
	// read command line arguments
	for ( int i=1; i<argc; ++i )
	{
		std::string argument = argv[i];
		argument = argv[i];

		if ( argument == "help" )
		{
			printHelp();
			return 1;
		}
		if ( argument == "file" )
		{
			readArg( &argument, &i, argc, argv ) ;
			if ( argument != "Missing" ) 
				infile = argument ;
		}
	}

	// read init configuration file
	ReadXML reader;
	reader.set_file( infile );
	reader.loadXMLFile( "passive_cells", "", "", "active_cells" );

	// shape of active cells: 0 : sphere, 1: disk
	int mode = 0;
	reader.getIntValue( "active_cells_0", "mode", &mode );
	// shape of passive cells: 0 : sphere, 1: cylindre, 2: rectangle
	int passmode = 0;
	reader.getIntValue( "passive_cells", "mode", &passmode );
	// active cells radius
	double radius = 8.5; 
	reader.getDoubleValue( "active_cells_0", "cell_radius", &radius );
	// passive cells radius
	double passradius = 6; 
	reader.getDoubleValue( "passive_cells", "cell_radius", &passradius );
	// length of cells geometry
	double length = 100;
	reader.getDoubleValue( "active_cells_0", "length", &length );
	// width of cells geometry
	double width = 100;
	reader.getDoubleValue( "active_cells_0", "width", &width );
	// length of passive cells geometry
	double passlength = 130;
	reader.getDoubleValue( "passive_cells", "length", &passlength );
	// min length of passive cells geometry
	double passminlength = length;
	reader.getDoubleValue( "passive_cells", "min_length", &passminlength );
	// active cells minimal spacing (init overlapping control density of cells)
	double spacing = 0.95; 
	reader.getDoubleValue( "active_cells_0", "cell_spacing", &spacing );
	// passive cells minimal spacing (init overlapping control density of cells)
	double passspacing = 0.95; 
	reader.getDoubleValue( "passive_cells", "cell_spacing", &passspacing );
	// proportion of first cell type 
	double prop = 1; 
	reader.getDoubleValue( "active_cells_0", "cell_proportion", &prop );
	// active cells initial phase
	int phase = 0; 
	reader.getIntValue( "active_cells_0", "cell_phase", &phase );
	// number of different active cell lines
	reader.getIntValue( "active_cells_0", "nb_cell_line", &ncelllines );


	// Create init cells positions file
	std::ofstream out ("init.txt");
    out << "%% Automatically generated initial file with PhysiCell_CreateInitFile executable" << std::endl;
	double type;

	//////////////////////////////////////////////////////	
	// Active cells
	Vector3d center(0, 0, 0);
	type = prop;
	switch( mode )
	{
		case 0:  // Sphere
			createSphere( length, 0, radius, spacing, type, out, phase );
			break;
		case 1:  // Cylindre
			createDisk( length, radius, spacing, type, out, phase );
			break;
		case 2:  // Rectangle
			createRectangle( length, width, radius, spacing, type, out, phase );
		default:
			break;
	}

	//////////////////////////////////////////////////////	
	// Passive cells 
	type = -1; //0;
	switch( passmode )
	{
		case 0:  // Sphere
			createSphere( passlength, passminlength, passradius, passspacing, type, out, phase );
			break;
		default:
			break;
	}

	out.close();
}

#endif
