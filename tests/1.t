<?php

require_once('simpletest/autorun.php');
include 'a2b.inc';

class Grade1TestCase extends UnitTestCase {

    function test_braille_of_letter_a() {
	$this->assertEqual(translate_to_braille('a'), '⠁');
    }
}

?>
