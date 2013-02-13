<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

function array_push_if_nonempty(&$a, $elem) {
    if ($elem) {
	array_push($a, $elem);
    } /* if */
} /* array_push_if_nonempty */

function cmp($a, $b) {
    return $a == $b? 0: ($a < $b? -1: 1);
} /* cmp */

?>
