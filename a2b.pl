#!/usr/bin/perl

use strict;
use integer;

use vars qw( %a2b );

sub read_data () {
    my $input = 'a2b.txt';
    open(INPUT, "<$input") || die "$input: $!\n";
    for (;;) {
	my $s = scalar <INPUT>;
    last unless defined $s;
	chomp $s;
	if ($s =~ /^(\S)+\t(\S+)(?:\s*)$/) {
	    $a2b{$1} = $2;
	} elsif ($s =~ /^\s*(?:[#;][^\t]*)?$/) {
	    ;
	} else {
	    print STDERR "$input:$.: Malformed data line\n";
	}
    }
    close INPUT;
}

sub translate ($) {
    my($s) = @_;
    my $it = '';
    for (my $i = 0; $i < length $s; $i += 1) {
	my $c = substr($s, $i, 1);
	if ($c eq ' ') {
	    $it .= $c;
	} elsif (defined $a2b{$c}) {
	    $it .= $a2b{$c};
	} else {
	    print STDERR "Warning: No mapping for character `$c'\n";
	}
    }
    return $it;
}

read_data;
printf "%s\n", translate join(' ', @ARGV);
