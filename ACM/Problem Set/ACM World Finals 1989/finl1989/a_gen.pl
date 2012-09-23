#! /usr/bin/perl

$maxlen = 20 ;

print <<'EOT' ;
XWY
ZX
ZXY
ZXW
YWWX
#
T
T
#
U
V
#
EOT

open( WORDS, "sort -f /usr/dict/words|" ) ;
while(<WORDS>) {
	next if length > $maxlen ;
	next unless /^[a-zA-Z]+$/ ;
	y/a-z/A-Z/ ;
	y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/XYZABCDEFGHIJWVUTSRQPONKLM/ ;
	print ;
}
close WORDS ;
print "#\n" ;

open( WORDS, "/usr/dict/words" ) ;
outer: while(<WORDS>) {
	next unless /^[a-zA-Z]+$/ ;
	chomp ;
	y/a-z/A-Z/ ;
	my @a ;
	foreach( split( //, $_ ) ) {
		next outer if( $a[ord]++ ) ;
	}
	print join( "\n", split( //, $_ ) ) ;
	print "\n#\n" ;
}
close WORDS ;
