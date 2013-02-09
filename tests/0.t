<?php

include 'a2b.inc';

class FileTestCase extends UnitTestCase {

    function test_existence_of_function_translate_into_braille() {
	assertTrue(function_exists('translate_into_braille'));
    }
}

?>
