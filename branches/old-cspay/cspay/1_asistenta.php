<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(PROFESOR);//verifica daca este cont de secretara

if(isset($_POST['trimite']))
{
foreach($_POST as $index => $val)
	{
		$$index = $val;
	}
	$msg =  'Nume :'.$nume.'<br>Email : '.$email.'<br><br>Mesaj : '.$mesaj;
	$res = @mail("mail",$subiect,$msg);
}

$layout->get_template('include/template.html');
$layout->replace('TITLE','Trimite mesaj');

$meniu = show_menu(menu,$_SESSION['tip_cont'],2);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],-1,-1);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Asistenta</div>');

$form = '';
add($form,'
	<blockquote>
	<form action="" method="post">
		Numele : <br>
		<input type="text" name="nume" value="">
		<br>
		Email : <br>
		<input type="text" name="email" value="">
		<br>
		Subiect : 
		<br>
		<input type="text" name="subiect" value="">
		<br>
		Mesaj : <br>
		<textarea name="mesaj" style="width:300px;height:90px"></textarea>
		<br>
		<input type="submit" name="trimite" value="Trimite">
	</form>
	</blockquote>
	');
add($content,$form);

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

