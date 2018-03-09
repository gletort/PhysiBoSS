<h1>Example: Cell sorting by differential adhesion </h1>

<div class="textblock"><p>Step-by-step example of running a simulation with PhysiBoSS: spontaneous cell sorting due to differences in cell adhesion strength. </p>
<p>In this example, we showed all the steps required to simulate the mechanical sorting of cell from two different cell lines, with the necessary files provided so that it can be reproduced by the user.</p>

[Preparation](#preparation)

[Run the simulation](#run-the-simulation)

[Visualize the results](#visualize-the-results)

[Results](#results)

<h2>Preparation</h2>
<p> We are simulating here a spheroid of active cells from two cell lines (blue and red), surrounded by extra-cellular-matrix represented as passive spheres (transparent grey). The two cell lines are identical, except that their adhesion strength parameters are different. Here, we ignored cell cycling and no boolean network is needed. </p>

<p>First the simulation folder must be prepared. We used a precise organization of the simulation to be able to automatize the creation, running and analyse of numerous simulations. The main folder (called in this example <code>example_hello_world</code>) contains the <code>run*</code> folders. </p>

<p>
Each run folder contain an individual simulation. This is especially usefull to simulate a similar condition, with only one parameter changing in each run folder, or all the same to repeat it for robutness. In this example, we did two runs (<code>run0, run1</code> folder). In <code>run0</code>, the first cell line doesn't adhere to the matrix (parameters <code>ecm_adhesion_min =0 </code> and <code>ecm_adhesion_max = 0</code>), while in the second folder <code>run1</code>, those values are put to 10, so the cell can attach to the matrix. <br/>

To specify different parameters for each cell line, there are two <code>cell_properties</code> elements in the parameter file and their order in the parameter file defines the corresponding cell line (the first <code> cell_properties</code> element defined the cell line 0, the second one the cell line 1...).<br/>

A <code>run</code> folder must contain the parameter XML file. It can also contains the initial configuration file, as in this example <code>init.txt</code>. It was created with the <code>PhysiBoSS_CreateInitTxtFile</code> executable. The two output subfolders <code>output</code> and <code>microutput</code> that will be used by <code>PhysiBoSS</code> to write the results files must be present.
</p>

<h2>Run the simulation</h2>
<p>To run the simulation, open a Terminal window in the (here <code>run0</code>) folder of the simulation and call <code>PhysiBoSS</code> executable: </p>
<code>path_to_PhysiBoSS/bin/PhysiBoSS -p parameters.xml </code>

<p> During the simulations, summary informations are printed to the terminal (initialisation, number of cells dividing, dying, time spent...). It can be more convenient to redirect it to an other file to supress any output when running multiple simulations at once: </p>
<code>path_to_PhysiBoSS/bin/PhysiBoSS -p parameters.xml &gt; msg.txt </code>

<p>Output files are written in the two output folders, and svg snapshots can be written at given frequency to directly visualize the simulation evolution.</p>

<p> This simulation took less than 1 min to run on our cluster. </p>

<h2>Visualize the results</h2>
<p>To visualize 3D+t results of the simulations, we used the software <code>Paraview</code> which allows for high flexibility and intereactive viewing options. In the files available for this example, the state file <code>example_hello_world.pvsm</code> contains the setting-up of paraview to draw the output files in 3D view. To use it, open <code>Paraview</code>, click on <code></code>( File -&gt; Load State...) and select the state file <code>example_cell_sorting.pvsm</code>. Then a window to choose the files to visualize will pop up, select in the output folder of the simulation the passive cells files stack first, then the active cells stack. </p>

<h2>Results</h2>
<p>Below are snapshots of the simulation that you should obtain by re-running it and visualizing it with our customized paraview state file. Cells of the first cell line (the most adhesive one) are in blue, cells of the second cell line are in red. Transparent grey spheres are the passive spheres. First row of the figure correspond to snapshots of the simulation in <code> run0 </code> folder, so when the blue cell line doesn't adhere to the ECM. In the second row, it is the results of the <code>run1</code> folder, blue cells can adhere to ECM. </p>
<img src="https://github.com/gletort/PhysiBoSS/blob/master/doc/imgs/example_cell_sorting.png" alt="Example results" width="800"/>