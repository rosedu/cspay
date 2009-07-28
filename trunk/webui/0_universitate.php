<?php
//include headers

include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(SECRETARA);//verifica daca este cont de secretara

function write_data($data,$text,$data_explicit)
{
global $luna;

$aux = strstr($data_explicit,"-");
$an = substr($data_explicit,0,strlen($data_explicit)-strlen($aux));
$data_explicit = substr($aux,1,strlen($aux)-1);
$aux = strstr($data_explicit,"-");
$index_luna = substr($data_explicit,0,strlen($data_explicit)-strlen($aux));
$data_explicit = substr($aux,1,strlen($aux)-1);
$zi = $data_explicit;

$output = '<tr>
		<td>'.$text.'</td>
		<td><select name="'.$data.'_zi">';

for($i=1;$i<=31;$i++)
	{
		add($output,'<option '.( ($i== $zi)?'selected':'').'>'.$i.'</option>');
	}
add($output,'</select>&nbsp;');

add($output,'<select name="'.$data.'_luna">');

for($i=0;$i<12;$i++)
	add($output,'<option value='.($i+1).(($index_luna-1 == $i)?' selected':'').'>'.$luna[$i].'</option>');
add($output,'</select>&nbsp;');

add($output,'<select name="'.$data.'_an">');
for($i=0;$i<=12;$i++)
	{
		add($output,'<option '.( (($i+2008)== $an)?'selected':'').'>'.($i+2008).'</option>');
	}

add($output,'</select></tr>');

return $output;
}//sfarsit write_data()

$mesaj = "";
//tratare formulare
if(isset($_POST['univ_mod']))//formularul de modificare al universitatii
{
if (!checkdate($data_start_luna,$data_start_zi,$data_start_an))
	{
	add($mesaj,'Data de inceput incorecta<br>');
	}

elseif (!checkdate($data_sfarsit_luna,$data_sfarsit_zi,$data_sfarsit_an))
	{
	add($mesaj,'Data de sfarsit incorecta<br>');
	}
else
	{
	foreach($_POST as $index=>$val)
		{
		$$index = addslashes(html_entity_decode($val));
		}
	$data_start = $data_start_an . '-' . (($data_start_luna<10)?'0':'') . $data_start_luna . '-' .
				  (($data_start_zi)<10?'0':''). $data_start_zi;
	$data_sfarsit = $data_sfarsit_an . '-' . (($data_sfarsit_luna<10)?'0':'') . $data_sfarsit_luna . '-' .
				  (($data_sfarsit_zi)<10?'0':''). $data_sfarsit_zi;
	//actualizarea tabelei `universitate`
	$query = "UPDATE `universitati` SET `nume`='".$nume_univ."',`data_start`='".$data_start."',`data_stop`='".$data_sfarsit."' WHERE 
			  `univ_id`='".$_SESSION['univ_id']."' LIMIT 1";
	if(mysql_query($query))
	{
		add($mesaj,'Modificarea a fost realizata cu succes.<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare aparuta la aplicarea modificarii.</div>');
	}
}//sfarsit formular

if(isset($_POST['univ_add']))
{
if (!checkdate($data_start_luna,$data_start_zi,$data_start_an))
	{
	add($mesaj,'Data de inceput incorecta<br>');
	}

elseif (!checkdate($data_sfarsit_luna,$data_sfarsit_zi,$data_sfarsit_an))
	{
	add($mesaj,'Data de sfarsit incorecta<br>');
	}
	
else
	{
	foreach($_POST as $index=>$val)
		{
		$$index = addslashes(html_entity_decode($val));
		}
	$data_start = $data_start_an . '-' . (($data_start_luna<10)?'0':'') . $data_start_luna . '-' .
				  (($data_start_zi)<10?'0':''). $data_start_zi;
	$data_sfarsit = $data_sfarsit_an . '-' . (($data_sfarsit_luna<10)?'0':'') . $data_sfarsit_luna . '-' .
				  (($data_sfarsit_zi)<10?'0':''). $data_sfarsit_zi;
	//actualizarea tabelei `universitate`
	$query = "INSERT INTO `universitati` (`nume`, `data_start`, `data_end`) 
				VALUES ($nume_univ., $data_start, $data_sfarsit)";
	if(mysql_query($query))
	{
		add($mesaj,'Adaugarea a fost realizata cu succes.<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare aparuta la adaugare.</div>');
	}
}
$layout->get_template('include/template.html');
$layout->replace('TITLE','Universitate');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,0);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Universitate</div>');

add($content,$mesaj);

$today=getdate();
$data_start =$today['year']."-01-01";
$data_stop = $today['year']."-01-01";
$uni_nume="";
$finish = '"univ_add" value="Adauga"';

if(isset($_GET['modifica']))
	{
		$query = "SELECT * FROM universitati WHERE univ_id=".$_GET['modifica'];
		$result = mysql_query($query);
		$uni_nume = stripslashes(mysql_result($res_univ,0,'nume'));
		$data_start = mysql_result($result,0,'data_start');
		$data_stop = mysql_result($result,0,'data_stop');
		$finish = '"univ_mod" value="Modifica"';
	}
	
if(isset($_GET['sterge']))
	{
	$query = "DELETE FROM `universitati` WHERE `univ_id`='".$_GET['sterge']."' LIMIT 1";
	$result = mysql_query($query);
	if($result)
	{
		add($mesaj,'Universitatea a fost stearsa din baza de date.<br><br>');
	}
		else
			dd($mesaj,'<div class="eroare">Eroare : Universitatea nu a putut fi stearsa din baza de date.');
	}

//adaug formularul in care poate modifica numele universitatii, data de start si de sfarsit
add($content,'
	<form action="" method="post">
	<table width="500px" cellpadding="3" cellspacing="3" class="formular">
		<tr>
		<td>Nume Universitate</td>
			<td><input type="text" name="nume_univ" size="35" value="'.htmlspecialchars($uni_nume,ENT_QUOTES).'">
		</tr>'.
		write_data("data_start","Data inceput",$data_start) . write_data("data_sfarsit","Data sfarsit",$data_stop)
		.'<tr>
		<td colspan="2"><input type="submit" name='.$finish.'>
		</tr>
	</table>');

$tabel = '';
$query = "SELECT * FROM `universitati`";
$res_univ = mysql_query($query);
$nr = mysql_num_rows($res_univ);

if($nr)
{
add($tabel,'<table class="special" cellpading="1" cellspacing="1" width="550px">
			<tr class="tr_head">
				<td>Nr</td>
				<td>Nume</td>
				<td>Data inceput</td>
				<td>Data sfarsit</td>
			</tr>');
			
for($i=0;$i<$nr;$i++)
{
	$Tnume = mysql_result($res_univ,$i,'nume');
	$Tnume = htmlspecialchars(stripslashes($Tnume),ENT_QUOTES);
	
	$Tdata_start = mysql_result($res_univ,$i,'data_start');
	$Tdata_stop = mysql_result($res_univ,$i,'data_stop');
	$un_ID = mysql_result($res_univ,$i,'univ_id');
	
	add($tabel,'<tr class="'.$class_select[$i%2].'">
					<td>'.($i+1).'</td>
					<td>'.$Tnume.'</td>
					<td>'.translate_date($Tdata_start).'</td>
					<td>'.translate_date($Tdata_stop).'</td>
					<td><a href="0_universitate.php?sterge='.$un_ID.'">sterge</a>
						<a href="0_universitate.php?modifica='.$un_ID.'">modifica</a></td>
				</tr>');

}

add($tabel,'</table>');
}
else
	add($tabel,'Nu exista utilizatori definiti.<br>');

add($content,$tabel);
$layout->replace('CONTENT',$content);



$layout->print_template();
?>

