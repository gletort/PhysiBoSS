#include "simul.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <fstream>
#include <sstream>
#include <time.h>
#include <exception> 

#include "../BioFVM/BioFVM.h"  

using namespace BioFVM;

int omp_num_threads = 10; // set number of threads for parallel computing
// set this to # of CPU cores x 2 (for hyperthreading)


Simul::Simul(): microenvironment(), reader(), cell_container()
{
	cell_types.resize(0);
	type_prop.resize(0);
	dt = 0;
	ndensities = 1;
	maboss = NULL;
	write_prop = true;
	concentrations.resize(1);
	conc_names.resize(1);
	clear();
}

Simul::~Simul()
{
	if (maboss) 
		delete maboss;
}

/* Put default values in all attributes */
void Simul::clear()
{
	duct_radius = 100;  // set of cells radius
	init_file = "";      // if read initial state from a file
	ecm_file = "";
	cell_radius = 8.413; // MCF7 cell radius
	min_voxel_size = 30;
	max_z = 0;
	concentrations[0] = 7.1930;   //check this value to make sure it has support from literature
	conc_names[0] = "oxygen";
	dt = 0.01;         // time step
	mechanics_dt= 0.1; // mechanical time step
	cell_cycle_dt= 6;  // cell cycle time step
	t_output_interval = 60.0; // printing interval
	t_output_densities = 600.0; // printing microenvironement interval
	t_max = 3*24*60;       // maximal time
	npassive = 0;       // no passive cells (obstacles, dextran)
	tegf = t_max*2;
	ttnf = t_max*2;
	ttnf_pulse = 10;    // injection during 10 minutes
	ttnf_next = 0;		// when to add tnf
	tremtnf = t_max*2;
	tpassive = t_max*2;
	fric_passive = 0.0001;
	mode_injection = -1;
	ratio_write = 0.5;	

	membrane_shape = "none";
	membrane_rad = 0;
	mode_cycle = 0; // cell cycle mode used	
}

/* Add one cell with given properties */
void Simul::addCell( double x, double y, double z, double rad, int cl, int pha, double elap )
{
	// active cell
	if ( cl >= 0 )
	{			
		Cell* pCell = cell_container.create_cell();
		pCell->set_cell_line( cell_types[cl] );
		pCell->set_cycle( maboss, pha, elap );
		if (mode_cycle == 0)
			pCell->init_cell_cycle_classic( 0, elap );		
		pCell->load_default_values();

		pCell->register_microenvironment(&microenvironment);
		Vector3d position( x, y, z );
		if ( !pCell->assign_position( position ) ) 
		{
			cell_container.delete_cell( pCell->index );
			return;
		}

		pCell->set_type(PhysiCell_constants::TUMOR_TYPE);		
		pCell->init_env_rates();
		pCell->set_new_volume( get_volume(rad) );
	}
	else
		add_one_passive_cell( Vector3d(x,y,z), rad, fric_passive );
}

/* Add one cell to the all_cells set, of given position */	
void Simul::add_one_cell( Vector3d position )
{
	int which = 0;
	if ( uniform_random() > type_prop[0] )
	{
		which = 1;
	}
	addCell( position[0], position[1], position[2], cell_radius, which, PhysiCell_constants::Ki67_negative, 0 ); 

}

/* Add one passive cell to the all_cells set, of given position */	
void Simul::add_one_passive_cell( Vector3d position, double r, double fric )
{
	Sphere* pCell = cell_container.create_sphere();

	pCell->set_total_volume( get_volume(r) );
	pCell->set_type( PhysiCell_constants::PASSIVE_TYPE );		
	// to organize differently
	pCell->register_microenvironment(&microenvironment);
	if ( !pCell->assign_position( position ) ) 
	{
		cell_container.delete_cell( pCell->index );
		return;
	}
	pCell->set_repulsion(0);
	pCell->set_friction(fric);
}

void Simul::create_sphere( double sphere_radius )
{
	int xc=0,yc=0,zc=0;
	double x_spacing= cell_radius * sqrt(3);
	double y_spacing= cell_radius * 2;
	double z_spacing= cell_radius * sqrt(3);

	Vector3d tempPoint;

	for(double z=-sphere_radius;z<sphere_radius;z+=z_spacing, zc++)
		for(double x=-sphere_radius;x<sphere_radius;x+=x_spacing, xc++)
			for(double y=-sphere_radius;y<sphere_radius;y+=y_spacing, yc++)
			{
				tempPoint[0] = x + (zc)%2 * 0.5 * cell_radius ;
				tempPoint[0] += x_spacing * (0.5 - uniform_random()) / 2.0; // add some noise
				tempPoint[1] = y + (xc)%2 * cell_radius;
				tempPoint[1] += y_spacing * (0.5 - uniform_random()) / 2.0; // add some noise
				tempPoint[2] = z;
				tempPoint[2] += z_spacing * (0.5 - uniform_random()) / 2.0; // add some noise

				if( tempPoint.norm() < sphere_radius )
				{
					add_one_cell( tempPoint );
				}
			}
}

double dist( std::vector<double> one, std::vector<double> two ) 
{
	return sqrt( (one[0]-two[0]) * (one[0]-two[0]) + (one[1]-two[1]) * (one[1]-two[1]) + (one[2]-two[2])*(one[2]-two[2]) );
}

void Simul::run_time_step( double t )
{
	// Inject given concentration on first voxel only
	if ( t < ttnf_next )
	{
		int k = microenvironment.get_index("tnf");
		if ( k >= 0 ) 
			inject_density(k);
	}
	cell_container.simulate_cell_sources_and_sinks( &microenvironment, dt );
	microenvironment.simulate_diffusion_decay( dt );
	cell_container.update_all_cells(t, cell_cycle_dt, mechanics_dt);		
}

/* Initialize simulation parameters */
void Simul::initSimul() throw()
{
	reader.loadXMLFile( "simulation", "initial_configuration", "network", "cell_properties");
	// Time parameters
	reader.getDoubleValue( "simulation", "time_step", &dt );
	reader.getDoubleValue( "simulation", "mechanics_time_step", &mechanics_dt  );
	reader.getDoubleValue( "simulation", "cell_cycle_time_step", &cell_cycle_dt );
	reader.getDoubleValue( "simulation", "output_interval", &t_output_interval ); 
	reader.getDoubleValue( "simulation", "output_densities", &t_output_densities ); 
	reader.getDoubleValue( "simulation", "maximal_time", &t_max );
	// Space parameters
	reader.getDoubleValue( "simulation", "minimum_voxel_size", &min_voxel_size );

	reader.getIntValue( "simulation", "number_of_densities", &ndensities );
	concentrations.resize(ndensities);
	conc_names.resize(ndensities);
	reader.getIntValue( "simulation", "mode_cell_cycle", &mode_cycle );
	// openmp setup
	reader.getIntValue( "simulation", "number_of_threads", &omp_num_threads );
	omp_set_num_threads(omp_num_threads);

	// options
	int write_pass = 0; // write passive cells output files or not
	reader.getIntValue( "simulation", "write_passive_cells", &write_pass );
	if ( write_pass )
		cell_container.write_all();
	reader.getDoubleValue( "simulation", "write_ratio_voxels", &ratio_write );
	reader.getDoubleValue( "simulation", "friction_passive_cells", &fric_passive );

	// Initial configuration
	reader.getDoubleValue( "initial_configuration", "sphere_radius", &duct_radius );
	reader.getStringValue( "initial_configuration", "load_cells_from_file", &init_file );
	reader.getStringValue( "initial_configuration", "create_ecm_from_file", &ecm_file );
	reader.getDoubleValue( "initial_configuration", "nb_passive_cells", &npassive );
	reader.getDoubleValue( "initial_configuration", "time_passive_cells", &tpassive );
	reader.getDoubleValue( "initial_configuration", "time_add_egf", &tegf );
	reader.getDoubleValue( "initial_configuration", "time_add_tnf", &ttnf );
	reader.getDoubleValue( "initial_configuration", "duration_add_tnf", &ttnf_pulse );
	reader.getDoubleValue( "initial_configuration", "time_remove_tnf", &tremtnf );
	reader.getIntValue( "initial_configuration", "mode_injection", &mode_injection );
	for ( int i = 0; i < ndensities; i++ )
	{
		std::string curname = "oxygen";
		reader.getStringValue("simulation", "density_"+std::to_string(i), &curname );
		strip( &curname );
		conc_names[i] = curname;
		concentrations[i] = 0;
		reader.getDoubleValue( "initial_configuration", conc_names[i]+"_concentration", &(concentrations[i]) );
	}
	// Membrane shape: so far, none, duct or sphere
	reader.getStringValue("initial_configuration", "membrane_shape", &membrane_shape);
	cell_container.set_membrane_shape( membrane_shape );
	reader.getDoubleValue("initial_configuration", "membrane_length", &membrane_rad);
	cell_container.set_membrane_length( membrane_rad );

	// Write properties to file
	if ( write_prop )
	{
		std::ofstream prop_file( "properties.xml" );
		write_properties( prop_file );
		prop_file << std::endl;
		prop_file.close();
	}
}

/* Create the microenvironement object */
void Simul::initEnvironment()
{
	double dx = 20; 
	double dy = 20;  
	double dz = 20;

	// figure out the bounding box 
	std::vector<double> bounding_box( 6, 0.0 );
	// default value (in case not in parameters file)
	double xmin = -200, xmax = 1000, ymin = -200, ymax = 200, zmin = -200, zmax = 200;
	reader.getDoubleValue( "simulation", "bounding_box_xmin", &xmin );
	reader.getDoubleValue( "simulation", "bounding_box_xmax", &xmax );
	reader.getDoubleValue( "simulation", "bounding_box_ymin", &ymin );
	reader.getDoubleValue( "simulation", "bounding_box_ymax", &ymax );
	reader.getDoubleValue( "simulation", "bounding_box_zmin", &zmin );
	reader.getDoubleValue( "simulation", "bounding_box_zmax", &zmax );
	reader.getDoubleValue( "simulation", "minimum_voxel_size", &dx );

	dy = dx;
	dz = dx;
	bounding_box[0] = xmin; bounding_box[3] = xmax; 
	bounding_box[1] = ymin; bounding_box[4] = ymax; 
	bounding_box[2] = zmin; bounding_box[5] = zmax; 

	// create a Multiscale_Microenvironment; 
	microenvironment.name="substrate scale";
	// add a microenvironment for simulating substrates 	
	// microenvironment.create_microenvironment( "substrate scale"); 
	microenvironment.set_density(0, conc_names[0] , "mmHg" );
	for ( int i = 1; i < ndensities; i++ )
	{
		microenvironment.add_density( conc_names[i] , "dimensionless" );
	}
	std::cout << bounding_box << std::endl; 

	microenvironment.resize_space( bounding_box[0] , bounding_box[3] , bounding_box[1], bounding_box[4] , bounding_box[2] , bounding_box[5] ,dx,dy,dz );
	microenvironment.spatial_units = "microns";
	microenvironment.time_units = "minutes";
	microenvironment.mesh.units = "microns";

	cell_container.initialize( bounding_box[0] , bounding_box[3] , bounding_box[1], bounding_box[4] , bounding_box[2] , bounding_box[5], min_voxel_size );
	std::cout << "num voxels4: " << cell_container.mesh_size() << std::endl;

	// Add given initial concentration (everywhere or on the extremities only or on one point only (first voxel))
	/**Vector3d cent;
	for( int n=0; n < microenvironment.number_of_voxels() ; n++ )
	{
		cent = microenvironment.voxel_center(n); 
		if ( ! cell_container.inside_BM( &cent ) )
		{
			for ( int k = 0; k < ndensities; k++ )	
				microenvironment.density_vector(n)[k] = concentrations[k]; 	
		}
	}*/
	// Calculate concentrations to add by voxel:
	max_z = zmax * 0.85;
	for ( int k = 0; k < ndensities; k++ )
	{
			concentrations[k] *= microenvironment.voxel_volume(0) * 0.000001;  // in fg/µm^3, to add to have given concentrations	
			inject_density(k);
	}

	if ( ecm_file != "" )
		load_ecm_file();

	// register the diffusion solver 
	microenvironment.diffusion_decay_solver = diffusion_decay_solver__constant_coefficients_LOD_3D; 

	// register substrates properties 
	for ( int k = 0; k < ndensities; k++ )
	{
		//microenvironment.diffusion_coefficients[k] = 300; // 300 microns^2 / min, for growth factors ~ 300, Fallahi-Sichani et al., 2010 & Nugent 1984
		microenvironment.diffusion_coefficients[k] = 1200; // microns^2 / min, for TNF Cheong et al
		microenvironment.decay_rates[k] = 0.0275;   // /min. Value for TNF, Cheong et al., 2006
	}	
	if ( microenvironment.get_index("oxygen") >= 0 )
	{
		microenvironment.diffusion_coefficients[microenvironment.get_index("oxygen")] = 1.0e5; // microns^2 / min 
		microenvironment.decay_rates[microenvironment.get_index("oxygen")] = 0.1;
	}
	if ( microenvironment.get_index("ecm") >= 0 )
	{
		microenvironment.diffusion_coefficients[microenvironment.get_index("ecm")] = 1e-85; // microns^2 / min 
		microenvironment.decay_rates[microenvironment.get_index("ecm")] = 0;
	}	

	microenvironment.display_information( std::cout );

	/** \todo put ratio of voxels to write as a parameter */
	microenvironment.list_indexes( ratio_write );	

	//add Dirichlet node for all the voxels located outside of the limits for the first density (\todo to make more flexible)
	int dirichlet = 0;
	reader.getIntValue( "simulation", "dirichlet_boundary", &dirichlet );
	if ( dirichlet && ndensities >= 0 )
	{
		std::vector<double> dirichlet_o2( concentrations );

		Vector3d cent;
		for( int i=0; i < microenvironment.number_of_voxels(); i++ )
		{
			cent = microenvironment.voxel_center(i); 
			strip(&membrane_shape);
			// in sheet geometry (~2D), put oxygen source only in between the two planes
			if ( membrane_shape == "sheet" )
			{
				if ( fabs(cent[2]) <= membrane_rad )
				{
					if ( sqrt( cent[1]*cent[1] + cent[0]*cent[0] ) >= bounding_box[3]*0.9)
						microenvironment.add_dirichlet_node( i , dirichlet_o2 );
				}
			}
			else
			{	
				if ( ! cell_container.inside_BM( &cent ) )
					microenvironment.add_dirichlet_node( i , dirichlet_o2 );
			}
		}
	}
}

/* Add a given density at extremities : top zs only */
void Simul::inject_density( int density_index )
{
	// Inject given concentration on the extremities only
	Vector3d cent;
	for( int n=0; n < microenvironment.number_of_voxels() ; n++ )
	{
		switch (mode_injection)
		{
			// mode -1: no injection
			case -1:
				return;
				break;
			// mode 0: everywhere
			case 0:
				microenvironment.density_vector(n)[density_index] = concentrations[density_index]; 	
				break;
			// mode 1: all boundary
			case 1:
				cent = microenvironment.voxel_center(n);
				if ( ! cell_container.inside_BM( &cent ) )
					microenvironment.density_vector(n)[density_index] = concentrations[density_index]; 	
				break;
			default:
				break;
		}

	}
}


/* Remove a given density everywhere */
void Simul::remove_density( int density_index )
{	
	for( int n=0; n < microenvironment.number_of_voxels() ; n++ )
	{
		microenvironment.density_vector(n)[density_index] = 0; 	
	}
	std::cout << "Removal done" << std::endl;
}

/* Load ecm density values from given file */
void Simul::load_ecm_file()
{
	strip( &ecm_file );
	std::cout << "Loading ECM file " << ecm_file << std::endl;
	std::ifstream infile;
	infile.open( ecm_file );
	std::string array[4];
	int i = 0;
	std::string line;
	//skip first line: title
	getline( infile, line, '\n' ); 
	while ( getline( infile, line, '\n') )
	{
		std::stringstream ss;
		ss.str( line );
		i = 0;
		while ( getline( ss, array[i], ';') )
		{
			i++;
		}
		double x = std::stod(array[0]);
		double y = std::stod(array[1]);
		double z = std::stod(array[2]);
		double amount = std::stod(array[3]);
		Vector3d pos(x, y, z);
		int voxel_index = microenvironment.nearest_voxel_index( pos );
		microenvironment.density_vector(voxel_index)[microenvironment.get_index("ecm")] += amount; 	
	}
	infile.close();
	std::cout << "File loaded " << std::endl;

}

/* Initialize a cell line with given type */
void Simul::initCellLine( CellLine* cl, int type )
{
	cl->setLineIndex(type);
	//cl->set_cancer_cell_line_MCF7(); 
	cl->display_information( std::cout );

	// properties for a cell type
	std::string propname = "cell_properties_"+std::to_string(type);
	cl->read_properties( &reader, propname );
	for ( int i = 0; i < ndensities; i++ )
	{
		cl->read_threshold( &reader, propname, conc_names[i] );
	}
	cl->scaleOxygenLevels( microenvironment.voxel_volume(0) * 0.000001 );  // in fg/µm^3

	double proportion = 0;
	/** \todo should be in initial_configuration instead */
	reader.getDoubleValue(propname, "initial_proportion", &proportion);
	type_prop.push_back(proportion);

	//std::string phenname = "cell_phenotype_"+std::to_string(0);
	//(cl->get_phenotype())->read_properties( &reader, propname, phenname );
	cell_radius = cl->get_init_radius();

	std::cout << "cell line initialized \n" << std::endl;
}

/* Initialize boolean network if used */
void Simul::initNetwork()
{
	// Initialize network
	maboss = new MaBossNetwork();
	maboss->init( "../BN/bn_nodes.bnd", "../BN/bn_conf.cfg");
	std::string propname = "network";
	maboss->read_properties( &reader, propname );
}

/* Load cells from given input file*/
void Simul::load_txt_file()
{
	strip( &init_file );
	std::cout << "Loading initial state file " << init_file << std::endl;
	std::ifstream infile;
	infile.open( init_file );
	std::string array[25];
	//double t0, id1, x2, y3, z4, rad5, v06, v17, v28, v39, v410, v511, cl12, pha13, elap14, ang15;
	int i = 0;
	std::string line;
	//skip first line: title
	getline( infile, line, '\n' ); 
	while ( getline( infile, line, '\n') )
	{
		std::stringstream ss;
		ss.str( line );
		i = 0;
		while ( getline( ss, array[i], ';') )
		{
			i++;
		}
		addCell( std::stod(array[2]), std::stod(array[3]), std::stod(array[4]), std::stod(array[5]), std::stod(array[12]), std::stod(array[13]), std::stod(array[14]) );
	}
	infile.close();
	std::cout << "File loaded " << std::endl;
}

/* Initialize all the cells */
void Simul::initCells( std::string input_file )
{
	// create everything from scratch
	if ( input_file != "" )
		init_file = input_file;

	if ( init_file == "" )
	{
		double sphere_radius = duct_radius - 15;
		create_sphere( sphere_radius );

		// Add passive cells if asked
		if ( npassive > 0 )
		{
			for (int i = 0; i < npassive; i++ )
			{	
				Vector3d pos(0,0,0);
				pos.randomize_normed();
				pos	*= (duct_radius);
				add_one_passive_cell( pos, 6, fric_passive );	
			}
		}
	}
	// or load given input file
	else
		load_txt_file();

	cell_container.set_internal_uptake(dt); 
}

void handle_eptr(std::exception_ptr eptr) 
{
	try {
		if (eptr) {
			std::rethrow_exception(eptr);
		}
	} catch(const std::exception& e) {
		std::cout << "Caught exception \"" << e.what() << "\"\n";
	}
}

/* Initialize all the simulation (parameters, environment, cells...) */
void Simul::initialize( std::string input_file )
{
	// initialize parameters, read xml parameters file	
	try
	{
		initSimul();	
	}
	catch(...)
	{
		std::exception_ptr p = std::current_exception();
		std::cerr << "Error in initialisation phase " << std::endl;
		handle_eptr( p );
		return;
	}

	// initialize microenvironemnt
	initEnvironment();	
	// do small diffusion steps alone to initialize densities
	for ( int i = 0; i < 20; i ++ )
		microenvironment.simulate_diffusion_decay( 5*dt );
				
	// tmp: to have tnf OFF then ON, to make it more automatisable
	//int k = microenvironment.get_index("tnf");
	//remove_density(k);

	// initialize cells
	if ( mode_cycle == 1 )
	{
		initNetwork();
	}	

	int ntyp = reader.nbCellProp();
	//std::ofstream prop_file ("properties.xml");
	for ( int i = 0; i < ntyp; i++ )
	{
		CellLine* cancer = new CellLine();
		initCellLine( cancer, i );
		cell_types.push_back( cancer );
		if ( write_prop )
		{
			std::ofstream prop_file( "properties.xml", std::ios_base::app );
			cancer->write_properties( prop_file );
			if ( maboss )
				maboss->write_properties( prop_file );
			prop_file << "</cell_properties>" << std::endl;
			prop_file << std::endl;
			prop_file.close();
		}
	}

	initCells( input_file );
}

/* Write Density values to output file */
void Simul::writeDensity( int index, double t )
{
	std::string ecmname; 
	ecmname.resize( 1024 );
	sprintf( (char*) ecmname.c_str() , "_t%05d.txt", (int)round(t) );
	ecmname = "microutput//"+conc_names[index]+ecmname;
	std::ofstream outecm_file( ecmname );
	microenvironment.write_density( outecm_file, index );
	outecm_file.close();
}

/** tmp version: do everything */
void Simul::run()
{
	// PNRG setup 	
	SeedRandom(); 

	initialize( "" );

	DoSVG svg;
	int svg_mode_coloring = 0;
	reader.getIntValue( "simulation", "svg_coloring_mode", &svg_mode_coloring );

	double t = 0.0; 
	double t_next_output_time = 0;
	double tsvg = 0;	

	int output_index = 0;
	int trep = 0;
	int tputegf = 0;
	double tputtnf = ttnf;	
	ttnf_next = ttnf_pulse;	
	BioFVM::RUNTIME_TIC();
	BioFVM::TIC();
	std::cout << "CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << "\n";
	std::ofstream report_file ("report.txt");
	report_file<<"simulated time\tnum cells\tnum division\tnum death\twall time";
	for ( int e = 0; e < ndensities; e++ )
		report_file << "\t" << conc_names[e];
   	report_file << std::endl;
	try 
	{
		// do mechanical runs to "stabilize" initial state
		for ( int i = 0; i < 10; i ++ )
		{
			cell_container.update_all_cells_mechanics( mechanics_dt, mechanics_dt); 
		}

		while( t < t_max )
		{
			if(  fabs( t - t_next_output_time ) < 0.0001 )
			{
				cell_container.log_output(t, output_index, &microenvironment, report_file, mode_cycle);
				// temp: to write ECM values at all times 
				int e = microenvironment.get_index("ecm");
				if ( e >= 0 )
					writeDensity(e, t);
				t_next_output_time += t_output_interval;						
			}
			if ( t >= tsvg )
			{
				//std::string svgname; 
				//svgname.resize( 1024 );
				//sprintf( (char*) svgname.c_str() , "output//svg_t%05d.svg" , (int)round(t) );
				//svg.SVG_plot( svgname, &microenvironment, &cell_container, 0, t, svg_mode_coloring );
				for ( int j = 0; j < ndensities; j++ )
				{
					writeDensity(j, t);
				}

				tsvg += t_output_densities;
			}

			if ( !trep && t > tpassive )
			{
				cell_container.set_passive_repulsion(50);
				trep = 1;
			}
			if ( !tputegf && t > tegf )
			{
				int k = microenvironment.get_index("egf");
				for( int n=0; n < microenvironment.number_of_voxels() ; n++ )
				{
					microenvironment.density_vector(n)[k] = 145; 	
				}
				tegf += 100; // reput frequently
			}
			if ( t > tputtnf )
			{
				// tmp version
				//int ktn = microenvironment.get_index("tnf");
				//concentrations[ktn] *= 10.0;
				//tputtnf += 50000;

				ttnf_next = t + ttnf_pulse;
				if ( mode_injection == 0 ) tremtnf = t + dt + ttnf_pulse;  // need to remove only when it's mode 0 = microflu
				tputtnf += ttnf; 
			}
			if ( t > tremtnf )
			{
				int k = microenvironment.get_index("tnf");
				if ( k >= 0 )
					remove_density(k);
				tremtnf += t_max;
			}

			run_time_step(t);
			t += dt; 
			output_index++;
		}
			
		cell_container.log_output(t, output_index, &microenvironment, report_file, mode_cycle);
		std::string svgname; 
		svgname.resize( 1024 );
		sprintf( (char*) svgname.c_str() , "output//svg_t%05d.svg" , (int)round(t) ); 
		svg.SVG_plot( svgname, &microenvironment, &cell_container, 0, t_max, svg_mode_coloring );
		for ( int j = 0; j < ndensities; j++ )
		{
				writeDensity(j, t);
		}
		report_file.close();

		// Delete cell lines
		while( !cell_types.empty() )
		{
			CellLine* cl = cell_types.back();
			cell_types.pop_back();
			delete cl;
		}
	}
	catch( const std::exception& e ) 
	{ 
		std::cout << e.what(); // information from length_error printed
	}
	return ; 
}

void Simul::plotFile( double t, std::string input_file )
{
	write_prop = false;
	initialize( input_file );
	DoSVG svg;

	std::string svgname; 
	svgname.resize( 1024 );
	sprintf( (char*) svgname.c_str() , "output//svg_t%05d.svg" , (int)round(t) ); 
	svg.SVG_plot( svgname, &microenvironment, &cell_container, 0, t, 0);
	std::cout << "File " << svgname << " plotted" << std::endl;

	cell_types.clear();

	for ( auto cl: cell_types )
		delete cl;
}

/* Write properties of the simulation */
void Simul::write_properties(std::ostream& os)
{
	os << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
	os << std::endl;	
	os << "<!-- ------------------------------------------------------- -->" << std::endl;
	os << "<!-- -------------- Simulation properties ------------------- -->" << std::endl;
	os << "<simulation>" << std::endl;
	os << "\t<time_step> " << dt << " </time_step>" << std::endl;
	os << "\t<mechanics_time_step> " << mechanics_dt << " </mechanics_time_step>" << std::endl;
	os << "\t<cell_cycle_time_step> " << cell_cycle_dt << " </cell_cycle_time_step>" << std::endl;
	os << "\t<output_interval> " << t_output_interval << " </output_interval>" << std::endl;
	os << "\t<output_densities> " << t_output_densities << " </output_densities>" << std::endl;
	os << "\t<maximal_time> " << t_max << " </maximal_time>" << std::endl;
	os << "\t<minimum_voxel_size> " << min_voxel_size << " </minimum_voxel_size>" << std::endl;
	os << "\t<mode_cell_cycle> " << mode_cycle << " </mode_cell_cycle>" << std::endl;
	os << "\t<number_of_threads> " << omp_num_threads << " </number_of_threads>" << std::endl;
	os << "\t<number_of_densities> " << ndensities << " </number_of_densities>" << std::endl;
	for ( int i = 0; i < ndensities; i++ )
	{
		os << "\t<density_"+std::to_string(i)+"> " << conc_names[i] << " </density_"+std::to_string(i)+">" << std::endl;
	}
	os << "\t<write_ratio_voxels> " << ratio_write << " </write_ratio_voxels>" << std::endl;
	os << "</simulation>" << std::endl;

	os << std::endl;
	os << "<!-- ------------------------------------------------------- -->" << std::endl;
	os << "<!-- -------------- Initial configuration ------------------- -->" << std::endl;
	os << "<initial_configuration>" << std::endl;
	os << "\t<sphere_radius> " << duct_radius << " </sphere_radius>" << std::endl;
	os << "\t<load_cells_from_file> " << init_file << " </load_cells_from_file>" << std::endl;
	os << "\t<nb_passive_cells> " << npassive << " </nb_passive_cells>" << std::endl;
	os << "\t<time_passive_cells> " << tpassive << " </time_passive_cells>" << std::endl;
	os << "\t<time_add_egf> " << tegf << " </time_add_egf>" << std::endl;
	os << "\t<time_add_tnf> " << ttnf << " </time_add_tnf>" << std::endl;
	os << "\t<duration_add_tnf> " << ttnf_pulse << " </duration_add_tnf>" << std::endl;
	os << "\t<time_remove_tnf> " << tremtnf << " </time_remove_tnf>" << std::endl;
	for ( int i = 0; i < ndensities; i++ )
	{
		os << "\t<"+conc_names[i]+"_concentration> " << concentrations[i] << " </"+conc_names[i]+"_concentration>" << std::endl;
	}
	os << "\t<membrane_shape> " << membrane_shape << " </membrane_shape>" << std::endl;
	os << "\t<membrane_length> " << membrane_rad << " </membrane_length>" << std::endl;
	os << "</initial_configuration>" << std::endl;
	os << std::endl;

}
