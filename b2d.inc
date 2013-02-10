<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once 'data.inc';

class PigeonDots {

    static $b2d;

    function __construct() {
	if (gettype($this->b2d) != 'array' || count($this->b2d) == 0) {
	    $this->b2d = read_colon_separated_data('b2d.txt');
	} /* if */
    } /* __construct */

    public function dots_in_cell($s) {
	$it = array();

	if (mb_strlen($s) != 1) {
	    throw new LengthException("Braille cell length != 1");
	} /* if */

	if (array_key_exists($s, $this->b2d)) {
	    $it = $this->b2d[$s];
	} elseif (!preg_match('/^\s$/u', $s)) {
	    throw new InvalidArgumentException("Unrecognized code point");
	} /* if */
	return $it;
    } /* dots_in_cell */

} /* PigeonDots */

?>
