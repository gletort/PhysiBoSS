# PhysiBoSS   
Multiscale simulation of multi-cellular system
 
Overview:

 * [Presentation](#presentation)
 * [Documentation](#documentation)
 * [Usage](#usage)
 * [Remarks](#remarks)

## Presentation 
PhysiBoSS (PhysiCell-MaBoSS) is C++ software for multiscale simulation of heterogeneous multi-cellular system. It integrates together cell's internal signalling pathway model (boolean formalism), physical representation of cell (agent-based) and extra-cellular matrix diffusing or fixed entities. 
It is adapted from [PhysiCell](http://physicell.mathcancer.org) sources, with the boolean network computation inside each cell from [MaBoSS](http://maboss.curie.fr) software. 
  
![Hello world image](./doc/imgs/hello.png?raw=true "PhysiBoSS simulation example") 
 
 
## Documentation 
Code-oriented documentation can be generated with Doxygen:
~~~bash
make doc
~~~  
in the main directory. 
It can be configured in the Doxyfile file present in this directory.
It will generate the html documentation in the doc/html directory. 
You can visualize it in a browser, e.g.:
~~~bash
firefox doc/html/index.html &
~~~

You can also refer to (future) publications with PhysiBoSS for scientific applications of this software and description of the models.

Step-by-step examples with the necessary files to run them are also proposed in the 'examples' directory and on the Wiki of this repository.

## Usage
### Compiling PhysiBoSS
PhysiBoSS should run and be easily installed on Linux and MacOS system. We will provide (soon) a Docker version for Windows operating system or if it doesn't run on your machine.
It requires moderatly recent version of C++ (at least c++11) and OpenMP support.

To install it on Linux system, from a Terminal:
Clone the repository on your local machine, and go inside the main directory.
Install and compile  MaBoSS, by going into the MaBoSS directory and type 'make'. Or directly type 'make maboss' in the main PhysiBoSS directory.
Then type 'make' in the main directory to compile everything. The executables will be created in the 'bin' directory if all goes well. 
It can be compiled in 'Debug', 'Release' or 'Proliling' modes, to set in the 'Makefile' file. Default is 'Release' mode (fastest).
You might also have to change your c++ compiler in the Makefile according to your operating system.

Commands list:
~~~bash
git clone https://github.com/gletort/PhysiBoSS.git
cd PhysiBoSS
cd MaBoSS
make
cd ..
make
~~~

### Running one simulation
To run a simulation, you need (at least) a XML parameter file indicating the conditions of the simulation, and the networks file (you can find some on [MaBoSS website](http://maboss.curie.fr)). 
Other options are possible, cf the code-documentation or this repository wiki for more informations.  
 
Example of a parameter file (with only few parameters shown):
~~~xml
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
~~~ 

## Remarks

Please, refer to the Wiki of this repository for a much more extended documentation, with step by step examples instructions. 
 
PhysiCell is developped in [Paul Macklin's lab](http://mathcancer.org). 
MaBoSS and PhysiBoSS are developped in the [Computational Systems Biology of Cancer group](http://sysbio.curie.fr) at Institut Curie (Paris, France). 

We invit you to use PhysiBoSS for you research and give feedbacks to us. Any help in developping it further is more than welcome.
Do not hesitate to contact us for any comments or difficulties in using PhysiBoSS: physiboss@gmail.com.


Wishing you to enjoy using PhysiBoSS, 

PhysiBoSS's team. 
