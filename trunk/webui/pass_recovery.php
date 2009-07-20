<?php
//include headers
include("include/config.php");
include("include/header.php");

$mesaj = "";

//verificare formular de inregistrare
if(isset($_POST['recupereaza']))
{
	$email = $_POST['email'];//preia adresa de email

	//...de completat  : trimis parola pe email
	
	//echo 'adresa email : ' . $email;
	
	$query = "SELECT * FROM `asocieri` WHERE `email`='".$email."' LIMIT 1";
	$result = mysql_query($query);
	$nr_asoc = mysql_num_rows($result);
	
	//echo "<br>nr inreg $nr_asoc";
	
	if($nr_asoc == 0 || $result == null)
	{
		$mesaj = "Nu exista nici un utilizator cu aceasta adresa de email.<br>";
	}
	else
	{
		$link_admin = mysql_result($result,0,'link_admin');
		$query = "SELECT * FROM `utilizatori` WHERE `admin_id`='".$link_admin."' LIMIT 1";
		$res_admin = mysql_query($query);
		
		$materie = mysql_result($res_admin,0,'materie');
		$utilizator = mysql_result($res_admin,0,'utilizator');
		$parola = mysql_result($res_admin,0,'parola');
		
		$to = $email;
		$subject = "Recuperare parola cont CSpay";
		$from = "no_reply@cspay.rosedu.org";
		$msg = "Recuperare parola cont CSpay:<br>
			Cont asociat materiei :".$materie."<br>
			Utilizator :" .$utilizator."<br>
			Parola : ".$parola;
		
		if(mail($to,$subject,$msg))
			$mesaj = 'Parola a fost trimisa pe adresa de email <b>'.$email.'</b>';//genereaza mesaj
		else
			$mesaj = '<div class="eroare">Eroare aparuta la trimiterea email-ului.<br></div>';
	}
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


