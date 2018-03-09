# How to use Paraview to visualize PhysiBoSS output

[Paraview](https://www.paraview.org/) is a multi-platform visualisation software, that we used to render interactively our data files. Here we explained how to use it with PhysiBoSS results:

[Use the prepared state files](#use-the-prepared-state-files)

[Or do it all step by step](#or-do-it-all-step-by-step-how-to-configure-paraview-to-draw-points-as-spheres)
 
[Save image/make movie](#save-image-or-make-movie)

## Use the prepared state files...
As explained in the main page and example, you can directly use the customized paraview state files (with `File->Load State...`) to directly visualize the output with cells represented as colorfull spheres.

However, if you encouter issues in using our state files, or in case you want to know how to configure Paraview to read the output files all by yourself, below we present the different steps to do.

## Or do it all step by step: how to configure Paraview to draw points as spheres

### Step 1
First you must load your output files that you want to visualize, which are the `output/cells*.txt` files. In Paraview, go to 'File->Open...` and select the desired files. Each different file will be a different time point in Paraview.
In the window containing the settings for opening the files, one option is `Field Delimeter Characters`. Change the `,` by `;` which is the delimeter that we used in PhysiBoSS to seperate columns. Click on `Apply`.
You should obtain something similar to the image below. 

<p align="center">
<img src="https://github.com/gletort/PhysiBoSS/blob/master/doc/imgs/paraviewing_step1.png" alt="end of Step 1" width="800"/>
</p>

Now, Paraview has read the output files, and retrieve all the different fields value for each cell at each time point, which you can see in the `SpreadSheetView` window.

### Step 2
The next step is to indicate to Paraview which fields are the spatial coordinates of the point to draw.

Go to `Filters->Alphabetical->Table to points` which will convert your spreadsheet in a list of spatial points. For this, you have to define which fields are the coordinates: in the `Properties (TableToPoints1)` setting window, in the `X Colum` field, select in the rolling list your x coordinates field, which in PhysiBoSS output is called, surprisingly, `x`. Idem for `Y Column` and `Z Column`. Click on `Apply`. It will now display something similar as the image below, with in white the points (each cell here) to draw.

<p align="center">
<img src="https://github.com/gletort/PhysiBoSS/blob/master/doc/imgs/paraviewing_step2.png" alt="end of Step 2" width="800"/>
</p>

You may have to click on the little eye at the left of `TableToPoints1` to visualize it.

### Step 3
The last step is to convert the representation of the points to spheres, with radii should be the PhysiBoSS output radii of each cell. 

For this, you have to add a `Glyph` on the points.
Go to `Filters->Alphabetical->Glyph`. A setting window allows you to configure it.
In this setting window, in the `Glyph Source` part, select `Sphere` in `Glyph Type`, and put the `Radius` field as 1.
In `Active Attributes`, select the field `radius` to draw the radii of the cell. You could select `nuclear_radius` if you want to draw the nuclei instead. In `Scaling`, select `scalar` as the `Scale Mode` and put 1 in `Scale Factor`. Those settings precised to draw sphere of radius `radius` and multiply it by 1. Finally, to apply it on all the points (cells), in `Masking` select `All Points` in `Glyph Mode`.

Now the settings are finished, and you should obtain the image below.

<p align="center">
<img src="https://github.com/gletort/PhysiBoSS/blob/master/doc/imgs/paraviewing_step3.png" alt="end of Step 3" width="800"/>
</p>

You can then choose the coloring of the cells according to one field (e.g. phase to see cell phase), change the colors scales, the background...

## Save image or make movie
To save the current visualisation as an image, go to `File->Save screenshot...`. To make a movie of all the time frames with the current setup, go to `File->Save Animation...` which will then write an image for each time point, in the same folder with the given name. To convert the stack of images to a movie, you can for example use `ffmpeg` with this [script](https://github.com/gletort/scripts/blob/master/seeing/movise.sh).