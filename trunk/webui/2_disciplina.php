<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");
include("include/display.php");

check_auth(ADMINISTRATOR);//verifica daca este cont de administrator

$mesaj = '';
$select_materie = display_select_materie();//conturile disponibile
$select_tip = display_select_tip();//tipuri posibile

$layout->get_template('include/template.html');
$layout->replace('TITLE','Administrare');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],4,0);
$layout->replace('SUBMENU',$submeniu);

//tratare formulare
if(isset($_POST['adauga_disc']))
{
	$nume = addslashes(html_entity_decode($_POST['nume']));
	$nume_sc = addslashes(html_entity_decode($_POST['nume_scurt']));
	$cat = $_POST['catedra'];
	
	$query = "INSERT INTO `discipline` (`nume`,`nume_scurt`, `link_cat`) 
			  VALUES('".$nume."','".$nume_sc."',$cat)";
	$result = mysql_query($query);
	
	if($result)
	{
		add($mesaj,'Disciplina a fost adaugata in baza de date.<br><br>');
	}
		else
			add($mesaj,'<div class="eroare">Eroare : Disciplina nu a putut fi adaugata in baza de date.');
	@mysql_free_result($result);
}
else
if(isset($_POST['modif_disc']))
	{
		foreach($_POST as $index=>$val)
			{
			$$index = $val;
			}
		
		$query = "UPDATE `discipline` SET `link_cat`=".$catedra.",`nume`='".$nume."',
										  `nume_scurt`='".$nume_scurt."'
				WHERE `disc_id`=".$disc_id." LIMIT 1";

		if(mysql_query($query))
		{
			add($mesaj,'Modificarea a fost realizat&#x103; cu succes.<br>');
		}
		else
			add($mesaj,'<div class="eroare">Eroare ap&#x103;rut&#x103; la aplicarea modific&#x103;rii.</div>');
	}

if(isset($_GET['sterge']))
{
	$query = "DELETE FROM `discipline` WHERE `disc_id`='".$_GET['sterge']."' LIMIT 1";
	$result = mysql_query($query);
	if($result)
	{
		add($mesaj,'Disciplina a fost stearsa din baza de date.<br><br>');
	}
		else
			dd($mesaj,'<div class="eroare">Eroare : Disciplina nu a putut fi stearsa din baza de date.');
	@mysql_free_result($result);
}

$content = '';
add($content,'<div class="title" align="center">Discipline</div>');
add($content,'<br>');
add($content,$mesaj);

if(isset($_GET['modifica']))
	{
	$msj = "Modifica disciplina";
	$id = $_GET['modifica'];
	$query = "SELECT * FROM `discipline` WHERE `disc_id`='".$id."' LIMIT 1";
	$result = mysql_query($query);
	$nr = mysql_num_rows($result);
	$nume = stripslashes(mysql_result($result,0,'nume'));
	$numesc = stripslashes(mysql_result($result,0,'nume_scurt'));
	$nume = htmlspecialchars($nume,ENT_QUOTES);
	$herarch = display_select_herarch(3,$id);
	$finish = '"modif_disc" value="Modific&#259;"></td>';
	$finish .= '<td colspan="2"><input type="submit" name="renunta" value="Renun&#x21B;&#259;">';
	$hide = '<input type="hidden" name="disc_id" value="'.$id.'">';
	}
else
	{
	$msj = "Adauga disciplina";
	$nume = "";
	$numesc = "";
	$herarch = display_select_herarch(3,0);
	$finish = '"adauga_cat" value="Adaug&#259;">';
	$hide ='';
	}
//formular de adaugare cont
add($utilizator_plus,'<form action="2_disciplina.php" method="post">
				<table class="formular">
					<tr>
						<td colspan="5">'.$msj.'</td>
					</tr>
					<tr>'.
						$herarch
					.'</tr>
					<tr>
						<td>Nume </td><td><input type="text" name="nume" value="'.$nume.'"></td>
						<td>Nume scurt</td><td><input type="text" name="nume_scurt" value="'.$numesc.'"></td> 
					</tr>
					<tr>
						<td><input align="center" type="submit" name='.$finish.'</td>
					</tr>
				</table>'.$hide.'				
				</form><br>');
				
//afisare utilizatori existenti
$tabel = '';
$query = "SELECT * FROM `discipline` ORDER BY 'link_cat'";
$res_asoc = mysql_query($query);
$nr = mysql_num_rows($res_asoc);

if($nr)
{
add($tabel,'<table class="special" cellpading="1" cellspacing="1" width="90%">
			<tr class="tr_head">
				<td>Nr</td>
				<td>Nume</td>
				<td>Nume scurt</td>
				<td>Catedra</td>
				<td>Facultatea</td>
				<td>Universitatea</td>
				<td>Optiuni</td>
			</tr>');
			
for($i=0;$i<$nr;$i++)
{
	$Tnume = mysql_result($res_asoc,$i,'nume');
	$Tnume = htmlspecialchars(stripslashes($Tnume),ENT_QUOTES);
	
	$Tnumesc = mysql_result($res_asoc,$i,'nume_scurt');
	$Tnumesc = stripslashes($Tnumesc);
	$Tnumesc = htmlspecialchars($Tnumesc,ENT_QUOTES);

	$dc_ID = mysql_result($res_asoc,$i,'disc_id');
	
	$val = mysql_result($res_asoc,$i,'link_cat');
	if($val)
		{
		$query = "SELECT * FROM `catedre` WHERE `cat_id` = '$val'";
		$cat_result = mysql_query($query);
		$val = mysql_result($cat_result,0,'nume');
		$Tcat = html_entity_decode($val);
		$val = mysql_result($cat_result,0,'link_fac');
		}
	else
		{
		$Tcat='';
		$val = mysql_result($res_asoc,$i,'link_fac');
		}
		
	$query = "SELECT * FROM `facultati` WHERE `fac_id` = '$val'";
	$cat_result = mysql_query($query);
	$val = mysql_result($cat_result,0,'nume_scurt');
	$Tfac = html_entity_decode($val);
		
	$val = mysql_result($cat_result,0,'link_univ');
	$query = "SELECT * FROM `universitati` WHERE `univ_id` = '$val'";
	$cat_result = mysql_query($query);
	$val = mysql_result($cat_result,0,'nume');
	$Tuniv = html_entity_decode($val);
	
	add($tabel,'<tr class="'.$class_std[$i%2].'">
					<td>'.($i+1).'</td>
					<td>'.$Tnume.'</td>
					<td>'.$Tnumesc.'</td>
					<td>'.$Tcat.'</td>
					<td>'.$Tfac.'</td>
					<td>'.$Tuniv.'</td>
					<td><a href="2_disciplina.php?sterge='.$dc_ID.'">sterge</a>
						<a href="2_disciplina.php?modifica='.$dc_ID.'">modifica</a></td>
				</tr>');

}

add($tabel,'</table>');
}
else
	add($tabel,'Nu exista catedre definite.<br>');

mysql_free_result($res_asoc);

add($content,$utilizator_plus);
add($content,$tabel);

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

