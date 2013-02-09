<?php

require_once('simpletest/autorun.php');
include 'a2b.inc';

class BaseTestCase extends UnitTestCase {

    function test_existence_of_function_translate_to_braille() {
	$this->assertTrue(function_exists('translate_to_braille'));
    }

    function test_braille_of_null_string() {
	$this->assertEqual(translate_to_braille(''), '');
    }
}

?>
