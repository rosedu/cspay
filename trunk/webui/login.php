<?php
//include headers
include("include/config.php");
include("include/header.php");



if($_SESSION['logged'] == true)//daca deja este inregistrat
{
		header("Location: index.php");//redirectioneaza catre pagina principala
		die();
}

$mesaj = "";
$eroare = 0;

//verificare formular de inregistrare
if(isset($_POST['login']))
{
	//preia datele transmise in formular
	$utilizator = addslashes($_POST['utilizator']);
	$parola = addslashes($_POST['parola']);
	
	$query = "SELECT * FROM `utilizatori` WHERE `utilizator` = '".$utilizator."' AND `parola` = '".$parola."'";
	$result = mysql_query($query);//realizeaza interogarea
	$count = mysql_num_rows($result);//numarul de intrari selectate
	
	if($count == 0)
	{
		add($mesaj,'<div class="eroare">Eroare : Utilizator sau parola incorecte.</div>');
		$_SESSION['utilizator'] = "";
		$_SESSION['parola'] = "";
		$_SESSION['tip_cont'] = -1;
		$eroare = 1;
	}
	else 
	if($count == 1)//daca s-a gasit utilizatorul in tabela admin
	{
		$_SESSION['logged'] = true;
		$_SESSION['utilizator'] = $utilizator; 
		$_SESSION['parola'] = $parola;
		$_SESSION['tip_cont'] = mysql_result($result,0,'tip_cont');
		$link_cat = mysql_result($result,0,'link_cat');
		
		$query = "SELECT * FROM `catedre` WHERE `cat_id` = '$link_cat'";
		$cat_result = mysql_query($query);
		$_SESSION['cat_nume'] = stripslashes(mysql_result($cat_result,0,'nume'));
		$link_fac = mysql_result($cat_result,0,'link_fac');
		
		$query = "SELECT * FROM `facultati` WHERE `fac_id` = '$link_fac'";
		$fac_result = mysql_query($query);
		$_SESSION['fac_nume'] = stripslashes(mysql_result($fac_result,0,'nume'));
		$link_univ = mysql_result($fac_result,0,'link_univ');
		
		$query = "SELECT * FROM `universitati` WHERE `univ_id` = '$link_univ'";
		$univ_result = mysql_query($query);
		$_SESSION['univ_id'] = mysql_result($univ_result,0,'univ_id');
		$_SESSION['univ_nume'] = stripslashes(mysql_result($univ_result,0,'nume'));
		
		header("Location: index.php");//redirectioneaza catre pagina principala
		die();
	}	
}

$layout->get_template('include/template.html');
$layout->replace('TITLE','Inregistrare');
$layout->replace('MENU','');
$layout->replace('SUBMENU','');

$content = "";
add($content,'<div class="title" align="center">Inregistrare</div>');

if($eroare == 1)//utilizator sau parola gresite
{
	add($content,$mesaj . '<br>');
}

add($content,'<div id="login">
	<form action="login.php" method="post">
	Utilizator<br />
	<input type="text" name="utilizator"><br>
	Parola<br />
	<input type="password" name="parola"><br><br>
	<input type="submit" name="login" value="Inregistrare"><br><br>
	<a href="pass_recovery.php">Recuperare parola</a>
	</form>
	</div>');
add($content,'<br><br><p class="debug">Utilizator : secretara , profesor sau admin <br>Parola : root</p>');

$layout->replace('CONTENT',$content);

$layout->print_template();
?>


