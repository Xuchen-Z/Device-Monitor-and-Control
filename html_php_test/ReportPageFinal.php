<!DOCTYPE html>
<html lang="en">

<?php
copy("log.txt", "log_copy.txt");
$fileR = fopen("log_copy.txt", "r");

$data = array();
$counter = 0;

while(!feof($fileR)){
 $data[$counter] = fgets($fileR);
 $counter++;
}
fclose($fileR);
?>

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, shrink-to-fit=no">
    <title>ReportPage</title>
    <link rel="stylesheet" href="assets/bootstrap/css/bootstrap.min.css">
    <link rel="stylesheet" href="assets/css/styles.css">
</head>

<body>
    <h1 class="text-center" style="padding-top: 50px;padding-bottom: 50px;color: var(--bs-blue);">Environment Report</h1>
    <div>
        <div class="table-responsive text-left" style="padding-top: 20px;padding-right: 250px;padding-left: 250px;">
            <table class="table table-striped">
                <thead class="text-left">
                    <tr>
                        <th>Variables</th>
                        <th>Real-Time Data</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td>Temperature</td>
						<?php
						echo "<td>" . $data[0] . "</td>";
						?>
                    </tr>
                    <tr>
                        <td>Humidity</td>
                        <?php
						echo "<td>" . $data[1] . "</td>";
						?>
                    </tr>
                    <tr>
                        <td>Heading Degree</td>
                        <?php
						echo "<td>" . $data[2] . "</td>";
						?>
                    </tr>
                    <tr>
                        <td>Pressure</td>
                        <?php
						echo "<td>" . $data[3] . "</td>";
						?>
                    </tr>
                    <tr>
                        <td>Time &amp; Date</td>
                        <?php
						echo "<td>" . $data[4] . "</td>";
						?>
                    </tr>
                </tbody>
            </table>
			</div>
        <h3 class="text-center" style="padding-top: 50px;padding-bottom: 30px;color: var(--bs-blue);">LED Mode Setting</h3>
        <div style="text-align: center;">
            <div class="btn-group" role="group" style="width: 120px;margin-right: auto;margin-left: auto;">
				<button onclick="newPageOn(); closeWin(); alert('LED is now on')" class="btn btn-primary" type="button" style="width: 60px;">ON</button>
				<button onclick="newPageOff(); closeWin(); alert('LED is now off')" class="btn btn-primary" type="button" style="width: 60px;">OFF</button>
			</div>
			<footer class="text-center">
                <div class="container text-muted py-4 py-lg-5">
                    <ul class="list-inline"></ul>
                    <p class="mb-0">Developed by Xuchen Zheng using Bootstrap Studio</p>
                    <p class="mb-0">To see real-time data please contact me</p>
                </div>
            </footer>
        </div>
    </div>
    <script src="assets/bootstrap/js/bootstrap.min.js">
	</script>
	<script>
	let myWindow;
	
	function newPageOn(){
		myWindow = window.open("LedOn.php");
	}
	function newPageOff(){
		myWindow = window.open("LedOff.php");
	}
	function closeWin(){
		myWindow.close();
	}
	</script>
</body>

<?php
echo "<meta http-equiv='refresh' content='3'>";
?>

</html>