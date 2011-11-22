#!/usr/bin/perl
use strict;
use warnings;


my @rooms = map {
	$_ =~ /rooms\/\d+-(.*?)\.room\.c\+\+/ or die "Error: Weird room filename: $_\n";
	my $id = $1;
	$id =~ s/[^a-zA-Z0-9_]/_/g;
	{
		file => $_,
		id => $id,
	}
} grep {
	/\.room\.c\+\+/
} glob 'rooms/*';

print <<"END";

extern Def def [];
${\(join "\n", map "static Def*const $rooms[$_]{id} = def+$_;", 0..$#rooms)}
static const uint n_rooms = ${\(0+@rooms)};

namespace file {
${\(join "\n", map "\tnamespace $_->{id} {\n\t\t#define THIS_ROOM room::$_->{id}\n\t\t#include \"$_->{file}\"\n\t\t#undef THIS_ROOM\n\t}", @rooms)}
}

Def def [n_rooms] = {
${\(join ",\n", map "\tfile::$_->{id}::def", @rooms)}
};


END



