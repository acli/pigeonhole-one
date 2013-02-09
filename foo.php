<?php

include 'a2b.inc';

ob_start('mb_output_handler');

print translate_to_braille('“The next 50 years will belong to the most creative problem seekers.” – Christopher Simmons') . "\n";

?>
