<?php

require_once 'Excel/reader.php';
$data = new Spreadsheet_Excel_Reader();
$data->setOutputEncoding('CP1251');
$data->read('Book1.xls');
error_reporting(E_ALL ^ E_NOTICE);

$con = mysql_connect("localhost","cspay","C2p4yp422");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  }
mysql_select_db("test", $con);

/*
$con = mysql_connect("localhost","root","zaraki");
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  }
mysql_select_db("test", $con);
*/
for ($i = 1; $i <= $data->sheets[0]['numRows']; $i++) {
	$sql="INSERT INTO `test`.`test` (`columne1`, `columne2`, `columne3`) VALUES ('".$data->sheets[0]['cells'][$i][1]."', '".$data->sheets[0]['cells'][$i][2]."', '".$data->sheets[0]['cells'][$i][3]."')";
	echo $data->sheets[0]['cells'][$i][3];
mysql_query($sql,$con);
}

mysql_close($con);



?>