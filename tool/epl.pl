#!/usr/bin/perl

use strict;
use warnings;
use Cwd qw(realpath cwd chdir);

our $EPL_IN_FILENAME = "(unknown file)";
our $EPL_OUT_FILENAME = "(unknown file)";

my @dirs;
sub pushd {
    push @dirs, cwd;
#    print "pushd from $dirs[-1] to $_[0]\n";
    chdir $_[0];
}
sub popd {
    my $dir = cwd;
#    print "popd from $dir to $dirs[-1]\n";
    chdir pop @dirs;
}

sub actor_info {
	our $EPL_IN_FILENAME =~ /^src\/actor\/(\d+)-(\w+)\.cpp.epl$/ or die "$EPL_IN_FILENAME: actor_info called in a non-actor file\n";
	my ($num, $name) = ($1, $2);
	"uint id () { return $1; } CStr name () { return \"$2\"; }"
}


sub epl {
	my ($IN, $debug) = @_;
	my $code;

	while (length $IN and $IN =~ s/^(.*?)<%//s) {
		(my $text = $1) =~ s/("|\\(?!\$))/\\$1/sg;
		$code .= "\$OUT.=\"$text\";";
		if (length $IN and $IN =~ s/^(.*?)%>//s) {
			my $perl = $1;
			if ($perl =~ s/^=//s) {
				$code .= "\$OUT.=do{$perl};";
			}
			else {
				$code .= "$perl;";
			}
		}
	};
	$IN =~ s/("|\\(?!\$))/\\$1/sg;
	$code .= "\$OUT.=\"$IN\";";
	
	my $OUT = '';
	if ($debug) { print STDERR $code; }
	eval $code;
	if ($@) {
		die "$EPL_IN_FILENAME: $@";
	}
	return $OUT;
}

sub eplf {
	my ($outf, $inf, $debug) = @_;
	$EPL_IN_FILENAME = $inf;
	$EPL_OUT_FILENAME = $outf;
	open my $INF, '<', $inf or die "$0: Could not open $inf for reading: $!\n";
	open my $OUTF, '>', $outf or die "$0: Could not open $outf for writing: $!\n";
    realpath($inf) =~ /^(.*)\/[^\/]*$/ or die "$0: Something went wrong matching the realpath of $inf!\n";
    pushd $1;
    local $/;
	print $OUTF epl(readline($INF), $debug);
    popd;
	$EPL_IN_FILENAME = '(unknown file)';
	$EPL_OUT_FILENAME = '(unknown file)';
	close $INF;
	close $OUTF;
}

if (defined $^S and $^S == 0) {
	local $/;
	print(epl(<>));
}

1;