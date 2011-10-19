#!/usr/bin/perl


print <<'END';

namespace snd {
	struct Sound {
		sf::SoundBuffer sfsb;
		sf::Sound sfs;
		void play () {
			sfs.Play();
		}
		void stop () {
			sfs.Stop();
		}
		void play (float pitch) {
			sfs.SetPitch(pitch);
			sfs.Play();
		}
	}
END

my @imgs = grep /\.flac$/ && ! /^snd\/\!/, glob 'snd/*';
for (@imgs) {
	$_ =~ /^snd\/(.*?)\.flac$/ or die "Error: Weird sound filename: $_\n";
	my $id = $1;
	$id =~ s/[^a-zA-Z0-9_]/_/g;
	print "\t$id,\n";
}

print "\t_COMMA_EATER;\n\n}\n\nvoid load_snd () {\n\tbool good = true;\n";
for (@imgs) {
	$_ =~ /^snd\/(.*?).flac$/ or die "Error: Weird sound filename: $_\n";
	my $id = $1;
	$id =~ s/[^a-zA-Z0-9_]/_/g;
	print "\tgood &= snd::$id.sfsb.LoadFromFile(\"$_\");\n\tsnd::$id.sfs.SetBuffer(snd::$id.sfsb);\n";
}


print "\tif (!good) fprintf(stderr, \"Error: At least one sound failed to load!\\n\");\n}\n\n\n\n";

