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

    function test_braille_of_letter_a() {
	$this->assertEqual(translate_to_braille('a'), '⠁');
    }

    function test_braille_for_the_quick_brown_fox_jumps_over_the_lazy_dog () {
	$this->assertEqual(translate_to_braille('the quick brown fox jumps over the lazy dog'), '⠞⠓⠑ ⠟⠥⠊⠉⠅ ⠃⠗⠕⠺⠝ ⠋⠕⠭ ⠚⠥⠍⠏⠎ ⠕⠧⠑⠗ ⠞⠓⠑ ⠇⠁⠵⠽ ⠙⠕⠛');
    }

    function test_braille_of_digit_1() {
	$this->assertEqual(translate_to_braille('1'), '⠼⠁');
    }

    function test_braille_of_digits_1234567890() {
	$this->assertEqual(translate_to_braille('1234567890'), '⠼⠁⠃⠉⠙⠑⠋⠛⠓⠊⠚');
    }

    function test_braille_of_a1() {
	$this->assertEqual(translate_to_braille('a1'), '⠁⠼⠁');
    }

    function test_braille_of_1a() {
	$this->assertEqual(translate_to_braille('1a'), '⠼⠁⠰⠁');
    }

    function test_braille_of_letter_capital_A() {
	$this->assertEqual(translate_to_braille('A'), '⠠⠁');
    }

    function test_braille_of_letter_capital_AA() {
	$this->assertEqual(translate_to_braille('AA'), '⠠⠠⠁⠁');
    }

    function test_braille_of_letter_lowercase_a_uppercase_A() {
	$this->assertEqual(translate_to_braille('aA'), '⠁⠠⠁');
    }

    function test_braille_of_letter_uppercase_A_lowercase_a() {
	$this->assertEqual(translate_to_braille('Aa'), '⠠⠁⠁');
    }

    function test_braille_for_lowercase_22b () {
	# The Rules of Unified English Braille, p. 51
	$this->assertEqual(translate_to_braille('22b'), '⠼⠃⠃⠰⠃');
    }

    function test_braille_for_uppercase_22B () {
	# The Rules of Unified English Braille, p. 51
	$this->assertEqual(translate_to_braille('22B'), '⠼⠃⠃⠠⠃');
    }

#    function test_braille_for_C_is_for_candy () {
#	# The Rules of Unified English Braille, p. 57
#	$this->assertEqual(translate_to_braille('C is for candy.'), '⠠⠉ ⠊⠎ ⠋⠕⠗ ⠉⠁⠝⠙⠽⠲');
#    }

    function test_braille_for_C_is_for_candy () {
	# The Rules of Unified English Braille, p. 57
	$this->assertEqual(translate_to_braille('Question 3c'), '⠠⠟⠥⠑⠎⠞⠊⠕⠝ ⠼⠉⠰⠉');
    }

#    function test_braille_for_M5T_1W1() {
#	$this->assertEqual(translate_to_braille('M5T 1W1'), '⠠⠠⠍⠼⠑⠰⠞ ⠼⠁⠰⠠⠺⠼⠁');
#    }

}

?>
