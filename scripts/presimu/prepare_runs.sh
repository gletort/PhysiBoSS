##################################################################################
##### Prepare one set of runs folder, ready for PhysiBoSS simulation
#####
##### Generate one/multiple parameter files from template file
##### Generate one/multiple initial configuration files from template file
##### Create necessary subfolders
#####
#### Parameters: 
##      1 - name of the simulation main folder
##		2 - parameter file template (with path)
##		3 - initial file template (with path)
##		4 - boolean network folder (containing the two necessary files)
##			Empty folder is fine if no BN files are necessary
##		[5 - number of repetition]
##
#### Creation: April 2017, G. Letort, Institut Curie

phboss="/bioinfo/users/gletort/Invade/PhysiBoSS-GitHub/PhysiBoSS"   ## Path to PhysiBoSS folder
binpath=$phboss"/bin" ## Path the bin folder in PhysiBoSS release
toolpath=$phboss"/scripts"  ## Path to the scripts folder, including this file

# create parameter files from the template
python $toolpath"/presimu/templating.py" $2 $5
# create initial configuration files from the template
python $toolpath"/presimu/templating.py" $3

paramname=$(basename $2)          ## get the name of the parameter file
filename="${paramname%%.*}"       ## get all files which have this name+number
nsim=$(ls $filename*.xml | wc -l) ## number of different parameter files created 

initallname=$(basename $3)		  ## get the name of the initial configuration file	
initfilename="${initallname%%.*}" ## get all files which have this name+number
ninitsim=$(ls $initfilename*.xml | wc -l)  ## number of initial files created

# create main folder of the simulation
mkdir $1
mkdir $1/BN/
cp -r $4/*.cfg $1/BN/bn_conf.cfg
cp -r $4/*.bnd $1/BN/bn_nodes.bnd

# create each run subfolders and put appropriate files in it
k=0
for j in $(seq 0 $((ninitsim-1)) )
do
	initnum=$(printf "%.4d" $j)
	for i in $( seq 0 $(($nsim-1)) ) 
	do
		num=$(printf "%.4d" $i)
		$toolpath/presimu/prepare_onefolder.sh $1/run$k $filename$num.xml
		$binpath/PhysiBoSS_CreateInitTxtFile file $initfilename$initnum.xml >> creation.txt
		mv init.txt $1/run$k/
		k=$(($k+1))
	done
done

# clean temp files
rm $filename*.xml
rm $initfilename*.xml
rm creation.txt
