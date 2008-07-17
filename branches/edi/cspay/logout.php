<?php
include("include/config.php");
$_SESSION['logged'] = false;
$_SESSION['utilizator'] = "";
$_SESSION['parola'] = "";
$_SESSION['tip_cont'] = -1;
$_SESSION['materie'] = '';
session_destroy();
include("include/check_login.php");


?>