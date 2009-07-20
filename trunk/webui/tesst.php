<html>
	<head>
		<title>
		</title>
	</head>
	
	<body>
		
<?php
	print "O sa execut un script<br>";
	$limba=array();
	exec("/user/bin/python /home/cspay/web-exec-scripts/call.py \"2008/2009\" \"Universitatea Politehnica Bucuresti\" \"Facultatea de Automatica si Calculatoare\" \"Calculatoare\" \"Marius Zaharia\"",&$liwba);
	
	foreach($liwba as $lisk)
		print $lisk."<br>";

	echo "<br>Am terminat";
?>
	</body>
</html>