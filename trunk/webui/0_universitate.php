<?php
//include headers

include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(ADMINISTRATOR);//verifica daca este cont de secretara

$mesaj = "";
//tratare formulare
if(isset($_POST['univ_mod']))//formularul de modificare al universitatii
{
foreach($_POST as $index=>$val)
{
	$$index = $val;
}
$nume_univ = addslashes(html_entity_decode($_POST['nume_univ']));

if (!checkdate($data_start_luna,$data_start_zi,$data_start_an))
	{
	add($mesaj,'Data de început incorectã<br>');
	}

elseif (!checkdate($data_sfarsit_luna,$data_sfarsit_zi,$data_sfarsit_an))
	{
	add($mesaj,'Data de sfârºit incorectã<br>');
	}
else
	{
	$data_start = $data_start_an . '-' . (($data_start_luna<10)?'0':'') . $data_start_luna . '-' .
				  (($data_start_zi)<10?'0':''). $data_start_zi;
	$data_sfarsit = $data_sfarsit_an . '-' . (($data_sfarsit_luna<10)?'0':'') . $data_sfarsit_luna . '-' .
				  (($data_sfarsit_zi)<10?'0':''). $data_sfarsit_zi;
	//actualizarea tabelei `universitate`
	$query = "UPDATE `universitati` SET `nume`='".$nume_univ."',`data_start`='".$data_start."',`data_stop`='".$data_sfarsit."' WHERE 
			  `univ_id`='".$univ_id."' LIMIT 1";
	if(mysql_query($query))
	{
		add($mesaj,'Modificarea a fost realizatã cu succes.<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare apãrutã la aplicarea modificãrii.</div>');
	}
}//sfarsit formular

if(isset($_POST['univ_add']))
{

foreach($_POST as $index=>$val)
{
	$$index = $val;
}
$nume_univ = addslashes(html_entity_decode($_POST['nume_univ']));

if (!checkdate($data_start_luna,$data_start_zi,$data_start_an))
	{
	add($mesaj,'Data de început incorectã<br>');
	}

elseif (!checkdate($data_sfarsit_luna,$data_sfarsit_zi,$data_sfarsit_an))
	{
	add($mesaj,'Data de sfârºit incorectã<br>');
	}
	
else
	{

	$data_start = $data_start_an . '-' . (($data_start_luna<10)?'0':'') . $data_start_luna . '-' .
				  (($data_start_zi)<10?'0':''). $data_start_zi;
	$data_sfarsit = $data_sfarsit_an . '-' . (($data_sfarsit_luna<10)?'0':'') . $data_sfarsit_luna . '-' .
				  (($data_sfarsit_zi)<10?'0':''). $data_sfarsit_zi;
	//actualizarea tabelei `universitate`
	$query = "INSERT INTO `universitati` (`nume`, `data_start`, `data_stop`) 
				VALUES ('".$nume_univ."', '".$data_start."', '".$data_sfarsit."')";
	if(mysql_query($query))
	{
		add($mesaj,'Adãugarea a fost realizatã cu succes.<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare aparutã la adaãgare.</div>');
	}
}
$layout->get_template('include/template.html');
$layout->replace('TITLE','Universitãþi');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,0);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Universitãþi</div>');

add($content,$mesaj);

$today=getdate();
$data_start =$today['year']."-01-01";
$data_stop = $today['year']."-01-01";
$uni_nume="";
$finish = '"univ_add" value="Adaugã">';
$hide = '';

if(isset($_GET['modifica'])&&(!isset($_POST['renunta'])))
	{
		$query = "SELECT * FROM universitati WHERE univ_id=".$_GET['modifica'];
		$result = mysql_query($query);
		$uni_nume = stripslashes(mysql_result($result,0,'nume'));
		$data_start = mysql_result($result,0,'data_start');
		$data_stop = mysql_result($result,0,'data_stop');
		$finish = '"univ_mod" value="Modificã"></td>';
		$finish .= '<td colspan="2"><input type="submit" name="renunta" value="Renunþã">';
		$hide = '<input type="hidden" name="univ_id" value="'.$_GET['modifica'].'">';
		
	}
	
if(isset($_GET['sterge']))
	{
	$query = "DELETE FROM `universitati` WHERE `univ_id`='".$_GET['sterge']."' LIMIT 1";
	$result = mysql_query($query);
	if($result)
	{
		add($mesaj,'Universitatea a fost ºtearsã din baza de date.<br><br>');
	}
		else
			dd($mesaj,'<div class="eroare">Eroare : Universitatea nu a putut fi ºtearsã din baza de date.');
	}

//adaug formularul in care poate modifica numele universitatii, data de start si de sfarsit
add($content,'
	<form action="0_universitate.php" method="post">
	<table width="500px" cellpadding="3" cellspacing="3" class="formular">
		<tr>
		<td>Nume</td>
			<td><input type="text" name="nume_univ" size="35" value="'.htmlspecialchars($uni_nume,ENT_QUOTES).'">
		</tr>'.
		write_data("data_start","Data început",$data_start) . write_data("data_sfarsit","Data sfârºit",$data_stop)
		.'<tr>
		<td colspan="2"><input type="submit" name='.$finish.'</td>
		</tr>
	</table>'.$hide.'</form>');

$tabel = '';
$query = "SELECT * FROM `universitati`";
$res_univ = mysql_query($query);
$nr = mysql_num_rows($res_univ);

if($nr)
{
add($tabel,'<table class="special" cellpading="1" cellspacing="1" width="90%">
			<tr class="tr_head">
				<td>Nr</td>
				<td>Nume</td>
				<td>Data început</td>
				<td>Data sfârºit</td>
				<td>Opþiuni</td>
			</tr>');
			
for($i=0;$i<$nr;$i++)
{
	$Tnume = mysql_result($res_univ,$i,'nume');
	$Tnume = htmlspecialchars(stripslashes($Tnume),ENT_QUOTES);
	
	$Tdata_start = mysql_result($res_univ,$i,'data_start');
	$Tdata_stop = mysql_result($res_univ,$i,'data_stop');
	$un_ID = mysql_result($res_univ,$i,'univ_id');
	
	add($tabel,'<tr class="'.$class_std[$i%2].'">
					<td>'.($i+1).'</td>
					<td>'.$Tnume.'</td>
					<td>'.translate_date($Tdata_start).'</td>
					<td>'.translate_date($Tdata_stop).'</td>
					<td><a href="0_universitate.php?sterge='.$un_ID.'">ºterge</a>
						<a href="0_universitate.php?modifica='.$un_ID.'">modificã</a></td>
				</tr>');

}

add($tabel,'</table>');
}
else
	add($tabel,'Nu existã utilizatori definiþi.<br>');

add($content,$tabel);
$layout->replace('CONTENT',$content);



$layout->print_template();
?>

