<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once 'a2b.inc';
require_once 'b2d.inc';
require_once 'messages.inc';

ob_start('mb_output_handler');

$t = new PigeonUEB(1);
$d = new PigeonDots();

foreach (read_messages() as $data) {
    $seq = $data[0];
    $quote = $data[1];
    $attribution = $data[2];
    $message = sprintf(($attribution? '“%s” – %s': '%s'), $quote, $attribution);
    $braille = $t->translate_to_braille($message);
    $dots = $d->dots_in_string($braille);
    print "$message\n";
    print "$braille\n";
} /* foreach */


?>
