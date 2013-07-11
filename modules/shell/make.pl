#!/usr/bin/perl

use strict;
use warnings;
use FindBin;
use if !$^S, lib => "$FindBin::Bin/../../tool";
use Make_pl;

workflow {

    our @objects = qw<
        tmp/main.o
        tmp/console.o
        ../core/tmp/commands.o
        ../core/tmp/game.o
        ../core/tmp/images.o
        ../core/tmp/input.o
        ../core/tmp/phases.o
        ../core/tmp/opengl.o
        ../ent/tmp/bipeds.o
        ../ent/tmp/control.o
        ../ent/tmp/various.o
        ../geo/tmp/camera.o
        ../geo/tmp/rooms.o
        ../geo/tmp/tiles.o
        ../hacc/tmp/common.o
        ../hacc/tmp/documents.o
        ../hacc/tmp/dynamicism.o
        ../hacc/tmp/files.o
        ../hacc/tmp/haccable.o
        ../hacc/tmp/haccable_standard.o
        ../hacc/tmp/strings.o
        ../hacc/tmp/tree.o
        ../hacc/tmp/types.o
        ../phys/tmp/ground.o
        ../phys/tmp/phys.o
        ../vis/tmp/common.o
        ../vis/tmp/graffiti.o
        ../vis/tmp/models.o
        ../vis/tmp/sprites.o
        ../vis/tmp/text.o
        ../util/tmp/debug.o
        ../util/tmp/math.o
        ../../lib/libBox2D.a
    >;
    our @libs = qw(-lgc -lGL -lglfw -lSOIL);

    include '../..';

    objects(qw(main console));

     # Here's the main program
    ld_rule('../../rata', [@objects], [@libs]);

    clean_rule(glob 'tmp/*');

};


