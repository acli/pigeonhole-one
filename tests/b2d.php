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

    function test_space_should_have_zero_dots() {
	$t = new PigeonDots();
	$this->assertEqual($t->dots_in_cell(' '), array());
    }

    function test_dots_in_a_should_be_1() {
	$t = new PigeonDots();
	$this->assertEqual($t->dots_in_cell('⠁'), array(1));
    }

    function test_dots_in_b_should_be_1_2() {
	$t = new PigeonDots();
	$this->assertEqual($t->dots_in_cell('⠃'), array(1, 2));
    }

    function test_dots_in_c_should_be_1_4() {
	$t = new PigeonDots();
	$this->assertEqual($t->dots_in_cell('⠉'), array(1, 4));
    }

    function test_dots_in_ca() {
	$t = new PigeonDots();
	$this->assertEqual($t->dots_in_string('⠉⠁'), array(array(1, 4), array(1)));
    }

    function test_diff_matrix_null_null() {
	$t = new PigeonDots();
	$this->assertEqual($t->diff_matrix(array(), array()), array(0, 0, 0, 0, 0, 0));
    }

    function test_diff_matrix_null_1() {
	$t = new PigeonDots();
	$this->assertEqual($t->diff_matrix(array(), array(1)), array(1, 0, 0, 0, 0, 0));
    }

    function test_diff_matrix_1_null() {
	$t = new PigeonDots();
	$this->assertEqual($t->diff_matrix(array(1), array()), array(-1, 0, 0, 0, 0, 0));
    }

    function test_diff_matrix_1_2() {
	$t = new PigeonDots();
	$this->assertEqual($t->diff_matrix(array(1), array(2)), array(-1, 1, 0, 0, 0, 0));
    }

}

?>
