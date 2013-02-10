<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once('simpletest/autorun.php');
require_once 'b2d.inc';

class TestB2D extends UnitTestCase {

    function test_existence_of_function_dots_in_cell() {
	$t = new PigeonDots();
	$this->assertTrue(method_exists($t, 'dots_in_cell'));
    }

}

?>
