#!/usr/bin/perl

use strict;
use warnings;
use lib 'tool';
use make;
use autodie;


my $cppc = 'g++-4.7';
my @cppc_flags = qw(-std=c++11 -fmax-errors=5);
my @devel_flags = qw(-Wall -O1 -ggdb);
my @libs = qw(-lGL -lglfw -lSOIL lib/libBox2D.a);
my @cppc_output_flag = qw(-o);

my $main_program = 'rata';
my $main_cpp = 'src/main.cpp';
my @all_cpps = (glob 'src/*.cpp src/*.h');

rule $main_program, \@all_cpps, sub {
    system $cppc, @cppc_flags, @devel_flags, $main_cpp, @libs, @cppc_output_flag, $main_program;
};

phony 'clean', [], sub {
    no autodie;
    unlink $main_program;
};






exit(!make(@ARGV));


