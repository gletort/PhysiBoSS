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
# Tool: MBSS_MutBnd.pl
# Author: Gautier Stoll, gautier_stoll@yahoo.fr
# Date: Jan 2017
#

use strict;

my $MaxRate="1.7976931348623157E+308";
my $bnd_file=shift;
#Error test (produce a "help")
if (!$bnd_file){
    printf "Missing .bnd file, MBSS_MutBnd.pl  <file.bnd> \"<node_list>\"\n"; 
    exit;}

my $n_list=shift;
if (!$n_list){
    printf "Missing node list, MBSS_MutBnd.pl  <file.bnd> \"<node_list>\"\n";
    exit;}

my @node_list=split(/\s+/,$n_list);

$MaxRate=$MaxRate."/".($#node_list + 1);

#$_=$cfg_file;
#s/.cfg//;
#my $cfg_name=$_; #not use yet, new version may generates multiple cfg files
open(BND_F,$bnd_file) || die "Cannot find bnd file ".$bnd_file."\n";
my @MutBNDLineList;
do
{
    $_=<BND_F>;
    @MutBNDLineList=(@MutBNDLineList,$_);
    until((/node/ || /Node/ || eof(BND_F))) {$_=<BND_F>;@MutBNDLineList=(@MutBNDLineList,$_);} #catch line starting with "node" 
    if(!eof(BND_F))
    {
	foreach my $node (@node_list) #catch if node name correspond a name in the list of nodes
	{
	    if(/$node[\s\{\n]/)
	    {
		print "Catch node ".$node."\n";
		my $rate_up_flag=0;
		my $rate_down_flag=0;
		do {
		    $_=<BND_F>;
		    if(/rate_up/)
		    {
			my $up_var=" = ( \$Low_".$node." ? 0.0 : ( \$High_".$node." ? \@max_rate : ("; #Low_node wins
			s/=/$up_var/;
			s/;/)));/;
			@MutBNDLineList=(@MutBNDLineList,$_); #change the rate_up
			$rate_up_flag=1;	
		    }
		    elsif(/rate_down/)
		    {
			my $down_var=" = ( \$Low_".$node." ? \@max_rate : ( \$High_".$node." ? 0.0 : (";#Low_node wins
			s/=/$down_var/;
			s/;/)));/;
			@MutBNDLineList=(@MutBNDLineList,$_); #change the rate_down
			$rate_down_flag=1;
		    }
		    else{@MutBNDLineList=(@MutBNDLineList,$_);}
		}
		until(/\}/);
		if ($rate_up_flag==0)
		{
		    splice(@MutBNDLineList,$#MutBNDLineList-1,0,"\t rate_up = ( \$Low_".$node." ? 0.0 : ( \$High_".$node." ? \@max_rate : (\@logic ? 1.0 : 0.0 )));\n");
		    #if rate_up is absent, create it. Low_node wins
		}
		if ($rate_down_flag==0)
		{
		     splice(@MutBNDLineList,$#MutBNDLineList-1,0,"\t rate_down = ( \$Low_".$node." ? \@max_rate : ( \$High_".$node." ? 0.0 : (\@logic ? 0.0 : 1.0 ))) ;\n");
		     #if rate_down is absent, create it. Low_node wins
		}
		splice(@MutBNDLineList,$#MutBNDLineList,0,"\t max_rate = ".$MaxRate.";\n"); #local definition of Max_rate
		last; #if a node is catched, no need to further run the foreach loop
	    }
	}
    }
}
until(eof(BND_F));
close(BND_F);
$_=$bnd_file;
s/\.bnd/_mut\.bnd/;
open(BND_F_MUT,">".$_);
foreach my $line (@MutBNDLineList)
{print BND_F_MUT $line;}
close(BND_F_MUT);
