#!/usr/bin/perl -w
use strict;

my @CommandLine=@ARGV;

if ($#CommandLine < 2) {die "Bad argument, UpdatePop.pl <file.bnd> <file.cfg> <file.upp>";}
my $bndFile=$CommandLine[0];
my $cfgFile=$CommandLine[1];
my $uppFile=$CommandLine[2];

open(BND,$bndFile) or die "cannot find .bnd file"; #Construct list of nodes from .bnd
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

my $division_node="";
my $death_node="";
my $MaBoSS_exec="";
my %UpdateVar;
my $PopRatio=1;
my $StepNumber=-1;

open(UPPF,$uppFile) or die "cannot find upp file"; #parse .upp file
while(<UPPF>)
{
    s/\s//g;
    if (/^death/)
    {
	if ($death_node ne "") {die "Multiple definition of death node";}
	else
	{
	    my @SplittedLine=split(/=/);
	    $_=$SplittedLine[1];
	    s/;//;
	    s/\s+//g;
	    $death_node = $_;
	    print "Death node: ".$death_node."\n";
	}
    }
    if (/^division/)
    {
	if ($division_node ne "") {die "Multiple definition of division node";}
	else
	{
	    my @SplittedLine=split(/=/);
	    $_=$SplittedLine[1];
	    s/;//;
	    s/\s+//g;
	    $division_node = $_;
	    print "Division node: ".$division_node."\n";
	}
    }
    if (/^steps/)
    {
	if ($StepNumber != -1) {die "Multiple definition of division node";}
	else
	{
	    my @SplittedLine=split(/=/);
	    $_=$SplittedLine[1];
	    s/;//;
	    s/\s+//g;
	    $StepNumber = $_;
	    print "Number of steps: ".$StepNumber."\n";
	}
    }
    if (/^MaBoSS/)
    {
	if ($MaBoSS_exec ne "") {die "Multiple definition of MaBoSS exectuable";}
	else
	{
	    my @SplittedLine=split(/=/);
	    $_=$SplittedLine[1];
	    s/;//;
	    s/\s+//g;
	    $MaBoSS_exec = $_;
	    print "MaBoSS executable: ".$MaBoSS_exec."\n";
	}
    }
    if (/^\$/)   #external variables that need to be updated
    {
	 my @SplittedLine=split(/u=/); #careful, "u=" is the equality sign for variable update
	 $_=$SplittedLine[0];
	 s/\s+//g;
	 my $VarName=$_;
	 $_=$SplittedLine[1];
	 s/;//;
	 s/\s+//g;
	 if (defined $UpdateVar{$VarName}){die "Multiple defintion of ".$VarName;}
	 else 
	 {
	     $UpdateVar{$VarName}=$_;
	     print "Var ".$VarName." updated by value ".$_."\n";
	 }
    }
}
if ($MaBoSS_exec eq "") {$MaBoSS_exec = "./MaBoSS";}
close(UPPF); 

$_=$cfgFile; 
s/\.cfg//;
my $OutName=$_;
open(PopRatioF,">".$OutName."_PopR.csv"); #population ratio file
open(ResumeFile,">".$OutName."_PopProbTraj.csv");  #population trajectory file 
print PopRatioF "Step\t"."PopRatio\n";
print "Run MaBoSS step 0\n";
system($MaBoSS_exec." -c ".$cfgFile." -o ".$OutName."_Step_0 ".$bndFile); #Run of step 0
print PopRatioF "0\t".$PopRatio."\n";
open(FirstPTrjF,$OutName."_Step_0_probtraj.csv");
$_=<FirstPTrjF>;
s/^[^\t]*\t//;
print ResumeFile "Step\t"."PopRatio\t".$_;
close(FirstPTrjF);

my $cfgFileStep=$cfgFile;
for(my $StepIndex = 1;$StepIndex <= $StepNumber;$StepIndex++)  #loop of MaBoSS run
{
    my $LastLinePrevTraj;  #read last line of previous trajectory file
    open(PrStepTrajF,$OutName."_Step_".($StepIndex-1)."_probtraj.csv");
    while(<PrStepTrajF>){$LastLinePrevTraj=$_}
    close(PrStepTrajF);

    $PopRatio=$PopRatio*UpdatePopRatio($LastLinePrevTraj,$death_node,$division_node); #update popratio
    print PopRatioF $StepIndex."\t".$PopRatio."\n";
    my $Line4ResFile = $LastLinePrevTraj;
    $Line4ResFile =~ s/^[^\t]*\t//;
    print ResumeFile $StepIndex."\t".$PopRatio."\t".$Line4ResFile;

    $cfgFileStep=BuildUpdateCfg($cfgFileStep,$LastLinePrevTraj,$death_node,$division_node,\@NodeList,\%UpdateVar,$PopRatio); #build new .cfg
    if ($cfgFileStep eq "")
    {
	print "No cells left\n";
	last;
    }
    else
    {
	print "Running MaBoSS for step ".$StepIndex."\n";
	system($MaBoSS_exec." -c ".$cfgFileStep." -o ".$OutName."_Step_".$StepIndex." ".$bndFile); #run MaBoSS
    }
}
close(PopRatioF);
close(ResumeFile);
###############################################
sub UpdatePopRatio #update population ratio based on death node and division node probability
{
    my $ProbTrajLine=$_[0];
    my $death_node=$_[1];
    my $division_node=$_[2];
    my $UpPopRatio=0;
    
    my @ProbTrajList=split(/\t/,$ProbTrajLine);  #find index of first state
    my $IndexStateTrajList=-1;
    foreach (@ProbTrajList)
    {
	$IndexStateTrajList++;
	if ((/^[A-Z,a-z]/) || (/nil/)) {last;}
    }
    
    for (my $i=$IndexStateTrajList;$i<=$#ProbTrajList;$i=$i+3)
    {
	$_=$ProbTrajList[$i];
	if (!(($_ eq $death_node) || /^$death_node / || / $death_node / || / $death_node$/ ))
	{
	  if (($_ eq $division_node) || /^$division_node / || / $division_node / || / $division_node$/ )
	  {$UpPopRatio = $UpPopRatio + 2*$ProbTrajList[$i+1];}
	  else {$UpPopRatio = $UpPopRatio + $ProbTrajList[$i+1];}
	}
    }
    return($UpPopRatio);
}
#################################################
sub BuildUpdateCfg #Create new .cfg file based on last line of probtraj file
{
    my $LastCfgFile=$_[0];
    my $ProbTrajLine=$_[1];
    my $death_node=$_[2];
    my $division_node=$_[3];
    my @NodeList=@{$_[4]};
    my %UpdateVar=%{$_[5]};
    my $PopRatio=$_[6];
    
    ########update ProbTrajLine####################
    my @ProbTrajListFull=split(/\t/,$ProbTrajLine);  #remove global numbers before first state
    my @ProbTrajList=@ProbTrajListFull;
    foreach (@ProbTrajListFull)
    {if ((/^[A-Z,a-z]/) || (/nil/)) {last;} else {shift(@ProbTrajList);}}
    
    my $NormFactor=0; #Factor for probability normalization after death and division node update
    my $DeathProb=0;
    my $DivisionProb=0;
    for (my $i=0;$i<=$#ProbTrajList;$i=$i+3) #update prob according to death and division nodes
    {
	$_=$ProbTrajList[$i];
	if ((($_ eq $death_node) || /^$death_node / || / $death_node / || / $death_node$/ ))
	{
	    $DeathProb+=$ProbTrajList[$i+1];
	    $ProbTrajList[$i+1] = 0;
	}
	else
	{
	  if (($_ eq $division_node))
	  {
	      $DivisionProb+=$ProbTrajList[$i+1];
	      $ProbTrajList[$i+1] = 2*$ProbTrajList[$i+1];
	      $ProbTrajList[$i] = "<nil>";	  
	  }
	  elsif (/^$division_node /)
	  {
	      
	      $DivisionProb+=$ProbTrajList[$i+1];
	      $ProbTrajList[$i+1] = 2*$ProbTrajList[$i+1];
	      $ProbTrajList[$i] =~ s/^$division_node -- //;	  
	  }
	  elsif (/ $division_node /)
	  {
	      $DivisionProb+=$ProbTrajList[$i+1];
	      $ProbTrajList[$i+1] = 2*$ProbTrajList[$i+1];
	      $ProbTrajList[$i] =~ s/ -- $division_node//;	  
	  }
	  elsif (/ $division_node$/)
	  {
	      
	      $DivisionProb+=$ProbTrajList[$i+1];
	      $ProbTrajList[$i+1] = 2*$ProbTrajList[$i+1];
	      $ProbTrajList[$i] =~ s/ -- $division_node$//;	  
	  }
	  $NormFactor=$NormFactor+$ProbTrajList[$i+1];
	}
    }

    print "Norm Factor:".$NormFactor." probability of death: ".$DeathProb." probability of division: ".$DivisionProb."\n";
    
    if ($NormFactor == 0){return "";} #All cells are dead
    else #########Construct new .cfg file name##############
    {
	for (my $i=0;$i<=$#ProbTrajList;$i=$i+3) #Normalize probabilities
	{$ProbTrajList[$i+1]=$ProbTrajList[$i+1]/$NormFactor;}

	my $NewCfgFile; #construct new .cfg name
	my @tmpLine=split(/_Step_/,$LastCfgFile);
	if ($#tmpLine > 1) {die "Do not use the word \"Step\" in the name of the .cfg file";}
	if ($#tmpLine == 0)
	{
	    $_=$LastCfgFile;
	    s/\.cfg//;
	    $NewCfgFile=$_."_Step_1.cfg";
	}
	else
	{
	    $_=$tmpLine[1];
	    s/\.cfg//;
	    $NewCfgFile=$tmpLine[0]."_Step_".($_+1).".cfg";
	}
	
	open(LastCFG,$LastCfgFile);
	open(NewCFG,">".$NewCfgFile);
	while(<LastCFG>)
	{
	    if (!/istate/) #remove all initial condition
	    {
		my $UpVar="";
		foreach my $TestUpVar (keys(%UpdateVar)) #Search for line starting with var that needs to be updated
		{
		    $TestUpVar =~ s/\$//;
		    if (/^\$$TestUpVar/) 
		    {$UpVar="\$".$TestUpVar;}
		}
		if ($UpVar eq "")
		{print NewCFG $_;}
		else
		{
			my $UpdateVarLine=VarDef_Upp($UpdateVar{$UpVar},\@ProbTrajList); #update variables
			foreach my $tmp ($UpdateVarLine =~ /#rand/) #random number
			{
			    my $RNumber = rand(1);
			    $UpdateVarLine =~ s/#rand/$RNumber/;
			}
			foreach my $tmp ($UpdateVarLine =~ /#pop_ratio/) #pop ratio
			{  $UpdateVarLine =~ s/#pop_ratio/$PopRatio/;}
			print NewCFG $UpVar." = ".$UpdateVarLine."\n";
			print "Updated variable: ".$UpVar." = ".$UpdateVarLine."\n";
		}
	    }
	    
	}
	close(LastCFG);
	
	my $InitLine=InitCond_TrajLine(\@ProbTrajList,\@NodeList);#write initial condtion in last line of new .cfg#
	print NewCFG $InitLine;
	close(NewCFG);
	return($NewCfgFile);
    }
}

################################################################

sub InitCond_TrajLine  #Line of initial condition from probtraj file line (in list format) and list of nodes.
{
    my @ProbTrajList=@{$_[0]};
    my @NodeList=@{$_[1]};

    my $LineInitCond="[".$NodeList[0];
    foreach (@NodeList[1..$#NodeList]) {$LineInitCond=$LineInitCond.",".$_;}
    $LineInitCond=$LineInitCond."].istate =";
    for (my $index=0;$index<=$#ProbTrajList;$index = $index + 3) 
    {
	$_=$ProbTrajList[$index]; 
	$LineInitCond=$LineInitCond.$ProbTrajList[$index+1]." [";
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
    chop($LineInitCond);
    chop($LineInitCond);
    $LineInitCond=$LineInitCond.";\n";
    return($LineInitCond);
}

#####################
sub VarDef_Upp  
{
    my $UpdateLine=$_[0];
    my @ProbTrajList=@{$_[1]};
    
    my @ProbMatchList= $UpdateLine =~ /(p\[[^\[]*\])/g; #match probability p[(node,node,...)=(bool,bool,...)]
    
    foreach my $ProbMatch (@ProbMatchList)
    {
	$ProbMatch =~ s/\s+//g; #remove spaces and brackets
	my @StateVal=split(/=/,$ProbMatch);
	$StateVal[0] =~ s/p\[//g;
	$StateVal[0] =~ s/\]//g;
	$StateVal[0] =~ s/\(//g;
	$StateVal[0] =~ s/\)//g;
	$StateVal[1] =~ s/\[//g;
	$StateVal[1] =~ s/\]//g;
	$StateVal[1] =~ s/\(//g;
	$StateVal[1] =~ s/\)//g;
	
	my @NodeList=split(/,/,$StateVal[0]);
	my @BoolValList=split(/,/,$StateVal[1]);
	if ($#NodeList != $#BoolValList) {die "Wrong probability definitions for \"".$ProbMatch."\"\n";}

	my @UpNodeList;#find active and unactive nodes
	my @DownNodeList;
	for (my $i = 0;$i <= $#NodeList;$i++)
	{
	    if ($BoolValList[$i] == 0) {push(@DownNodeList,$NodeList[$i]);}
	    else {push(@UpNodeList,$NodeList[$i]);}
	}
	
	my $ProbValue = 0;
	for (my $i = 0;$i <= $#ProbTrajList;$i=$i+3)
	{
	    my @UpNodeProbTraj=split(/ -- /,$ProbTrajList[$i]);
	    my $Interlength=0;
	    foreach my $UpNodePT (@UpNodeProbTraj) #search intersection between active nodes
	    {foreach my $UpNode (@UpNodeList)
	     {if ($UpNodePT eq $UpNode) {$Interlength++;}}}
	    if ($Interlength == ($#UpNodeList+1))
	    {
		$Interlength=0;
		foreach my $UpNodePT (@UpNodeProbTraj) #compare active node in prob traj and unactive node in proba
		{
		    foreach my $DownNode (@DownNodeList)
		    {
			if ($UpNodePT eq $DownNode)
			{
			    $Interlength=1;
			    last;
			}
		    }
		    if ($Interlength==1) {last;}
		}
		if ($Interlength == 0) {$ProbValue=$ProbValue + $ProbTrajList[$i+1];}
	    }
	}
	
	$ProbMatch =~ s/\[/\\[/g;  #brackets for regexp
	$ProbMatch =~ s/\]/\\]/g;
	$ProbMatch =~ s/\(/\\(/g;
	$ProbMatch =~ s/\)/\\)/g;
	$UpdateLine =~ s/$ProbMatch/$ProbValue/;
    }
    $UpdateLine=$UpdateLine.";";
    return($UpdateLine);
}
