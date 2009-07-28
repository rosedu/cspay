<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

define("CATEDRA_MODIIFCA",1);
define("FACULTATE_MODIFICA",2);

$mesaj = "";//mesaj afisat ; poate cuprinde erori, mesajde de atentionare etc
$flag_form = false;//daca deja a fost afisat un alt formular

function write_table_head()
{
	$output = "";
	add($output,'<table width="100%" cellpadding="1" cellspacing="1" class="special">');
	add($output,'<tr class="tr_head"><td>Nr</td><td>Nume</td><td>Nume Scurt</td><td>Decan</td><td>Optiuni</td></tr>');
	
	return $output;
}
//tratare formulare
if(isset($_POST['facultate_adauga']))//formular de adaugare facultate
{
	$nume = addslashes(html_entity_decode($_POST['nume']));//preia datele transmise in formular
	$nume_scurt = addslashes(html_entity_decode($_POST['nume_scurt']));
	$decan = addslashes(html_entity_decode($_POST['decan']));
	
	//textul interogarii
	$query = "INSERT INTO `facultati` (`link_univ`,`nume`,`nume_scurt`,`decan`) VALUES ('".addslashes($_SESSION['univ_id'])."',
			  '".$nume."','".$nume_scurt."','".$decan."');";
	if(mysql_query($query))//daca interogarea este corecta
	{
		add($mesaj,'Facultatea a fost adaugata cu succes.<br>');
	}
	else	
		add($mesaj,'Eroare la adaugarea facultatii.<br>');
}//sfarsit adaugare facultate
else
if(isset($_POST['catedra_adauga']))//formularul de adaugare catedra
{
	$nume = addslashes(html_entity_decode($_POST['nume']));//preia datele transmise in formular
	$link_fac = addslashes(html_entity_decode($_POST['link_fac']));
	$sef = addslashes(html_entity_decode($_POST['sef']));
	
	//textul interogarii
	$query = "INSERT INTO `catedre` (`link_fac`,`nume`,`sef`) VALUES ('".$link_fac."','".$nume."','".$sef."');";
	if(mysql_query($query))//daca interogarea este corecta
	{
		add($mesaj,'Catedra a fost adaugata cu succes.<br>');
	}
	else	
		add($mesaj,'Eroare la adaugarea catedrei.<br>');	
}//sfarsit adaugare catedra
else
if(isset($_POST['catedra_modifica']))//formularul de modificare catedra
{
	foreach($_POST as $index=>$val)//preia variabile de tip $_POST si le transforma in variabile 
	{
		$$index = addslashes(html_entity_decode($val));
	}
	$query = "UPDATE `catedre` SET `link_fac`='".$link_fac."',`nume`='".$nume."',`sef`='".$sef."' 
			 WHERE `cat_id`='".$cat_id."' LIMIT 1";
	$result = mysql_query($query);
	if($result)
	{
		add($mesaj,'Catedra a fost modificata cu succes.<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare : Modificare catedrei nu a fost realizata<div>');
	
}//sfarsit formular modificare catedra
else
if(isset($_POST['facultate_modifica']))//formularul de modificare facultate
{
	foreach($_POST as $index=>$val)//preia variabile de tip $_POST si le transforma in variabile 
		{
		$$index = addslashes(html_entity_decode($val));
		//add($mesaj,$index . '=>' . $val . '<br>');
		}
	$query = "UPDATE `facultati` SET `nume`='".$nume."',`nume_scurt`='".$nume_scurt."',`decan`='".$decan."'
			  WHERE `fac_id`='".$fac_id."' LIMIT 1";

	$result = mysql_query($query);
	if($result)
	{
		add($mesaj,'Facultatea a fost modificata cu succes.<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare : Modificare facultatii nu a fost realizata<div>');
}//sfarsit formular modificare faculatate

//tratare pagina si parametri transmisi
if(isset($_GET['catedra_sterge']))//parametru pentru stergerea catedrei
{
	
	$query = "DELETE FROM `catedre` WHERE `cat_id`='".addslashes($_GET['catedra_sterge'])."'";
	if(mysql_query($query))
	{
		add($mesaj,'Catedra a fost stearsa cu succes.<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare aparuta la stergerea catedrei.</div>');
}//sfarsit pagina stergere catedra
else
if(isset($_GET['catedra_modifica']))//parametru pentru modificarea catedrei
{
	add($mesaj,'catedra_modifica = ' . addslashes($_GET['catedra_modifica']) );
	$flag_form = CATEDRA_MODIFICA;
}//sfarsit pagina modificarea catedra
else
if(isset($_GET['facultate_sterge']))//parametru pentru stergerea facultatiif
{
	$query = "DELETE FROM `facultati` WHERE `fac_id`='".addslashes($_GET['facultate_sterge'])."'";
	if(mysql_query($query))
	{
		add($mesaj,'Facultatea a fost stearsa cu succes.<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare aparuta la stergerea facultatii.</div>');
}//sfarsit pagina stergere facultate
else
if(isset($_GET['facultate_modifica']))//parametru pentru modificarea facultatii
{
	$flag_form = FACULTATE_MODIFICA;
}//sfarsit pagina modificare catedra


$layout->get_template('include/template.html');//incarca fisierul template
if($flag_form == false)
	$layout->replace('TITLE','Modificare facultati');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);//incarca meniul
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],1,0);//incarca submeniul
$layout->replace('SUBMENU',$submeniu);

$query = "SELECT * FROM `facultati`";//selectez facultatile
$res_fac = mysql_query($query);
$nr = mysql_num_rows($res_fac);//numarul de facultati


$content = "";//continutul paginii
if($flag_form == false)//daca nu a mai fost afisat alt formular inainte
{
add($content,'<div class="title" align="center">Modificare facultati</div><br>');//Titlul paginii

if($mesaj != "")
	add($content, $mesaj);
	
//formularul de adaugare facultate
add($content,'<form action="0_facultate.php" method="post"> 
			<table class="formular">
				<tr>
					<td colspan="4">
					Adauga Facultate
					</td>
				</tr>
				<tr>
				<td>
				Nume <input type="text" name="nume" value="">
				</td>
				<td>
				Nume scurt <input type="text" name="nume_scurt" value="">
				</td>
				<td>
				Decan <input type="text" name="decan" name="decan">
				</td>
				<td align="left"><input type="submit" name="facultate_adauga" value="Adauga"></td>
				</tr>
			</table>
			</form>');
if($nr)
{
//formularul de adaugare catedra
add($content,'<form action="0_facultate.php" method="post"> 
			<table class="formular">
				<tr>
					<td colspan="4">
					Adauga Catedra
					</td>
				</tr>
				<tr>
				<td>
				Facultate <select name="link_fac" size="1" style="width:180px">');
//selectez si afisez facultatile
for($i=0;$i<$nr;$i++)
	{
		$nume_fac = stripslashes(mysql_result($res_fac,$i,'nume'));
		$id_fac = mysql_result($res_fac,$i,'fac_id');
		add($content,'<option value="'.$id_fac.'">'.htmlspecialchars($nume_fac,ENT_QUOTES).'</option>');
	}
							
add($content,'</select>
				</td>
				<td>
				Nume <input type="text" name="nume" value="">
				</td>
				<td>
				Sef <input type="text" name="sef">
				</td>
				<td align="left"><input type="submit" name="catedra_adauga" value="Adauga"></td>
				</tr>
			</table>
			</form><br>');

add($content,write_table_head());//scriu capul de tabel pentru facultati

//clasa pentru rand par respectiv impar

for($i=0;$i<$nr;$i++)//scriu fiecare intrare din tabela `facultate`
{
	$id_fac = mysql_result($res_fac,$i,'fac_id');//preiau datele din intrarea cu numarul $i
	$nume = stripslashes(mysql_result($res_fac,$i,'nume'));
	$nume_scurt = stripslashes(mysql_result($res_fac,$i,'nume_scurt'));
	$decan = stripslashes(mysql_result($res_fac,$i,'decan'));
		
	//scriu datele in tabel pe un rand nou
	add($content,'<tr class="'.$class_std[$i%2].'">
				  <td>'.($i+1).'</td>
				  <td>'.htmlspecialchars($nume,ENT_QUOTES).'</td>
				  <td>'.htmlspecialchars($nume_scurt,ENT_QUOTES).'</td>
				  <td>'.htmlspecialchars($decan,ENT_QUOTES).'</td>
				  <td><a href="?facultate_sterge='.$id_fac.'">sterge</a> <a href="?facultate_modifica='.$id_fac.'">modifica</a></td></tr>');
	//selectez catedrele atasate facultatii curente
	$query = "SELECT * FROM `catedre` WHERE `link_fac` = '".$id_fac."'";
	$res_cat = mysql_query($query);
	$nr_cat = mysql_num_rows($res_cat);//numarul de catedre
	
	if($nr_cat != 0)//daca sunt catedre petru facultate
	{
	//adauga un nou rand in tabelul facultatilor si deschid un nou tabel
	add($content,'<tr id="catedra_'.$i.'" class="'.$class_std[$i%2].'"><td>Catedre</td><td colspan="3">
					<table class="none" width="100%">
					<tr class="tr_head"><td>Nume</td><td>Sef</td><td>Optiuni</td></tr>');
	for($j=0;$j<$nr_cat;$j++)
	    {
		$nume = stripslashes(mysql_result($res_cat,$j,'nume'));
		$sef = stripslashes(mysql_result($res_cat,$j,'sef'));
		$id_catedra = mysql_result($res_cat,$j,'cat_id');
		
		add($content,'<tr class="'.$class_std[$j%2].'"><td width="33%">'.htmlspecialchars($nume,ENT_QUOTES).'</td><td width="33%">'.htmlspecialchars($sef,ENT_QUOTES)
					 .'</td><td width="34%"><a href="?catedra_sterge='.$id_catedra.'">sterge</a> 
					 <a href="?catedra_modifica='.$id_catedra.'">modifica</a></td></tr>');	
	    }
	add($content,'</table><td>&nbsp;</td></tr>');//inchid tabelul 
	
	}//gata $nr_cat != 0
}

add($content,'</table><br>');//inchid tabelul facultatilor
}//sfarsit if($nr)//daca nr de facultati e diferit de 0
}//sfarsit if($flag_form == false)

else
if($flag_form == CATEDRA_MODIFICA)
{
	    $layout->replace('TITLE','Formular modificare Catedra');//modific numele paginii
		add($content,'<div class="title" align="center">Modificare Catedra</div><br>');//Titlul paginii

		//selectez catedra
		$res_cat = get_catedra($_GET['catedra_modifica']);
		if(mysql_num_rows($res_cat))//daca  s-a gasit o inregistrare
		{	
		
		$cat_id = mysql_result($res_cat,0,'cat_id');
		$nume = stripslashes(mysql_result($res_cat,0,'nume'));
		$sef = stripslashes(mysql_result($res_cat,0,'sef'));
		$link_fac = mysql_result($res_cat,0,'link_fac');
		
		$res_fac_select = get_facultate($link_fac);
		if(mysql_num_rows($res_fac_select))//daca s-a gasit o inregistrare
		$nume_fac_select = stripslashes(mysql_result($res_fac,0,'nume'));
		
		//formularul de modificare catedra
add($content,'<form action="0_facultate.php" method="post"> 
			<input type="hidden" name="cat_id" value="'.htmlspecialchars($cat_id,ENT_QUOTES).'"
			<table class="formular" width="650px">
				<tr>
					<td colspan="4">
					Modificare Catedra
					</td>
				</tr>
				<tr>
				<td>
				Facultate <select name="link_fac" size="1" style="width:180px">');
//selectez si afisez facultatile
for($i=0;$i<$nr;$i++)
	{
		$nume_fac = stripslashes(mysql_result($res_fac,$i,'nume'));
		$id_fac = mysql_result($res_fac,$i,'fac_id');
		add($content,'<option value="'.$id_fac.'"'.( ($nume_fac_select=$nume_fac)?' selected':'') .'>'.htmlspecialchars($nume_fac,ENT_QUOTES).'</option>');
	}
							
add($content,'</select>
				</td>
				<td>
				Nume <input type="text" name="nume" value="'.htmlspecialchars($nume,ENT_QUOTES).'">
				</td>
				<td>
				Sef <input type="text" name="sef" value="'.htmlspecialchars($sef,ENT_QUOTES).'">
				</td>
				<td align="left"><input type="submit" name="catedra_modifica" value="Modifica"></td>
				</tr>
			</table>
			</form><br>');
		}
		else//daca nu s-a gasit nici o inregistrare
		{
			add($content,'<div class="eroare">Eroare : Catedra nu a fost gasita in baza de date</div>');
		}

		
}//sfarsit if($flag_form == CATEDRA_MODIFICA)
else
if($flag_form == FACULTATE_MODIFICA)
{
		$res_fac = get_facultate($_GET['facultate_modifica']);//preiau datele facultatii
		if(mysql_num_rows($res_fac))
		{		
		$fac_id = mysql_result($res_fac,0,'fac_id');
		$nume = stripslashes(mysql_result($res_fac,0,'nume'));
		$nume_scurt = stripslashes(mysql_result($res_fac,0,'nume_scurt'));
		$decan = stripslashes(mysql_result($res_fac,0,'decan'));
		
		
		$layout->replace('TITLE','Formular modificare Facultate');
		add($content,'<div class="title" align="center">Modificare Facultate</div><br>');//Titlul paginii
		//forumlarul de modificare facultate
add($content,'<form action="0_facultate.php" method="post"> 
			<input type="hidden" name="fac_id" value="'.htmlspecialchars($fac_id).'">
			<table class="formular">
				<tr>
					<td colspan="4">
					Modificare Facultate
					</td>
				</tr>
				<tr>
				<td>
				Nume <input type="text" name="nume" value="'.htmlspecialchars($nume,ENT_QUOTES).'">
				</td>
				<td>
				Nume scurt <input type="text" name="nume_scurt" value="'.htmlspecialchars($nume_scurt,ENT_QUOTES).'">
				</td>
				<td>
				Decan <input type="text" name="decan" value="'.htmlspecialchars($decan,ENT_QUOTES).'">
				</td>
				<td align="left"><input type="submit" name="facultate_modifica" value="Modifica"></td>
				</tr>
			</table>
			</form>');
	}
	else
		add($content,'<div class="eroare">Eroare : Facultatea nu a fost gasita in baza de date</div>');
		
}//sfarsit if($flag_form == facultate_MODIFICA)

$layout->replace('CONTENT',$content);//inlocuiesc continutul

$layout->print_template();
?>

