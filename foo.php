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

function numeral($n) {
    global $numerals;
    return $numerals[$n];
} /* numeral */

function time_pp($t) {
    $tmp = array();
    $tmp['h'] = floor($t/3600);
    $tmp['min'] = floor($t/60)%60;
    $tmp['s'] = $t - 3600*$tmp['h'] - 60*$tmp['min'];
    $it = '';
    foreach ($tmp as $unit => $n) {
	if ($n) {
	    if ($it) {
		$it .= ' ';
	    } /* if */
	    $it .= "$n $unit";
	} /* if */
    } /* foreach */
    return $it? $it: '0 s';
} /* time_pp */

function movement_descriptor($dots, $direction_label) {
    $message = '';
    if (count($dots) == 1) {
	$message = sprintf('blind %s is going %s',
		numeral($dots[0]), $direction_label);
    } elseif (count($dots) == 2) {
	$message = sprintf('blinds %s and %s are going %s',
		numeral($dots[0]), numeral($dots[1]), $direction_label);
    } elseif (count($dots) > 2) {
	for ($i = 0; $i < count($dots) - 1; $i += 1) {
	    if ($i > 0) {
		$message .= ', ';
	    } /* if */
	    $message .= numeral($dots[$i]);
	} /* for */
	$message = sprintf('blinds %s and %s are going %s',
		$message, numeral($dots[count($dots) - 1]), $direction_label);
    } /* if */
    return $message;
}

function describe_state($s0) {
    ;
} /* describe_state */

function describe_blind_movements($movements) {
    $ups = array();
    $downs = array();

    for ($i = 0; $i < count($movements); $i += 1) {
	if ($movements[$i] < 0) {
	    array_push($ups, $i + 1);
	} elseif ($movements[$i] > 0) {
	    array_push($downs, $i + 1);
	} /* if */
    } /* for */

    $ups_message = movement_descriptor($ups, 'up');
    $downs_message = movement_descriptor($downs, 'down');

    if ($ups_message && $downs_message) {
	printf("Narrator: %s, and %s.\n", ucfirst($ups_message), $downs_message);
    } elseif ($ups_message) {
	printf("Narrator: %s.\n", ucfirst($ups_message));
    } elseif ($downs_message) {
	printf("Narrator: %s.\n", ucfirst($downs_message));
    } else {
	print "Narrator: Blinds are not moving.\n";
    } /* if */
} /* describe_blind_movements */

function describe_braille_cell($dots_in_cell) {
    if (count($dots_in_cell) > 0) {
	$description = ucfirst(join(', ', array_map(numeral, $dots_in_cell)) . '.');
    } else {
	$description = "blank";
    } /* if */
    print "Captioner: $description\n";
} /* describe_braille_cell */

function pretend_to_do_mechanical_control($movements) {
    $comment = '';
    for ($i = 0; $i < count($movements); $i += 1) {
	if ($movements[$i] && $comment) {
	    $comment .= ', ';
	} /* if */
	$action_label = ($movements[$i] < 0)? 'up':
			($movements[$i] > 0? 'down': '');
	if ($action_label) {
	    $amount = (float) abs($movements[$i]);
	    $amount_label = (1 == $amount)? 'fully':
			    (0.5 == $amount? 'halfway': $amount);
	    $comment .= sprintf('move blind %d %s %s',
		    $i + 1, $amount_label, $action_label);
	} /* if */
    } /* for */
    if (!$comment) {
	$comment = 'none';
    } /* if */
    print "% actions: $comment\n";
} /* pretend_to_do_mechanical_control */

function emulate_delay($t) {
    usleep($t*1000000);
} /* emulate_delay */

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
    $debug_pos = 0;
    $debug_end = count($dots);

    foreach ($dots as $dots_in_cell) {
	$debug_progress = $debug_pos/$debug_end;
	$debug_progress_percent = floor(100*$debug_pos/$debug_end);
	$debug_eta = $debug_pos? time_pp((time() - $t_0)/$debug_progress*(1 - $debug_progress)): 'unknown';
	print "% progress: $debug_pos/$debug_end ($debug_progress_percent%), eta: $debug_eta\n";

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
	    if ($k == 0) {
		$movements = array();
	    } /* if */
	    array_push($movements, $delta[$p]);
	    if ($j + 1 == 6 || $k + 1 == $number_of_windows) {
		pretend_to_do_mechanical_control($movements);
		emulate_delay($estimated_time_needed_for_state_change*0.2);
		describe_blind_movements($movements);
		emulate_delay($estimated_time_needed_for_state_change*0.3);
	    } /* if */
	} /* for */
	describe_braille_cell($dots_in_cell);
	$state = $dots_in_cell;
	$debug_pos += 1;
    } /* foreach */

    print "$braille\n";
    print "$message\n";

    $dt_label = time_pp(time() - $t_0);
    print "% $dt_label s were spent transmitting this message.\n";
} /* foreach */


?>
