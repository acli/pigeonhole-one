<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once 'a2b.inc';
require_once 'b2d.inc';
require_once 'messages.inc';
require_once 'scan_order.inc';

$t = new PigeonUEB(1);
$d = new PigeonDots;
$iter = new PigeonScanOrder(PigeonScanOrder::SCAN_ORDER_BRAILLE());

$number_of_windows = 3;
$estimated_time_needed_for_state_change = 5;

function describe_state($s0) {
    ;
} /* describe_state */

function describe_blind_movements($movements) {
    if (in_array(1, $movements) || in_array(-1, $movements)) {
	for ($w = 0; $w < $number_of_windows; $w += 1) {
	    if ($movements[$w] > 0) {
		printf("You see blind %d coming down\n", $w + 1);
	    } elseif ($movements[$w] < 0) {
		printf("You see blind %d going up\n", $w + 1);
	    } /* if */
	} /* for */
    } else {
	print "The blinds do not seem to be moving.\n";
    } /* if */
} /* describe_blind_movements */

ob_implicit_flush(TRUE);

$state = array();

foreach (read_messages() as $data) {
    $seq = $data[0];
    $quote = $data[1];
    $attribution = $data[2];
    $message = sprintf(($attribution? '“%s” – %s': '%s'), $quote, $attribution);
    $braille = $t->translate_to_braille($message);
    $dots = $d->dots_in_string($braille);

    $t_0 = time();

    foreach ($dots as $dots_in_cell) {
	describe_state($state);
	$delta = $d->diff_matrix($state, $dots_in_cell);
	print "% intent: [" . join(', ', $d->expand($state)) . '] -> [' . join(', ', $d->expand($dots_in_cell)) . "]\n";
	print "% delta = [" . join(', ', $delta) . "]\n";
	for (
	    $i = $iter->rewind(), $j = 0, $k = 0;
	    $i = $iter->valid();
	    $iter->next(), $j += 1, $k = ($k + 1) % $number_of_windows
	) {
	    $p = $iter->scan_order[$j] - 1; # braille cell numbers are 1-based
	    print "% DEBUG: j=$j, k=$k; p=$p\n";
	    if ($k == 0) {
		$movements = array($delta[$p]);
	    } else {
		array_push($movements, $delta[$p]);
	    }
	    if ($j + 1 == 6 || $k + 1 == $number_of_windows) {
		describe_blind_movements($movements);
		sleep($estimated_time_needed_for_state_change);
	    } /* if */
	} /* for */
	if (count($dots_in_cell) > 0) {
	    print join(', ', $dots_in_cell) . "\n";
	} else {
	    print "blank\n";
	} /* if */
	$state = $dots_in_cell;
    } /* foreach */

    print "$braille\n";
    print "$message\n";

    $dt = time() - $t_0;
    print "% $dt s were spent transmitting this message.\n";
} /* foreach */


?>
