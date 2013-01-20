#!/usr/bin/perl

use strict;
use warnings;
BEGIN {
    unless (defined $FindBin::Bin) {
        require FindBin;
        require "$FindBin::Bin/../../tool/make.pm";
    }
    make->import(':all');
}
use autodie qw<:all>;
use File::Path qw<remove_tree>;


workflow {

    include '../..';

    subdep('inc/strings.h', 'inc/hacc.h');
    subdep('inc/files.h', 'inc/hacc.h');
    subdep('inc/haccers.h', 'inc/hacc.h');
    subdep('inc/haccable.h', [qw<inc/haccable_getset.h inc/hacc.h>]);
    subdep('inc/haccable_standard.h', 'inc/haccable.h');
    subdep('inc/haccable_pointers.h', 'inc/haccable.h');
    subdep('inc/haccable_integration.h', [qw<inc/haccable.h inc/strings.h inc/files.h>]);
    subdep('inc/everything.h', [qw<inc/haccable_standard.h inc/haccable_pointers.h inc/haccable_integration.h inc/haccable_files.h>]);

    cppc_rule('tmp/hacc.o', [qw<src/hacc.cpp inc/hacc.h inc/haccable.h inc/haccable_files.h>]);
    cppc_rule('tmp/strings.o', [qw<src/strings.cpp inc/strings.h>]);
    cppc_rule('tmp/files.o', [qw<src/files.cpp inc/files.h>]);
    cppc_rule('tmp/haccable.o', [qw<src/haccable.cpp inc/haccable.h>]);
    cppc_rule('tmp/haccable_standard.o', [qw<src/haccable_standard.cpp inc/haccable_standard.h>]);
    cppc_rule('tmp/haccable_files.o', [qw<src/haccable_files.cpp inc/haccable_files.h inc/haccable.h>]);
    cppc_rule('tmp/hacc.t.o', [qw<test/hacc.t.cpp inc/hacc.h>]);
    cppc_rule('tmp/strings.t.o', [qw<test/strings.t.cpp inc/strings.h>]);
    cppc_rule('tmp/files.t.o', [qw<test/files.t.cpp inc/files.h>]);
    cppc_rule('tmp/haccable.t.o', [qw<test/haccable.t.cpp inc/haccable.h>]);
    cppc_rule('tmp/everything.t.o', [qw<test/everything.t.cpp inc/everything.h>]);
    cppc_rule('tmp/haccable_standard.t.o', [qw<test/haccable_standard.t.cpp inc/haccable_standard.h>]);
    cppc_rule('tmp/haccable_pointers.t.o', [qw<test/haccable_pointers.t.cpp inc/haccable_pointers.h>]);
    cppc_rule('tmp/haccable_integration.t.o', [qw<test/haccable_integration.t.cpp inc/haccable_integration.h>]);
    cppc_rule('tmp/haccable_files.t.o', [qw<test/haccable_files.t.cpp inc/haccable_files.h inc/haccable_integration.h inc/haccable_standard.h inc/haccable_pointers.h>]);
    ld_rule('tmp/t', [targetmatch(qr/^tmp\/[^\/]*\.o$/), '../tap/tmp/tap.o', '../tap/tmp/tap_make_test_main.o']);

    test_rule('tmp/t');

    clean_rule(glob 'tmp/*');

    defaults 'test';

};



