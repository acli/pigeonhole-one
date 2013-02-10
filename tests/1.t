<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once('simpletest/autorun.php');
include 'a2b.inc';

class Grade1TestCase extends UnitTestCase {

    function test_ueb_tokenize_of_empty_string_should_have_0_elements() {
	$t = new PigeonUEB(1);
	$this->assertEqual(count($t->ueb_tokenize('')), 0);
    }

    function test_ueb_tokenize_of_a_should_have_1_element() {
	$t = new PigeonUEB(1);
	$this->assertEqual(count($t->ueb_tokenize('a')), 1);
    }

    function test_braille_of_letter_a() {
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('a'), '⠁');
    }

    function test_braille_for_the_quick_brown_fox_jumps_over_the_lazy_dog () {
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('the quick brown fox jumps over the lazy dog'), '⠞⠓⠑ ⠟⠥⠊⠉⠅ ⠃⠗⠕⠺⠝ ⠋⠕⠭ ⠚⠥⠍⠏⠎ ⠕⠧⠑⠗ ⠞⠓⠑ ⠇⠁⠵⠽ ⠙⠕⠛');
    }

    function test_braille_of_digit_1() {
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('1'), '⠼⠁');
    }

    function test_braille_of_digits_1234567890() {
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('1234567890'), '⠼⠁⠃⠉⠙⠑⠋⠛⠓⠊⠚');
    }

    function test_braille_of_a1() {
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('a1'), '⠁⠼⠁');
    }

    function test_braille_of_1a() {
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('1a'), '⠼⠁⠰⠁');
    }

    function test_braille_of_letter_capital_A() {
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('A'), '⠠⠁');
    }

    function test_braille_of_letter_capital_AA() {
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('AA'), '⠠⠠⠁⠁');
    }

    function test_braille_of_letter_lowercase_a_uppercase_A() {
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('aA'), '⠁⠠⠁');
    }

    function test_braille_of_letter_uppercase_A_lowercase_a() {
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('Aa'), '⠠⠁⠁');
    }

    function test_braille_for_lowercase_22b () {
	# The Rules of Unified English Braille, p. 51
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('22b'), '⠼⠃⠃⠰⠃');
    }

    function test_braille_for_uppercase_22B () {
	# The Rules of Unified English Braille, p. 51
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('22B'), '⠼⠃⠃⠠⠃');
    }

    function test_braille_for_C_is_for_candy () {
	# The Rules of Unified English Braille, p. 57
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('C is for candy.'), '⠠⠉ ⠊⠎ ⠋⠕⠗ ⠉⠁⠝⠙⠽⠲');
    }

    function test_braille_for_Question_3c () {
	# The Rules of Unified English Braille, p. 57
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('Question 3c'), '⠠⠟⠥⠑⠎⠞⠊⠕⠝ ⠼⠉⠰⠉');
    }

    function test_braille_for_point_7 () {
	# The Rules of Unified English Braille, p. 60
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('.7'), '⠼⠲⠛');
    }

    function test_braille_for_4_500_000 () {
	# The Rules of Unified English Braille, p. 60
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('4 500 000'), '⠼⠙⠐⠑⠚⠚⠐⠚⠚⠚');
    }

    function test_braille_for_ab_slash_cd () {
	# The Rules of Unified English Braille, p. 17
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('ab/cd'), '⠁⠃⠸⠌⠉⠙');
    }

    function test_braille_for_l__f () {
	# The Rules of Unified English Braille, p. 17
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('l__f'), '⠇⠨⠤⠨⠤⠋');
    }

    function test_braille_for_report3_doc () {
	# The Rules of Unified English Braille, p. 62
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('report3.doc'), '⠗⠑⠏⠕⠗⠞⠼⠉⠲⠰⠙⠕⠉');
    }

    function test_braille_for_report3_xls () {
	# The Rules of Unified English Braille, p. 62
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('report3.xls'), '⠗⠑⠏⠕⠗⠞⠼⠉⠲⠭⠇⠎');
    }

    # This is grade 2
#    function test_braille_for_If_I_go_1st_will_you_go_2nd () {
#	# The Rules of Unified English Braille, p. 63
#	$this->assertEqual($t->translate_to_braille('If you go 1st—will I go 2nd?'), '⠠⠊⠋ ⠽ ⠛ ⠼⠁⠎⠞⠠⠤⠺ ⠠⠊ ⠛ ⠼⠃⠝⠙⠦');
#    }

    function test_braille_for_U_S_A_ () {
	# The Rules of Unified English Braille, p. 70
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('U.S.A.'), '⠠⠥⠲⠠⠎⠲⠠⠁⠲');
    }

    function test_braille_for_2_point_5_percent () {
	# The Rules of Unified English Braille, p. 70
	$t = new PigeonUEB(1);
	$this->assertEqual($t->translate_to_braille('2.5%'), '⠼⠃⠲⠑⠨⠴');
    }

}

?>
