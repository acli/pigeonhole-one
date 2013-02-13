<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once 'a2b.inc';
require_once 'b2d.inc';
require_once 'messages.inc';
require_once 'scan_order.inc';
require_once 'windows.inc';
require_once 'en_words.inc';

$t = new PigeonUEB(1);
$d = new PigeonDots;

$number_of_windows = 2;
$estimated_time_needed_for_state_change = 5;

$iter = new PigeonScanOrder($number_of_windows == 2?
	PigeonScanOrder::SCAN_ORDER_LTR():
	PigeonScanOrder::SCAN_ORDER_BRAILLE());
$win = new PigeonWindows($number_of_windows);

function describe_progress($debug_pos, $debug_end, $t_0) {
    $debug_progress = $debug_pos/$debug_end;
    $debug_progress_percent = floor(100*$debug_pos/$debug_end);
    $debug_eta = $debug_pos? PigeonWords::time_qty_si((time() - $t_0)/$debug_progress*(1 - $debug_progress)): 'unknown';
    print "% progress: $debug_pos/$debug_end ($debug_progress_percent%), eta: $debug_eta\n";
} /* describe_progress */

function describe_blind_movements($movements) {
    global $win;
    print "Narrator: " . $win->describe_blind_movements($movements) . "\n";
} /* describe_blind_movements */

function describe_blinds($state, $delta) {
    global $win;
    print "Narrator: " . ucfirst($win->describe_blinds($state, $delta)) . ".\n";
} /* describe_blind_movements */

function describe_braille_cell($dots_in_cell) {
    if (count($dots_in_cell) > 0) {
	$description = ucfirst(join(', ', array_map('PigeonWords::numeral',
						    $dots_in_cell)) . '.');
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
for ($i = 0; $i < $number_of_windows; $i += 1) { /* Ruby is so much better... */
    array_push($state, 0);
} /* for */

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

    print "% message: $braille ($message)\n";

    foreach ($dots as $dots_in_cell) {
	describe_progress($debug_pos, $debug_end, $t_0);

	# Permute the dots in case we are not using natural Braille order
	$unpermuted_dots = $d->expand($dots_in_cell);
	$permuted_dots = array();
	for ($iter->rewind(); $iter->valid(); $iter->next()) {
	    array_push($permuted_dots, $unpermuted_dots[$iter->current() - 1]);
	} /* for */

	print '% cell: [' . join(', ', $permuted_dots)
		. '] for [' . join(', ', $unpermuted_dots) . "]\n";

	# Pad it with the inter-cell pattern
	for (;;) {
	    array_push($permuted_dots, 0.5);
	if (count($permuted_dots) % $number_of_windows == 0) { break; }
	} /* for */

	# Transmit the (permuted and padded) dot patterns
	$n = count($permuted_dots);
	for ($i = 0; $i < $n; $i += $number_of_windows) {
	    for ($j = 0; $j < $number_of_windows; $j += 1) {
		$target = array_slice($permuted_dots, $i, $number_of_windows);
		$delta = $d->diff_matrix($state, $target);
		print "% intent: [" . join(', ', $state) . "] -> [" . join(', ', $target) . "]\n";
		print "% delta = [" . join(', ', $delta) . "]\n";
		pretend_to_do_mechanical_control($delta);
		emulate_delay($estimated_time_needed_for_state_change*0.2);
		describe_blind_movements($delta);
		emulate_delay($estimated_time_needed_for_state_change*0.3);
		describe_blinds($target, $delta);
		$state = $target;
	    } /* for */
	} /* for */
	describe_braille_cell($dots_in_cell);
	$debug_pos += 1;
    } /* foreach */

    print "$braille\n";
    print "$message\n";

    $dt_label = PigeonWords::time_qty_si(time() - $t_0);
    print "% $dt_label s were spent transmitting this message.\n";
} /* foreach */


?>
