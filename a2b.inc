<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once 'data.inc';

class PigeonUEB {

const dot_locator_for_use = '⠐⠐⠿';

public $grade = 2;

var $a2b = array();
var $n2a = array(
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

function __construct($grade = 2) {
    if (gettype($this->a2b) != 'array' || count($this->a2b) == 0) {
	$this->a2b = read_data('a2b.txt');
    } /* if */
    $this->grade = $grade;
} /* __construct */

public function ueb_tokenize($s) {
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
	} elseif (preg_match('/^([\.,])(.*)$/u', $s, $matches)) {
	    # Punctuation that can occur in a numeric sequence
	    array_push($tokens, array('punct1', $matches[1]));
	    $s = $matches[2];
	} elseif (preg_match('/^([\/])(.*)$/u', $s, $matches)) {
	    # Punctuation that can occur in a numeric sequence
	    array_push($tokens, array('punct2', $matches[1]));
	    $s = $matches[2];
	} elseif (preg_match('/^(.)(.*)$/u', $s, $matches)) {
	    array_push($tokens, array('other', $matches[1]));
	    $s = $matches[2];
	} else {
	    throw new Exception("OOPS... I don't know how to handle \"$s\"");
	} /* if */
    } /* for */
    return $tokens;
} /* ueb_tokenize */

function ueb_a2b($c) {
    if (!array_key_exists($c, $this->a2b)) {
	throw new Exception("unhandled character $c");
    } /* if */
    return $this->a2b[$c];
} /* ueb_a2b */

public function ueb_encode($t) {
    $mode_numeric = false;
    $mode_upper = false;
    $it = '';
    for ($i = 0; $i < count($t); $i += 1) {
	$type = $t[$i][0];
	$data = $t[$i][1];

	if ($this->grade != 1) {	# Allow any grade if null string passed
	    throw new Exception("Unimplemented grade $this->grade");
	} /* if */

	if ($type == 'alpha') {
	    if ($mode_numeric) {
		# Numeric mode self-terminates if the letter cannot be a digit.
		# v. The Rules of Unified English Braille, §6.2.1 (p. 60)
		if (in_array(mb_substr($data, 0, 1), $this->n2a, true)) {
		    $it .= '⠰';
		} /* if */
		$mode_numeric = false;
	    } /* if */

	    $w = '';
	    for ($j = 0; $j < mb_strlen($data); $j += 1) {
		$w .= $this->ueb_a2b(mb_substr($data, $j, 1));
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
		if (array_key_exists($c, $this->a2b)) {
		    if (!$all_caps) {
			$w .= '⠠';
		    } /* if */
		    $w .= $this->a2b[$c];
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
		if (array_key_exists($c, $this->n2a)) {
		    $w .= $this->ueb_a2b($this->n2a[$c]);
		} else {
		    throw new Exception("unhandled digit $c");
		} /* if */
	    } /* for */
	    $it .= $w;

	} elseif ($type == 'punct1' || $type == 'punct2') {
	    # See The Rules of Unified English Braille, §6.2.1 (p. 60)
	    if ($type == 'punct1' && !$mode_numeric && $i <= count($t) && $t[$i + 1][0] == 'numeric') {
		$it .= '⠼';
		$mode_numeric = true;
	    } /* if */
	    if ($mode_numeric && (
			($type == 'punct2' && $i <= count($t)
					   && $t[$i + 1][0] == 'numeric')
		     || ($type == 'punct1'))) {

		for ($j = 0; $j < mb_strlen($data); $j += 1) {
		    $c = mb_substr($data, $j, 1);
		    if ($c == '/') {
			$it .= '⠌'; # FIXME
		    } else {
			$it .= $this->ueb_a2b($c);
		    } /* if */
		} /* for */
	    } else {
		$mode_numeric = false;
		for ($j = 0; $j < mb_strlen($data); $j += 1) {
		    $it .= $this->ueb_a2b(mb_substr($data, $j, 1));
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

	} elseif ($type == 'other') {
	    $w = $this->ueb_a2b($data);

	    if ($mode_numeric) {
		# Numeric mode self-terminates if the letter cannot be a digit.
		# v. The Rules of Unified English Braille, §6.2.1 (p. 60)
		$mode_numeric = false;
	    } /* if */
	    $it .= $w;

	} else {
	    throw new Exception("FIXME: unhandled type \"$type\"");
	} /* if */
    } /* for */
    $mode_numeric = false;
    return $it;
}

public function translate_to_braille($s) {
    $tokens = $this->ueb_tokenize($s);
    return $this->ueb_encode($tokens);
}

} /* PigeonUEB */

?>
