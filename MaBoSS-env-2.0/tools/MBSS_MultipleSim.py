#!/usr/bin/env python
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
# Tool: MBSS_MultipleSim.py
# Author: Aurelien Dugourd, dugourdaurelien@gmail.com
# Date: Jan 2017
#

import string
import re
import sys
import os

def compatible_input(message):
    if sys.version_info[0] >= 3:
        return input(message)
    else:
        return raw_input(message)

def createSimulation (profilFileName):
    with open(profilFileName,  'a') as profilFile:
            profilFile.write(compatible_input("Simulation name : ")+'\n')
            profilFile.write(compatible_input("Inactive inputs (ex : O_stress,AA_starvation,TLR4) : ")+'\n')
            profilFile.write(compatible_input("Active inputs (ex : Inflamation,DAN_damage) : ")+'\n')
            profilFile.write(compatible_input("Custom rate nodes (ex : u:node1:0.15,u:node2:0.1,d:node2:0.1)  : ")+'\n')
            profilFile.write(compatible_input("Custom state nodes ( ex : node1:0.1,node2:0.4) : ") +'\n')
            profilFile.write(compatible_input("Outputs (ex : BECN1,Ulk_C,PIK3C3) : ")+'\n')
  
######################################################################  

def createProfil (configFileName):
    choice = compatible_input("No profil file was given. Do you wish to create a new profil file ? (y/n)")
    if choice == "n":
        print("Quitting...")
        exit(-3)
    else :
        name = compatible_input("Base name of the profil file (no path) : ")
        dirName = os.path.dirname(configFileName)
        if dirName != "" :
            profileFileName = os.path.dirname(configFileName)+name
        else :
            profileFileName = name
        createSimulation(profileFileName)
        return  profileFileName

######################################################################  
        
def displaySimulations (profilFileName):
    with open(profilFileName,  'r') as profilFile:
        profilFileLines = profilFile.readlines()
    print("Existing simulations :")
    i = 0
    while i < len(profilFileLines):
        print(str((i/6)+1) + ")" + profilFileLines[i].rstrip())
        i = i+6

######################################################################  
        
def deleteSimulation(profilFileName):
    displaySimulations(profilFileName)
    simulationNumbers = compatible_input("Select single (ex : 1) or multiple simulations to be deleted (ex : 1,2,3) :").split(',')
    
    with open(profilFileName,  'r') as profilFile:
        profilFileLines = profilFile.readlines()
    
    simulationNumbers = [ (int(simulationNumber)-1)*5 for simulationNumber in simulationNumbers ]
    simulationNumbers = [ (x,x+1,x+2,x+3, x+4) for x in simulationNumbers ]
    simulationNumbers = [ x for sublist in simulationNumbers for x in sublist ]
    
    with open(profilFileName,  'w') as profilFile:
        i = 0
        while i < len(profilFileLines):
            if i not in simulationNumbers:
                profilFile.write(profilFileLines[i])
            i = i+1
    print(sum(1 for line in open(profilFileName)))
    if sum(1 for line in open(profilFileName)) < 2:
        print("All simulations have been erased. Consider creating new simulations to start simulations.")
        commandLine = "rm "+profilFileName
        os.system(commandLine)
        
#######################################################################

def createConfigFiles(configFileName, profilFileName,  mutationFileName): 
    displaySimulations(profilFileName)
    
    with open(profilFileName,  'r') as profilFile:
        profilFileLines = profilFile.readlines()
    
    with open(configFileName,  'r') as cfgFile:
        cfgFileLines = cfgFile.readlines()
    
    selectedSimulations = compatible_input("Select simulations to use(ex : 1,3,5,6) : ").split(',')
    selectedSimulations = [(int(number)-1)*6 for number in selectedSimulations]
    
    configFileList = []
    
    if mutationFileName != "":
        with open(mutationFileName,  'r') as mutationFile:
            mutationFileLines = mutationFile.readlines()
            mutationTable = [mutationFileLine.rstrip().split(',') for mutationFileLine in mutationFileLines] 
            #print(mutationTable)
            mutationTable = [ [mutation.split(':') for mutation in mutationGroup] for mutationGroup in mutationTable]
            #print(mutationTable)
    
    for simulation in selectedSimulations:
        
        simulationName = profilFileLines[simulation].rstrip()
        inactiveInputs = profilFileLines[simulation+1].rstrip().split(',') 
        activeInputs = profilFileLines[simulation+2].rstrip().split(',') 
        customRates = profilFileLines[simulation+3].rstrip().split(',') 
        customRates = [ customRate.split(':') for customRate in customRates ]
        customStates = profilFileLines[simulation+4].rstrip().split(',')
        customStates = [ customState.split(':')  for customState in customStates ]
        outputs = profilFileLines[simulation+5].rstrip().split(',') 
        
        ratesTable = []
        internalTable = []
        parametersTable = []
        
        for cfgFileLine in cfgFileLines:
            if cfgFileLine[0] == '$':
                ratesTable.append(cfgFileLine)
            elif re.search('istate=0',  cfgFileLine):
                internalTable.append(re.sub('istate=0',  'is_internal=1',  cfgFileLine))
            else:
                parametersTable.append(cfgFileLine)
          
#####Parsing of input files
        
        j=0
        while j < len(parametersTable):
            if re.search('sample_count',  parametersTable[j]):
                parametersTable[j] = re.sub('500000',  '50000',  parametersTable[j])
            elif re.search('max_time',  parametersTable[j]):
                parametersTable[j] = re.sub('5',  '100',  parametersTable[j])
            elif re.search('time_tick',  parametersTable[j]):
                parametersTable[j] = re.sub('0[.]01',  '0.1',  parametersTable[j])
            elif re.search('thread_count',  parametersTable[j]):
                parametersTable[j] = re.sub('4',  '8',  parametersTable[j])
            elif re.search('statdist_traj_count',  parametersTable[j]):
                parametersTable[j] = re.sub('100',  '0',  parametersTable[j])
            j = j+1
            
        j=0
        while j < len(internalTable):
            for output in outputs:
                pattern = "^"+output+"[.]"
                if  re.search(pattern,  internalTable[j]):
                    internalTable[j] = re.sub('=1',  '=0',  internalTable[j])
            j = j+1
            
        j = 0
        if customRates != [['']]:
            while j < len(ratesTable):
                for customRate in customRates:
                    if re.match("[$]"+customRate[0]+"_"+customRate[1],  ratesTable[j]):
                        ratesTable[j] = re.sub("=1",  "="+customRate[2],  ratesTable[j])
                j = j+1
         
        if mutationFileName == "" :            
            newCfgFileName = re.sub("[.]cfg", "", configFileName) + "_" + simulationName + ".cfg"
            configFileList.append(newCfgFileName)
            with open(newCfgFileName,  'w') as newCfgFile:
                for rate in ratesTable:
                    newCfgFile.write(rate)
                newCfgFile.write('\n')
                for inputNode in activeInputs:
                    if inputNode != "":
                        newCfgFile.write(inputNode+".istate=1;\n")
                for inputNode in inactiveInputs:
                    if inputNode != "":
                        newCfgFile.write(inputNode+".istate=0;\n")
                for customState in customStates:
                    if customState != ['']:
                        newCfgFile.write("["+customState[0]+"]"+".istate="+str(1-float(customState[1]))+"[0],"+customState[1]+"[1];\n")
                newCfgFile.write('\n')
                for internal in internalTable:
                    newCfgFile.write(internal)
                for parameter in parametersTable:
                    newCfgFile.write(parameter)
                    
        else :
            for mutationGroup in mutationTable:

                tempRatesTable = list(ratesTable)
                tempActiveInputs = list(activeInputs)
                tempInactiveInputs = list(inactiveInputs)
                
                for mutation in mutationGroup :
                    if mutation[0] == 'u' :
                        tempActiveInputs.append(mutation[1])
                        i = 0
                        while i < len(tempRatesTable) :
                            if re.match("[$]d_"+mutation[1],  tempRatesTable[i]):
                                tempRatesTable[i] = re.sub("=.+",  "=0;",  tempRatesTable[i])
                            i = i+1
                            
                    else : 
                        tempInactiveInputs.append(mutation[1])
                        i = 0
                        while i < len(tempRatesTable) :
                            if re.match("[$]u"+"_"+mutation[1],  tempRatesTable[i]):
                                tempRatesTable[i] = re.sub("=.+",  "=0;",  tempRatesTable[i])
                            i = i+1
                
                newCfgFileName = re.sub("[.]cfg", "", configFileName) + "_" + simulationName + "_"  + '_'.join([ '_'.join(mutation) for mutation in mutationGroup]) + ".cfg"
                configFileList.append(newCfgFileName)
                with open(newCfgFileName,  'w') as newCfgFile:
                    for rate in tempRatesTable:
                        newCfgFile.write(rate)
                    newCfgFile.write('\n')
                    for inputNode in tempActiveInputs:
                        if inputNode != "":
                            newCfgFile.write(inputNode+".istate=1;\n")
                    for inputNode in tempInactiveInputs:
                        if inputNode != "":
                            newCfgFile.write(inputNode+".istate=0;\n")
                    for customState in customStates:
                        if customState != [''] and customState[0] not in tempInactiveInputs and customState[0] not in tempActiveInputs :
                            print(customState[0])
                            newCfgFile.write("["+customState[0]+"]"+".istate="+str(1-float(customState[1]))+"[0],"+customState[1]+"[1];\n")
                    newCfgFile.write('\n')
                    for internal in internalTable:
                        newCfgFile.write(internal)
                    for parameter in parametersTable:
                        newCfgFile.write(parameter)
                
                
    return configFileList
    
#############################################################

def PBSProcedure(configFileList):
    i=1
    for configFile in configFileList:
        if os.path.dirname(configFile) != '' :
            os.chdir(os.path.dirname(configFile))
        scriptName = "MaBoSS_simulation_"+str(i)+".sh"
        
        with open(scriptName,  'w') as scriptFile:
            scriptFile.write("#!/bin/bash\n## Torque Configuration\n# resources \n#PBS -l walltime=06:00:00\n#PBS -l mem=24gb\n#PBS -l nodes=1:ppn=8\n#PBS -q batch\n## Command section\ncd $PBS_O_WORKDIR\n \necho Running on host `hostname`\necho Time is `date`\necho Directory is `pwd`\n\n./MBSS_FormatTable.pl "+" "+os.path.basename(re.sub("[.]cfg", ".bnd", sys.argv[1]))+" "+os.path.basename(configFile)+" 0.1")
        
        commandLine = "qsub "+scriptName
        os.system(commandLine)
        
        i = i+1
        
############################################################

def  internalLaunchProcedure(configFileList):
    
    for configFile in configFileList:
        if os.path.dirname(configFile) != '' :
            os.chdir(os.path.dirname(configFile))
        commandLine = "MBSS_FormatTable.pl "+" "+os.path.basename(re.sub("[.]cfg", ".bnd", sys.argv[1]))+" "+os.path.basename(configFile)+" 0.1"
        os.system(commandLine)

###############################################################"

if len(sys.argv) != 2 and len(sys.argv) != 3 and  len(sys.argv) != 4:
    print("Requires at least one argument (a config file)")
    exit(-2)
    
configFileName = sys.argv[1]

if len(sys.argv) == 2:
    profilFileName = createProfil(configFileName)
else :
    profilFileName = sys.argv[2]
    
if len(sys.argv) == 4:
    mutationFileName = sys.argv[3]
else :
    mutationFileName = ""
    
choice = "1"
while (choice != "0" ):
    choice = compatible_input("generate config files using exiting simulations (0), create a new simulation (1) or delete a simulation (2): ")
    if choice == "0" or choice == "1" or choice == "2":
        if choice == "1":
            createSimulation(profilFileName)
        if choice == "2":
           deleteSimulation(profilFileName)
        if choice == "0":
            if os.path.isfile(profilFileName) :
                configFileList = createConfigFiles(configFileName, profilFileName,  mutationFileName)
            else :
                choice = "1"
                print("The profil file doesn't exist. Creating new simulation... Please folow instructions")
                createSimulation(profilFileName)

while (choice != "y" and choice != "n"):
    choice = compatible_input("Launch simulation on cluster ? (y/n)")
    if choice == "y":
        PBSProcedure(configFileList)
    else :
        internalLaunchProcedure(configFileList)
