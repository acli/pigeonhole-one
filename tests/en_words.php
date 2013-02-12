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
	$this->expectException('Unimplemented');
	PigeonWords::numeral(-123456789);
    }

}

?>
