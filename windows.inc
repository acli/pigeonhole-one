<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once 'en_words.inc';

class PigeonWindows {

    public $number_of_windows = 1;

    function __construct($number_of_windows) {
	$this->number_of_windows = $number_of_windows;
    } /* __construct */

    function describe_one_something($dots, $label) {
	$message = '';
	if (count($dots) == 1) {
	    if ($this->number_of_windows == 1) {
		$blind_label = 'the blind';
	    } else {
		$blind_label = 'blind ' . PigeonWords::numeral($dots[0]);
	    } /* if */
	    $message = sprintf('%s is %s',
		    $blind_label, $label);
	} elseif (count($dots) == 2) {
	    $message = sprintf('blinds %s and %s are %s',
		    PigeonWords::numeral($dots[0]),
		    PigeonWords::numeral($dots[1]),
		    $label);
	} elseif (count($dots) > 2) {
	    for ($i = 0; $i < count($dots) - 1; $i += 1) {
		if ($i > 0) {
		    $message .= ', ';
		} /* if */
		$message .= PigeonWords::numeral($dots[$i]);
	    } /* for */
	    $message = sprintf('blinds %s and %s are %s',
		    $message,
		    PigeonWords::numeral($dots[count($dots) - 1]),
		    $label);
	} /* if */
	return $message;
    } /* describe_one_something */

    function describe_one_direction($dots, $direction_label) {
	return $this->describe_one_something($dots, "going $direction_label");
    } /* describe_one_direction */

    function describe_one_state($dots, $state_label) {
	return $this->describe_one_something($dots, $state_label);
    } /* describe_one_state */

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
	    $it = sprintf("%s, and %s.",
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

    function describe_blinds($state) {
	$ups = array();
	$downs = array();
	$sortofs = array();

	for ($i = 0; $i < count($state); $i += 1) {
	    if ($state[$i] == 0) {
		array_push($ups, $i + 1);
	    } elseif ($state[$i] == 1) {
		array_push($downs, $i + 1);
	    } else {
		array_push($sortofs, $i + 1);
	    } /* if */
	} /* for */

	$ups_message = $this->describe_one_state($ups, 'rolled up');
	$downs_message = $this->describe_one_state($downs, 'rolled down');
	$sortofs_message = $this->describe_one_state($sortofs, 'halfway down');

	if ($ups_message && $downs_message && $sortofs_message) {
	    $it = sprintf("%s, %s, and %s.",
		    ucfirst($ups_message), $downs_message, $sortofs_message);
	} elseif ($ups_message && $downs_message) {
	    $it = sprintf("%s, and %s.",
		    ucfirst($ups_message), $downs_message);
	} elseif ($ups_message && $sortofs_message) {
	    $it = sprintf("%s, and %s.",
		    ucfirst($ups_message), $sortofs_message);
	} elseif ($downs_message && $sortofs_message) {
	    $it = sprintf("%s, and %s.",
		    ucfirst($downs_message), $sortofs_message);
	} elseif ($ups_message) {
	    $it = sprintf("%s.", ucfirst($ups_message));
	} elseif ($downs_message) {
	    $it = sprintf("%s.", ucfirst($downs_message));
	} elseif ($sortofs_message) {
	    $it = sprintf("%s.", ucfirst($sortofs_message));
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
