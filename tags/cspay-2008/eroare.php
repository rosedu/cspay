<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

$layout->get_template('include/template.html');
$layout->replace('TITLE','prima pagina');

$meniu = show_menu(menu,$_SESSION['tip_cont'],-1);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,-1);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Eroare</div>');
add($content,'<div class="eroare">Nu aveti permisiunea pentru a accesa aceasta pagina.</div>');

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

