<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");
include("include/display.php");

check_auth(ADMINISTRATOR);//verifica daca este cont de administrator

$mesaj = '';
$select_materie = display_select_materie();//conturile disponibile

$layout->get_template('include/template.html');
$layout->replace('TITLE','Administrare');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,0);
$layout->replace('SUBMENU',$submeniu);

//tratare formulare
if(isset($_POST['adauga_cont']))
{
	$nume = addslashes(html_entity_decode($_POST['nume']));
	$email = addslashes(html_entity_decode($_POST['email']));
	$materie = addslashes(html_entity_decode($_POST['materie']));
	
	$query = "INSERT INTO `asociatie` (`nume`,`email`,`link_admin`) 
			  VALUES('".$nume."','".$email."','".$materie."')";
	$result = mysql_query($query);
	
	if($result)
	{
		add($mesaj,'Utilizatorul a fost adaugat in baza de date.<br><br>');
	}
		else
			add($mesaj,'<div class="eroare">Eroare : Utilizatorul nu a putut fi adaugat in baza de date.');
	@mysql_free_result($result);
}
else
if(isset($_POST['modifica_cont']))
{
	foreach($_POST as $index=>$val)
	{
		$$index = addslashes(html_entity_decode($val));
		//echo $index.' = '.$val.'<br>';
	}
	$query = "UPDATE `asociatie` SET `nume`='".$nume."',`email`='".$email."',`link_admin`='.$materie.'
		  WHERE `asoc_id`='".$utilizator_id."' LIMIT 1";
	$result = mysql_query($query);
	@mysql_free_result($result);	
}

if(isset($_GET['sterge']))
{
	$query = "DELETE FROM `asociatie` WHERE `asoc_id`='".$_GET['sterge']."' LIMIT 1";
	$result = mysql_query($query);
	if($result)
	{
		add($mesaj,'Utilizatorul a fost sters din baza de date.<br><br>');
	}
		else
			dd($mesaj,'<div class="eroare">Eroare : Utilizatorul nu a putut fi sters din baza de date.');
	@mysql_free_result($result);
}
else
if(isset($_GET['modifica']))//trebuie afisat formularul pentru modificat datele unui utilizator
{
	$id = addslashes($_GET['modifica']);
	
	$query = "SELECT * FROM `asociatie` WHERE `asoc_id`='".$id."' LIMIT 1";
	$result = mysql_query($query);
	$nr = mysql_num_rows($result);
	
	if($nr == 0)
		{
		add($mesaj,'<div class="eroare">Eroare : Utilizator inexistent.</div></br>');
		break;
		}
	else
	{	
	
	$nume = stripslashes(mysql_result($result,0,'nume'));
	$email =stripslashes(mysql_result($result,0,'email'));
	
	$nume = htmlspecialchars($nume,ENT_QUOTES);
	$email = htmlspecialchars($email,ENT_QUOTES);
	
	$utilizator_modi = '';
	add($utilizator_modi,'<form action="2_utilizatori.php" method="post">
			<input type="hidden" name="utilizator_id" value="'.$id.'">
			<table class="formular">
					<tr>
					<td colspan="5">Modifica utilizator</td>
					</tr>
					<tr>
					<td>Nume <input type="text" name="nume" value="'.$nume.'"></td>
					<td>Email <input type="text" name="email" value="'.$email.'"></td> 
					<td>Materie/Tip '.$select_materie.'</td>
					<td><input type="submit" name="modifica_cont" value="Modifica"></td>
					</tr>
					</table>				
					</form><br>');
	}
	@mysql_free_result($result);
}

$content = "";
add($content,'<div class="title" align="center">Utilizatori</div>');
add($content,'<br>');
add($content,$mesaj);

//adaug formularul de modificare cont
if(isset($utilizator_modi))
{
	if(!empty($utilizator_modi))
	{
		add($content,$utilizator_modi);
	}
}

//formular de adaugare cont
add($utilizator_plus,'<form action="" method="post">
				<table class="formular">
					<tr>
						<td colspan="5">Adauga utilizator</td>
					</tr>
					<tr>
						<td>Nume <input type="text" name="nume"></td>
						<td>Email <input type="text" name="email"></td> 
						<td>Materie/Tip '.$select_materie.'</td>
						<td><input type="submit" name="adauga_cont" value="Adauga"></td>
					</tr>
				</table>				
				</form><br>');

//afisare utilizatori existenti
$tabel = '';
$query = "SELECT * FROM `asociatie`";
$res_asoc = mysql_query($query);
$nr = mysql_num_rows($res_asoc);

if($nr)
{
add($tabel,'<table class="special" cellpading="1" cellspacing="1" width="550px">
			<tr class="tr_head">
				<td>Nr</td>
				<td>Nume</td>
				<td>Email</td>
				<td>Materie/Tip</td>
				<td>Optiuni</td>
			</tr>');
			
for($i=0;$i<$nr;$i++)
{
	$asoc_id = mysql_result($res_asoc,$i,'asoc_id');
	$nume = mysql_result($res_asoc,$i,'nume');
	$nume = htmlspecialchars(stripslashes($nume),ENT_QUOTES);
	
	$email = mysql_result($res_asoc,$i,'email');
	$email = stripslashes($email);
	$email = htmlspecialchars($email,ENT_QUOTES);
	
	$link_admin = mysql_result($res_asoc,$i,'link_admin');
	
	$query = "SELECT * FROM `admin` WHERE `admin_id`='".$link_admin."' LIMIT 1";
	$res_cont = mysql_query($query);
	$nr_admin = mysql_num_rows($res_cont);
	
	if($nr_admin == 0)
		$materie = "nedefinit";
	else
	{
	$cont = mysql_result($res_cont,0,'tip_cont');
	$materie = html_entity_decode(mysql_result($res_cont,0,'materie'));
	}
	
	$vector_cont = array("Secretara","Profesor","Administrator");
	
	add($tabel,'<tr class="'.$class_select[$i%2].'">
					<td>'.($i+1).'</td>
					<td>'.$nume.'</td>
					<td>'.$email.'</td>
					<td>'.$materie.'</td>
					<td><a href="2_utilizatori.php?sterge='.$asoc_id.'">sterge</a>
						<a href="2_utilizatori.php?modifica='.$asoc_id.'">modifica</a></td>
				</tr>');
	mysql_free_result($res_cont);
}

add($tabel,'</table>');
}
else
	add($tabel,'Nu exista utilizatori definiti.<br>');

mysql_free_result($res_asoc);

add($content,$utilizator_plus);
add($content,$tabel);

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

