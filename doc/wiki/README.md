<h1><a class="anchor" id="sec1"></a>
Presentation</h1>
<p><code>PhysiBoSS</code> (from the merging of PhysiCell and MaBoSS) is an adapted version of <code>PhysiCell</code> (see <a href="http://physicell.mathcancer.org">physicell.mathcancer.org </a>) to integrate in it Boolean network computation inside each cell. <code>PhysiCell</code> is developped in Paul Macklin's lab (<a href="mathcancer.org">mathcancer.org</a>). <code>MaBoSS</code> and <code>PhysiBoSS</code> are developped in the Computational Systems Biology of Cancer group (<a href="http://sysbio.curie.fr">sysbio.curie.fr</a>) <br/>
 Code is written in <code>C++</code> and open-source. In <code>PhysiCell</code>, there are two main part: <code><a class="el" href="namespace_bio_f_v_m.html">BioFVM</a></code> part which handle the diffusion/production of molecules (e.g. oxygen) and the <code>PhysiCell</code> part that handle the cells' physics. A cell can have a "classic" implementation of the cell cycle (adapted from <code>PhysiCell1.0</code> initial implementation), or a cell cycle progression mode determined by a Boolean network assigned to each individual cell by <code>MaBoSS</code>. Interestingly, this framework could be easily adapted to other networks or implementations.</p>
<p><br/>
</p>

![](https://github.com/gletort/PhysiBoSS/blob/master/doc/imgs/hello.png "PhysiCell-MaBoSS example")

<p> <br/>

3 executables are proposed in the PhysiBoSS release: <code>PhysiBoSS</code> which handles the actual simulation, <code>PhysiBoSS_CreateInitTxtFile</code> which offers the possibility to automatically generate an initial state of the simulation file and <code>PhysiBoSS_Plot</code> which allows the user to create an svg file showing a snapshot of the simulation at a specific time point.</p>

<p> Overview:

[How to use](#how-to-use):

 * [Compiling PhysiBoSS](#compiling-physiboss)
 * [Running one simulation](#running-one-simulation)
 * [Create initial file](#create-initial-file)
 * [Plot a given time point](#plot-a-given-time-point)
 * [Visualization](#visualization)

[Examples](#examples)

[Future development](#future-development)

[Availability/License](#availabilitylicense)

[References/Links](#referenceslinks)

</p>

<h1><a class="anchor" id="sec2"></a>
How to use</h1>
<h2><a class="anchor" id="sub"></a>
Compiling PhysiBoSS</h2>
<p> PhysiBoSS should be compatible with most Linux and MacOS systems that have a C++ compiler with OpenMP support. For Windows operating system or others non compatible systems, a Docker version is available <a class="el" href="https://hub.docker.com/r/gletort/physiboss/">here</a>.</p>
<p>First, clone the PhysiBoSS repository, and compile it by going into the <code>PhysiBoSS</code> directory and type <code>make install</code>. The executables will be created in the <code>bin</code> directory if all goes well. It requires a recent version of c++, at least c++11. <code>PhysiBoSS</code> can be compiled in Debug, Release or Proliling modes, to set in the <code>Makefile</code> file, default is Release mode which is the fastest one.</p>
<p> Summary of the steps to download and install PhysiBoSS on Linux system:
<pre>
git clone https://github.com/gletort/PhysiBoSS.git
cd PhysiBoSS
make install
</pre>
If the compilation fails, refer to this documentation <a class="el" href="Installation">page </a> for more explanations.
</p>

<p>To generate/update code-specific documentation, type 'make doc' in the main directory. The documentation is generated with <code>doxygen</code>, which can be configured in the <code>Doxyfile</code> file present in this directory.
It will generate the documentation files in the 'doc/html' folder (initially empty in the repository). You can visualize it in any browser by opening the 'doc/html/index.html' file. 
</p>

<h2><a class="anchor" id="ub0"></a>
Running one simulation</h2>
<p>An 'output' and 'microutput' directories should be present in the main directory of the simulation from which the executable will be called. Ideal structure would be to have a simulation directory containing the parameter file (e.g. <code>parameter.xml</code>), the initial configuration file (optional, e.g. <code>init.txt</code>) and the outputs folder. Informations files will be created in this directory (<code>report.txt</code>). Information on how the simulation is running (initialisation completed or not, current time, wall time) are printed to the terminal, we redirected them to the file <code>msg.txt</code> when running them using an automated script. <code>Report.txt</code> gives a quick summary of the simulation, with the number of cells that divided or died in between output times. In the folder 'output', txt files named 'cells_' followed by the time value are generated during the simulation, containing the current cells states (position/size/cycle state).</p>

![](https://github.com/gletort/PhysiBoSS/blob/master/doc/imgs/simulation.png "Simulation organization")

 <h3><a class="anchor" id="sub0"></a>
Input parameter file</h3>
<p>XML format is used for input parameter file, as proposed in the MultiCellDS standardization initiative (see <a href="http://multicellds.org">here </a>). To parse the xml files, we integrated in our code the Tinyxml2 files, developed by Lee Thomason (<a href="http://www.grinninglizard.com">website </a>) and freely available. In the current version, we have 4 main parts: a set of parameters defining the simulation features (time steps, spatial limits..), a set of parameters defining the cell properties (one for each cell strain to be simulated), the definition of the network parameters and a set of parameters to define the initial configuration (either a geometry specification or input files names).</p>
<p>Example of a parameter file (with only few parameters shown): </p>
<pre class="fragment">&lt;?xml version="1.0" encoding="UTF-8" ?&gt;

&lt;simulation&gt;
        &lt;time_step&gt; 0.2 &lt;/time_step&gt;
        &lt;mechanics_time_step&gt; 0.1 &lt;/mechanics_time_step&gt;
        ....
&lt;/simulation&gt;

&lt;cell_properties&gt;
        &lt;mode_motility&gt; 1 &lt;/mode_motility&gt;
        &lt;polarity_coefficient&gt; 0.5 &lt;/polarity_coefficient&gt;
        ...
&lt;/cell_properties&gt;

&lt;network&gt;
        &lt;network_update_step&gt; 10 &lt;/network_update_step&gt;
        ...
&lt;/network&gt;

&lt;initial_configuration&gt;
        &lt;load_cells_from_file&gt; init.txt &lt;/load_cells_from_file&gt;
        ...
&lt;/initial_configuration&gt;
</pre><p> Complete examples of parameter files are given with the release of PhysiBoSS.</p>
<h2><a class="anchor" id="sub1"></a>
Create initial file</h2>
<p>Simulation can be loaded from a given initial file containing the position of all initial cells, their state, size... With <code>PhysiBoSS</code> code, an executable <code>PhysiBoSS_CreateInitTxtFile</code> is given, which allows the user to create a .txt file containing those informations for given parameters and chosen modes (e.g. a sphere of radius 300 µm). See the page <a class="el" href="PhysiBoSS_CreateInitTxtFile">PhysiBoSS_CreateInitTxtFile</a> for more informations on what it can do and how to use it.</p>
<p>It's also possible to create a custom initial file from a binary image like it was done to generate the initial state of the example image above. <br/>
For this, we can use use an output of a previous simulation as the input initial file or build our own using semi-colon separated (';') columns such as: Time;ID;x;y;z;radius;volumetotal;volumenuclearsolid, etc. 
Additionally, we have made an R script that generates such a file from the list of positive pixels in a binary image (that can be generated with ImageJ) <a class="el" href="https://github.com/gletort/PhysiBoSS/blob/master/scripts/presimu/prepareInitialFileFromBinary.R/">available here</a>. </p>
<h2><a class="anchor" id="sub2"></a>
Plot a given time point</h2>
<p>The executable <code>PhysiBoSS_Plot</code> given with the release allows the user to plot a specific time point of the simulation after it has run, directly from the command line. The time point must have been saved in the 'output' directory, and the plot will be a .svg file. Several options can be specified to <code>PhysiBoSS_Plot</code>, as can be seen <a class="el" href="PhysiBoSS_Plot">here</a></p>
<h2><a class="anchor" id="para"></a>
Visualization</h2>
<p>To visualize time evolution of 3D simulation results, we used the software <code>Paraview</code> which allows high flexibility and interactive viewing options. We provide within <code>PhysiBoSS</code> release several paraview state files to directly set-up the visualization of <code>PhysiBoSS</code> output files on <code>Paraview</code>. See the image below for an example of how a typical <code>PhysiBoSS</code> result looks in <code>Paraview</code> with our state files:</p>

![](https://github.com/gletort/PhysiBoSS/blob/master/doc/imgs/paraview.png "Visualization with Paraview")

<p>
If you have issues using our state files or if you want to modify the initial set-up, the steps to visualize the output files as spheres in Paraview are explained <a class="el" href="Paraviewing">here</a>
</p> 
<h1>Examples</h1>
<ul>
<li>Step by step simulation of a sphere of 100 µm radius of active cells, under TNF injection: <a class="el" href="Example_Spheroid_TNF">see here</a>.</li>
<li>Step by step simulation of a cell population with initial 'Hello World' shape, under TNF injection: <a class="el" href="Example_Hello_World">see here</a>.</li>
<li>Step by step simulation of cell sorting by differential adhesion, <a class="el" href="Example_Cell_Sorting">see here</a>.</li>
<li>Step by step simulation of a cell population composed of 2 different strains <a class="el" href="Example_Mutants_paper">see here</a>.</li>
<li>Step by step simulation of a cell population composed of 3 different strains <a class="el" href="Example_Mutants">see here</a>.</li>
<li>Step by step simulation of a cell population surrounded by degradable ECM, constraining the growth of the population <a class="el" href="Example_Cells_With_ECM">see here</a>.</li>
</ul>

<h1><a class="anchor" id="sce"></a>
Future development</h1>
<ul>
<li>Extension of the possible inputs/outputs between PhysiCell and MaBoSS network. </li>
<li>Improvement of PhysiBoSS outputs: format, visualization, analysis </li>
<li>Development of a detailed fiber-shaped extra-cellular matrix representation. </li>
<li>Implementation of graphical interface.</li>
</ul>
<h1><a class="anchor" id="lic"></a>
Availability/License</h1>
<p>PhysiBoSS is totally open-source, in the hope that it will be used by other teams and improved and developped by other users. If you are interested in working with/on it, please don't hesitate to contact us.</p>
<p>All source code is freely available and can be modified by third-parts. Please, cite the original publications of PhysiBoSS and PhysiCell if you use it. Refer to the licenses in 'Licenses' folder of this repository for more informations.

<h1>References/Links</h1>
<p>For PhysiCell: </p>
<ul>
<li><a href="http://physicell.mathcancer.org">Paul Macklin's lab website </a> </li>
<li>PhysiCell publication: A. Ghaffarizadeh, S.H. Friedman, S.M. Mumenthaler, and P. Macklin, PhysiCell: an Open Source Physics-Based <a class="el" href="class_cell.html" title="Dynamic (alive) cell (move, interact, divide, die...) ">Cell</a> Simulator for 3-D Multicellular Systems, bioRxiv 088773, 2016. DOI: 10.1101/088773. </li>
<li><a href="http://biofvm.mathcancer.org">BioFVM website </a> </li>
<li><a class="el" href="namespace_bio_f_v_m.html">BioFVM</a> publication: A. Ghaffarizadeh, S.H. Friedman, and P. Macklin. <a class="el" href="namespace_bio_f_v_m.html">BioFVM</a>: an efficient, parallelized diffusive transport solver for 3-D biological simulations. Bioinformatics, 2015. <br/>
 For MaBoSS: </li>
<li><a href="http://maboss.curie.fr">MaBoSS website </a> </li>
<li>MaBoSS publication: Stoll G, Viara E, Barillot E, Calzone L. Continuous time Boolean modeling for biological signaling: application of Gillespie algorithm. BMC Syst Biol. 2012 Aug 29;6:116. doi: 10.1186/1752-0509-6-116. </li>
<li><a href="http://sysbio.curie.fr">Our team </a> <br/>
 <br/>
 <dl class="section author"><dt>Author</dt><dd>Contact for comments/questions: <br/>
 Gaelle Letort, Institut Curie, U900. <br/>
 physiboss@gmail.com </dd></dl>
