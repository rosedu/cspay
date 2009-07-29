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

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],3,0);
$layout->replace('SUBMENU',$submeniu);

//tratare formulare
if(isset($_POST['adauga_cat']))
{
	$nume = addslashes(html_entity_decode($_POST['nume']));
	$shef = addslashes(html_entity_decode($_POST['shef']));
	$fac = $_POST['facultate'];
	
	$query = "INSERT INTO `catedre` (`nume`,`sef`, `link_fac`) 
			  VALUES('".$nume."','".$shef."',$fac)";
	$result = mysql_query($query);
	
	if($result)
	{
		add($mesaj,'Catedra a fost adaugata in baza de date.<br><br>');
	}
		else
			add($mesaj,'<div class="eroare">Eroare : Catedra nu a putut fi adaugata in baza de date.');
	@mysql_free_result($result);
}
else
if(isset($_POST['modif_cat']))
	{
		foreach($_POST as $index=>$val)
			{
			$$index = $val;
			}
		
		$query = "UPDATE `catedre` SET `link_fac`=".$facultate.",`nume`='".$nume."',
										  `sef`='".$shef."'
				WHERE `cat_id`=".$cat_id." LIMIT 1";

		if(mysql_query($query))
		{
			add($mesaj,'Modificarea a fost realizat&#x103; cu succes.<br>');
		}
		else
			add($mesaj,'<div class="eroare">Eroare ap&#x103;rut&#x103; la aplicarea modific&#x103;rii.</div>');
	}

if(isset($_GET['sterge']))
{
	$query = "DELETE FROM `catedre` WHERE `cat_id`='".$_GET['sterge']."' LIMIT 1";
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
add($content,'<div class="title" align="center">Catedre</div>');
add($content,'<br>');
add($content,$mesaj);

if(isset($_GET['modifica']))
	{
	$msj = "Modifica catedra";
	$id = $_GET['modifica'];
	$query = "SELECT * FROM `catedre` WHERE `cat_id`='".$id."' LIMIT 1";
	$result = mysql_query($query);
	$nr = mysql_num_rows($result);
	$nume = stripslashes(mysql_result($result,0,'nume'));
	$sef = stripslashes(mysql_result($result,0,'sef'));
	$nume = htmlspecialchars($nume,ENT_QUOTES);
	$sef = htmlspecialchars($sef,ENT_QUOTES);
	$herarch = display_select_herarch(2,$id);
	$finish = '"modif_cat" value="Modific&#259;"></td>';
	$finish .= '<td colspan="2"><input type="submit" name="renunta" value="Renun&#x21B;&#259;">';
	$hide = '<input type="hidden" name="cat_id" value="'.$id.'">';
	}
else
	{
	$msj = "Adauga catedra";
	$nume = "";
	$sef = "";
	$herarch = display_select_herarch(2,0);
	$finish = '"adauga_cat" value="Adaug&#259;">';
	$hide ='';
	}
//formular de adaugare cont
add($utilizator_plus,'<form action="2_catedra.php" method="post">
				<table class="formular">
					<tr>
						<td colspan="5">'.$msj.'</td>
					</tr>
					<tr>'.
						$herarch
					.'</tr>
					<tr>
						<td>Nume </td><td><input type="text" name="nume" value="'.$nume.'"></td>
						<td>Sef catedra</td><td><input type="text" name="shef" value="'.$sef.'"></td> 
					</tr>
					<tr>
						<td><input align="center" type="submit" name='.$finish.'</td>
					</tr>
				</table>'.$hide.'				
				</form><br>');
				
//afisare utilizatori existenti
$tabel = '';
$query = "SELECT * FROM `catedre` ORDER BY 'link_univ'";
$res_asoc = mysql_query($query);
$nr = mysql_num_rows($res_asoc);

if($nr)
{
add($tabel,'<table class="special" cellpading="1" cellspacing="1" width="90%">
			<tr class="tr_head">
				<td>Nr</td>
				<td>Nume</td>
				<td>Sef catedra</td>
				<td>Facultatea</td>
				<td>Universitatea</td>
				<td>Optiuni</td>
			</tr>');
			
for($i=0;$i<$nr;$i++)
{
	$Tnume = mysql_result($res_asoc,$i,'nume');
	$Tnume = htmlspecialchars(stripslashes($Tnume),ENT_QUOTES);
	
	$Tshef = mysql_result($res_asoc,$i,'sef');
	$Tshef = stripslashes($Tshef);
	$Tshef = htmlspecialchars($Tshef,ENT_QUOTES);

	$ct_ID = mysql_result($res_asoc,$i,'cat_id');
	
	$val = mysql_result($res_asoc,$i,'link_fac');
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
					<td>'.$Tshef.'</td>
					<td>'.$Tfac.'</td>
					<td>'.$Tuniv.'</td>
					<td><a href="2_catedra.php?sterge='.$ct_ID.'">sterge</a>
						<a href="2_catedra.php?modifica='.$ct_ID.'">modifica</a></td>
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

