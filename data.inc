<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

function read_tab_separated_data($input) {
    $it = array();
    $h = fopen($input, 'r');
    $re_data_line = '/^(\S+)\t(\S+)(?:\s*)(?:\s[#;].*)?$/u';
    $re_comment_line = '/^\s*(?:[#;][^\t]*)?$/u';
    if ($h) {
	$lc = 0;
	for (;;) {
	    $s = fgets($h);
	if ($s === false) { break; }
	    $lc += 1;
	    $s = rtrim($s);
	    if (preg_match($re_data_line, $s, $matches)) {
		$it[$matches[1]] = $matches[2];
	    } elseif (preg_match($re_comment_line, $s)) {
		;
	    } else {
		error_log("$input:$lc: Malformed data line");
	    } /* if */
	} /* for */
    } else {
	throw new Exception("Cannot open input file \"$input\"");
    }
    fclose($h);
    return $it;
} /* read_tab_separated_data */

function read_colon_separated_data($input) {
    $it = array();
    $h = fopen($input, 'r');
    $re_data_line = '/^(\S+):\s*(.+?)(?::([^:]*))?$/u';
    $re_comment_line = '/^\s*(?:[#;][^\t]*)?$/u';
    if ($h) {
        $lc = 0;
        for (;;) {
            $s = fgets($h);
        if ($s === false) { break; }
            $lc += 1;
            $s = rtrim($s);
            if (preg_match($re_comment_line, $s)) {
                ;
            } elseif (preg_match($re_data_line, $s, $matches)) {
                array_push($it, array($matches[1], $matches[2], $matches[3]));
            } else {
                error_log("$input:$lc: Malformed data line");
            } /* if */
        } /* for */
    } else {
        throw new Exception("Cannot open input file \"$input\"");
    }
    fclose($h);
    return $it;
} /* read_messages */

?>
