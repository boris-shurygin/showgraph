#!/usr/bin/perl

##
# @file: perf.pl
# performace testing script for console version of ShowGraph
#
##
# Testing in ShowGraph tool.
# Copyright (C) 2010  Boris Shurygin
##

use Time::HiRes qw( gettimeofday);
use Data::Dumper;
use Cwd 'abs_path';
use Cwd 'getcwd';

#Debug print control
my $print_conf = 
{ 
    TESTS => 1,
    RUN => 1,
    CHECKING => 1,
    CONFIG => 1
};

#MAIN
dprint( RUN, "Running performance testing");

#Initialization
my $info = initInfo();
exit 1 unless checkConfig( $info);

#Testing
runTests( $info);

#Print resutls and report complition
printResults( $info);
dprint( RUN, "Performance testing complete");
exit 0;

#Set configuration
sub initInfo
{
    my $info = {};
    
    $info->{results} = {};
    $info->{tests} = tests();
    $info->{cwd} = abs_path( getcwd());
    $info->{bindir} = abs_path( addPath( $info->{cwd}, "..\\bin"));
    $info->{testdir} = abs_path( addPath( $info->{cwd}, "..\\testing\\tests"));
    dprint( CONFIG, "Work directory is: ".$info->{cwd});
    dprint( CONFIG, "Binaries directory is: ".$info->{bindir});
    dprint( CONFIG, "Binaries directory is: ".$info->{testdir});
    
    #should be OS-dependent
    $info->{showgraph} = addPath( $info->{bindir},"showgraphcld.exe");
    
    #iteration number for taking average runtime to get more stable results
    $info->{iterations} = 1;
    return $info;
}

#Set test names and input files' names
sub tests
{
    my $tests = 
    {
        'test1' => 'test1.xml',
        'buildPar_gcc' => 'buildPar_gcc4.xml',
        'main_gcc' => 'main_gcc4.xml',
        'disasm_gcc' => 'disasm_gcc4.xml',
        'buildPar_icc' => 'buildPar_icc.xml',
        'main_icc' => 'main_icc.xml',
        'disasm_icc' => 'disasm_icc.xml'
    };
    
}

#Check configuration: build and test set integrity
sub checkConfig
{
    my ( $info) = @_;
    
    return ( checkBinaries( $info) && checkTests( $info));
}

#Check that all directories and binaries are present
sub checkBinaries
{
    my ( $info) = @_;
    
    #Check binaries' directory
    if ( ! ( -d $info->{bindir}))
    {
        dprint( CHECKING, "Missing binaries dir \'".$info->{bindir}."\'");
        return 0;
    }
    #check showgraph binary
    if ( ! ( -x $info->{showgraph}))
    {
        dprint( CHECKING, "Missing showgraph executable \'".$info->{bindir}."\'");
        return 0;
    }
    
    return 1;
}

#Check that all test are in place
sub checkTests
{
    my ( $info) = @_;
    
    #Check test directory
    if ( !( -d $info->{testdir}))
    {
        dprint( CHECKING, "Missing test dir \'".$info->{testdir}."\'");
        return 0;
    }    
    
    #Check tests
    foreach my $test ( keys %{$info->{tests}})
    {
        my $filename = $info->{tests}->{$test};
        if ( !( -f addPath( $info->{testdir}, $filename)))
        {
            dprint( CHECKING, "missing $test: ".addPath( $info->{testdir}, $filename));
            return 0;
        }
    }    
    return 1;
}

#Run tests and collect rutimes
sub runTests
{
    my ( $info) = @_;
    foreach my $test ( keys %{$info->{tests}})
    {
        my $filename = $info->{tests}->{$test};
        runTest( $info, $test, $filename);
    }
}

# Print runtimes for tests
sub printResults
{
    my ( $info) = @_;
    print "Results:\n";
    foreach my $test ( keys %{$info->{results}})
    {
        print $test.": ".$info->{results}->{$test}."\n";
    }
}

#Run one test and measure its run time
sub runTest
{
    my ( $info, $test, $filename) = @_;
    my $outname = $test.".png";
    my $cmd = $info->{showgraph}." -f ".addPath( $info->{testdir}, $filename)." -o ".$outname;
    
    dprint( TESTS, "Running ".$cmd);
    
    # Run test
    my $runtime = 0;
    for ( my $i = 0; $i < $info->{iterations}; $i++)
    {
        my $start = gettimeofday();
        
        #remove picture file if it is there
        if ( -f $outname)
        {
            unlink $outname;
        }
        
        #run test
        system $cmd;
        my $stop = gettimeofday();
        
        #check that file with picture is present
        if ( !(-f $outname))
        {
            $info->{results}->{$test} = FAIL;
            return;
        }
        $runtime += $stop - $start;
    }
    # Save arithmetic mean
    $info->{results}->{$test} = $runtime / $info->{iterations};
}

#Print str if mode is enabled for print
sub dprint
{
    my ( $mode, $str) = @_;
    print $mode.": ".$str."\n" if ( $print_conf->{$mode});
}

# Path concatenation
sub addPath
{
    my ( $first, $second) = @_;
    return $first."\/".$second;
}
