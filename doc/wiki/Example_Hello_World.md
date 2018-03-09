<h1>Example: Hello World under TNF treatment </h1>

<div class="textblock"><p>Step-by-step example of running a simulation with PhysiBoSS: growth of a cell population, initially shaped as "Hello World" under TNF injection.</p>
<p>In this example, we showed all the steps required to simulate the growth of a population with customized initial configuration under TNF injection, with the necessary files provided so that it can be reproduced by the user.</p>

[Preparation](#preparation)

[Run the simulation](#run-the-simulation)

[Visualize the results](#visualize-the-results)

[Results](#results)

<h2>Preparation</h2>
<p>First the simulation folder must be prepared. We used a precise organization of the simulation to be able to automatize the creation, running and analyse of numerous simulations. The main folder (called in this example <code>example_hello_world</code>) contains: </p>
<ul>
<li>the <code>BN</code> folder that contains the boolean network configuration files, which must be named <code> bn_conf.cfg</code> and <code>bn_nodes.bnd</code>. More explanation of those network files, and other networks files for other biological questions (cell cyle, drosophilia patterning, EGF-TNF signalling...) are available on <a href="http://maboss.curie.fr">MaBoSS website </a>. In this example, we used the cell fate network. </li>
<li>the <code>runs</code> folders: each run folder contain an individual simulation. This is especially usefull to simulate a similar condition, with only one parameter changing in each run folder, or all the same to repeat it for robutness. In this example, we did two runs (<code>run0, run1</code> folder). <br/>
 A <code>run</code> folder must contain the parameter XML file. It can also contains the initial configuration file, as in this example <code>init.txt</code>. The two output subfolders <code>output</code> and <code>microutput</code> that will be used by <code>PhysiBoSS</code> to write the results files must be present.</li>
</ul>

<p> The file 'init.txt' specifies the complex initial state of the population. It contains the list of the cells with their position within the given 'Hello World' shape. 
Here are how the first lines of this file looks like:
<pre>
"X1";"X2";"X3";"X4";"X5";"X6";"X7";"X8";"X9";"X10";"X11";"X12";"X13";"X14";"X15";"X16"
0;0;21.8638762449091;-252.054799958931;6.67393156934998;8.49326387243345;-1;-1;-1;-1;-1;-1;0;3;0;-1
0;0;178.863876244909;-252.054799958931;10.3745346621146;10.0973893917166;-1;-1;-1;-1;-1;-1;0;3;0;-1
0;0;2.86387624490908;-251.054799958931;0.344690447890004;9.30956707578152;-1;-1;-1;-1;-1;-1;0;3;0;-1
0;0;159.863876244909;-251.054799958931;10.1567348923527;10.0129560484551;-1;-1;-1;-1;-1;-1;0;3;0;-1
0;0;-16.1361237550909;-250.054799958931;-7.55692825976344;9.42130373334512;-1;-1;-1;-1;-1;-1;0;3;0;-1
</pre> 
The first line contains the title of the column, then each line contains the initial configuration of a cell.
Fields that must be initialised are: 
<li> x, y, z coordinates of the cell (3rd, 4th, 5th columns)</li>
<li> initial cell radius (6th column) </li>
<li> the number of the cell line in the parameter file (13th column), -1 for a passive cell </li>
<li> the initial phase of the cell in its cycle (14th column) </li>
<li> the time the cell has already passed in this phase (15th column) </li>
The format of this initial file is dictated by the format of the output files: as such, an already runned simulation can be used as an initial file state.
</p>

<p> Note: to create this file, we used a binary image of 'Hello World', and generated an initial file by placing randomly cells on the positive area on the image. </p> 

<h2>Run the simulation</h2>
<p>To run the simulation, open a Terminal window in the (here <code>run0</code>) folder of the simulation and call <code>PhysiBoSS</code> executable: </p>
<code>path_to_PhysiBoSS/bin/PhysiBoSS -p parameters.xml </code>

<p> During the simulations, summary informations are printed to the terminal (initialisation, number of cells dividing, dying, time spent...). It can be more convenient to redirect it to an other file to supress any output when running multiple simulations at once: </p>
<code>path_to_PhysiBoSS/bin/PhysiBoSS -p parameters.xml &gt; msg.txt </code>

<p>Output files are written in the two output folders, and svg snaphots can be written at given frequency to directly visualize the simulation evolution.</p>

<p> This simulation was runned in about half an hour. </p>

<h2>Visualize the results</h2>
<p>To visualize 3D+t results of the simulations, we used the software <code>Paraview</code> which allows for high flexibility and intereactive viewing options. In the files available for this example, the state file <code>example_hello_world.pvsm</code> contains the setting-up of paraview to draw the output files in 3D view. To use it, open <code>Paraview</code>, click on <code></code>( File -&gt; Load State...) and select the state file <code>example_spheroid_TNF.pvsm</code>. Then a window to choose the files to visualize will pop up, select in the output folder of the simulation the cells files stack. </p>

<h2>Results</h2>
<p>Below are snapshots of the simulation that you should obtain by re-running it and visualizing it with our customized paraview state file: </p>
<img src="https://github.com/gletort/PhysiBoSS/blob/master/doc/imgs/example_hello_world.png" alt="Example results" width="800"/>