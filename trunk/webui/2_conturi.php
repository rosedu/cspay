<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(ADMINISTRATOR);//verifica daca este cont de administrator

$layout->get_template('include/template.html');
$layout->replace('TITLE','Administrare');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],-1,0);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Conturi</div>');
add($content,'<br>');

add($content,'<div class="content_center">');

add($content,'<h1>Optiuni posibile :</h1><ul><li><i><b>utilizatori</b></i><br>Permite adaugarea, editarea si stergerea
			  datelor apartinand persoanelor cu drept de autentificare.<br>Datele inserate sunt reprezentate de numele 
			  persoanei, adresa de email, catedra/materia si tipul contului asociat.<br>Exista trei tipuri de conturi:<br>
			  <ul><li>secretara - acces la incarcarea orarului si exportul tabelelor de plata si orarelor</li>
			  	  <li>profesor - acces limitat la editarea orarului</li>
				  <li>administrator - configurarea conturilor si a orarului initial</li>
			  </ul>
			  <li><i><b>asociere materie</b></i><br>Permite configurarea conturilor necesare autentificarii.<br>
			  Modificarea intrarilor din orar asociate unei materii poate fi realizata doar de profesorii care predau materia respectiva.<br>
			  Astfel, este configurat un singur cont per materie, fiind posibile setarea unui nume de utilizator si a unei parole.</li></ul>');

add($content,'</div>');

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

