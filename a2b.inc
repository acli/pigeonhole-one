<?php

#$dot_locator_for_use = '⠐⠐⠿';
#$numeric_indicator = '⠼';

function ueb_tokenize($s) {
    $tokens = array();
    while (strlen($s)) {
	if (preg_match('/^([a-z]+)(.*)$/', $s, $matches)) {
	    array_push($tokens, array('alpha', $matches[1]));
	    $s = $matches[2];
	    print "GOT HERE\n";
	} else {
	    print "OOPS\n";
	    die;
	} /* if */
    } /* for */
    print "RESULT: "; var_dump($tokens);
    return $tokens;
} /* ueb_tokenize */

function ueb_encode($t) {
    $it = '';
    return $it;
}

function translate_to_braille($s) {
    $tokens = ueb_tokenize($s);
    return ueb_encode($tokens);
}

?>
