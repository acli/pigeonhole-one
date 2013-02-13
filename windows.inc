<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once 'en_words.inc';

class PigeonWindows {

    public $number_of_windows = 1;

    function __construct($number_of_windows) {
	$this->number_of_windows = $number_of_windows;
    } /* __construct */

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
	return $message;
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
	    $it = sprintf("%s, but %s.",
		    ucfirst($ups_message), $downs_message);
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

	$ups_static_message = $this->describe_one_something($ups_static,
		'remains', 'remain', 'rolled up');
	$ups_moved_message = $this->describe_one_something($ups_moved,
		'is', 'are', 'now rolled up');
	$downs_static_message = $this->describe_one_something($downs_static,
		'remains', 'remain', 'rolled down');
	$downs_moved_message = $this->describe_one_something($downs_moved,
		'is', 'are', 'now rolled down');
	$sortofs_static_message = $this->describe_one_something($sortofs_static,
		'remains', 'remain', 'halfway down');
	$sortofs_moved_message = $this->describe_one_something($sortofs_moved,
		'is', 'are', 'now halfway down');

	$messages = array();
	if ($ups_static_message) {
	    array_push($messages, $ups_static_message);
	} /* if */
	if ($ups_moved_message) {
	    array_push($messages, $ups_moved_message);
	} /* if */
	if ($downs_static_message) {
	    array_push($messages, $downs_static_message);
	} /* if */
	if ($downs_moved_message) {
	    array_push($messages, $downs_moved_message);
	} /* if */
	if ($sortofs_static_message) {
	    array_push($messages, $sortofs_static_message);
	} /* if */
	if ($sortofs_moved_message) {
	    array_push($messages, $sortofs_moved_message);
	} /* if */
	if (count($messages) > 2) {
	    $it = join(', ', array_slice($messages, 0, count($messages)))
		    . ', and ' . $messages[count($messages) - 1];
	} elseif (count($messages) == 2) {
	    $it = join('; ', $messages);
	} elseif (count($messages)) {
	    $it = $messages[0];
	} else {
	    $it = sprintf("%s not moving.",
		    ($this->number_of_windows == 1?
			    'The blind is':
			    'Blinds are'));
	} /* if */
	return $it;
    } /* describe_blind_movements */

} /* PigeonWindows */

?>
