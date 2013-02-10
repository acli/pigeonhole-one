<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

include 'a2b.inc';
include 'messages.inc';

ob_start('mb_output_handler');

$t = new PigeonUEB(1);

foreach (read_messages() as $data) {
    $seq = $data[0];
    $quote = $data[1];
    $attribution = $data[2];
    $message = sprintf(($attribution? '“%s” – %s': '%s'), $quote, $attribution);
    print "$message\n";
    print $t->translate_to_braille($message) . "\n";
} /* foreach */


?>
