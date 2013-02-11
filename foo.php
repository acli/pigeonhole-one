<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once 'a2b.inc';
require_once 'b2d.inc';
require_once 'messages.inc';

$t = new PigeonUEB(1);
$d = new PigeonDots();

foreach (read_messages() as $data) {
    $seq = $data[0];
    $quote = $data[1];
    $attribution = $data[2];
    $message = sprintf(($attribution? '“%s” – %s': '%s'), $quote, $attribution);
    $braille = $t->translate_to_braille($message);
    $dots = $d->dots_in_string($braille);

    $t_0 = time();

    foreach ($dots as $dots_in_cell) {
	sleep(5);
	if (count($dots_in_cell) > 0) {
	    print join(', ', $dots_in_cell) . "\n";
	} else {
	    print "blank\n";
	} /* if */
    } /* foreach */

    print "$braille\n";
    print "$message\n";

    $dt = time() - $t_0;
    print "[$dt s were spent transmitting this message.]\n";
} /* foreach */


?>
