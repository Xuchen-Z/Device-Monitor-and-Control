<?php
copy("state_LED.txt", "state_LED_copy.txt");
$LED = fopen("state_LED_copy.txt", "r");
if(!feof($LED)){
	$data = fgets($LED);
}
echo $data;
fclose($LED);
?>
