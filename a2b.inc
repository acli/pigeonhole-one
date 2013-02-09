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
    $re_comment_line = '/^\s*(?:[#;][^\t]*)?$/u';
    if ($h) {
	$lc = 0;
	for (;;) {
	    $s = fgets($h);
	if ($s === false) { break; }
	    $lc += 1;
	    $s = rtrim($s);
	    if (preg_match($re_data_line, $s, $matches)) {
		$a2b[$matches[1]] = $matches[2];
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
	} elseif (preg_match('/^([\.,\/])(.*)$/u', $s, $matches)) {
	    # Punctuation that can occur in a numeric sequence
	    array_push($tokens, array('punct1', $matches[1]));
	    $s = $matches[2];
	} else {
	    throw new Exception("OOPS... I don't know how to handle \"$s\"");
	} /* if */
    } /* for */
    print "RESULT: "; var_dump($tokens);
    return $tokens;
} /* ueb_tokenize */

function ueb_a2b($c) {
    global $a2b;
    if (!array_key_exists($c, $a2b)) {
	throw new Exception("unhandled character $c");
    } /* if */
    return $a2b[$c];
} /* ueb_a2b */

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

	if ($type == 'alpha') {
	    if ($mode_numeric) {
		# Numeric mode self-terminates if the letter cannot be a digit.
		# v. The Rules of Unified English Braille, §6.2.1 (p. 60)
		if (in_array(mb_substr($data, 0, 1), $n2a, true)) {
		    $it .= '⠰';
		} /* if */
		$mode_numeric = false;
	    } /* if */

	    $w = '';
	    for ($j = 0; $j < mb_strlen($data); $j += 1) {
		$w .= ueb_a2b(mb_substr($data, $j, 1));
	    } /* for */
	    $it .= $w;

	} elseif ($type == 'upper') {
	    if ($mode_numeric) {
		# Numeric mode self-terminates on a capital letter indicator.
		# v. The Rules of Unified English Braille, §6.2.1 (p. 60)
		$mode_numeric = false;
	    } /* if */

	    # Look ahead to see if we can lock the shift state
	    $all_caps = true;
	    for ($j = $i; $j < count($t); $j += 1) {
		$chk_type = $t[$j][0];
		$chk_data = $t[$j][1];
	    if ($chk_type == 'space') { break; }
		if ($chk_type == 'alpha') {
		    $all_caps = false;
		} /* if */
	    if (!$all_caps) { break; }
	    } /* for */

	    $w = '';

	    if ($all_caps && mb_strlen($data) > 1) {
		$w .= '⠠⠠';
	    } else {
		$all_caps = false;	/* reset $all_caps if length = 1 */
	    } /* if */

	    for ($j = 0; $j < mb_strlen($data); $j += 1) {
		$c = mb_strtolower(mb_substr($data, $j, 1));
		if (array_key_exists($c, $a2b)) {
		    if (!$all_caps) {
			$w .= '⠠';
		    } /* if */
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
		    $w .= ueb_a2b($n2a[$c]);
		} else {
		    throw new Exception("unhandled digit $c");
		} /* if */
	    } /* for */
	    $it .= $w;

	} elseif ($type == 'punct1') {
	    # See The Rules of Unified English Braille, §6.2.1 (p. 60)
	    if ($mode_numeric && $i <= count($t) && $t[$i + 1][0] == 'numeric') {
		for ($j = 0; $j < mb_strlen($data); $j += 1) {
		    $c = mb_substr($data, $j, 1);
		    if ($c == '/') {
			$it .= '⠌'; # FIXME
		    } else {
			$it .= ueb_a2b($c);
		    } /* if */
		} /* for */
	    } else {
		$mode_numeric = false;
		for ($j = 0; $j < mb_strlen($data); $j += 1) {
		    $it .= ueb_a2b(mb_substr($data, $j, 1));
		} /* for */
	    } /* if */

	} elseif ($type == 'space') {
	    # See The Rules of Unified English Braille, §6.2.1 (p. 60)
	    if ($mode_numeric && $i <= count($t) && $t[$i + 1][0] == 'numeric') {
		$it .= '⠐'; # FIXME
	    } else {
		$mode_numeric = false;
		$it .= $data;
	    } /* if */

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
