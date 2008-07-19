<?php

require_once '../excel/reader.php';
include '../config/config.php';

$xls_file = "test.xls";

/* open MS Excel spreadsheet file */
$data = new Spreadsheet_Excel_Reader();
$data->setOutputEncoding('CP1251');
$data->read($xls_file);
error_reporting(E_ALL ^ E_NOTICE);

/* connect to database */
$con = mysql_connect($hostname, $dbusser, $dbpass);
if (!$con) {
	die('Could not connect: ' . mysql_error());
}

mysql_select_db($dbname, $con);

/* clear everything first */
$sql = "TRUNCATE TABLE $table";
mysql_query($sql, $con);

/* add spreadsheet data to database */
for ($i = 1; $i <= $data->sheets[0]['numRows']; $i++) {
	$sql = "INSERT INTO `$table`(`col1`, `col2`, `col3`) VALUES ('".$data->sheets[0]['cells'][$i][1]."', '" . $data->sheets[0]['cells'][$i][2] . "', '" . $data->sheets[0]['cells'][$i][3] . "')";
	echo "added row " . $i . "\n";
	mysql_query($sql,$con);
}

mysql_close($con);

?>
