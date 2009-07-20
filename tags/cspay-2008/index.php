<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

$layout->get_template('include/template.html');
$layout->replace('TITLE','prima pagina');

$meniu = show_menu(menu,$_SESSION['tip_cont'],9);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],-1,-1);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Pagina principala</div>');
add($content,'<br><div class="content_center">');

if($_SESSION['tip_cont'] == SECRETARA)
{
	add($content,'Informatii privind posibilitatile de editare pentru Secretara');
}
else
if($_SESSION['tip_cont'] == PROFESOR)
{
	add($content,'Informatii privind posibilitatile de editare pentru Profesor');
}
else
if($_SESSION['tip_cont'] == ADMINISTRATOR)
{
	add($content,'Informatii privind posibilitatile de editare pentru Administrator');
}

add($content,'<br><br>
	<b>CSpay</b><br>
CSpay este un proiect initial pentru a reduce munca asistentilor Politehnicii.<br> Intr-o lume a tehnicii, este nevoie de o permanenta ajustare a uneltelor pe care le folosim.<br> De ce sa pierzi o zi intreaga sa te gandesti daca e ziua de 15 pica luni sau marti sau miercuri, sau oare trebuie sa ma duc la cursuri maine ? .<br> Pentru toate acestea exista CSpay. <br>');

add($content,'</div>');

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

