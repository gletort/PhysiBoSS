#!/usr/bin/env perl
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
# Tool: MBSS_SensitivityAnalysis.pl
# Author: Gautier Stoll, gautier_stoll@yahoo.fr
# Date: Jan 2017
#

use strict;

#my $MaBossScript="PlMaBoSS_3.0.pl"; #script that run MaBoSS
my $bnd_file=shift;
if (!$bnd_file){
    printf "Missing bnd file, MBSS_SensitivityAnalysis.pl  <file.bnd> <file.cfg> \"<add_string>\", the string will be added to every external variable\n"; 
    exit;}
my $cfg_file=shift;
if (!$cfg_file){
    printf "Missing cfg file, MBSS_SensitivityAnalysis.pl  <file.bnd> <file.cfg> \"<add_string>\", the string will be added to every external variable\n";
    exit;}
my $mod_string=shift;
if (!$mod_string){
    printf "Missing string to add, MBSS_SensitivityAnalysis.pl  <file.bnd> <file.cfg> \"<add_string>\", the string will be added to every external variable\n";
    exit;}
$mod_string=$mod_string.";";
my $MaBossScript="MBSS_FormatTable.pl";
#if (!$mod_string){
#    printf "Missing string to add, ParamSensFormat.pl  <file.bnd> <file.cfg> \"<add_string>\", the string will be added to every external variable\n";
#    exit;}

$_=$cfg_file;
s/.cfg//;
my $cfg_name=$_;

my @CfgLineList;
my @CfgHitLineIndex;
my @CfgVarList;
open(CFG_F,$cfg_file);
my $LineIndex=0;
while(<CFG_F>)
{
    @CfgLineList=(@CfgLineList,$_);
    if (/^\$/)
    {
	s/^\$//;
	@CfgHitLineIndex=(@CfgHitLineIndex,$LineIndex);
	my @LineSplit=split(/=/);
	$_=$LineSplit[0];
	s/\s//g;
	@CfgVarList=(@CfgVarList,$_);
    }

    $LineIndex++;	    
}
close(CFG_F);
system("mkdir Sensitivity_".$cfg_name);   #added to renamed script"
system("cp ".$bnd_file." Sensitivity_".$cfg_name."/"); #added to renamed script
open(SRC_F,">Sensitivity_".$cfg_name."/Sensitivity_".$cfg_name.".sh"); #modified to renamed script
print SRC_F "#!/bin/bash\n"; #added to renamed script
for (my $i=0;$i<=$#CfgHitLineIndex;$i++)
{
    my $tmpCfgFile=$cfg_name."_".$CfgVarList[$i].".cfg";
    print SRC_F $MaBossScript." ".$bnd_file." ".$tmpCfgFile." 0.01\n"; #line that lauch MaBoSS
#    print SRC_F $MaBossScript." -c ".$tmpCfgFile." -o Out_".$CfgVarList[$i]." ".$bnd_file."\n"; #line that lauch MaBoSS
    open(CFG_F,">Sensitivity_".$cfg_name."/".$tmpCfgFile); #modified to renamed script
    for ($LineIndex=0;$LineIndex<$CfgHitLineIndex[$i];$LineIndex++)
    {
	print CFG_F $CfgLineList[$LineIndex];
    }
    $_=$CfgLineList[$CfgHitLineIndex[$i]];
    
    s/;/$mod_string/;
    print CFG_F $_;
    
    for ($LineIndex=$CfgHitLineIndex[$i]+1;$LineIndex<=$#CfgLineList;$LineIndex++)
    {
	print CFG_F $CfgLineList[$LineIndex];
    }
    close(CFG_F);
}
close(SRC_F);
system("chmod a+x Sensitivity_".$cfg_name."/Sensitivity_".$cfg_name.".sh"); #added to renamed script

	
    

