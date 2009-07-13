<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(SECRETARA);//verifica daca este cont de secretara

$layout->get_template('include/template.html');
$layout->replace('TITLE','prima pagina');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],-1,0);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Calendar</div>');
add($content,'<br>Tipul Contului : '.$_SESSION['tip_cont']);

$layout->replace('CONTENT',$content);


$layout->print_template();
?>

