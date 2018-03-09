<h1>Example: Population composed of 3 mutants under TNF treatment </h1>

<div class="textblock"><p>Step-by-step example of running a simulation with PhysiBoSS: cell population composed of 3 mutants response to TNF continuous injection. </p>
<p>In this example, we showed all the steps required to simulate the response of cell population to TNF injection, with 3 different cell lines that have different mutation in their internal signalling pathways, with the necessary files provided so that it can be reproduced by the user.</p>

[Preparation](#preparation)

[Mutant definition](#mutant-definition)

[Run the simulation](#run-the-simulation)

[Visualize the results](#visualize-the-results)

[Results](#results)

<h2>Preparation</h2>
<p> We are simulating here a spheroid of active cells from three cell lines (blue, white and red). The three cell lines are mechanically identical (in the parameter files, all their <code>cell_properties</code> parameters are the same. It can be easily modified for more complex simulations.</p>

<p>First the simulation folder must be prepared. We used a precise organization of the simulation to be able to automatize the creation, running and analyse of numerous simulations. The main folder (called in this example <code>example_mutants</code>) contains the <code>run*</code> folders and the <code>BN</code> folder. </p>

<p>
Each run folder contain an individual simulation. This is especially usefull to simulate a similar condition, with only one parameter changing in each run folder, or all the same to repeat it for robutness. In this example, we did two similar runs (<code>run0, run1</code> folder). <br/>

A <code>run</code> folder must contain the parameter XML file.
To specify different parameters for each cell line, there are two <code>cell_properties</code> elements in the parameter file and their order in the parameter file defines the corresponding cell line (the first <code> cell_properties</code> element defined the cell line 0, the second one the cell line 1...). Here, we kept the same values for all cell lines. <br/>

It can also contains the initial configuration file, as in this example <code>init.txt</code>. It was created with the <code>PhysiBoSS_CreateInitTxtFile</code> executable, with an option to initialize 3 types of cell with equal probabilities (0.33). The two output subfolders <code>output</code> and <code>microutput</code> that will be used by <code>PhysiBoSS</code> to write the results files must be present.
</p>

<p>
The <code>BN</code> folder contains two files, <code>bn_conf.cfg</code> and <code>bn_nodes.bnd</code> which are MaBoSS configuration files of a boolean network. In this example, the network used is the Cell Fate boolean network used in PhysiBoSS publication.
</p>

<h2>Mutant definition</h2>
<p>
To define a mutant in PhysiBoSS, it must be configured in the MaBoSS input files. In the file, containing the model definitions, the activation and de-activation rates are defined according to other nodes value. A mutant can be introduced with a variable (e.g. <code>$Low_CASP8</code>) defined in the configuration file, of value 0 for no mutation and 1 for active mutation in general. Here is an example of a possibly mutated node:
<pre>
node CASP8
{
  logic = (DISC_TNF | (DISC_FAS | CASP3) ) & (!cFLIP);
  rate_up = ($Low_CASP8 ? 0.0 : ( $High_CASP8 ? 1E+100 : ( @logic ? 1.0: 0.0 )) );
  rate_down = ($Low_CASP8 ? 1E+100 : ( $High_CASP8 ? 0.0 : (@logic ? 0.0 : 1.0 )) );
}
</pre> 
In this example, CASP8 gene can be mutated with either inhibition (<code>$Low_CASP8</code>), meaning that its activation rate is 0 and de-activation rate is very fast, or with over-expression (<code>$High_CASP8</code>) meaning that its activation rate is very fast and de-activation rate is 0. 
In the network files that we provided with this example, 12 different mutations are introduced, but it's straightforward to add other mutation by mimicking the code above.
</p>

<p>
By default, all the mutation parameter value (<code>$Low_CASP8</code>...) are 0 (not mutated). To add a mutation and link it to a specific cell line in PhysiBoSS, it must be specified in the parameter file in the <code>network</code> section: 
<pre> 
&lt;network&gt;
	&lt;network_update_step&gt; 10 &lt;/network_update_step&gt;
        &lt;mutation_0&gt;
		&lt;symbol_name&gt; $Low_CASP8 &lt;/symbol_name&gt;
		&lt;cell_line&gt; 0 &lt;/cell_line&gt;
		&lt;rate&gt; 0.0 &lt;/rate&gt;
	&lt;/mutation_0&gt;
        &lt;mutation_1&gt;
		&lt;symbol_name&gt; $Low_CASP8 &lt;/symbol_name&gt;
		&lt;cell_line&gt; 1 &lt;/cell_line&gt;
		&lt;rate&gt; 0.0 &lt;/rate&gt;
	&lt;/mutation_1&gt;...
&lt;/network&gt;
</pre>
for all cell lines and mutation introduced. 
In this example, we have 3 cell lines, one without mutations (WT, blue); one with inhibited CASP8 (survival favored, white); and one with over-expressed CASP8 (apoptosis favored, red).
</p>

<h2>Run the simulation</h2>
<p>To run the simulation, open a Terminal window in the (here <code>run0</code>) folder of the simulation and call <code>PhysiBoSS</code> executable: </p>
<code>path_to_PhysiBoSS/bin/PhysiBoSS -p parameters.xml </code>

<p> During the simulations, summary informations are printed to the terminal (initialisation, number of cells dividing, dying, time spent...). It can be more convenient to redirect it to an other file to supress any output when running multiple simulations at once: </p>
<code>path_to_PhysiBoSS/bin/PhysiBoSS -p parameters.xml &gt; msg.txt </code>

<p>Output files are written in the two output folders, and svg snapshots can be written at given frequency to directly visualize the simulation evolution.</p>

<p> This simulation was runned in less than 10 min. </p>

<h2>Visualize the results</h2>
<p>To visualize 3D+t results of the simulations, we used the software <code>Paraview</code> which allows for high flexibility and intereactive viewing options. In the files available for this example, the state file <code>example_mutants.pvsm</code> contains the setting-up of paraview to draw the output files in 3D view. To use it, open <code>Paraview</code>, click on <code></code>( File -&gt; Load State...) and select the state file <code>example_cell_sorting.pvsm</code>. Then a window to choose the files to visualize will pop up, select in the output folder of the simulation the active cell file stack. </p>

<h2>Results</h2>
<p>Below are snapshots of the simulation that you should obtain by re-running it and visualizing it with our customized paraview state file. Cells of the first cell line (WT) are in blue, cells of the second cell line (CASP8 inhibited) are in white, cells of the third cell line (CASP8 over-expressed) are in red. 
As expected, red cells are comitted to apoptosis under TNF treatment and disappear fast, while white cells proliferate and some WT cells (blue) resisted. </p>
<img src="https://github.com/gletort/PhysiBoSS/blob/master/doc/imgs/example_mutants.png" alt="Example results" width="800"/>