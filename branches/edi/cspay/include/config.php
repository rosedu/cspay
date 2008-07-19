<?php
set_time_limit(0);
error_reporting(0);
session_name("cspay");
session_start();
// Informatii baza de date 


$AdresaBazaDate = "localhost";
$NumeBazaDate = "cspay";
$UtilizatorBazaDate = "cspay";
$ParolaBazaDate = "C2p4yp422";


$conexiune = mysql_connect($AdresaBazaDate,$UtilizatorBazaDate,$ParolaBazaDate)
	     or die("I can't connect to MySQL");
mysql_select_db($NumeBazaDate,$conexiune)
	     or die("I can't find the ".$NumeBazaDate." database");
?>
