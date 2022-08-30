<?php
 
$Temp = $_GET['Temp'];
$Humi = $_GET['Humi'];
$headingDegrees = $_GET['headingDegrees'];
$pressure = $_GET['pressure'];
$date = new DateTime("now", new DateTimeZone('Asia/Shanghai') );


$string = $Temp . "\n" . $Humi . "\n" . $headingDegrees . "\n" . $pressure . "\n" . $date->format('Y-m-d H:i:s');

$fp = fopen("log.txt","w");
fwrite($fp, $string);  
fclose($fp);  
 
echo "<h1>Data logged</h1>";
echo $date->format('Y-m-d H:i:s');
?>
