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
# Tool: MBSS_FormatTable.pl
# Author: Gautier Stoll, gautier_stoll@yahoo.fr
# Date: Jan 2017
#
# Can use .bnd .cfg and executable in other dir, but should be given by absolute directory
#

use strict;

my @CommandLine=@ARGV;
if ($#CommandLine <1){{die "Bad arguments, MBSS_FormatTable.pl <file.bnd> <file.cfg> <(optional)threshold> <(optional)-mb MaBoSS executable name>\n";}} 
my $bndFile=$CommandLine[0];
my $cfgFile=$CommandLine[1];
my $NoStatTable=0;
my $ExecMaBoSS="MaBoSS";
my $ProbThreshold;
if ($#CommandLine > 1)
{
    
    if ($CommandLine[2] eq "-mb")
    {
        $NoStatTable=1;
	if ($#CommandLine < 3) {die "Bad arguments, MBSS_FormatTable.pl <file.bnd> <file.cfg> <(optional)threshold> <(optinal)-mb MaBoSS executable name>\n";}
	else {$ExecMaBoSS=$CommandLine[3];}
    }
    else
    {
	$_=$CommandLine[2];	
	s/^[0-9]*//;
	s/[0-9]*$//;
	if (length($_) == 0 || $_ eq "\.") {$ProbThreshold=$CommandLine[2];}
	else {die "Bad arguments, MBSS_FormatTable.pl <file.bnd> <file.cfg> <(optional)threshold> <(optinal)-mb MaBoSS executable name>\n";}
	if ($#CommandLine > 2)
	    {
		if ($#CommandLine < 4) {die "Bad arguments, MBSS_FormatTable.pl <file.bnd> <file.cfg> <(optional)threshold> <(optinal)-mb MaBoSS executable name>\n";}
		elsif (!($CommandLine[3] eq "-mb")) {die "Bad arguments, MBSS_FormatTable.pl <file.bnd> <file.cfg> <(optional)threshold> <(optinal)-mb MaBoSS executable name>\n";}
		else {$ExecMaBoSS=$CommandLine[4];}
	    }
    }
}
else {$NoStatTable=1;}
$_=$cfgFile;
s/\.cfg//;
my @tempList=split(/\//,$_);
my $ProjectName=$tempList[$#tempList];
if (-e $ProjectName)
{
    my $i;
    for($i=1;(-e $ProjectName."_".$i);$i++)
    {}
    $ProjectName=$ProjectName."_".$i; #Construct the name of the project
}
system("mkdir ".$ProjectName);
system("cp ".$bndFile." ".$ProjectName);
@tempList=split(/\//,$bndFile);
my $bndFileLoc=$tempList[$#tempList]; #local name of bnd, with no /dir
system("cp ".$cfgFile." ".$ProjectName); #copy the files in the created directory
@tempList=split(/\//,$cfgFile);
my $cfgFileLoc=$tempList[$#tempList]; #local name of cfg, with no /dir
if (-r $ExecMaBoSS) #if executable file is readable, copy it in directory
{
    system("cp ".$ExecMaBoSS." ".$ProjectName);
    chdir($ProjectName);
    @tempList=split(/\//,$ExecMaBoSS); #remove folder in front of executable name
    system("./".$tempList[$#tempList]." -c ".$cfgFileLoc." -o ".$ProjectName." ".$bndFileLoc); #run MaBoSS
}
else 
{
    if ((-x $ExecMaBoSS) && (substr($ExecMaBoSS,0,1) ne "/")){$ExecMaBoSS="../".$ExecMaBoSS;} #adapt exectable name
    chdir($ProjectName);
    system($ExecMaBoSS." -c ".$cfgFileLoc." -o ".$ProjectName." ".$bndFileLoc); #run MaBoSS
}

#construct the table of clustered stationary distributions
my $StatDistFile=$ProjectName."_statdist.csv";
open(StatDist,$StatDistFile) or die;
my @StatDistProb; #Probability of the cluster/indecompsable stationary distribution
my $NbTraj=0; #Number of trajectories used for clustering stationary distribution
while(<StatDist>) #Go the the clusters, find the number of trajectories for each cluster
{
    if (m/Cluster/)
    {last;}
    if (m/cluster=#\d+/)
    {
	m/\d+/;
	my $ClusterIndex=$&-1; #clustering number start at 1
	$_=$'; #';
	m/\d+/;
	$StatDistProb[$ClusterIndex]=$&;
	$NbTraj=$NbTraj+$&;
    }
}
for(my $i=0;$i<=$#StatDistProb;$i++)
{
    $StatDistProb[$i]= $StatDistProb[$i]/$NbTraj;
}

if ($NoStatTable == 0)
{
    open(TSTAT,">".$ProjectName."_statdist_table.csv") or die;
    while(<StatDist>)
    {
	if (!(m/^#/))
	{last;}
	my $line=$_;
	chomp($line); 
	my @linelist=split(/\t/,$line);
	my %ClusterProbTh0; #Prob and error with no 0
	my %ClusterErrorTh0;
	my %ClusterProb; #Prob and error for threshold given
	my %ClusterError;
	for(my $IndexLine=1; $IndexLine<=$#linelist;$IndexLine=$IndexLine+3)
	{
	    $ClusterProbTh0{$linelist[$IndexLine]}=$linelist[$IndexLine+1];
	    $ClusterErrorTh0{$linelist[$IndexLine]}=$linelist[$IndexLine+2];
	    
	    if ($linelist[$IndexLine+1]>$ProbThreshold)
	    {
		$ClusterProb{$linelist[$IndexLine]}=$linelist[$IndexLine+1];
		$ClusterError{$linelist[$IndexLine]}=$linelist[$IndexLine+2];
	    }
	}
	
	my @StateList=keys(%ClusterProb);
	print TSTAT "Probability threshold=".$ProbThreshold."\n";
	print TSTAT "Prob[Cluster ".$linelist[0]."]\t";
	foreach my $State (@StateList)
	{
	    $_=$State;
	    s/ //g;
	    print TSTAT "Prob[".$_." | Cluster ".$linelist[0]."]\t";
	}
	print TSTAT "\n";
	$_=$linelist[0];
	s/#//g;
	print TSTAT $StatDistProb[($_)-1]."\t"; 
	foreach my $State (@StateList)
	{
	    print TSTAT $ClusterProb{$State}."\t";
	}
	print TSTAT "\n";
	print TSTAT "ErrorProb\t";
	foreach my $State (@StateList)
	{
	    print TSTAT $ClusterError{$State}."\t";
	}
	print TSTAT "\n\n";
    
#    print TSTAT "Probability threshold=0.0\n";
#    my @StateListTh0=keys(%ClusterProbTh0);
#    print TSTAT "Prob[Cluster ".$linelist[0]."]\t";
#    foreach my $State (@StateListTh0)
#    {
#	$_=$State;
#	s/ //g;
#	print TSTAT "Prob[".$_." | Cluster ".$linelist[0]."]\t";
#    }
#    print TSTAT "\n";
#    $_=$linelist[0];
#    s/#//g;
#    print TSTAT $StatDistProb[($_)-1]."\t"; 
#    foreach my $State (@StateListTh0)
#    {
#	print TSTAT $ClusterProbTh0{$State}."\t";
#    }
#    print TSTAT "\n";
#    print TSTAT "ErrorProb\t";
#    foreach my $State (@StateListTh0)
#    {
#	print TSTAT $ClusterErrorTh0{$State}."\t";
#    }
#    print TSTAT "\n\n";


    }
    close(TSTAT);
}
#if ($_) #write fixed points file
#{
#    open(FP,">".$ProjectName.".FP") or die;
#    s/FP\t/\n/;
#    print FP $_;
#    while(<StatDist>)
#    {
#	s/FP\t/\n/;
#	print FP $_;
#    }
#    close(FP);
#}
close(StatDist);

#Construct the table of probabilities trajectory
my $ProbTrajfile=$ProjectName."_probtraj.csv";
open(ProbTraj,$ProbTrajfile) or die;
my $line=<ProbTraj>;
my @firstlinelist=split(/\t/,$line);
my $name;
my $IndexProba=4;
my @firstline_woutH=@firstlinelist[4..$#firstlinelist];

foreach $name (@firstline_woutH)
{
    $_=$name;
    if (! m/HD=/)
    {
	last;}
    $IndexProba++;
}
my $IndexLine=0;
$line=<ProbTraj>;
chomp($line);
my @linelist=split(/\t/,$line);
my %FullGlobal;
$FullGlobal{Time}[$IndexLine]=$linelist[0];
$FullGlobal{TH}[$IndexLine]=$linelist[1];
$FullGlobal{ErrTH}[$IndexLine]=$linelist[2];
$FullGlobal{H}[$IndexLine]=$linelist[3];
my %ProbState;
my %ErrProbState;
for (my $i=$IndexProba;$i<=$#linelist;$i=$i+3)
{
    my $State="[".$linelist[$i]."]";
    $ProbState{$State}[$IndexLine]=$linelist[$i+1];
    $ErrProbState{$State}[$IndexLine]=$linelist[$i+2];
}
while(<ProbTraj>)
{   
    $IndexLine++;
    foreach my $State (keys(%ProbState))
    {
	$ProbState{$State}[$IndexLine]=0;
	$ErrProbState{$State}[$IndexLine]=0;
    }
    chomp($_);
    @linelist=split(/\t/,$_);
    $FullGlobal{Time}[$IndexLine]=$linelist[0];
    $FullGlobal{TH}[$IndexLine]=$linelist[1];
    $FullGlobal{ErrTH}[$IndexLine]=$linelist[2];
    $FullGlobal{H}[$IndexLine]=$linelist[3];   
    for (my $i=$IndexProba;$i<=$#linelist;$i=$i+3)
    {
	my $State="[".$linelist[$i]."]";
	if (defined $ProbState{$State}[$IndexLine-1])
	{
	    $ProbState{$State}[$IndexLine]=$linelist[$i+1];
	    $ErrProbState{$State}[$IndexLine]=$linelist[$i+2];
	}
	else
	{
	    for(my $PastIndexLine=0;$PastIndexLine<$IndexLine;$PastIndexLine++)
	    {
		$ProbState{$State}[$PastIndexLine]=0;
		$ErrProbState{$State}[$PastIndexLine]=0;	
	    }
	    $ProbState{$State}[$IndexLine]=$linelist[$i+1];
	    $ErrProbState{$State}[$IndexLine]=$linelist[$i+2];
	}
    }
}
close(ProbTraj);

#write the trajectory table in file
my @StateList=keys(%ProbState);
open(TTRAJ,">".$ProjectName."_probtraj_table.csv") or die;
print TTRAJ "Time\t";
print TTRAJ "TH\t";
print TTRAJ "ErrTH\t";
print TTRAJ "H\t" ;
foreach my $State (@StateList)
{
    $_=$State;
    s/ //g;
    print TTRAJ "Prob".$_."\t"."ErrProb".$_."\t";
}
print TTRAJ "\n";
for (my $i=0;$i<=$IndexLine;$i++)
{
    print TTRAJ $FullGlobal{Time}[$i]."\t";
    print TTRAJ $FullGlobal{TH}[$i]."\t";
    print TTRAJ $FullGlobal{ErrTH}[$i]."\t";
    print TTRAJ $FullGlobal{H}[$i]."\t";
    foreach my $State (@StateList)
    {
	print TTRAJ $ProbState{$State}[$i]."\t".$ErrProbState{$State}[$i]."\t";
    }
    print TTRAJ "\n";
}
close(TTRAJ);


