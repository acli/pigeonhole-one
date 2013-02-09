<?php

require_once('simpletest/autorun.php');
include 'a2b.inc';

class Grade1TestCase extends UnitTestCase {

    function test_ueb_tokenize_of_empty_string_should_have_0_elements() {
	$this->assertEqual(count(ueb_tokenize('')), 0);
    }

    function test_ueb_tokenize_of_a_should_have_1_element() {
	$this->assertEqual(count(ueb_tokenize('a')), 1);
    }

#    function test_braille_of_letter_a() {
#	$this->assertEqual(translate_to_braille('a'), '⠁');
#    }
}

?>
