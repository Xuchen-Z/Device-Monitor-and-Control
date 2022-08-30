<?php
$LED = fopen("state_LED.txt", "w");
fwrite($LED, "Off");  
fclose($LED);
?>
