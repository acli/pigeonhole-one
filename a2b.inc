<?php

#$dot_locator_for_use = '⠐⠐⠿';
#$numeric_indicator = '⠼';

$a2b = array();

function ueb_read_data() {
    global $a2b;
    $input = 'a2b.txt';
    $h = fopen($input, 'r');
    $re_data_line = '/^(\S+)\t(\S+)(?:\s*)$/u';
    $re_comment_line = '/^\s*(?:[#;][^\t]*)$/u';
    if ($h) {
	for (;;) {
	    $s = fgets($h);
	if ($s === FALSE) { break; }
	    $s = rtrim($s);
	    if (preg_match($re_data_line, $s, $matches)) {
		$a2b[$matches[1]] = $matches[2];
	    } elseif (preg_match($re_comment_line, $s)) {
		;
	    } else {
		error_log("$input: Malformed data line");
	    } /* if */
	} /* for */
    } else {
	die;
    }
    fclose($h);
} /* ueb_read_data */

# FIXME BEGIN
ueb_read_data();
ob_start('mb_output_handler');
# FIXME END

function ueb_tokenize($s) {
    $tokens = array();
    while (mb_strlen($s)) {
	if (preg_match('/^([a-z]+)(.*)$/u', $s, $matches)) {
	    array_push($tokens, array('alpha', $matches[1]));
	    $s = $matches[2];
	} elseif (preg_match('/^(\s+)(.*)$/u', $s, $matches)) {
	    array_push($tokens, array('space', $matches[1]));
	    $s = $matches[2];
	} else {
	    print "OOPS\n";
	    die;
	} /* if */
    } /* for */
    print "RESULT: "; var_dump($tokens);
    return $tokens;
} /* ueb_tokenize */

function ueb_encode($t) {
    global $a2b;
    $it = '';
    for ($i = 0; $i < count($t); $i += 1) {
	$type = $t[$i][0];
	$data = $t[$i][1];
	print "DEBUG: type=($type), data=($data)\n";
	if ($type == 'alpha') {
	    $w = '';
	    for ($j = 0; $j < mb_strlen($data); $j += 1) {
		$c = mb_substr($data, $j, 1);
		if (array_key_exists($c, $a2b)) {
		    $w .= $a2b[$c];
		} else {
		    print "OOPS, unhandled $type character $c\n";
		    die;
		} /* if */
	    } /* for */
	    $it .= $w;
	} elseif ($type == 'space') {
	    $it .= $data;
	} else {
	    print "FIXME: unhandled type \"$type\"\n";
	    die;
	} /* if */
    } /* for */
    return $it;
}

function translate_to_braille($s) {
    $tokens = ueb_tokenize($s);
    return ueb_encode($tokens);
}

?>
