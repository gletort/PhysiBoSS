## How to use PhysiBoSS docker image

If PhysiBoSS cannot be compiled on your operating system, it is possible to use [PhysiBoSS docker image](http://hub.docker.com/r/gletort/physiboss).
For this, you need Docker engine to be installed on your computer.
Docker allows you to run an image of a software/environment build in a given operating system with all necessary dependencies, from any operating system.
We built the image with a CentOS distribution in which we installed and compiled PhysiBoSS.
Thus when you have copied PhysiBoSS docker iamge, it is ready to be used in a simulation directly.

### Running a simulation with docking.sh script
You can find in PhysiBoSS GitHub repository in the `Docker` folder a bash script to use the docker image directly.
You need to give as parameter to this script the full path to the main folder on your computer of the simulation that you want to run, and as second parameter the name of the subfolder containing the parameter file.
It will output the results in the folders 'output' and 'microutput' of this folder, as explained in PhysiBoSS description.

For example, to run with docker the Hello World Example from this repository, that you have copied on your machine:
~~~bash
./docking.sh /path/to/Hello/World/Example/folder run0
~~~


### Running a simulation
To install/update the docker image of PhysiBoSS, you have to pull it on your machine:
~~~bash 
docker pull gletort/physiboss
~~~

To run a simulation, you could run it directly inside the Docker image, but you don't access the files directly then.
To link it to your local folders, you have to mount the local directory in which you have prepare the simulation. Outputs will then also be written in that directory.
The command is the following:
~~~bash
docker run -d -it -v /path/to/local/directory:/home/PhysiBoSS/running/name_of_simulation gletort/physiboss sh -c "cd /home/PhysiBoSS/running/name_of_simulation/subfolder_of_simulation_to_run; /home/PhysiBoSS/bin/PhysiBoSS > docking.txt"
~~~

With:
	* -d to run the docker image in background
	* -v path1:path2 allows you to mount the local directory path1 and placed it in PhysiBoSS image as directory path2. 
	You have to precise the full path to the directories. 
	In the PhysiBoSS image, we created a folder '/home/PhysiBoSS/running' in which to place all running simulations. But of course you can specified another path.
	* 'gletort/physiboss" precises the image that you want to run
	* sh -c "cmd" allows you to define which commands you want to run in the image. 
	Here, you move to the simulation folder, in the subfolder that you want to run. From there, you call PhysiBoSS executable and run the simulation.  
