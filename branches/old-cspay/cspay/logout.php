<?php
include("include/config.php");
$_SESSION['logged'] = false;
$_SESSION['utilizator'] = "";
$_SESSION['parola'] = "";
$_SESSION['tip_cont'] = -1;
$_SESSION['materie'] = '';
$_SESSION['univ_id'] = '';
session_destroy();
include("include/check_login.php");


?>