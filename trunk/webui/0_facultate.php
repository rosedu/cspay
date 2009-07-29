<?php
//Eduard Tuþescu & Daniel Urdã
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
if(isset($_POST['fac_adauga']))
	{
	foreach($_POST as $index=>$val)
		{
		$$index = $val;
		}
	
	$query = "INSERT INTO `facultati` (`link_univ`,`nume`,`nume_scurt`, `decan`)
								VALUES ('".$universitate."','".$nume."','".$nume_sc."','".$decan."')";
	$res_add_vac = mysql_query($query);
	if($res_add_vac)
		{
		add($mesaj,'Facultatea a fost ad&#x103;ugat&#x103; cu succes.<br>');
		}
	else
		add($mesaj,'<div class="eroare">Eroare : Facultatea nu a putut fi ad&#x103;ugat&#x103;</div>');
	}//sfarsit tratare formular
else
	if(isset($_POST['fac_modifica']))
	{
		foreach($_POST as $index=>$val)
			{
			$$index = $val;
			}
		$query = "UPDATE `facultati` SET `link_univ`=".$universitate.",`nume`='".$nume."',
										  `nume_scurt`='".$nume_sc."',`decan`='".$decan."'
				WHERE `fac_id`=".$fac_id." LIMIT 1";

		if(mysql_query($query))
		{
			add($mesaj,'Modificarea a fost realizat&#x103; cu succes.<br>');
		}
		else
			add($mesaj,'<div class="eroare">Eroare ap&#x103;rut&#x103; la aplicarea modific&#x103;rii.</div>');
	}

//tratare pagina si parametri transmisi
if(isset($_GET['fac_sterge']))
	{
	$query = "DELETE FROM `facultati` WHERE `fac_id`='".$_GET['fac_sterge']."' LIMIT 1";
	$result = mysql_query($query);
	if($result)
	{
		add($mesaj,'Facultatea a fost &#x15F;tears&#x103; cu succes<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare : Facultatea nu a putut fi &#x15F;tears&#x103;</div>');
	}//sfarsit fac_sterge

$layout->get_template('include/template.html');
$layout->replace('TITLE','Facult&#x103;&#x163;i');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],1,0);
$layout->replace('SUBMENU',$submeniu);

$content = "";

add($content,'<div class="title" align="center">Facult&#x103;&#x163;i</div>');
add($content,$mesaj);

if(isset($_GET['fac_modifica'])&&isset($_GET['univ_id']))
	{
	$msj = "Modificare facultate";
	$univ_id = $_GET['univ_id'];
	$query = "SELECT * FROM facultati WHERE fac_id=".$_GET['fac_modifica'];
	$result = mysql_query($query);
	$fac_nume = stripslashes(mysql_result($result,0,'nume'));
	$fac_nume_sc = stripslashes(mysql_result($result,0,'nume_scurt'));
	$fac_decan = stripslashes(mysql_result($result,0,'decan'));
	$finish = '"fac_modifica" value="Modific&#x103;"></td>';
	$finish .= '<td colspan="2"><input type="submit" name="renunta" value="Renun&#x163;&#x103;">';
	$hide = '<input type="hidden" name="fac_id" value="'.$_GET['fac_modifica'].'">';
	}
else
	{
	$msj="Ad&#x103;ugare facultate";
	$univ_id = 0;
	$fac_nume = '';
	$fac_nume_sc = '';
	$fac_decan = '';
	$finish = '"fac_adauga" value="Adaug&#x103;">';
	$hide = '';
	}

add($content,'<br>
	<form action="0_facultate.php" method="post">
	<table  width="500px" cellpadding="3" cellspacing="3" class="formular">
		<tr><td colspan="2">'.$msj.'</td></tr>
		<tr><td>'.gen_univ_sel($univ_id).'</td></tr>
		<tr>
			<td>Nume</td>
			<td><input type="text" name="nume" size="35" value="'.htmlspecialchars($fac_nume,ENT_QUOTES).'"><td>
			<td>Nume scurt</td>
			<td><input type="text" name="nume_sc" size="35" value="'.htmlspecialchars($fac_nume_sc,ENT_QUOTES).'"><td>
		</tr>
			<td>Decan</td>
			<td><input type="text" name="decan" size="35" value="'.htmlspecialchars($fac_decan,ENT_QUOTES).'"><td>
		</tr>
		<tr><td colspan="2"><input type="submit" name='.$finish.'</td></tr>
	</table><br>'.$hide.'</form>');

//selectare din baza de date si afisare perioade
$query = "SELECT * FROM `facultati`";
$res_fac = mysql_query($query);
$nr_fac = mysql_num_rows($res_fac);

if($nr_fac)
	{
	add($content,'<table class="special" cellpading="1" cellspacing="1" width="90%">
				<tr class="tr_head">
					<td>Nr</td>
					<td>Universitate</td>
					<td>Nume</td>
					<td>Nume scurt</td>
					<td>Decan</td>
					<td>Op&#x163;iuni</td>
				</tr>');

	for($i=0;$i<$nr_fac;$i++)
		{
		$fac_id = mysql_result($res_fac,$i,'fac_id');
		$nume = stripslashes(mysql_result($res_fac,$i,'nume'));
		$decan = stripslashes(mysql_result($res_fac,$i,'decan'));
		$nume_scurt = stripslashes(mysql_result($res_fac,$i,'nume_scurt'));
		$univ_id = mysql_result($res_fac,$i,'link_univ');
		$query = "SELECT * FROM universitati WHERE univ_id=".$univ_id;
		$result = mysql_query($query);
		$nume_univ = mysql_result($result,0,'nume');
	
		add($content,'<tr class="'.$class_std[$i%2].'">
						<td>'.($i+1).'</td>
						<td>'.$nume_univ.'</td>
						<td>'.htmlspecialchars($nume,ENT_QUOTES).'</td>
						<td>'.htmlspecialchars($nume_scurt,ENT_QUOTES).'</td>
						<td>'.htmlspecialchars($decan,ENT_QUOTES).'</td>
						<td><a href="0_facultate.php?fac_sterge='.$fac_id.'">&#x15F;terge</a> 
							<a href="0_facultate.php?fac_modifica='.$fac_id.'&univ_id='.$univ_id.'">modific&#x103;</a>
						</td>
					</tr>');
		}
	add($content,'</table>');
	}//end if($nr_vac)

$layout->replace('CONTENT',$content);
$layout->print_template();
?>