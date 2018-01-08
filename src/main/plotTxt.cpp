
#ifndef _PLOTTXT_
#define _PLOTTXT_

#include <iostream>
#include <exception>
#include <fstream>

#include <stdlib.h>
#include <getopt.h>

namespace BioFVM
{
	extern std::string PhysiCell_Version; 
	extern std::string PhysiCell_URL; 
};

#include "../core/simul.h"

/* Flag set by ‘--verbose’. */
static int verbose_flag;

/* Time point to plot */
double tplot;


void help( std::ostream & os = std::cout )
{
	os << "-----------------------------------------------------" << std::endl;
	os << "----------------- Help Menu -------------------------" << std::endl;
	os << std::endl;
	os << "Command line options:" << std::endl;
	os << "  --help (-h)            print this message" << std::endl;
	os << "  --time (-t)       time point to plot - Default is 0" << std::endl;
	os << std::endl;
	os << "-----------------------------------------------------" << std::endl;
}

int read_commandline( int argc, char **argv ) throw()
{
	int c;

	while (1)
	{
		static struct option long_options[] =
		{
			/* These options set a flag. */
			{"verbose", no_argument,       &verbose_flag, 1},
			{"brief",   no_argument,       &verbose_flag, 0},
			/* These options don’t set a flag.
			 *              We distinguish them by their indices. */
			{"help",     no_argument,       0, 'h'},
			{"time",  required_argument,    0, 't'}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "ht:", long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
			case 0:
				/* If this option set a flag, do nothing else now. */
				if (long_options[option_index].flag != 0)
					break;
				printf ("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
				printf ("\n");
				break;

			case 'h':
				help();
				return 0;
				break;

			case 't':
				tplot = std::stod(optarg);
				printf ("option -t (time point) `%s'\n", optarg);
				break;

			case '?':
				/* getopt_long already printed an error message. */
				break;

			default:
				abort ();
		}
	}

	/* Instead of reporting ‘--verbose’
	 *      and ‘--brief’ as they are encountered,
	 *           we report the final status resulting from them. */
	if (verbose_flag)
		puts ("verbose flag is set");

	/* Print any remaining command line arguments (not options). */
	if (optind < argc)
	{
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		putchar ('\n');
	}
	return 1;
}

//-----------------------------------------------
// ===============  MAIN  =======================                       
//-----------------------------------------------

/** \page plot PhysiBoSS_Plot exectubale 
 *
 * \brief Executable to plot a txt file containing cells positions and sizes as SVG file.
 *
 * This executable is created in the \p bin folder after compilation.
 * It allows the user to directly plot a given time point state of the simulation (2D slice) without having to use Paraview. Thus it can be easily automatised to different views. 
 *
 *\section usage Usage
 * To plot a given time point of the simulation, call PhysiBoSS_Plot executable from a Terminal window in the main folder of the simulation to plot.
 * The time point to plot (\p tplot) can be precised as argument of the fonction. It should be an integer, and a time point that has been outputted during the simulation, so \p cells_tplot.txt file should be present in the \p output folder.
 *
 * \section ex Example
 * Command: \n
 * \code path_to_physiboss/bin/PhysiBoSS_Plot -t 80 \endcode
 *	Which gives the following svg file: \n
 *	\htmlonly <style>div.image img[src="svg_t00080.svg"]{width:400px;}</style> \endhtmlonly 
 *	\image html svg_t00080.svg "SVG output example"
 *
 * \n
 * \remark
 *  This a first version, no microenvironment can be plotted. It will be extended and we will make it more customizable in future releases.
 * \remark
 * It is also restricted to a 2D slice view of the simulation.
 * \remark 
 * In the current version, this executable is usefull to plot an overview of the simulation directly from the command line, and to automatize visualisation. However, in future development it is planned to propose graphical interface and visualisation which will make this tool deprecated.
 *
 * \todo Modify to add possibility to read microenvironment file as well
 * \todo Makes it more customizable, more parameters (an xml file maybe defining plotting options ?)
 *
 *
 * \author G. Letort, Institute Curie 
 * \date June 2017
 * */
int main( int argc, char* argv[] ) 
{
	// time to plot
    tplot = 0;

	try
	{
		// parse command line arguments
		int read = read_commandline( argc, argv );
		if ( !read )
			return EXIT_FAILURE;
	
		// do the plotting
        std::string input_file;
        input_file.resize(1024);
        sprintf( (char*) input_file.c_str(), "output/cells_%05d.txt", (int) round(tplot) );
		Simul simu;
		simu.plotFile( tplot, input_file );
	}
	catch( std::exception& e )
	{
		std::cerr << "\n !!!!!! Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Program end, bye" << std::endl;
	return EXIT_SUCCESS;
}

#endif
