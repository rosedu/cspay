<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(PROFESOR);//verifica daca este cont de secretara

$layout->get_template('include/template.html');
$layout->replace('TITLE','prima pagina');

$meniu = show_menu(menu,$_SESSION['tip_cont'],1);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],-1,1);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Orar</div>');
add($content,'<br>');

add($content,'<div class="content_center">');

add($content,'<h1>Optiuni posibile :</h1>');
add($content,'<ul>
			  <li><i><b>vizualizare</b></i><br>
					Permite vizualizarea orarului sub forma unui tabel. In cazul in care se doreste editarea unui camp anume, <br>
					aceste poate fi modificat direct din tabel. Pentru a salva modificarile efectuate este necesara folosirea<br>
					butonului "Salveaza".
			  </li>
			  <li><i><b>salvare xls</b></i><br>
			  		Permite salvarea orarului sub forma unui fisier in format Excel pentru anumite persoane, pe parcursul <br>
					uneia sau mai multor luni.
			  </li>
			  </ul>');

add($content,'</div>');

$layout->replace('CONTENT',$content);


$layout->print_template();
?>

