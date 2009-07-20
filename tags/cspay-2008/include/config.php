<?php
set_time_limit(0);
//error_reporting(0);
session_name("cspay");
session_start();
// Informatii baza de date 

/* 
$AdresaBazaDate = "localhost";
$NumeBazaDate = "cspay";
$UtilizatorBazaDate = "root";
$ParolaBazaDate = "tutzy";
 */


$AdresaBazaDate = "koala.cs.pub.ro";
$NumeBazaDate = "rosedu_cspay";
$UtilizatorBazaDate = "rosedu_cspay";
$ParolaBazaDate = "huashaquou";


$conexiune = mysql_connect($AdresaBazaDate,$UtilizatorBazaDate,$ParolaBazaDate)
	     or die("I can't connect to MySQL");
mysql_select_db($NumeBazaDate,$conexiune)
	     or die("I can't find the ".$NumeBazaDate." database");
?>
