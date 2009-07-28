<?php
//Eduard Tuþescu & Daniel Urdã
//July 2008 - July 2009
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(ADMINISTRATOR);//verifica daca este cont de secretara

$mesaj = "";

if(isset($_GET['sterge']))
	{
		$query = "DELETE FROM `universitati` WHERE `univ_id`='".$_GET['sterge']."' LIMIT 1";
		$result = mysql_query($query);
		if($result)
			{
			add($mesaj,'Universitatea a fost &#x15F;tears&#259; din baza de date.<br><br>');
			}
		else
			add($mesaj,'<div class="eroare">Eroare : Universitatea nu a putut fi &#x15F;tears&#259; din baza de date.');
	}

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
		add($mesaj,'Data de &#xEE;nceput incorect&#259;<br>');
		}
	elseif (!checkdate($data_sfarsit_luna,$data_sfarsit_zi,$data_sfarsit_an))
		{
		add($mesaj,'Data de sf&#xE2;r&#x15F;it incorect&#259;<br>');
		}
	elseif ( ($data_sfarsit_an < $data_start_an) || 
			 ($data_sfarsit_an == $data_start_an && $data_sfarsit_luna < $data_start_luna) ||
			 (	$data_sfarsit_an == $data_start_an && 
				$data_sfarsit_luna == $data_start_luna &&
				$data_sfarsit_zi < $data_start_zi
			 )
			)
		{
		add($mesaj,'Data de &#xEE;nceput mai mare de&#xEE;t cea de sf&#xE2;r&#x15F;it<br>');
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
			$dropit = array();
			$command = "cd /home/cspay/web-exec-scripts/ && /usr/bin/python parity.py";
			exec($command,&$dropit);
			add($mesaj,'Modificarea a fost realizat&#259; cu succes.<br>');
			}
		else
			add($mesaj,'<div class="eroare">Eroare ap&#259;rut&#259; la aplicarea modific&#259;rii.</div>');
		}
	}//sfarsit formular

if(isset($_POST['univ_add']))//formularul de adaugare al universitatii
	{

	foreach($_POST as $index=>$val)
		{
		$$index = $val;
		}
	$nume_univ = addslashes(html_entity_decode($_POST['nume_univ']));

	if (!checkdate($data_start_luna,$data_start_zi,$data_start_an))
		{
		add($mesaj,'Data de &#xEE;nceput incorect&#259;<br>');
		}
	elseif (!checkdate($data_sfarsit_luna,$data_sfarsit_zi,$data_sfarsit_an))
		{
		add($mesaj,'Data de sf&#xE2;r&#x15F;it incorect&#259;<br>');
		}
	elseif ( ($data_sfarsit_an < $data_start_an) || 
			 ($data_sfarsit_an == $data_start_an && $data_sfarsit_luna < $data_start_luna) ||
			 (	$data_sfarsit_an == $data_start_an && 
				$data_sfarsit_luna == $data_start_luna &&
				$data_sfarsit_zi < $data_start_zi
			 )
			)
		{
		add($mesaj,'Data de &#xEE;nceput mai mare de&#xEE;t cea de sf&#xE2;r&#x15F;it<br>');
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
			$dropit = array();
			$command = "cd /home/cspay/web-exec-scripts/ && /usr/bin/python parity.py";
			exec($command,&$dropit);
			add($mesaj,'Ad&#259;ugarea a fost realizat&#259; cu succes.<br>');
			}
		else
			add($mesaj,'<div class="eroare">Eroare aparut&#259; la ad&#259;ugare.</div>');
		}
	}


$layout->get_template('include/template.html');
$layout->replace('TITLE','Universit&#259;&#x21B;i');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,0);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Universit&#259;&#x21B;i</div>');

add($content,$mesaj);

if(isset($_GET['modifica']))
	{
		$query = "SELECT * FROM universitati WHERE univ_id=".$_GET['modifica'];
		$result = mysql_query($query);
		$uni_nume = stripslashes(mysql_result($result,0,'nume'));
		$data_start = mysql_result($result,0,'data_start');
		$data_stop = mysql_result($result,0,'data_stop');
		$finish = '"univ_mod" value="Modific&#259;"></td>';
		$finish .= '<td colspan="2"><input type="submit" name="renunta" value="Renun&#x21B;&#259;">';
		$hide = '<input type="hidden" name="univ_id" value="'.$_GET['modifica'].'">';	
	}
else
	{
		$today=getdate();
		$data_start =$today['year']."-01-01";
		$data_stop = $today['year']."-01-01";
		$uni_nume="";
		$finish = '"univ_add" value="Adaug&#259;">';
		$hide = '';
	}
	
//adaug formularul in care poate modifica numele universitatii, data de start si de sfarsit
add($content,'
	<form action="0_universitate.php" method="post">
	<table width="500px" cellpadding="3" cellspacing="3" class="formular">
		<tr>
		<td>Nume</td>
			<td><input type="text" name="nume_univ" size="35" value="'.htmlspecialchars($uni_nume,ENT_QUOTES).'">
		</tr>'.
		write_data("data_start","Data &#xEE;nceput",$data_start) . write_data("data_sfarsit","Data sf&#xE2;r&#x15F;it",$data_stop)
		.'<tr>
		<td colspan="2"><input type="submit" name='.$finish.'</td>
		</tr>
	</table>'.$hide.'</form>');

//afisez universitatile de la momentul curent
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
					<td>Data &#xEE;nceput</td>
					<td>Data sf&#xE2;r&#x15F;it</td>
					<td>Op&#x21B;iuni</td>
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
							<td><a href="0_universitate.php?sterge='.$un_ID.'">&#x15F;terge</a>
								<a href="0_universitate.php?modifica='.$un_ID.'">modific&#259;</a></td>
						</tr>');
		}
	add($tabel,'</table>');
	}
else
	add($tabel,'Nu exist&#259; utilizatori defini&#x21B;i.<br>');
add($content,$tabel);

$layout->replace('CONTENT',$content);
$layout->print_template();
?>

