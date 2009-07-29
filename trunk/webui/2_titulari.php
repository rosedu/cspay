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

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],5,0);
$layout->replace('SUBMENU',$submeniu);

//tratare formulare
if(isset($_POST['adauga_tit']))
{
	$nume = addslashes(html_entity_decode($_POST['nume']));
	$an = $_POST['an'];
	$serie = $_POST['serie'];
	$disc = $_POST['disciplina'];
	
	$query = "INSERT INTO `titulari` (`nume`,`an`, `serie`, `link_disc`) 
			  VALUES('".$nume."','".$an."','".$serie."',$disc)";
	$result = mysql_query($query);
	
	if($result)
	{
		add($mesaj,'Titularul a fost adaugat in baza de date.<br><br>');
	}
		else
			add($mesaj,'<div class="eroare">Eroare : Titularul nu a putut fi adaugat in baza de date.');
	@mysql_free_result($result);
}
else
if(isset($_POST['modif_tit']))
	{
		foreach($_POST as $index=>$val)
			{
			$$index = $val;
			}
		
		$query = "UPDATE `titulari` SET `link_disc`=".$disciplina.",`nume`='".$nume."',
										  `serie`='".$serie."', `an`=".$an."
				WHERE `tit_id`=".$tit_id." LIMIT 1";

		if(mysql_query($query))
		{
			add($mesaj,'Modificarea a fost realizat&#x103; cu succes.<br>');
		}
		else
			add($mesaj,'<div class="eroare">Eroare ap&#x103;rut&#x103; la aplicarea modific&#x103;rii.</div>');
	}

if(isset($_GET['sterge']))
{
	$query = "DELETE FROM `titulari` WHERE `tit_id`='".$_GET['sterge']."' LIMIT 1";
	$result = mysql_query($query);
	if($result)
	{
		add($mesaj,'Titularul a fost sters din baza de date.<br><br>');
	}
		else
			dd($mesaj,'<div class="eroare">Eroare : Titularul nu a putut fi sters din baza de date.');
	@mysql_free_result($result);
}

$content = '';
add($content,'<div class="title" align="center">Titulari</div>');
add($content,'<br>');
add($content,$mesaj);

if(isset($_GET['modifica']))
	{
	$msj = "Modifica titular";
	$id = $_GET['modifica'];
	$query = "SELECT * FROM `titulari` WHERE `tit_id`='".$id."' LIMIT 1";
	$result = mysql_query($query);
	$nr = mysql_num_rows($result);
	$nume = stripslashes(mysql_result($result,0,'nume'));
	$serie = stripslashes(mysql_result($result,0,'serie'));
	$nume = htmlspecialchars($nume,ENT_QUOTES);
	$an = mysql_result($result,0,'an');
	$herarch = display_select_herarch(5,$id);
	$finish = '"modif_tit" value="Modific&#259;"></td>';
	$finish .= '<td colspan="2"><input type="submit" name="renunta" value="Renun&#x21B;&#259;">';
	$hide = '<input type="hidden" name="tit_id" value="'.$id.'">';
	}
else
	{
	$msj = "Adauga titular";
	$nume = "";
	$serie = "";
	$an = "";
	$herarch = display_select_herarch(5,0);
	$finish = '"adauga_cat" value="Adaug&#259;">';
	$hide ='';
	}

//formular de adaugare cont
add($utilizator_plus,'<form action="2_titulari.php" method="post">
				<table class="formular">
					<tr>
						<td colspan="5">'.$msj.'</td>
					</tr>
					<tr>'.
						$herarch
					.'</tr>
					<tr>
						<td>Nume </td><td><input type="text" name="nume" value="'.$nume.'"></td>
						<td>An</td><td><input type="text" name="an" value='.$an.'></td> 
						<td>Serie</td><td><input type="text" name="serie" value="'.$serie.'"></td>
					</tr>
					<tr>
						<td><input align="center" type="submit" name='.$finish.'</td>
					</tr>
				</table>'.$hide.'				
				</form><br>');
				
//afisare utilizatori existenti
$tabel = '';
$query = "SELECT * FROM `titulari` ORDER BY 'an', 'link_disc', 'serie'";
$res_asoc = mysql_query($query);
$nr = mysql_num_rows($res_asoc);

if($nr)
{
add($tabel,'<table class="special" cellpading="1" cellspacing="1" width="90%">
			<tr class="tr_head">
				<td>Nr</td>
				<td>Nume</td>
				<td>Disciplina</td>
				<td>An</td>
				<td>Serie</td>
				<td>Catedra</td>
				<td>Facultatea</td>
				<td>Universitatea</td>
				<td>Optiuni</td>
			</tr>');
			
for($i=0;$i<$nr;$i++)
{
	$Tnume = mysql_result($res_asoc,$i,'nume');
	$Tnume = htmlspecialchars(stripslashes($Tnume),ENT_QUOTES);
	
	$Tan = mysql_result($res_asoc,$i,'an');
	$Tserie = mysql_result($res_asoc,$i,'serie');

	$tt_ID = mysql_result($res_asoc,$i,'tit_id');
	
	$val = mysql_result($res_asoc,$i,'link_disc');
	$query = "SELECT * FROM `discipline` WHERE `disc_id` = '$val'";
	$cat_result = mysql_query($query);
	$val = mysql_result($cat_result,0,'nume');
	$Tdisc = html_entity_decode($val);
	
	$val = mysql_result($cat_result,0,'link_cat');
	$query = "SELECT * FROM `catedre` WHERE `cat_id` = '$val'";
	$cat_result = mysql_query($query);
	$val = mysql_result($cat_result,0,'nume');
	$Tcat = html_entity_decode($val);
	
	$val = mysql_result($cat_result,0,'link_fac');
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
					<td>'.$Tdisc.'</td>
					<td>'.$Tan.'</td>
					<td>'.$Tserie.'</td>
					<td>'.$Tcat.'</td>
					<td>'.$Tfac.'</td>
					<td>'.$Tuniv.'</td>
					<td><a href="2_titulari.php?sterge='.$tt_ID.'">sterge</a>
						<a href="2_titulari.php?modifica='.$tt_ID.'">modifica</a></td>
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

