<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(SECRETARA);//verifica daca este cont de secretara

$layout->get_template('include/template.html');
$layout->replace('TITLE','Calendar');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],-1,0);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Calendar</div>');
add($content,'<br>');

add($content,'<div class="content_center">');

add($content,'<h1>Optiuni posibile :</h1>');
add($content,'<ul>
			  <li><i><b>universitate</b></i><br>
					Permite modificarea numelui Universitatii, si perioada de desfasurare a semestrului. 
			  </li>
			  <li><i><b>facultate</b></i><br>
			  		Permite adaugarea, stergerea si editarea facultatilor din cadrul universitatii. <br>
					Pentru fiecare facultate in parte se pot modifica catedrele componente.
			  </li>
			  <li><i><b>vacante</b></i><br>
			  		Editarea perioadelor de vacanta se face adaugand data de inceput si data de sfarsit a perioadei.<br>
					Vacantele de o zi incep si se termina in ziua respectiva.
			  </li>
			  </ul>');

add($content,'</div>');

$layout->replace('CONTENT',$content);


$layout->print_template();
?>

