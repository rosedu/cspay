<?php
//Eduard Tu�escu & Daniel Urd�
//July 2008 - July 2009
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(ADMINISTRATOR);//verifica daca este cont de secretara

$mesaj = "";

function gen_univ_sel( $sel )
	{
	$result = mysql_query("SELECT * FROM universitati");
	$nr = mysql_num_rows($result);
	$univ ='Universitate</td><td>';
	$univ .= '<select style="font-size:10pt;" name="universitate">';
	$univ .= '<option value="0">&nbsp;</option>';
	for ($i = 0; $i <$nr;$i++)
		{
		$val = mysql_result($result,$i,'univ_id');
		$sele = "";
		if ($val == $sel)
			$sele = "selected";
		$univ .= '<option '.$sele.' value="'.$val.'">'.mysql_result($result,$i,'nume')."</option>";
		}
	$univ .= "</select>";
	return $univ;
	}

//tratare formular
if(isset($_POST['vacanta_adauga']))
	{
	foreach($_POST as $index=>$val)
		{
		$$index = $val;
		}
	
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
		
		$query = "INSERT INTO `vacante` (`link_univ`,`data_start`,`data_stop`) VALUES
				('".$universitate."','".$data_start."','".$data_sfarsit."')";
		$res_add_vac = mysql_query($query);
		if($res_add_vac)
			{
			$dropit = array();
			$command = "cd /home/cspay/web-exec-scripts/ && /usr/bin/python parity.py";
			exec($command,&$dropit);
			add($mesaj,'Perioada a fost ad&#259;ugat&#259; cu succes.<br>');
			}
		else
			add($mesaj,'<div class="eroare">Eroare : Perioada nu a putut fi ad&#259;ugat&#259;</div>');
		}
	}//sfarsit tratare formular
else
	if(isset($_POST['vacanta_modifica']))
	{
	foreach($_POST as $index=>$val)
		{
		$$index = $val;
		}
	
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
	
		$query = "UPDATE `vacante` SET `data_start`='".$data_start."',`data_stop`='".$data_sfarsit."'
				WHERE `vac_id`='".$vac_id."' LIMIT 1";
		if(mysql_query($query))
			{
			$dropit = array();
			$command = "cd /home/cspay/web-exec-scripts/ && /usr/bin/python parity.py";
			exec($command,&$dropit);
			add($mesaj,'Modificarea a fost realizat&#259; cu succes.<br>');
			}
		else
			add($mesaj,'<div class="eroare">Eroare aparut&#259; la aplicarea modific&#259;rii.</div>');
		}
	}

//tratare pagina si parametri transmisi
if(isset($_GET['vacanta_sterge']))
	{
	$query = "DELETE FROM `vacante` WHERE `vac_id`='".$_GET['vacanta_sterge']."' LIMIT 1";
	$result = mysql_query($query);
	if($result)
		{
		$dropit = array();
		$command = "cd /home/cspay/web-exec-scripts/ && /usr/bin/python parity.py";
		exec($command,&$dropit);
		add($mesaj,'Perioada de vacan&#x21B;&#259; a fost &#x15F;tears&#259; cu succes<br>');
		}
	else
		add($mesaj,'<div class="eroare">Eroare : Perioada de vacan&#x21B;a nu a putut fi &#x15F;tears&#259;</div>');
	}//sfarsit vacanta_sterge

$layout->get_template('include/template.html');
$layout->replace('TITLE','Vacan&#x21B;e');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],2,0);
$layout->replace('SUBMENU',$submeniu);

$content = "";

add($content,'<div class="title" align="center">Vacan&#x21B;e</div>');
add($content,$mesaj);


if(isset($_GET['vacanta_modifica'])&&isset($_GET['univ_id']))
	{
	$msj = "Modificare perioade de vacan&#x21B;&#259;";
	$univ_id = $_GET['univ_id'];
	$query = "SELECT * FROM vacante WHERE vac_id=".$_GET['vacanta_modifica'];
	$result = mysql_query($query);
	$data_start = mysql_result($result,0,'data_start');
	$data_stop = mysql_result($result,0,'data_stop');
	$finish = '"vacanta_modifica" value="Modific&#259;"></td>';
	$finish .= '<td colspan="2"><input type="submit" name="renunta" value="Renun&#x21B;&#259;">';
	$hide = '<input type="hidden" name="vac_id" value="'.$_GET['vacanta_modifica'].'">';
	}
else
	{
	$msj="Ad&#259;ugare perioade de vacan&#x21B;&#259;";
	$univ_id = 0;
	$today=getdate();
	$data_start =$today['year']."-01-01";
	$data_stop = $today['year']."-01-01";
	$finish = '"vacanta_adauga" value="Adaug&#259;">';
	$hide ='';
	}

add($content,'<br>
	<form action="0_vacanta.php" method="post">
	<table  width="500px" cellpadding="3" cellspacing="3" class="formular">
		<tr><td colspan="2">'.$msj.'</td></tr>
		<tr><td>'.gen_univ_sel($univ_id).'</td></tr>
		<tr>'.write_data("data_start","Data inceput",$data_start) . write_data("data_sfarsit","Data sfarsit",$data_stop).'</tr>
		<tr><td colspan="2"><input type="submit" name='.$finish.'</td></tr>
	</table><br>'.$hide.'</form>');

//selectare din baza de date si afisare perioade
$query = "SELECT * FROM `vacante`";
$res_vac = mysql_query($query);
$nr_vac = mysql_num_rows($res_vac);

if($nr_vac)
	{
	add($content,'<table class="special" cellpading="1" cellspacing="1" width="90%">
				<tr class="tr_head">
					<td>Nr</td>
					<td>Universitate</td>
					<td>Data &#xEE;nceput</td>
					<td>Data sf&#xE2;r&#x15F;it</td>
					<td>Op&#x21B;iuni</td>
				</tr>');

	for($i=0;$i<$nr_vac;$i++)
		{
		$vac_id = mysql_result($res_vac,$i,'vac_id');
		$data_start = mysql_result($res_vac,$i,'data_start');
		$data_stop = mysql_result($res_vac,$i,'data_stop');
		$univ_id = mysql_result($res_vac,$i,'link_univ');
		$query = "SELECT * FROM universitati WHERE univ_id=".$univ_id;
		$result = mysql_query($query);
		$nume_univ = mysql_result($result,0,'nume');
	
		add($content,'<tr class="'.$class_std[$i%2].'">
						<td>'.($i+1).'</td>
						<td>'.$nume_univ.'</td>
						<td>'.translate_date($data_start).'</td>
						<td>'.translate_date($data_stop).'</td>
						<td><a href="0_vacanta.php?vacanta_sterge='.$vac_id.'">&#x15F;terge</a> 
							<a href="0_vacanta.php?vacanta_modifica='.$vac_id.'&univ_id='.$univ_id.'">modific&#259;</a>
						</td>
					</tr>');
		}
	add($content,'</table>');
	}//end if($nr_vac)

$layout->replace('CONTENT',$content);
$layout->print_template();
?>