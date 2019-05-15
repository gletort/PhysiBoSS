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
# Package: MBSS_PrepareProjectFile.pm
# Author: Gautier Stoll, gautier_stoll@yahoo.fr
# Date: Jan 2017
#

package MBSS_PrepareProjectFile;
use strict;

#use warnings;
#Version 1.2, with correct variable sensitivity analysis and mutation combinaision with node names with "_"
##############################################################################################################
sub main
{
    my $LibName="MBSS_PrepareProjectFile";
    my $tmpError;
    my @MutVar; #list of mutated nodes
    my $PMBSSFile=$_[0];
    $_=$PMBSSFile;
    s/\.pmbss//;
    my  @tempList=split(/\//,$_);
#    my @tempList=split(/\./,$PMBSSFile);
#    @tempList=split(/\//,$tempList[$#tempList-1]);
    my $ProjectName=$tempList[$#tempList];
    if (-e $ProjectName)
    {
	my $i;
	for($i=1;(-e $ProjectName."_".$i);$i++)	{}
	$ProjectName=$ProjectName."_".$i; #Construct the name of the project
    }
    print "Project Name: ".$ProjectName."\n";
    system("mkdir ".$ProjectName); #create project directory
    ##########################################################################################################
    my %Hash_pmbss = HashStr_pmbss($PMBSSFile); #Hash table associated to pmbss file
    $tmpError = system("cp ".$PMBSSFile." ".$ProjectName."/"); if ($tmpError) {die "cannot copy pmbss file";}

    if (-r $Hash_pmbss{MABOSS}) #if executable file is readable, copy it in directory
    {
	system("cp ".$Hash_pmbss{MABOSS}." ".$ProjectName."/");
	@tempList=split(/\//,$Hash_pmbss{MABOSS}); 
	$Hash_pmbss{MABOSS}="./".$tempList[$#tempList]; #adapt MaBoSS executable name by removing foler names in front of it
    }
    else 
    {
	if ((-x $Hash_pmbss{MABOSS}) && (substr($Hash_pmbss{MABOSS},0,1) ne "/")){$Hash_pmbss{MABOSS}="../".$Hash_pmbss{MABOSS};} #adapt exectable name
    }
    
    $tmpError = system("cp ".$Hash_pmbss{CFG}." ".$ProjectName."/"); if ($tmpError) {die "cannot find .cfg file";}
    @tempList=split(/\//,$Hash_pmbss{CFG});
    $Hash_pmbss{CFG}=$tempList[$#tempList];

    $tmpError = system("cp ".$Hash_pmbss{BND}." ".$ProjectName."/"); if ($tmpError) {die "cannot find .bnd file";}
    @tempList=split(/\//,$Hash_pmbss{BND});
    $Hash_pmbss{BND}=$tempList[$#tempList];
    
    if (-r $LibName.".pm") 
    {
	$tmpError = system("cp ".$LibName.".pm ".$ProjectName."/"); if ($tmpError) {die "cannot find PreMaBoSS.pm file";} #copy library file in project directory
    }
    my $BndFile=$Hash_pmbss{BND};
    ###############################################################################################
    my $Init_cond_line="";
    if (defined $Hash_pmbss{INIT_COND})    #construct line of initial condition from probtraj file
    {
	@tempList=split(/\s*\[\s*/,$Hash_pmbss{INIT_COND});
	@tempList=split(/\s*\]\s*/,$tempList[1]);
	@tempList=split(/\s*\,\s*/,$tempList[0]);
	$Init_cond_line=Init_Cond_Traj($tempList[0],$tempList[1],$tempList[2]); #contruct the line of initial condition from specified file
    }
    chdir($ProjectName); # go to project name directory
    my $CfgBaseFile=($Hash_pmbss{CFG});
    my @CfgFileList;
    if (length($Init_cond_line) > 0){$CfgBaseFile=Constr_CFG_InitCond($CfgBaseFile,$Init_cond_line)} # construct new cfg file, with initial condition from file
    ############################################################################
    if (defined $Hash_pmbss{MUT}) 
    {
	if (defined($Hash_pmbss{COMB_MUT})) {@CfgFileList=Node_Mut($Hash_pmbss{BND},$Hash_pmbss{MUT},$Hash_pmbss{COMB_MUT},$CfgBaseFile);}
	else {@CfgFileList=Node_Mut($Hash_pmbss{BND},$Hash_pmbss{MUT},1,$CfgBaseFile);} #default value fo mut combinations is 1
	$_=$BndFile;
	s/\.bnd/_mut\.bnd/;
	$BndFile=$_;	    
    } #construct mutation bnd file and mutation cfg files
    else {@CfgFileList=($CfgBaseFile);}
    ############################################################################################
    my %VarSensHash; #construct variable sensitivity cfg, by adding suffix to external variables
    if (defined $Hash_pmbss{VAR_SENS})
    {
	my $comb_var_sens;
	if (defined($Hash_pmbss{"COMB_VAR_SENS"})) {$comb_var_sens=$Hash_pmbss{"COMB_VAR_SENS"};}
	else {$comb_var_sens=1;} #default value of sensitivity variable combinations is 1;
	@tempList=split(/\s*;/,$Hash_pmbss{VAR_SENS});
	my @SensList=split(/\s*\]\s*\[\s*/,$tempList[0]);
	foreach $_ (@SensList)
	{
	    s/\s*\[\s*//;
	    s/\s*\]\s*//;
	    my @tmpList=split(/\s*\,\s*/,$_);
	    $VarSensHash{$tmpList[0]}=$tmpList[1];
	}
	my @CfgFileList4VarSens=@CfgFileList;
	my @CfgListIndexVar;
	foreach my $tmp (@CfgFileList4VarSens) {@CfgListIndexVar=(@CfgListIndexVar,-1);}
	my @KeyListVar4Comb=keys(%VarSensHash);
	for(my $Comb=1;$Comb<=$comb_var_sens;$Comb++)  #loop over combination of variable sentitivities
	{
	    my @tmpCfgFileList4VarSens;
	    my @tmpCfgListIndexVar;
	    for(my $IndexFile=0;$IndexFile<=$#CfgFileList4VarSens;$IndexFile++) #loop over list of cfg files
	    {
		for(my $IndexVar=$CfgListIndexVar[$IndexFile]+1;$IndexVar<=$#KeyListVar4Comb;$IndexVar++) #loop over external variables considered for sensitivity
		{
		    my $Suffix=$VarSensHash{$KeyListVar4Comb[$IndexVar]};
		    $_=$KeyListVar4Comb[$IndexVar];
		    s/\$//;
		    my $Var=$_;
		    my $NewFileName=InsertSuffCfg($CfgFileList4VarSens[$IndexFile],$Var,$Suffix);
		    @tmpCfgFileList4VarSens=(@tmpCfgFileList4VarSens,$NewFileName);
		    @CfgFileList=(@CfgFileList,$NewFileName);
		    @tmpCfgListIndexVar=(@tmpCfgListIndexVar,$IndexVar);
		}
	    }
	    @CfgFileList4VarSens=@tmpCfgFileList4VarSens;
	    @CfgListIndexVar=@tmpCfgListIndexVar;
	}
    }
    ###################################################################################################
    open(PROJECTSH,">".$ProjectName.".sh"); #create shell script, that lauch MaBoSS and create tables
    print PROJECTSH  "#!/bin/bash\n";
    foreach my $File (@CfgFileList)
    {
	$_=$File;
	s/.cfg//;
	print PROJECTSH $Hash_pmbss{MABOSS}." -c ".$File." -o ".$_." ".$BndFile."\n";
	print PROJECTSH "echo \"".$File."\";\n";
    }
    if (defined $Hash_pmbss{TRAJ_TABLE}) #probtraj_table in shell script
    {
	if ($Hash_pmbss{TRAJ_TABLE} eq "yes")
	{
	    foreach my $File (@CfgFileList)
	    {
		$_=$File;
		s/.cfg/_probtraj\.csv/;
		print PROJECTSH "perl -M".$LibName." -e \"".$LibName."::ProbTraj2Table(\'".$_."\')\";\n";
	    }	
	}
	#new for MBSS_PrepSim
	elsif ($Hash_pmbss{TRAJ_TABLE} eq "fig")
	{
	   foreach my $File (@CfgFileList)
	    {
		$_=$File;
		s/.cfg/_probtraj\.csv/;
		print PROJECTSH "perl -M".$LibName." -e \"".$LibName."::ProbTraj2Table(\'".$_."\')\";\n";
		$_=$File;
		s/.cfg//;
		print PROJECTSH "MBSS_PrepareProjectFilePieChart.py ".$_."\n";
		print PROJECTSH "MBSS_PrepareProjectFileTrajectoryFig.py ".$_."\n";
	    }	
	}   
    }
    if (defined $Hash_pmbss{STAT_TABLE}) #statdist_table in shell script
    {
	@tempList=split(/\[/,$Hash_pmbss{STAT_TABLE});
	@tempList=split(/\]/,$tempList[1]);
	@tempList=split(/\,/,$tempList[0]);	
	if ($tempList[0] eq "yes")
	{
	  foreach my $File (@CfgFileList)
	    {
		$_=$File;
		s/.cfg/_statdist\.csv/;
		print PROJECTSH "perl -M".$LibName." -e \"".$LibName."::StatDist2Table(\'".$_."\',".$tempList[1].")\";\n";
	    }	  
	}

    }
    close(PROJECTSH);
    system("chmod a+x ".$ProjectName.".sh");
}
##############################################################
sub HashStr_pmbss #construct hash table from pmbss file
{
    my $pmbssFile = $_[0];
    open(PMBSSF,$pmbssFile) or die "cannot find pmbss file";
    my %HashStrpmbss;
    while (<PMBSSF>)
    {
	my @tmplist=split(/\s*;/);
	my @LineStr=split(/\s*=\s*/,$tmplist[0]);
	$_=($LineStr[0]);  
	{
	    if (/^MABOSS$/) {
		if (defined $HashStrpmbss{"MABOSS"}) {die "MABOSS should appear only once";} 
		else {$HashStrpmbss{"MABOSS"} = $LineStr[1];}}
	    if (/^CFG$/) {
		if (defined $HashStrpmbss{"CFG"}) {die "CFG should appear only once";} 
		else {$HashStrpmbss{"CFG"} = $LineStr[1];}}
	    if (/^BND$/) {
		if (defined $HashStrpmbss{"BND"}) {die "BND should appear only once";} 
		else {$HashStrpmbss{"BND"} = $LineStr[1];}}
	    if (/^INIT_COND$/) {
		if (defined $HashStrpmbss{"INIT_COND"}) {die "INIT_COND should appear only once";} 
		else {$HashStrpmbss{"INIT_COND"} = $LineStr[1];}}
	    if (/^MUT$/) {
		if (defined $HashStrpmbss{"MUT"}) {die "MUT should appear only once";} 
		else {$HashStrpmbss{"MUT"} = $LineStr[1];}}
	     if (/^COMB_MUT$/) {
		if (defined $HashStrpmbss{"COMB_MUT"}) {die "COMB_MUT should appear only once";} 
		else {$HashStrpmbss{"COMB_MUT"} = $LineStr[1];}}
	    if (/^VAR_SENS$/) {
		if (defined $HashStrpmbss{"VAR_SENS"}) {die "VAR_SENS should appear only once";} 
		else {$HashStrpmbss{"VAR_SENS"} = $LineStr[1];}}
	    if (/^COMB_VAR_SENS$/) {
		if (defined $HashStrpmbss{"COMB_VAR_SENS"}) {die "COMB_VAR_SENS should appear only once";} 
		else {$HashStrpmbss{"COMB_VAR_SENS"} = $LineStr[1];}}
	    if (/^TRAJ_TABLE$/) {
		if (defined $HashStrpmbss{"TRAJ_TABLE"}) {die "TRAJ_TABLE should appear only once";} 
		else {$HashStrpmbss{"TRAJ_TABLE"} = $LineStr[1];}}
	    if (/^STAT_TABLE$/) {
		if (defined $HashStrpmbss{"STAT_TABLE"}) {die "STAT_TABLE should appear only once";} 
		else {$HashStrpmbss{"STAT_TABLE"} = $LineStr[1];}}

	} #Fill Hash table
    }
    close(PMBSSF);
    return(%HashStrpmbss);
}
###########################################################################################################################
sub Init_Cond_Traj #Line of initial condition from file at given line number. BND file is used to define the list of nodes.
{
    my $BndFile=$_[0];
    my $TrajFile=$_[1];
    my $LineNumber=$_[2];
    open(BND,$BndFile) or die "cannot find .bnd file";
    my @NodeList;
    while(<BND>) #find the list of nodes
    {
	if ( (/node/) || (/Node/)) #Hmmm, don't consider NODE or nODE?
	{
	    s/node//;
	    s/Node//;
	    s/\{.*//;
	    s/\s+//g;
	    @NodeList=(@NodeList,$_);
	}
    }
    close(BND);
    open(TRJF,$TrajFile) or die "cannot find trajectory file";
    for (my $i=0;$i <= $LineNumber;$i++) {$_=<TRJF> or die "trajectory file is empty";} #line number does not include header of probtraj
    close(TRJF);
    my @LineList=split(/\t/);
    my $LineInitCond="[".$NodeList[0];
    foreach (@NodeList[1..$#NodeList]) {$LineInitCond=$LineInitCond.",".$_;}
    $LineInitCond=$LineInitCond."].istate =";
    for (my $index=0;$index<=$#LineList;$index++) 
    {
	$_=$LineList[$index];  
#	if (/[A-Z,a-z]/) 
	if ((/^[A-Z,a-z]/) || (/nil/))
	{
	    $LineInitCond=$LineInitCond.$LineList[$index+1]." [";
#	    if (/$NodeList[0]/) {$LineInitCond=$LineInitCond."1";}
	if (($_ eq $NodeList[0]) || /^$NodeList[0] / || / $NodeList[0] / || / $NodeList[0]$/) 
	{$LineInitCond=$LineInitCond."1";}
	    else {$LineInitCond=$LineInitCond."0";}
	    foreach my $Node (@NodeList[1..$#NodeList])
	    {
		$LineInitCond=$LineInitCond.",";
#		if (/$Node/) {$LineInitCond=$LineInitCond."1";}
		if (($_ eq $Node) || /^$Node / || / $Node / || / $Node$/) {$LineInitCond=$LineInitCond."1";}
		else {$LineInitCond=$LineInitCond."0";}
	    }
	    $LineInitCond=$LineInitCond."] , ";	
	}
    }
    chop($LineInitCond);
    chop($LineInitCond);
    $LineInitCond=$LineInitCond.";\n";
    return($LineInitCond);
}
#################################################################################################################
sub Node_Mut #Contruct a new BND file (with "_mut" at the end, return the list of controlling mutation variables) 
    # and the set of cfg files. Return the list of cfg files, including the original one, accordig to number of
    #mutation combinaitions
{
    my $MaxRate="1.7976931348623157E+308"; #maximum rate in bnd to force immediate transition
    my $bnd_file=$_[0];
    my @node_list=split(/\s+/,$_[1]);
    
    $MaxRate=$MaxRate."/".($#node_list + 1);

    my @node_catched_list; #node list obtained by catching $node_list in .bnd file
    my $mut_comb=$_[2]; #number of mutation combinations
    my $CfgFile=$_[3];
    my @RetCfgFileList; #list of all cfg, including the NoMut one
    my @MutVarList;
    ######################################################################
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
		    @node_catched_list=(@node_catched_list,$node);
		    @MutVarList=(@MutVarList,"\$High_".$node,"\$Low_".$node);
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
		    {splice(@MutBNDLineList,$#MutBNDLineList-1,0,"\t rate_up = ( \$Low_".$node." ? 0.0 : ( \$High_".$node." ? \@max_rate : (\@logic ? 1.0 : 0.0 )));\n");
			#if rate_up is absent, create it. Low_node wins
		    }
		    if ($rate_down_flag==0)
		    {splice(@MutBNDLineList,$#MutBNDLineList-1,0,"\t rate_down = ( \$Low_".$node." ? \@max_rate : ( \$High_".$node." ? 0.0 : (\@logic ? 0.0 : 1.0 ))) ;\n");
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
    foreach my $line (@MutBNDLineList) {print BND_F_MUT $line;}
    close(BND_F_MUT);
    ###################################################
    my @CfgLines;
    open(CFG,$CfgFile) or die "cannot open .cfg file";
    while(<CFG>){push(@CfgLines,$_);}
    close(CFG);
    ######################################################    
    $_=$CfgFile; #construct cfg file with no mutations
    s/\.cfg//;
    open(NFCFG,">".$_."_NoMut.cfg");
    foreach my $tmpLine (@CfgLines){print NFCFG $tmpLine;}
    foreach my $MutVar (@MutVarList){print NFCFG $MutVar."=0;\n";}
    close(NFCFG);
    @RetCfgFileList=($_."_NoMut.cfg");
    ##############################################################
    my @cfg_file_list4comb=@RetCfgFileList; #contruct the combinaition of mutations
    for (my $i_comb_mut=1;$i_comb_mut<=$mut_comb;$i_comb_mut++)
    {
	my @new_cfg_file_list4comb;
	foreach my $file (@cfg_file_list4comb)
	{
	    $_=$file;
	    s/.cfg//;
	    s/_NoMut//;
	    my $filePrefix=$_;
	    my $index_node4mut = 0;
	    if ((/High/) || (/Low/))
	    {
		# s/High_//g;
		# s/Low_//g;
		my @SplitFileName=split(/_High_|_Low_/);
		for(my $nodeindex = 0;$nodeindex<=$#node_catched_list;$nodeindex++)
		{if ($node_catched_list[$nodeindex] eq $SplitFileName[$#SplitFileName]) {$index_node4mut=$nodeindex+1;}} 
	    }
	    foreach my $node4mut (@node_catched_list[$index_node4mut..$#node_catched_list])
	    {
		my $NewCfgFile=$filePrefix."_High_".$node4mut.".cfg";
		@new_cfg_file_list4comb=(@new_cfg_file_list4comb,$NewCfgFile);
		open(NHFCFG,">".$NewCfgFile);
		$NewCfgFile=$filePrefix."_Low_".$node4mut.".cfg";
		open(NLFCFG,">".$NewCfgFile);
		@new_cfg_file_list4comb=(@new_cfg_file_list4comb,$NewCfgFile);
		open(FCFG,$file);
		while(<FCFG>)
		{
		    my $tmp1="High_".$node4mut;
		    my $tmp2="Low_".$node4mut;
		    if (/$tmp1/) {print NLFCFG $_;s/=0/=1/;print NHFCFG $_;}
		    elsif (/$tmp2/) {print NHFCFG $_;s/=0/=1/;print NLFCFG $_;}
		    else {print NLFCFG $_;print NHFCFG $_;}    
		}
		close(NHFCFG);
		close(NLFCFG);
		close(FCFG);
	    }
	}
	@cfg_file_list4comb=@new_cfg_file_list4comb;
	@RetCfgFileList=(@RetCfgFileList,@new_cfg_file_list4comb);
    }
    return(@RetCfgFileList);
}
##########################################################################
sub Constr_CFG_InitCond #contruct cfg file inserting intial condition line
{
    my $FileName=$_[0];
    my $istate_line=$_[1];
    $_=$FileName;
    s/\.cfg//;
    my $NewFileName=$_."_InitFrTraj.cfg";
    open(FCFG,$FileName) or die "cannot file .cfg file";
    open(NFCFG,">".$NewFileName);
    while(<FCFG>) {if (!(/istate/)) {print NFCFG $_;} }# remove all previous istate lines from .cfg
    print NFCFG $istate_line."\n";
    close(FCFG);
    close(NFCFG);
    return($NewFileName);
}
######################################################################
sub InsertSuffCfg #Insert suffix to an external variable in a cfg file
{
    my $cfg_file=$_[0];
    my $ExtVar=$_[1];
    #print "External variable :".$ExtVar."\n";
    my $mod_string=$_[2];
    $mod_string=$mod_string.";";
    $_=$cfg_file;
    s/.cfg//;
    my $cfg_name=$_;
    my @CfgLineList;
    my $CfgHitLineIndex=-1;
    open(CFG_F,$cfg_file) or die "cannot find .cfg file";
    my $LineIndex=0;
    while(<CFG_F>)
    {
	if (/^\$$ExtVar\s*=/) {$CfgHitLineIndex=$LineIndex;}
	@CfgLineList=(@CfgLineList,$_);
	$LineIndex++;	    
    }
    if ($CfgHitLineIndex == -1) {die "Cannot fine variable ".$ExtVar."\n";}
    close(CFG_F);
    my $tmpCfgFile=$cfg_name."_".$ExtVar.".cfg";
    open(CFG_F,">".$tmpCfgFile);
    for ($LineIndex=0;$LineIndex<$CfgHitLineIndex;$LineIndex++) {print CFG_F $CfgLineList[$LineIndex];}
    $_=$CfgLineList[$CfgHitLineIndex];
    my @tmpLine=split(/=/,$CfgLineList[$CfgHitLineIndex]);
    if ($#tmpLine > 1) {die "strange variable definition";}
    $_=$tmpLine[1];
    $mod_string = ")".$mod_string;
    s/;/$mod_string/;
    print CFG_F $tmpLine[0]."=(".$_;
    for ($LineIndex=$CfgHitLineIndex+1;$LineIndex<=$#CfgLineList;$LineIndex++) {print CFG_F $CfgLineList[$LineIndex];}
    close(CFG_F);
    return($tmpCfgFile);
}
###############################################################################
sub ProbTraj2Table  #contruct the probtraj_table file from the protraj file
{
    my $ProbTrajFile=$_[0];
    open(ProbTraj,$ProbTrajFile) or die "cannot find trajectory file";
    my $line=<ProbTraj>;
    my @firstlinelist=split(/\t/,$line);
    my $name;
    my $IndexProba=4;
    my @firstline_woutH=@firstlinelist[4..$#firstlinelist];
    foreach $name (@firstline_woutH)
    {
	$_=$name;
	if (! m/HD=/) {last;}
	$IndexProba++;
    }
    my $IndexLine=0;
    $line=<ProbTraj>;
    chomp($line);
    my @linelist=split(/\t/,$line);
    my %FullGlobal;   #Hash table of time, TH, ErrTH and H
    $FullGlobal{Time}[$IndexLine]=$linelist[0];
    $FullGlobal{TH}[$IndexLine]=$linelist[1];
    $FullGlobal{ErrTH}[$IndexLine]=$linelist[2];
    $FullGlobal{H}[$IndexLine]=$linelist[3];
    my %ProbState;   #Hash table of probabilities
    my %ErrProbState; #Hash table of probability errors
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
    ############################################################################  
    my @StateList=keys(%ProbState); #write the trajectory table in file
    $_=$ProbTrajFile;
    s/\.csv/_table\.csv/;
    open(TTRAJ,">".$_) or die "cannot create probtraj_table file";
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
	foreach my $State (@StateList) {print TTRAJ $ProbState{$State}[$i]."\t".$ErrProbState{$State}[$i]."\t";}
	print TTRAJ "\n";
    }
    close(TTRAJ);   
}
#################################################################################################
sub StatDist2Table   #contruct the statdist_table file from the statdist file
{
    my $StatDistFile=$_[0];
    my $ProbThreshold=$_[1];
    open(StatDist,$StatDistFile) or die "cannot find statdist file";
    my @StatDistProb; #Probability of the cluster/indecompsable stationary distribution
    my $NbTraj=0; #Number of trajectories used for clustering stationary distribution
    while(<StatDist>) #Go the the clusters, find the number of trajectories for each cluster
    {
	if (m/Cluster/) {last;}
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
    for(my $i=0;$i<=$#StatDistProb;$i++) {$StatDistProb[$i]= $StatDistProb[$i]/$NbTraj;}    
    $_=$StatDistFile;
    s/\.csv/_table\.csv/;
    open(TSTAT,">".$_) or die "cannot create statdist_table file";
    while(<StatDist>)
    {
	if (!(m/^#/)) {last;}
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
	{print TSTAT $ClusterProb{$State}."\t";}
	print TSTAT "\n";
	print TSTAT "ErrorProb\t";
	foreach my $State (@StateList)
	{print TSTAT $ClusterError{$State}."\t";}
	print TSTAT "\n\n";
    }
    close(TSTAT);
    close(StatDist);
}
1;
