<h1>Example: Cells interaction with Extra-Cellular Matrix (ECM).</h1>

<div class="textblock"><p>Step-by-step example of running a simulation with PhysiBoSS: cells interaction with ECM.</p>
<p>In this example, we showed all the steps required to simulate the growth of a population surrounded by ECM that it can degrade, with the necessary files provided so that it can be reproduced by the user. The parameter file provided is commented to explain the main parameters used in this simulation. The initial file configuration was created with the <a class="el" href="PhysiBoSS_CreateInitTxtFile">PhysiBoSS_CreateInitTxtFile</a> executable.</p>

[Preparation](#preparation)

[Run the simulation](#run-the-simulation)

[Visualize the results](#visualize-the-results)

<h2>Preparation</h2>
<p>First the simulation folder must be prepared. We used a precise organization of the simulation to be able to automatize the creation, running and analyse of numerous simulations. The main folder (called in this example <code>example_cells_with_ECM</code>) contains: </p>
<ul>
<li>the <code>BN</code> folder that contains the boolean network configuration files, which must be named <code> bn_conf.cfg</code> and <code>bn_nodes.bnd</code>. More explanation of those network files, and other networks files for other biological questions (cell cyle, drosophilia patterning, EGF-TNF signalling...) are available on <a href="http://maboss.curie.fr">MaBoSS website </a>. In this example, we used the boolean network for metastasis study from <a href="https://github.com/ArnauMontagud/Logical_modelling_pipeline">logical modelling pipeline repository</a>. </li>
<li>the <code>runs</code> folders: each run folder contain an individual simulation. This is especially usefull to simulate a similar condition, with only one parameter changing in each run folder, or all the same to repeat it for robutness. In this example, we just did one run (<code>run0</code> folder). <br/>
 A <code>run</code> folder must contain the parameter XML file. It can also contains the initial configuration file, as in this example <code>init.txt</code>. The two output subfolders <code>output</code> and <code>microutput</code> that will be used by <code>PhysiBoSS</code> to write the results files must be present.<br/>
It also contains the file <code>ecm.txt</code> which defines the repartition of the ECM in the simulated space (here as a smiley shape). The file contains the spatial position of matrix compenent with a local density value. </li>
</ul>
 
<h2>Run the simulation</h2>
<p>To run the simulation, open a Terminal window in the (here <code>run0</code>) folder of the simulation and call <code>PhysiBoSS</code> executable: </p>
<code>path_to_PhysiBoSS/bin/PhysiBoSS -p parameters.xml </code>

<p> During the simulations, summary informations are printed to the terminal (initialisation, number of cells dividing, dying, time spent...). It can be more convenient to redirect it to an other file to supress any output when running multiple simulations at once: </p>
<code>path_to_PhysiBoSS/bin/PhysiBoSS -p parameters.xml &gt; msg.txt </code>

<p>Output files are written in the two output folders, and svg snaphots can be written at given frequency to directly visualize the simulation evolution.</p>

<p> This simulation took around 5 min to run on our cluster. </p>

<h2>Visualize the results</h2>
<p>To visualize 3D+t results of the simulations, we used the software <code>Paraview</code> which allows for high flexibility and intereactive viewing options. In the files available for this example, the state file <code>withECM.pvsm</code> contains the setting-up of paraview to draw the output files in 3D view. To use it, open <code>Paraview</code>, click on <code></code>( File -&gt; Load State...) and select the state file <code>withECM.pvsm</code>. Then a window to choose the files to visualize will pop up, with two rows. In the first row, select the output folder of the simulation the cells files stack. In the second row, select in the <code>microutput</code> folder the ecm files stack. </p>
<p>
Below are snapshots of the results of the simulation. Colors indicates the amount of cell motility magnitude, and in black ECM density is represented. </br>
<img src="https://github.com/gletort/PhysiBoSS/blob/master/doc/imgs/example_cells_with_ECM.png" alt="example results" width="800"/>
</p>