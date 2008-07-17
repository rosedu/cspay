<?php
//include headers
include("include/config.php");
include("include/header.php");

$mesaj = "";

//verificare formular de inregistrare
if(isset($_POST['recupereaza']))
{
	$utilizator = $_POST['email'];//preia adresa de email

	//...de completat  : trimis parola pe email
	
	echo 'adresa email : ' . $email;
	

	$mesaj = 'Parola a fost trimisa pe adresa de email <b>'.$email.'</b>';//genereaza mesaj
}

//incarca fisierul template
$layout->get_template('include/template.html');
$layout->replace('TITLE','Recuperare Parola');
$layout->replace('MENU','');
$layout->replace('SUBMENU','');

//creeaza continut
$content = "";
add($content,'<div class="title" align="center">Recuperare Parola</div>');
if($mesaj == "")// afiseaza formularul normal de recuperare 
{
add($content,'<div id="login">
	<form action="pass_recovery.php" method="post">
	Email<br />
	<input type="text" name="email"><br><br>
	<input type="submit" name="recupereaza" value="Recupereaza"><br><br>
	</form>
	</div>');
}
else //daca a fost primit formularul
	add($content,$mesaj);
$layout->replace('CONTENT',$content);//inlocuieste continutul 

$layout->print_template();//afiseaza pagina
?>


