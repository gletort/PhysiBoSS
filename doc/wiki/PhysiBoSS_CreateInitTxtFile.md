<h1>PhysiBoSS_CreateInitTxtFile executable </h1>


<div class="textblock"><p>Executable to create a txt file containing cells positions and sizes, ready to be used as initial condition of a simulation.</p>
<p>This executable is created in the <code>bin</code> folder after compilation. It allows the user to directly create an initial configuration file correctly formatted to be used in a simulation. It reads an input XML file (init.xml by default) containing the configuration choices (sphere, rectangle, cylinder...) and create the <code>init.txt</code> file in output. Thus different initial conditions can be easily automatically created.</p>

<h2><a class="anchor" id="usage"></a> Usage</h2>
<p>To create the initial file, call 'PhysiBoSS_CreateInitTxtFile' executable from a Terminal window. The XML file to read (<code>init.xml</code>) containing the user choices can be precised as argument of the command.</p>
<p>The file <code>init.xml</code> (or spheroid.xml here in the example below) contained two parts: the description of active cells initial geometry and sizes, and the description of eventual passive cells. An example of such configuration file is given in the release and also shown in the example below.</p>

<p>Command: <br/> </p>
<code> path_to_physiboss/bin/PhysiBoSS_CreateInitTxtFile file spheroid.xml </code>

<h2><a class="anchor" id="e"></a>Current Possibilities</h2>
<p>In the current version, 3 geometries are proposed: spheric (mode=0), cylindric (mode=1) or rectangles (mode=2). The created shape is centered around (0,0,0) but this can be very easily modified in the source code. Active and/or passive cells can be placed in those geometries. The mean radius of cells can be precised, as well as the dimensions of the geometries (sphere radius, cylinder length and radius, rectangle width and length). If 2 cell types are present the proportion to which the first cell type should be initialised can be precised with <code>cell_proportion</code> parameter.</p>

<h2><a class="anchor" id="ex"></a> Example</h2>

<p>Example of the init configuration file <code>spheroid.xml:</code> </p>
<pre class="fragment">&lt;?xml version="1.0" encoding="UTF-8" ?&gt;

&lt;active_cells&gt;
        &lt;mode&gt; 0 &lt;/mode&gt;
        &lt;cell_radius&gt; 9.5 &lt;/cell_radius&gt;
        &lt;length&gt; 100 &lt;/length&gt;
        &lt;cell_spacing&gt; 0.95 &lt;/cell_spacing&gt;
&lt;/active_cells&gt;

&lt;passive_cells&gt;
        &lt;mode&gt; 0 &lt;/mode&gt;
        &lt;cell_radius&gt; 10 &lt;/cell_radius&gt;
        &lt;length&gt; 150 &lt;/length&gt;
        &lt;min_length&gt; 100 &lt;/min_length&gt;
        &lt;cell_spacing&gt; 1 &lt;/cell_spacing&gt;
&lt;/passive_cells&gt;</pre><p> With this file, we created a spheroid of radius 100 µm, composed of cells with a mean radius of 9.5 µm, and spaced with 0.95% of their size (small overlap). The spheroid is surrounded by a spheric extra-cellular matrix, with a radius of 150 µm (but empty in the 100 µm internal sphere) represented by passive cells of mean radius 10 µm.</p>
<dl class="todo"><dt><b><a class="el">Todo:</a></b></dt><dd>Add more options of configurations (more geometries, distributions of cell size and phases...). </dd></dl>
<dl class="section author"><dt>Author</dt><dd>G. Letort, Institut Curie </dd></dl>
<dl class="section date"><dt>Date</dt><dd>June 2017 </dd></dl>
</div>
