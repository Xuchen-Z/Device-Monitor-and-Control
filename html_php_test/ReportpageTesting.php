<?php
echo "<style>
		table, th, td {
			border:5px solid black;
		}
	</style>";
echo "<body style = 'background-color:LightGray;'>";
echo "<br>";
echo "<br>";
echo "<br>";
echo "<h1 style = 'background-color:Tomato; text-align:center; color:white; font-size:350%;'>Enviroment Report</h1>";
echo "<br>";
echo "<br>";

copy("log.txt", "log_copy.txt");
$fileR = fopen("log_copy.txt", "r");

echo "<table style='width:70%; table-layout:fixed; border-collapse: collapse; margin-left: auto; margin-right: auto; font-size:175%; color:SlateBlue; text-align:center'>
		<tr>
			<th>Tempreture</th>
			<th>Humidity</th>
			<th>Heading</th>
			<th>Pressure</th>
			<th>Time</th>
		</tr>
		<tr>";
			while(!feof($fileR)){
				echo "<td>" . fgets($fileR) . "</td>";
			}
	echo "</tr>
	</table>";
fclose($fileR);

echo "<meta http-equiv='refresh' content='3'>";
?>