<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once 'a2b.inc';
require_once 'b2d.inc';
require_once 'messages.inc';
require_once 'scan_order.inc';
require_once 'windows.inc';
require_once 'en_words.inc';

$number_of_windows = 2;
$estimated_time_needed_for_state_change = 5;
$debug_comments_enabled = false;

$t = new PigeonUEB(1);
$d = new PigeonDots;

$iter = new PigeonScanOrder($number_of_windows == 2?
	PigeonScanOrder::SCAN_ORDER_LTR():
	PigeonScanOrder::SCAN_ORDER_BRAILLE());
$win = new PigeonWindows($number_of_windows);

function msg($label, $message) {
    printf("%s  %s: %s\n", strftime('%H:%M:%S', time()), $label, $message);
} /* labelled_message */

function d_log() {
    global $debug_comments_enabled;
    if ($debug_comments_enabled) {
	msg('DEBUG', call_user_func_array(sprintf, func_get_args()));
    } /* if */
} /* d_log */

function narrate() {
    msg('Narrator', call_user_func_array(sprintf, func_get_args()));
} /* narrate */

function caption() {
    msg('Captioner', call_user_func_array(sprintf, func_get_args()));
} /* caption */

function describe_progress($debug_pos, $debug_end, $t_0) {
    $debug_progress = $debug_pos/$debug_end;
    $debug_progress_percent = floor(100*$debug_pos/$debug_end);
    $debug_eta = $debug_pos? PigeonWords::time_qty_si((time() - $t_0)/$debug_progress*(1 - $debug_progress)): 'unknown';
    d_log("progress: %d/%d (%d%%), eta: %s", $debug_pos, $debug_end, $debug_progress_percent, $debug_eta);
} /* describe_progress */

function describe_blind_movements($movements) {
    global $win;
    narrate($win->describe_blind_movements($movements));
} /* describe_blind_movements */

function describe_blinds($state, $delta) {
    global $win;
    narrate(ucfirst($win->describe_blinds($state, $delta)));
} /* describe_blind_movements */

function describe_braille_cell($dots_in_cell) {
    if (count($dots_in_cell) > 0) {
	$description = ucfirst(join(', ', array_map('PigeonWords::numeral',
						    $dots_in_cell)) . '.');
    } else {
	$description = "blank";
    } /* if */
    caption($description);
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
    d_log("actions: %s", $comment);
} /* pretend_to_do_mechanical_control */

function emulate_delay($t) {
    usleep($t*1000000);
} /* emulate_delay */

ob_implicit_flush(TRUE);
date_default_timezone_set('EST5EDT');

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

    d_log("message: %s (%s)", $braille, $message);

    foreach ($dots as $dots_in_cell) {
	describe_progress($debug_pos, $debug_end, $t_0);

	# Permute the dots in case we are not using natural Braille order
	$unpermuted_dots = $d->expand($dots_in_cell);
	$permuted_dots = array();
	for ($iter->rewind(); $iter->valid(); $iter->next()) {
	    array_push($permuted_dots, $unpermuted_dots[$iter->current() - 1]);
	} /* for */

	d_log('cell: [%s] for [%s]', join(', ', $permuted_dots),
		join(', ', $unpermuted_dots));

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
		d_log("intent: [%s] -> [%s]", join(', ', $state), join(', ', $target));
		d_log("delta = [%s]", join(', ', $delta));
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
    d_log("%s s were spent transmitting this message.", $dt_label);
} /* foreach */


?>
