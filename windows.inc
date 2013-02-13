<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once 'en_words.inc';
require_once 'util.inc';

class PigeonWindows {

    public $number_of_windows = 1;

    function __construct($number_of_windows) {
	$this->number_of_windows = $number_of_windows;
    } /* __construct */

    function array_push_if_no_info(&$a, $elem) {
	if ($elem && $elem[1]) {
	    array_push($a, $elem);
	} /* if */
    } /* array_push_if_nonempty */

    function describe_one_something($dots, $verb_s, $verb_p, $complement) {
	$message = '';
	if (count($dots) == 1) {
	    if ($this->number_of_windows == 1) {
		$blind_label = 'the blind';
	    } else {
		$blind_label = 'blind ' . PigeonWords::numeral($dots[0]);
	    } /* if */
	    $message = sprintf('%s %s %s', $blind_label, $verb_s, $complement);
	} elseif (count($dots) == $this->number_of_windows) {
	    $message = sprintf('all the blinds %s %s', $verb_p, $complement);
	} elseif (count($dots) == 2) {
	    $message = sprintf('blinds %s and %s %s %s',
		    PigeonWords::numeral($dots[0]),
		    PigeonWords::numeral($dots[1]),
		    $verb_p,
		    $complement);
	} elseif (count($dots) > 2) {
	    for ($i = 0; $i < count($dots) - 1; $i += 1) {
		if ($i > 0) {
		    $message .= ', ';
		} /* if */
		$message .= PigeonWords::numeral($dots[$i]);
	    } /* for */
	    $message = sprintf('blinds %s and %s %s %s',
		    $message,
		    PigeonWords::numeral($dots[count($dots) - 1]),
		    $verb_p,
		    $complement);
	} /* if */
	return str_replace('  ', ' ', $message);
    } /* describe_one_something */

    function describe_one_direction($dots, $direction_label) {
	return $this->describe_one_something($dots, 'is going', 'are going',
		$direction_label);
    } /* describe_one_direction */

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

	$ups_message = $this->describe_one_direction($ups, 'up');
	$downs_message = $this->describe_one_direction($downs, 'down');

	if ($ups_message && $downs_message) {
	    if ($ups[0] < $downs[0]) {
		$a = $ups_message;
		$b = $downs_message;
	    } else {
		$a = $downs_message;
		$b = $ups_message;
	    } /* if */
	    $it = sprintf("%s, but %s.", ucfirst($a), $b);
	} elseif ($ups_message) {
	    $it = sprintf("%s.", ucfirst($ups_message));
	} elseif ($downs_message) {
	    $it = sprintf("%s.", ucfirst($downs_message));
	} else {
	    $it = sprintf("%s not moving.",
		    ($this->number_of_windows == 1?
			    'The blind is':
			    'Blinds are'));
	} /* if */
	return $it;
    } /* describe_blind_movements */

    function describe_blinds($state, $movements) {
	$new_p = true;

	$ups_static = array();
	$ups_moved = array();
	$downs_static = array();
	$downs_moved = array();
	$sortofs_static = array();
	$sortofs_moved = array();

	for ($i = 0; $i < count($state); $i += 1) {
	    if ($state[$i] == 0) {
		if ($movements[$i] == 0) {
		    array_push($ups_static, $i + 1);
		} else {
		    array_push($ups_moved, $i + 1);
		} /* if */
	    } elseif ($state[$i] == 1) {
		if ($movements[$i] == 0) {
		    array_push($downs_static, $i + 1);
		} else {
		    array_push($downs_moved, $i + 1);
		} /* if */
	    } else {
		if ($movements[$i] == 0) {
		    array_push($sortofs_static, $i + 1);
		} else {
		    array_push($sortofs_moved, $i + 1);
		} /* if */
	    } /* if */
	} /* for */

	$info = array();

	if ($new_p) {
	    $ups = array_merge($ups_static, $ups_moved);
	    $downs = array_merge($downs_static, $downs_moved);
	    $sortofs = array_merge($sortofs_static, $sortofs_moved);

	    uasort($ups, function ($a, $b) { return cmp($a[0], $b[0]); });
	    uasort($downs, function ($a, $b) { return cmp($a[0], $b[0]); });
	    uasort($sortofs, function ($a, $b) { return cmp($a[0], $b[0]); });

	    $ups_info = array($ups[0],
			      $this->describe_one_something(
				  array_merge($ups_static, $ups_moved),
				  '', '', 'rolled up'));
	    $downs_info = array($downs[0],
				$this->describe_one_something(
				    array_merge($downs_static, $downs_moved),
				    '', '', 'rolled down'));
	    $sortofs_info = array($sortofs[0],
				  $this->describe_one_something(
				   array_merge($sortofs_static, $sortofs_moved),
				   '', '', 'halfway down'));

	    $this->array_push_if_no_info($info, $ups_info);
	    $this->array_push_if_no_info($info, $downs_info);
	    $this->array_push_if_no_info($info, $sortofs_info);

	} else {
	    $ups_static_info = array($ups_static[0],
			$this->describe_one_something(
			$ups_static, 'remains', 'remain', 'rolled up'));
	    $ups_moved_info = array($ups_moved[0],
			$this->describe_one_something(
			$ups_moved, 'is', 'are', 'now rolled up'));
	    $downs_static_info = array($downs_static[0],
			$this->describe_one_something(
			$downs_static, 'remains', 'remain', 'rolled down'));
	    $downs_moved_info = array($downs_moved[0],
			$this->describe_one_something(
			$downs_moved, 'is', 'are', 'now rolled down'));
	    $sortofs_static_info = array($sortofs_static[0],
			$this->describe_one_something(
			$sortofs_static, 'remains', 'remain', 'halfway down'));
	    $sortofs_moved_info = array($sortofs_moved[0],
			$this->describe_one_something(
			$sortofs_moved, 'is', 'are', 'now halfway down'));

	    $this->array_push_if_no_info($info, $ups_static_info);
	    $this->array_push_if_no_info($info, $ups_moved_info);
	    $this->array_push_if_no_info($info, $downs_static_info);
	    $this->array_push_if_no_info($info, $downs_moved_info);
	    $this->array_push_if_no_info($info, $sortofs_static_info);
	    $this->array_push_if_no_info($info, $sortofs_moved_info);

	} /* if */
	uasort($info, function ($a, $b) {
		return $a[0] == $b[0]? 0: ($a[0] < $b[0]? -1: 1); });

	if (count($info) > 2) {
	    $it = join(', ', array_map(function ($a) {
			return $a[1]; }, array_slice($info, 0, count($info))))
		    . ', and ' . $info[count($info) - 1][1];
	} elseif (count($info) == 2) {
	    $it = join(($new_p? ', ': '; '), array($info[0][1], $info[1][1]));
	} elseif (count($info)) {
	    $it = $info[0][1];
	} else {
	    throw new Exception('Internal error: No messages generated');
	} /* if */

	if ($new_p) {
	    $it = 'You see ' . $it;
	} /* if */
	$it .= '.';

	return $it;
    } /* describe_blinds */

} /* PigeonWindows */

?>
