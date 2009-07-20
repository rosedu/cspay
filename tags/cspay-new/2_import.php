<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(ADMINISTRATOR);//verifica daca este cont de administrator

$layout->get_template('include/template.html');
$layout->replace('TITLE','Administrare');

$meniu = show_menu(menu,$_SESSION['tip_cont'],1);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],-1,1);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Import</div>');
add($content,'<br>');

add($content,'<div class="content_center">');

add($content,'<h1>Optiuni posibile :</h1><ul><li><i><b>orar XLS</b></i><br>
			  Permite configurarea initiala a orarului prin popularea unei tabele, folosind un fisier XLS.
			  <li><i><b>asociere XLS</b></i><br>Permite importarea datelor personale din format XLS, ale persoanelor cu drept de autentificare.
			  </li></ul>');

add($content,'</div>');

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

