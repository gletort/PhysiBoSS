##################################################################################
##### Prepare one run folder, ready for PhysiBoSS simulation
#####
##### Create necessary subfolders
##### Create init file if specified 
#####
## Parameters: 
##		1 - name and path to main folder of the simulation
##      2 - parameter file
##
## Creation: April 2017, G. Letort, Institut Curie

mkdir $1
mkdir $1/microutput
mkdir $1/output
cp $2 $1/parameters.xml
