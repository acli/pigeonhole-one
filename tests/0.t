<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once('simpletest/autorun.php');
include 'a2b.inc';

class BaseTestCase extends UnitTestCase {

    function test_existence_of_function_translate_to_braille() {
	$t = new PigeonUEB();
        $this->assertTrue(method_exists($t, 'translate_to_braille'));
    }

    function test_braille_of_null_string() {
	$t = new PigeonUEB();
	$this->assertEqual($t->translate_to_braille(''), '');
    }
}

?>
