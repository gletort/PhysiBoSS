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
# Tool: MBSS_InitCondFromTrajectory.pl
# Author: Gautier Stoll, gautier_stoll@yahoo.fr
# Date: Jan 2017
#

use strict;

my $bndFile=shift;
if (!$bndFile) {printf "Missing bnd file, MBSS_InitCondFromTrajectory.pl <file.bnd> <filetraj.csv> <linenumber>\n";exit;}
my $probtrajFile=shift;
if (!$probtrajFile) {printf "Missing bnd file, MBSS_InitCondFromTrajectory.pl <file.bnd> <filetraj.csv> <linenumber>\n";exit;}
my $linenumber=shift;
if (!$linenumber) {printf "Missing bnd file, MBSS_InitCondFromTrajectory.pl <file.bnd> <filetraj.csv> <linenumber>\n";exit;}

open(BND,$bndFile) or die "cannot find .bnd file";
my @NodeList;
while(<BND>)
{
    if ( (/node/) || (/Node/))
    {
	s/node//;
	s/Node//;
	s/\{.*//;
	s/\s+//g;
	@NodeList=(@NodeList,$_);
    }
}
close(BND);
open(TRJF,$probtrajFile) or die "cannot find trajectory file";
for (my $i=0;$i <= $linenumber;$i++){defined($_=<TRJF>) or die "traj file is empty";} #header of probtraj file is not consider
close(TRJF);
my @LineList=split(/\t/);

my $LineInitCond="[".$NodeList[0];
foreach (@NodeList[1..$#NodeList]){$LineInitCond=$LineInitCond.",".$_;}
$LineInitCond=$LineInitCond."].istate =";
for (my $index=0;$index<=$#LineList;$index++) 
{
    $_=$LineList[$index];  
#    if (/[A-Z,a-z]/) 
    if ((/^[A-Z,a-z]/) || (/nil/))
    {
	$LineInitCond=$LineInitCond.$LineList[$index+1]." [";
#	if (/$NodeList[0]/) {$LineInitCond=$LineInitCond."1";}
	if (($_ eq $NodeList[0]) || /^$NodeList[0] / || / $NodeList[0] / || / $NodeList[0]$/) 
	{$LineInitCond=$LineInitCond."1";}

	else {$LineInitCond=$LineInitCond."0";}
	foreach my $Node (@NodeList[1..$#NodeList])
	{
	 $LineInitCond=$LineInitCond.",";
	 if (($_ eq $Node) || /^$Node / || / $Node / || / $Node$/) {$LineInitCond=$LineInitCond."1";}
	else {$LineInitCond=$LineInitCond."0";}
	}
	$LineInitCond=$LineInitCond."] , ";	
    }
}
chop($LineInitCond);
chop($LineInitCond);
$LineInitCond=$LineInitCond.";\n";
printf $LineInitCond;
