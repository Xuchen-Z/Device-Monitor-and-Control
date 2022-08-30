<?php
$LED = fopen("state_LED.txt", "w");
fwrite($LED, "On");  
fclose($LED);
?>