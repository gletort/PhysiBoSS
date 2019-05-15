#!/usr/bin/env python3
# coding: utf-8
#
# MaBoSS (Markov Boolean Stochastic Simulator)
# Copyright (C) 2011-2017 Institut Curie, 26 rue d'Ulm, Paris, France
#   
# MaBoSS is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#   
# MaBoSS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#   
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 
#
# Tool: MBSS_DrugSim.py
# Author: Barthélémy Caron, caron.barth@gmail.com
# Date: Jan 2017
#

import sys
import subprocess
import time
import numpy as np
import os
import string
from os import listdir
import shutil
import stat
from subprocess import PIPE


if "drug_simulations" in listdir():
    shutil.rmtree("drug_simulations")

    
os.mkdir("drug_simulations")
src = "MBSS_FormatTable.pl"
src2 = "MaBoSS"
dst = "drug_simulations"

time.sleep(1.0)


#Create a folder containing all the generated files
#shutil.copy(src, dst)
#shutil.copy(src2, dst)


#LOAD PARAMETERS
config_file_name = sys.argv[1]
f = open("{}".format(config_file_name), "r")
model = str(f.readline()).strip("\n")
node_list = str(f.readline().replace(" ","").replace(",",", ").strip("\n")).split(", ")
istate_value_list = [float(i) for i in f.readline().replace(" ","").replace(",",", ").strip("\n").split(", ")]
istate_value_name_list = [str(i).replace(".", "_") for i in istate_value_list]
n_parallel_sim = int(f.readline().strip("\n"))
f.close()

print(node_list)

#GENERATE TEMPLATE BND FILE containing the inhibitor nodes and altering the logical rules corresponding to the inhibitions
print("Generate modified bnd file: adding inhibitor nodes and inhibitions")
"""Generate modified bnd file"""
f = open("{}.bnd".format(model))
file_content = f.read()
f.close()

file_content = file_content.replace("Node", "node")
file_content = file_content.replace(" {", "{")
file_content = file_content.replace("\n{", "{")
file_content = file_content.replace("{", "\n{")



node_list_from_file = file_content.split("node")[1:]
for item_index, item in enumerate(node_list_from_file):
    node_list_from_file[item_index] = "node" + item

    
    
new_node_list = str()
for node in node_list:
    mystring = "node {}\n".format(node)
    item_count = 0

    for item_index, item in enumerate(node_list_from_file):
        if mystring in (node_list_from_file[item_index]):
            item_count += 1
            for line_index, line in enumerate(node_list_from_file[item_index].split("\n")):

                if " logic" in line:
                    new_node_list += line.split(";")[0].split(" = ")[0] + " = (" + line.split(";")[0].split(" = ")[1] + ") AND NOT anti_{};\n".format(node)

                else:
                    new_node_list += line + "\n"
            node_list_from_file.pop(item_index)
    if item_count == 0:
        print("{} not found".format(node))


for last_item in node_list_from_file:
    new_node_list += last_item

for inhibitor_to_add in node_list:
    new_node_list += "node anti_{}".format(inhibitor_to_add) + " \n{\n" + "\tlogic = (anti_{});\n".format(inhibitor_to_add) + "\trate_up = @logic ? 0 : 0;\n\trate_down = @logic ? 0 : 0;\n}\n\n\n"

    
fw1 = open("{}_new.bnd".format(model), "w")
fw1.write(new_node_list)
fw1.close()


#GENERATE TEMPLATE CFG FILE setting the initial parameters of the new nodes
print("Generate modified cfg file: istate formatting")
"""Generate modified cfg file"""
f = open("{}.cfg".format(model))
file_content = f.read()
f.close()
new_cfg = str()

file_content = file_content.replace("] ,", "],")

print(file_content)
for line_index, line in enumerate(file_content.split("\n")):
    if ".istate = TRUE" in line:
        line = "[" + line.replace(".istate = TRUE", "].istate = 1 [1], 0 [0]")
    elif ".istate = FALSE" in line:
        line = "[" + line.replace(".istate = FALSE", "].istate = 0 [1], 1 [0]")
    elif "].istate = 0 [0], 1 [1]" in line:
        line = line.replace("].istate = 0 [0], 1 [1]", "].istate = 1 [1], 0 [0]")
    elif "].istate = 1 [0], 0 [1]" in line:
        line = line.replace("].istate = 1 [0], 0 [1]", "].istate = 0 [1], 1 [0]")
    else:
        line = line
    new_cfg += line + "\n"  
    

fw2 = open("{}_new.cfg".format(model), "w")
fw2.write(new_cfg)
fw2.close()


print(istate_value_list)

#Create bnd and cfg giles with the required inhibition levels when at least two drugs are specified
if len(node_list) != 1:
    for node_index_1, node_1 in enumerate(node_list):
        
        for node_index_2, node_2 in enumerate(node_list[1+node_index_1:]):
            print("creating {} {} combinations".format(node_1, node_2))
            for istate_index_1, value_1 in enumerate(istate_value_list):
                
                for istate_index_2, value_2 in enumerate(istate_value_list):

                    value_1_name = istate_value_name_list[istate_index_1]
                    value_2_name = istate_value_name_list[istate_index_2]
                
                
                    """GEN BND FILE"""
                    options_copy = ["{}_new.bnd".format(model), 
                                    "drug_simulations/{}_{}_{}_{}_{}.bnd".format(model,
                                                            node_1, 
                                                            istate_value_name_list[istate_index_1], 
                                                            node_2, 
                                                            istate_value_name_list[istate_index_2])]
                    
                    proc2 = subprocess.Popen(["cp"] + options_copy)
                    time.sleep(1.0)
                    
                    
                    
                    """GEN CFG FILE"""
                    f = open("{}_new.cfg".format(model),"r")
                    lines = f.readlines()
                    f.close()
                    save_name = "drug_simulations/{}_{}_{}_{}_{}.cfg".format(model, node_1, value_1_name, node_2, value_2_name)
    
                    nf = open(save_name, "w")
    
                    for line in lines:
    
                        if "[{}].istate = ".format(node_1) in line:
                            new_line = line
                            new_line = "[{}].istate = {} [1], {} [0];\n".format(node_1, value_1, np.round(1-value_1, 1))
                            print(new_line)
                            nf.write(str(new_line))
    
                        elif "[{}].istate = ".format(node_2) in line:
                            new_line = line
                            new_line = "[{}].istate = {} [1], {} [0];\n".format(node_2, value_2, np.round(1-value_2, 1))
                            print(new_line)
                            nf.write(str(new_line))
    
                        else:
                            nf.write(str(line))

                    nf.close()
                    
                    
#Create bnd and cfg giles with the required inhibition levels when only one drug has been added
else:
    for node_index_1, node_1 in enumerate(node_list):
        print("creating {} gradual inhibition".format(node_1))
        for istate_index_1, value_1 in enumerate(istate_value_list):
            value_1_name = istate_value_name_list[istate_index_1]         
            """GEN BND FILE"""
            options_copy = ["{}_new.bnd".format(model), 
                                    "drug_simulations/{}_{}_{}.bnd".format(model,
                                                            node_1, 
                                                            istate_value_name_list[istate_index_1])]
                    
            proc2 = subprocess.Popen(["cp"] + options_copy)
            time.sleep(1.0)
            """GEN CFG FILE"""
            f = open("{}_new.cfg".format(model),"r")
            lines = f.readlines()
            f.close()
            save_name = "drug_simulations/{}_{}_{}.cfg".format(model, node_1, value_1_name)
            nf = open(save_name, "w")
            for line in lines:
                
                if "[{}].istate = ".format(node_1) in line:
                    new_line = line
                    new_line = "[{}].istate = {} [1], {} [0];\n".format(node_1, value_1, np.round(1-value_1, 1))
                    print(new_line)
                    nf.write(str(new_line))    
                else:
                    nf.write(str(line))
            nf.close()

            

file_list = []
count = 0

for file_index, file in enumerate(listdir("drug_simulations/")):
    if ".bnd" in file:
        file_list.append(file)
number_of_simulations = len(file_list)

file_list = []


#create X bash files containing the commands to launch the simulations. 
#X is the number of parallel simulations to launch, specified by the user in the configuration file
for file_index, file in enumerate(listdir("drug_simulations/")):
    if ".bnd" in file:
        if len(file_list) < (number_of_simulations/n_parallel_sim):
            file_list.append(file)

        else:
            fw1 = open("task_{}.sh".format(count), "w")
            fw1.write("#!/bin/bash\n" )
            fw1.write("cd drug_simulations\n" )
#            fw1.write("chmod a+x MBSS_TableFolder.pl\n")
#            fw1.write("chmod a+x MaBoSS\n")


            for item in file_list:
                item = item.strip(".bnd")
                fw1.write("MBSS_FormatTable.pl " + "{}.bnd ".format(item) + "{}.cfg ".format(item) + "\n")
            fw1.close()
            file_list = []
            file_list.append(file)
            count += 1


            
if len(file_list) != 0:
    fw1 = open("task_{}.sh".format(count), "w")
    fw1.write("#!/bin/bash\n" )
    fw1.write("cd drug_simulations\n")
#    fw1.write("chmod a+x MBSS_TableFolder.pl\n")
#    fw1.write("chmod a+x MaBoSS\n")


    for item in file_list:
        item = item.strip(".bnd")
        fw1.write("MBSS_FormatTable.pl " + "{}.bnd ".format(item) + "{}.cfg ".format(item) + "\n")
    fw1.close()




error_file = open("errors.txt", "wb")
out_file = open("outputs.txt", "wb")

#Launch the bash files
for index in range(count+1):
    subprocess_command = ["bash", "task_{}.sh".format(index)]
    print(subprocess_command)
    subprocess.Popen(subprocess_command, stdout=out_file, stderr=error_file)
