<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(ADMINISTRATOR);//verifica daca este cont de administrator

$layout->get_template('include/template.html');
$layout->replace('TITLE','Administrare');

$meniu = show_menu(menu,$_SESSION['tip_cont'],2);
$layout->replace('MENU',$meniu);

//$submeniu = show_menu(submenu,$_SESSION['tip_cont'],-1,-1);
$layout->replace('SUBMENU','');

$content = "";
add($content,'<div class="title" align="center">Asistenta</div>');
add($content,'<br>');

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

