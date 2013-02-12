<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once('simpletest/autorun.php');
require_once 'en_words.inc';

class TestEnglishWords extends UnitTestCase {

    function test_1() {
	$this->assertEqual(PigeonWords::numeral(1), 'one');
    }

    function test_2() {
	$this->assertEqual(PigeonWords::numeral(2), 'two');
    }

    function test_3() {
	$this->assertEqual(PigeonWords::numeral(3), 'three');
    }

    function test_4() {
	$this->assertEqual(PigeonWords::numeral(4), 'four');
    }

    function test_5() {
	$this->assertEqual(PigeonWords::numeral(5), 'five');
    }

    function test_6() {
	$this->assertEqual(PigeonWords::numeral(6), 'six');
    }

    function test_unimplemented() {
	$this->expectException('Exception');
	PigeonWords::numeral(-123456789);
    }

    function test_0s() {
	$this->assertEqual(PigeonWords::time_qty_si(0), '0 s');
    }

    function test_1s() {
	$this->assertEqual(PigeonWords::time_qty_si(1), '1 s');
    }

    function test_59s() {
	$this->assertEqual(PigeonWords::time_qty_si(59), '59 s');
    }

    function test_60s() {
	$this->assertEqual(PigeonWords::time_qty_si(60), '1 min');
    }

    function test_61s() {
	$this->assertEqual(PigeonWords::time_qty_si(61), '1 min 1 s');
    }

    function test_3600s() {
	$this->assertEqual(PigeonWords::time_qty_si(3600), '1 h');
    }

    function test_3601s() {
	$this->assertEqual(PigeonWords::time_qty_si(3601), '1 h 1 s');
    }

    function test_3661s() {
	$this->assertEqual(PigeonWords::time_qty_si(3661), '1 h 1 min 1 s');
    }

}

?>
