<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

include 'a2b.inc';

function read_messages() {
    $input = 'messages.txt';
    $it = array();
    $h = fopen($input, 'r');
    $re_data_line = '/^(\d+):(.+?)(?::([^:]*))?$/u';
    $re_comment_line = '/^\s*(?:[#;][^\t]*)?$/u';
    if ($h) {
        $lc = 0;
        for (;;) {
            $s = fgets($h);
        if ($s === false) { break; }
            $lc += 1;
            $s = rtrim($s);
            if (preg_match($re_data_line, $s, $matches)) {
                array_push($it, array($matches[1], $matches[2], $matches[3]));
            } elseif (preg_match($re_comment_line, $s)) {
                ;
            } else {
                error_log("$input:$lc: Malformed data line");
            } /* if */
        } /* for */
    } else {
        throw new Exception("Cannot open input file \"$input\"");
    }
    fclose($h);
    return $it;
} /* read_messages */

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
