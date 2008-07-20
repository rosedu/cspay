<?php
include("/config/config.php"); 
require_once 'Excel/reader.php';
$data = new Spreadsheet_Excel_Reader();
$data->setOutputEncoding('CP1251');
$data->read('Model acopSemI-2006-07.xls');
error_reporting(E_ALL ^ E_NOTICE);

$con = mysql_connect($AdresaBazaDate, $UtilizatorBazaDate,$ParolaBazaDate);
if (!$con)
  {
  die('Could not connect: ' . mysql_error());
  }
mysql_select_db($NumeBazaDate, $con) or die(mysql_error());
$ok=1;$i=3;
while ($ok!=0)
{
	if($data->sheets[0]['cells'][$i][1]!=NULL)
		{
			$sql="INSERT INTO `".$NumeBazaDate."`.`orar` (".$NumColumn.") VALUES ('Automatica si Calculatoare', ";
			
			for ($j=2; $j <= 13;$j++)		
				{
					if($j==2)	$sql=$sql."'".$data->sheets[0]['cells'][$i][$j]."'";
					else $sql=$sql.", '".$data->sheets[0]['cells'][$i][$j]."'";
					if($j==13) $sql=$sql.")";
				}
		$i++;
		if(($sql!=$sql_anterior)&&(i!=3))	
		mysql_query($sql,$con) or die(mysql_error());
		$sql_anterior=$sql;
		}
	else $ok=0;
}

mysql_close($con);
?>