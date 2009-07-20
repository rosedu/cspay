<?php
include("config.php");
//verificare autentificare
if(!isset($_SESSION['logged']))
{
	$_SESSION['logged'] = false;
}

if($_SESSION['logged'] ==  false)
	{
	$_SESSION['utilizator'] = "";
	$_SESSION['parola'] = "";
	$_SESSION['tip_cont'] = -1;
	$_SESSION['materie'] = '';
	header("Location: login.php");
	die();
	}
?>