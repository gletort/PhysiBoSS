/** \page PhysiCell PhysiCell License
 *
 If you use PhysiCell in your project, please cite PhysiCell and the ver-  
 sion number, such as below:                                               
                                                                           
 We implemented and solved the model using PhysiCell (Version 1.0.0) [1].  
                                                                          

 [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          
     PhysiCell: an Open Source Physics-Based Cell Simulator for            
     Multicellular Systems, 2016 (in preparation).                         


Because PhysiCell extensively uses BioFVM, we suggest you also cite       
     BioFVM as below:                                                     


 We implemented and solved the model using PhysiCell (Version 1.0.0) [1],  
 with BioFVM [2] to solve the transport equations.                         


 [1] A Ghaffarizadeh, SH Friedman, SM Mumenthaler, and P Macklin,          
     PhysiCell: an Open Source Physics-Based Cell Simulator for            
     Multicellular Systems, 2016 (in preparation).                         


 [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient     
    parallelized diffusive transport solver for 3-D biological simulations,
    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 
                                                                           

                                                                           
 BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   


 Copyright (c) 2015-2016, Paul Macklin and the PhysiCell Project           
 All rights reserved.                                                      


 Redistribution and use in source and binary forms, with or without        
 modification, are permitted provided that the following conditions are    
 met:                                                                      


 1. Redistributions of source code must retain the above copyright notice, 
 this list of conditions and the following disclaimer.                     
                                                                           
 2. Redistributions in binary form must reproduce the above copyright      
 notice, this list of conditions and the following disclaimer in the       
 documentation and/or other materials provided with the distribution.      
                                                                           
 3. Neither the name of the copyright holder nor the names of its          
 contributors may be used to endorse or promote products derived from this 
 software without specific prior written permission.                       


 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
 TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER 
 OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              
                                                                           
*/

/** \page PhysiBoSSLicense PhysiBoSS License
 *
 If you use PhysiBoSS in your project, please cite PhysiBoSS (publication in preparation) and PhysiCell (see \ref PhysiCell "PhysiCell License"). 
 Precise the version of PhysiBoSS used for compatibility and results reproducibility.                                               
                                                                           
                                                                           
 BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   

 Copyright (c) 2017 Gaelle Letort           
 All rights reserved.                                                      

 Redistribution and use in source and binary forms, with or without        
 modification, are permitted provided that the following conditions are    
 met:                                                                      


 1. Redistributions of source code must retain the above copyright notice, 
 this list of conditions and the following disclaimer.                     
                                                                           
 2. Redistributions in binary form must reproduce the above copyright      
 notice, this list of conditions and the following disclaimer in the       
 documentation and/or other materials provided with the distribution.      
                                                                           
 3. Neither the name of the copyright holder nor the names of its          
 contributors may be used to endorse or promote products derived from this 
 software without specific prior written permission.                       


 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
 TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER 
 OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              
                                                                           
*/

/** \page spheroidEx PhysiBoSS Spheroid simulation example
 *
 * \brief Step-by-step example of running a simulation with PhysiBoSS: growth of a spheroid under TNF injection
 *
 * In this example, we showed all the steps required to simulate the growth of a spheroid under TNF injection, with the necessary files provided so that it can be reproduced by the user.
 * The parameter file provided is commented to explain the main parameters used in this simulation. The initial file configuration was created with the \ref init "PhysiBoSS_CreateInitTxtFile" executable.
 *
 * \section prep Preparation
 *
 * First the simulation folder must be prepared. 
 * We used a precise organization of the simulation to be able to automatize the creation, running and analyse of numerous simulations.
 * The main folder (called in this example \p example_spheroid_TNF) contains:
 * \li the \p BN folder that contains the boolean network configuration files, which must be \p named bn_conf.cfg and \p bn_nodes.bnd.
 * More explanation of those network files, and other networks files for other biological questions (cell cyle, drosophilia patterning, EGF-TNF signalling...) are available on <a href="http://maboss.curie.fr"> MaBoSS website </a>.
 * \li the \p runs folders: each run folder contain an individual simulation. This is especially usefull to simulate a similar condition, with only one parameter changing in each run folder, or all the same to repeat it for robutness. 
 * In this example, we just did one run (\p run0 folder).
 * \n
 * A \p run folder must contain the parameter XML file. It can also contains the initial configuration file, as in this example \p init.txt. 
 * The two output subfolders \p output and \p microutput that will be used by \p PhysiBoSS to write the results files must be present.
 *
 * Below is the image of the folder organization for this example provided with this distribution:
 *	\htmlonly <style>div.image img[src="folders.png"]{width:550px;}</style> \endhtmlonly 
 *	\image html folders.png "Input simulation folder and files organization"
 *	\image latex folders.png "Input simulation folder and files organization" width=10cm
 *
 * \section run Run the simulation
 * To run the simulation, open a Terminal window in the (here \p run0) folder of the simulation and call \p PhysiBoSS executable:
 * \code path_to_PhysiBoSS/bin/PhysiBoSS -p parameters.xml \endcode
 * During the simulations, summary informations are printed to the terminal (initialisation, number of cells dividing, dying, time spent...). 
 * It can be more convenient to redirect it to an other file to supress any output when running multiple simulations at once:
 * \code path_to_PhysiBoSS/bin/PhysiBoSS -p parameters.xml > msg.txt \endcode
 *
 * Output files are written in the two output folders, and svg snaphots can be written at given frequency to directly visualize the simulation evolution.
*
* \section see Visualize the results
 * To visualize 3D+t results of the simulations, we used the software \p Paraview which allows for high flexibility and intereactive viewing options. 
 * In the files available for this example, the state file \p example_spheroid_TNF.pvsm contains the setting-up of paraview to draw the output files in 3D view.
 * To use it, open \p Paraview, click on \p( File -> Load State...) and select the state file \p example_spheroid_TNF.pvsm. Then a window to choose the files to visualize will pop up, select in the output folder of the simulation the cells files stack.
 * See the image below for an example of how using \p Paraview look with our state file proposed on this example:
 *
 *	\htmlonly <style>div.image img[src="para.png"]{width:550px;}</style> \endhtmlonly 
 *	\image html para.png "Visualization using Paraview of the simulation results"
 *	\image latex para.png "Visualization using Paraview of the simulation results" width=12cm
 *
 *	\author Gaelle Letort, Institut Curie
 *	\date December 2017
 */

/** \mainpage PhysiBoSS (PhysiCell-MaBoSS) index 
 *
 * \section sec1 Presentation 
 *
 * \p PhysiBoSS (PhysiCell-MaBoSS) is adapted version of \p PhysiCell (see <a href="http://physicell.mathcancer.org"> physicell.mathcancer.org </a>) to integrate in it boolean network computation inside each cell. 
 * \p PhysiCell is developped in Paul Macklin's lab (<a href="mathcancer.org">mathcancer.org</a>). 
 * \p MaBoSS and \p PhysiBoSS are developped in the Computational Systems Biology of Cancer group (<a href="http://sysbio.curie.fr"> sysbio.curie.fr</a>) 
 * \n
 * Code is written in \p C++ and open-source.
 * In \p PhysiCell, there are two main part: \p BioFVM part which handle the diffusion/production of molecules (e.g. oxygen) and the \p PhysiCell part that handle the cells themselves.
 * A cell can have a "classic" implementation of the cell cycle (adapted from \p PhysiCell1.0 initial implementation), or a cell cycling mode determined by a boolean network assigned to each individual cell (using \p MaBoSS). Could be easily adapted to other network or implementation. 
 *
 * \n 
 * \image html hello.png "PhysiCell-MaBoSS example"
 * \n
 *
 * 3 executables are proposed in the PhysiBoSS release: \p PhysiBoSS which handles the actual simulation, \p PhysiBoSS_CreateInitTxtFile which offers the possibility to automatically generate an initial state of the simulation file and \p PhysiBoSS_Plot which allows the user to create an svg file showing a snapshot of the simulation at a specific time point.
 *
 * \section sec2 How to use
 *
 * \subsection sub Compiling PhysiCell-MaBoSS
 * First, install and compile \p MaBoSS, by going into the \p MaBoSS directory and type 'make'. Or directly type 'make maboss' in the main \p PhysiCell-MaBoSS directory.
 * Then type 'make' in the main directory to compile everything. The executables will be created in the 'bin' directory if all goes well. 
 * Should hopefully work in all operating system, tested only on Linux CentOS 7.3. 
 * It requires a recent version of c++, at least c++11.
 * Can be compiled in Debug, Release or Proliling modes, to set in the \p Makefile file, default is Release mode which is the fastest mode.
 * 
 * To generate/update this documentation, type 'make doc' in the main directory. The documentation is generated with \p doxygen, it can be configured in the \p Doxyfile file present in this directory.
 *
 * \subsection ub0 Running one simulation
 *
 * An 'output' and 'microutput' directories should be present in the main directory of the simulation from which the executable will be called. 
 * Ideal structure would be to have a simulation directory containing the parameter file (e.g. \p parameter.xml), the initial configuration file (optional, e.g. \p init.txt) and the outputs folder. Informations files will be created in this directory (\p report.txt). 
 * Information on how the simulation is running (initialisation completed or not, current time, wall time) are printed to the screen, we redirected them to the file \p msg.txt when running them through automated script.
 * \p report.txt gives a quick summary of the simulation, with the number of cells that divided or die in between output times.
 * In the folder 'output', txt files named 'cells_' followed by the time value are written during the simulation, containing the current cells states (position/size/cycle state).
 *
 *	\htmlonly <style>div.image img[src="simulation.png"]{width:550px;}</style> \endhtmlonly 
 *	\image html simulation.png "Simulation folder and files organization"
 *
 * \subsubsection sub0 Input parameter file
 *
 * XML format is used for input parameter file, as proposed in the MultiCellDS standardization initiative (see <a href="http://multicellds.org"> here </a>). 
 * To parse the xml files, we integrated in our code the Tinyxml2 files, developed by Lee Thomason (<a href="http://www.grinninglizard.com"> website </a>) and freely available.
 * In the current version, we have 4 main parts: a set of parameters defining the simulation features (time steps, spatial limits..), a set of parameters defining the cell properties (one for each cell line to simulate), the definition of the network parameters and a set of parameters to define the initial configuration (either a geometry specification or input files names).
 *
 * Example of a parameter file (with only few parameters shown):
 * \verbatim
  <?xml version="1.0" encoding="UTF-8" ?>
 
  <simulation>
 		<time_step> 0.2 </time_step>
 		<mechanics_time_step> 0.1 </mechanics_time_step>
 		....
  </simulation>
 
  <cell_properties>
 		<mode_motility> 1 </mode_motility>
 		<polarity_coefficient> 0.5 </polarity_coefficient>
 		...
  </cell_properties>
 
  <network>
 		<network_update_step> 10 </network_update_step>
 		...
  </network>
 
  <initial_configuration>
 		<load_cells_from_file> init.txt </load_cells_from_file>
 		...
  </initial_configuration>
  \endverbatim
 * Complete examples of parameter files are given with the release of PhysiBoSS.
 *
 * \subsection sub1 Create initial file
 *
 * Simulation can be loaded from a given initial file containing the position of all initial cells, their state, size... 
 * With \p PhysiBoSS code, an executable \p PhysiBoSS_CreateInitTxtFile is given, which allows the user to create a .txt file containing those informations for given parameters and chosen modes (e.g. a sphere of radius 300 µm). 
 * See the page \ref init "PhysiBoSS_CreateInitTxtFile" for more informations on what it can do and how to use it.
 *
 * It's also possible to write ones own file from a binary image like it was done to generate the initial state of the example image above.
 * \n Format of the file should be a semi-colon separated columns file (';'), similar to what a simulation with output. 
 * Then it is possible to use an output of a previous simulation as the input initial file.
 * Columns of the file are Time;ID;x;y;z;radius;volumetotal;volumenuclearsolid....
 *
 * \subsection sub2 Plot a given time point
 *
 * The executable \p PhysiBoSS_Plot given with the release allows the user to plot, directly from the command line, a specific time point of the simulation after it was runned. The time point must have been saved in 'output' directory, and will be plot as a .svg file. Options can be specified. 
 * See more informations about \p PhysiBoSS_Plot and how to use it \ref plot "here"
 *
 * \subsection para Visualization
 *
 * To visualize 3D+t results of the simulations, we used the software \p Paraview which allows for high flexibility and intereactive viewing options. 
 * We proposed within \p PhysiBoSS release paraview state files to directly set-up the visualization from \p PhysiBoSS's output files to \p Paraview.
 * See the image below for an example of how using \p Paraview look with our state file proposed:
 *
 *	\htmlonly <style>div.image img[src="paraview.png"]{width:550px;}</style> \endhtmlonly 
 *	\image html paraview.png "Example of visualization using Paraview and our state file sphere_quarter.pvsm"
 *
 * \section sec Examples
 *
 * \li Step by step simulation of a sphere of 100 µm of radius of active cells, under TNF injection: \ref spheroidEx "see here".
 *
 * \section sce Future development
 *
 * \li Extension of the possible inputs/outputs between PhysiCell and MaBoSS network.
 * \li Improvement of PhysiBoSS outputs: format, visualization, analysis
 * \li Development of the extra-cellular matrix representation.
 * \li Implementation of graphical interface.
 *
 *
 * \section lic Availability/License
 *
 * PhysiBoSS is totally open-source, in the hope that it will be used by other team and improved or developped by other users. 
 * If you are interested in working with/on it, please don't hesitate to contact us.
 *
 * All source code is freely available and can be modified by third-parts. Only requirement is for the original publication of PhysiBoSS and PhysiCell to be cited, see \ref PhysiCell "PhysiCell License" and \ref PhysiBoSSLicense "PhysiBoSS License" pages for more information.
 *
 * \section ref References/Links
 * For PhysiCell: 
 * \li <a href="http://physicell.mathcancer.org"> Paul Macklin's lab website </a>
 * \li PhysiCell publication: A. Ghaffarizadeh, S.H. Friedman, S.M. Mumenthaler, and P. Macklin, PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems, bioRxiv 088773, 2016. DOI: 10.1101/088773.
 * \li <a href="http://biofvm.mathcancer.org"> BioFVM website </a>
 * \li BioFVM publication:  A. Ghaffarizadeh, S.H. Friedman, and P. Macklin. BioFVM: an efficient, parallelized diffusive transport solver for 3-D biological simulations. Bioinformatics, 2015. 
 * \n
 * For MaBoSS:
 * \li <a href="http://maboss.curie.fr"> MaBoSS website </a>
 * \li MaBoSS publication: Stoll G, Viara E, Barillot E, Calzone L. Continuous time Boolean modeling for biological signaling: application of Gillespie algorithm. BMC Syst Biol. 2012 Aug 29;6:116. doi: 10.1186/1752-0509-6-116. 
 * \li <a href="http://sysbio.curie.fr"> Our team </a> 
 * \n 
 * \n
 * \author Contact for comments/questions:
 * \n Gaelle Letort, Institut Curie, u900.
 * \n gaelle.letort_at_curie.fr
 * */

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

/** Name of input parameter file */
std::string parameter_file;

void help( std::ostream & os = std::cout )
{
	os << "-----------------------------------------------------" << std::endl;
	os << "----------------- Help Menu -------------------------" << std::endl;
	os << std::endl;
	os << "Command line options:" << std::endl;
	os << "  --help (-h)            print this message" << std::endl;
	os << "  --parameter (-p)       path to input parameter XML file to read \n Default is ./parameters.xml" << std::endl;
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
			{"parameter",  required_argument,    0, 'p'}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "hp:", long_options, &option_index);

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

			case 'p':
				parameter_file = optarg;
				printf ("option -p (parameter file name with value) `%s'\n", optarg);
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

/** \brief Adapted as a main executable
 *
 * Do the simulation: read parameter file and run it.
 * Create output message files in current directory and output files in the 'output' folder.
 *
 * \warning need the folder in which simulation is runned to contain a folder 'output' and 'microutput'
 * \todo automatically create necessary folders if not presents
 * \date 20 March 2017 
 *  */
int main( int argc, char* argv[] ) 
{
	parameter_file = "parameters.xml"; // default parameter file name
	Simul simu;
	std::cout << "********************************************" << std::endl;
	std::cout << "**********      PhysiBoSS     **************" << std::endl;
	std::cout << "********************************************" << std::endl;

	try
	{
		// parse command line arguments
		int read = read_commandline( argc, argv );
		if ( !read )
			return EXIT_FAILURE;

		// set the parameters read from the command line
		simu.set_parameter_file( parameter_file );
		
		simu.run();
	}
	catch( std::exception& e )
	{
		std::cerr << "\n !!!!!! Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "********************************************" << std::endl;
	std::cout << "*********    Program end, bye    ***********" << std::endl;
	std::cout << "********************************************" << std::endl;
	return EXIT_SUCCESS;
}
