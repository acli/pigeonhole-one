<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

class PigeonScanOrder implements Iterator {
    public function SCAN_ORDER_BRAILLE() { return array(1, 2, 3, 4, 5, 6); }
    public function SCAN_ORDER_TTB() { return array(4, 5, 6, 1, 2, 3); }
    public function SCAN_ORDER_LTR() { return array(1, 4, 2, 5, 3, 6); }
    public function SCAN_ORDER_RTL() { return array(4, 1, 5, 2, 6, 3); }

    public $scan_order;
    public $position = 0;

    function __construct($scan_order = null) {
	if ($scan_order == null) {
	    $this->scan_order = $this->SCAN_ORDER_BRAILLE();
	} else {
	    $this->scan_order = $scan_order;
	} /* if */
	$this->position = 0;
    } /* __construct */

    function rewind() {
	$this->position = 0;
    } /* rewind */

    function current() {
	return $this->scan_order[$this->position];
    } /* current */

    function key() {
	return $this->position;
    } /* key */

    function next() {
	++$this->position;
    } /* next */

    function valid() {
	return isset($this->scan_order[$this->position]);
    } /* valid */

    function nth_from_current($n) {
	return $this->scan_order[$this->position + $n];
    } /* nth_from_current */
} /* PigeonScanOrder */

?>
