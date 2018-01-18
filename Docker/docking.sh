## To use PhysiBoSS docker image
## Param:
##		1: full path to the local machine directory of the simulation to run (main folder of the simulation. Will be mounted on the docker image
##      2: subfolder in which the simulation should be run (e.g. run0), where the parameter file is.


## install/update the image
docker pull gletort/physiboss
## run it by mounting local directory so that Inputs/Outputs are on local machine
simname=$(basename $1)
docker run -d -it -v $1:/home/PhysiBoSS/running/$simname gletort/physiboss sh -c "cd /home/PhysiBoSS/running/"$simname"/"$2; /home/PhysiBoSS/bin/PhysiBoSS > docking.txt"
