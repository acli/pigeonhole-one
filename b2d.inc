<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once 'data.inc';

class PigeonDots {

    static $b2d;

    function __construct() {
	if (gettype($this->b2d) != 'array' || count($this->b2d) == 0) {
	    $this->b2d = array();
	    foreach (read_colon_separated_data('b2d.txt') as $data) {
		$key = $data[0];
		$val = array();
		$det = str_replace(' ', '', $data[1]);
		for ($j = 0; $j < strlen($det); $j += 1) {
		    array_push($val, intval($det[$j]));
		} /* foreach */
		$this->b2d[$key] = $val;
	    } /* foreach */
	} /* if */
    } /* __construct */

    public function dots_in_cell($s) {
	$it = array();

	if (mb_strlen($s, 'utf-8') != 1) {
	    throw new LengthException("$s: Braille cell length != 1");
	} /* if */

	if (array_key_exists($s, $this->b2d)) {
	    $it = $this->b2d[$s];
	} elseif (!preg_match('/^\s$/u', $s)) {
	    throw new InvalidArgumentException("$s: Unrecognized code point");
	} /* if */
	return $it;
    } /* dots_in_cell */

    public function dots_in_string($s) {
	$it = array();

	$n = mb_strlen($s, 'utf-8');
	for ($i = 0; $i < $n; $i += 1) {
	    $c = mb_substr($s, $i, 1, 'utf-8');
	    array_push($it, $this->dots_in_cell($c));
	} /* for */
	return $it;
    } /* dots_in_string */

    public function expand($a) {
	if (count($a) == 6) { /* FIXME - assuming 6-elem ary already expanded */
	    $it = $a;
	} else {
	    $it = array(0, 0, 0, 0, 0, 0);
	    foreach ($a as $i) {
		if ($i >= 1 && $i <= 6) {
		    $it[$i - 1] = 1;
		} /* if */
	    } /* foreach */
	} /* if */
	return $it;
    } /* expand */

    public function diff_matrix($a, $b) {
	$it = array();
	if (count($a) != count($b)) {
	    throw new Exception('Cannot diff vectors of different lengths');
	} /* if */
	for ($i = 0; $i < count($a); $i += 1) {
	    $it[$i] = $b[$i] - $a[$i];
	} /* for */
	return $it;
    } /* diff_matrix */

} /* PigeonDots */

?>
