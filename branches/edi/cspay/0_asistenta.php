<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

$layout->get_template('include/template.html');
$layout->replace('TITLE','prima pagina');

$meniu = show_menu(menu,$_SESSION['tip_cont'],2);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],-1,2);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Asistenta</div>');
add($content,'<br>Tipul Contului : '.$_SESSION['tip_cont']);

$layout->replace('CONTENT',$content);

$query = "INSERT INTO `admin` (`materie`,`utilizator`) VALUES('uso','')";
mysql_query($query);

$layout->print_template();
?>

