#!/usr/bin/perl

use strict;
use warnings;
use FindBin;
use if !%::make, lib => "$FindBin::Bin/../../tool/make.pl";
use make;


workflow {

    include '../..';

    cppc_rule('tmp/game.o', 'src/game.cpp');
    cppc_rule('tmp/phases.o', 'src/phases.cpp');
    cppc_rule('tmp/state.o', 'src/state.cpp');
    cppc_rule('tmp/commands.o', 'src/commands.cpp');
    cppc_rule('tmp/input.o', 'src/input.cpp');
    cppc_rule('tmp/images.o', 'src/images.cpp');
    cppc_rule('tmp/opengl.o', 'src/opengl.cpp');

    clean_rule(glob 'tmp/*');

};



