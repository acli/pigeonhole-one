<?php

#$dot_locator_for_use = '⠐⠐⠿';

$a2b = array();
$n2a = array(
	'1' => 'a',
	'2' => 'b',
	'3' => 'c',
	'4' => 'd',
	'5' => 'e',
	'6' => 'f',
	'7' => 'g',
	'8' => 'h',
	'9' => 'i',
	'0' => 'j',
    );

function ueb_read_data() {
    global $a2b;
    $input = 'a2b.txt';
    $h = fopen($input, 'r');
    $re_data_line = '/^(\S+)\t(\S+)(?:\s*)$/u';
    $re_comment_line = '/^\s*(?:[#;][^\t]*)$/u';
    if ($h) {
	for (;;) {
	    $s = fgets($h);
	if ($s === false) { break; }
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
	throw new Exception("Cannot open input file \"$input\"");
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
	} elseif (preg_match('/^([A-Z]+)(.*)$/u', $s, $matches)) {
	    array_push($tokens, array('upper', $matches[1]));
	    $s = $matches[2];
	} elseif (preg_match('/^([0-9]+)(.*)$/u', $s, $matches)) {
	    array_push($tokens, array('numeric', $matches[1]));
	    $s = $matches[2];
	} elseif (preg_match('/^(\s+)(.*)$/u', $s, $matches)) {
	    array_push($tokens, array('space', $matches[1]));
	    $s = $matches[2];
	} else {
	    print "OOPS... I don't know how to handle \"$s\"\n";
	    die;
	} /* if */
    } /* for */
    print "RESULT: "; var_dump($tokens);
    return $tokens;
} /* ueb_tokenize */

function ueb_encode($t) {
    global $a2b;
    global $n2a;
    $mode_numeric = false;
    $mode_upper = false;
    $it = '';
    for ($i = 0; $i < count($t); $i += 1) {
	$type = $t[$i][0];
	$data = $t[$i][1];
	print "DEBUG: type=($type), data=($data)\n";

	if (($type == 'alpha' || $type == 'upper') && $mode_numeric) {
	    $it .= '⠰';
	    $mode_numeric = false;
	} /* if */

	if ($type == 'alpha') {
	    $w = '';
	    for ($j = 0; $j < mb_strlen($data); $j += 1) {
		$c = mb_substr($data, $j, 1);
		if (array_key_exists($c, $a2b)) {
		    $w .= $a2b[$c];
		} else {
		    throw new Exception("unhandled $type character $c");
		} /* if */
	    } /* for */
	    $it .= $w;

	} elseif ($type == 'upper') {
	    $w = '';
	    for ($j = 0; $j < mb_strlen($data); $j += 1) {
		$c = mb_substr($data, $j, 1);
		if (array_key_exists($c, $a2b)) {
		    $w .= $a2b[$c];
		} else {
		    throw new Exception("unhandled $type character $c");
		} /* if */
	    } /* for */
	    $it .= $w;

	} elseif ($type == 'numeric') {
	    if (!$mode_numeric) {
		$it .= '⠼';
		$mode_numeric = true;
	    } /* if */

	    $w = '';
	    for ($j = 0; $j < mb_strlen($data); $j += 1) {
		$c = mb_substr($data, $j, 1);
		if (array_key_exists($c, $n2a)) {
		    $c = $n2a[$c];
		    if (array_key_exists($c, $a2b)) {
			$w .= $a2b[$c];
		    } else {
			throw new Exception("unhandled alpha character $c");
		    } /* if */
		} else {
		    throw new Exception("unhandled digit $c");
		} /* if */
	    } /* for */
	    $it .= $w;
	} elseif ($type == 'space') {
	    $mode_numeric = false;
	    $it .= $data;
	} else {
	    throw new Exception("FIXME: unhandled type \"$type\"");
	} /* if */
    } /* for */
    $mode_numeric = false;
    return $it;
}

function translate_to_braille($s) {
    $tokens = ueb_tokenize($s);
    return ueb_encode($tokens);
}

?>
