#!/usr/bin/perl

use strict;
use integer;

use vars qw( %a2b );
use vars qw( $dot_locator_for_use );
use vars qw( $numeric_indicator );

$dot_locator_for_use = '⠐⠐⠿';
$numeric_indicator = '⠼';

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

sub tokenize ($) {
    my($s) = @_;
}

sub translate ($) {
    my($s) = @_;
    my $tokens = tokenize $s;
    my $it = $tokens;
    return $it;
}

read_data;
printf "%s\n", (@ARGV? translate join(' ', @ARGV): scalar <>);
