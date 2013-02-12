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

$numerals = array(
    1 => 'one',
    2 => 'two',
    3 => 'three',
    4 => 'four',
    5 => 'five',
    6 => 'six'
);

function describe_state($s0) {
    ;
} /* describe_state */

function describe_blind_movements($movements) {
    global $numerals;

    $ups = array();
    $downs = array();

    for ($i = 0; $i < count($movements); $i += 1) {
	if ($movements[$i] < 0) {
	    array_push($ups, $i + 1);
	} elseif ($movements[$i] > 0) {
	    array_push($downs, $i += 1);
	} /* if */
    } /* for */

    $ups_message = '';
    $downs_message = '';

    if (count($ups) == 1) {
	$ups_message = sprintf('blind %s going up', $numerals[$ups[0]]);
    } elseif (count($ups) == 2) {
	$ups_message = sprintf('blinds %s and %s going up',
		$numerals[$ups[0]], $numerals[$ups[1]]);
    } elseif (count($ups) > 2) {
	for ($i = 0; $i < count($ups) - 1; $i += 1) {
	    if ($i > 0) {
		$ups_message .= ', ';
	    } /* if */
	    $ups_message .= $ups[$i];
	} /* for */
	$ups_message = sprintf('blinds %s and %s going up',
		$ups_message, $numerals[$ups[count($ups) - 1]]);
    } /* if */

    if (count($downs) == 1) {
	$downs_message = sprintf('blind %s going down', $numerals[$downs[0]]);
    } elseif (count($downs) == 2) {
	$downs_message = sprintf('blinds %s and %s going down',
		$numerals[$downs[0]], $numerals[$downs[1]]);
    } elseif (count($downs) > 2) {
	for ($i = 0; $i < count($downs) - 1; $i += 1) {
	    if ($i > 0) {
		$downs_message .= ', ';
	    } /* if */
	    $downs_message .= $downs[$i];
	} /* for */
	$downs_message = sprintf('blinds %s and %s going down',
		$downs_message, $numerals[$downs[count($downs) - 1]]);
    } /* if */

    if ($ups_message && $downs_message) {
	print "Narrator: You see $ups_message, and $downs_message.\n";
    } elseif ($ups_message) {
	print "Narrator: You see $ups_message.\n";
    } elseif ($downs_message) {
	print "Narrator: You see $downs_message.\n";
    } else {
	print "Narrator: The blinds do not seem to be moving.\n";
    } /* if */
} /* describe_blind_movements */

function describe_braille_cell($dots_in_cell) {
    if (count($dots_in_cell) > 0) {
	$description = ucfirst(join(', ', array_map(function ($n) {
		    global $numerals;
		    return $numerals[$n];
		}, $dots_in_cell)) . '.');
    } else {
	$description = "blank";
    } /* if */
    print "Captioner: $description\n";
} /* describe_braille_cell */

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
	describe_braille_cell($dots_in_cell);
	$state = $dots_in_cell;
    } /* foreach */

    print "$braille\n";
    print "$message\n";

    $dt = time() - $t_0;
    print "% $dt s were spent transmitting this message.\n";
} /* foreach */


?>
