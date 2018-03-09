<h1>PhysiBoSS_Plot exectubale </h1> 

<div class="textblock"><p>Executable to plot a txt file containing cells positions and sizes as SVG file.</p>
<p>This executable is created in the <code>bin</code> folder after compilation. It allows the user to directly plot a given time point state of the simulation (2D slice) without having to use Paraview. Thus it can be easily automatised to different views.</p>
<h1><a class="anchor" id="usage"></a>
Usage</h1>
<p>To plot a given time point of the simulation, call PhysiBoSS_Plot executable from a Terminal window in the main folder of the simulation to plot. The time point to plot (<code>tplot</code>) can be precised as argument of the fonction. It should be an integer, and a time point that has been outputted during the simulation, so <code>cells_tplot.txt</code> file should be present in the <code>output</code> folder.</p>
<h1><a class="anchor" id="ex"></a>
Example</h1>
<p>Command: <br/>
</p>
<code>path_to_physiboss/bin/PhysiBoSS_Plot -t 80 </code>
<p><br/></p>
<p> Which gives the following svg file: <br/></p>

<img src="https://github.com/sysbio-curie/PhysiBoSS/blob/master/doc/imgs/physiboss_plot_example.svg" alt="SVG output example" width="400"/>

<p> <br/> </p>
<dl class="section remark"><dt>Remarks</dt><dd>This a first version, no microenvironment can be plotted. It will be extended and we will make it more customizable in future releases. </dd>
<dd>
It is also restricted to a 2D slice view of the simulation. </dd>
<dd>
In the current version, this executable is usefull to plot an overview of the simulation directly from the command line, and to automatize visualisation. However, in future development it is planned to propose graphical interface and visualisation which will make this tool deprecated.</dd></dl>
<dl class="todo"><dt><b><a class="el">Todo:</a></b></dt><dd><p class="startdd">Modify to add possibility to read microenvironment file as well </p>
<p class="enddd">Makes it more customizable, more parameters (an xml file maybe defining plotting options ?)</p>
</dd></dl>
