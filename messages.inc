<?php
/* vi: set sw=4 ai sm: */
/* vim: set filetype=php: */

require_once 'data.inc';

function read_messages() {
    return read_colon_separated_data('messages.txt');
} /* read_messages */

?>
