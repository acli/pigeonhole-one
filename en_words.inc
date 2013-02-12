<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

/* Miscellaneous English-language–related functions */

class PigeonWords {

    private static $numerals = array(
	1 => 'one',
	2 => 'two',
	3 => 'three',
	4 => 'four',
	5 => 'five',
	6 => 'six'
    );

    function numeral($n) {
	if (!array_key_exists($n, PigeonWords::$numerals)) {
	    throw new Exception('Unimplemented');
	} /* if */
	return PigeonWords::$numerals[$n];
    } /* numeral */

    function time_qty_si($t) {
	$tmp = array();
	$tmp['h'] = floor($t/3600);
	$tmp['min'] = floor($t/60)%60;
	$tmp['s'] = $t - 3600*$tmp['h'] - 60*$tmp['min'];
	$it = '';
	foreach ($tmp as $unit => $n) {
	    if ($n) {
		if ($it) {
		    $it .= ' ';
		} /* if */
		$it .= "$n $unit";
	    } /* if */
	} /* foreach */
	return $it? $it: '0 s';
    } /* time_qty_si */

} /* PigeonWords */

?>
