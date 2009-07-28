<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(SECRETARA);//verifica daca este cont de secretara

define("VACANTA_MODIFICA",1);

$mesaj = "";
$flag_form = false;

function write_table_head()
{
	$output = "";
	add($output,'<table width="500px" cellpadding="1" cellspacing="1" class="special">');
	add($output,'<tr class="tr_head"><td>Nr</td><td>Data inceput</td><td>Data sfarsit</td><td>Optiuni</td></tr>');
	
	return $output;
}

//tratare formular
if(isset($_POST['vacanta_adauga']))
{
	foreach($_POST as $index=>$val)
	{
		//add($mesaj,$index . ' => ' . $val . '<br>');
		$$inde = $val;
	}
	$data_start = $data_start_an . '-' . (($data_start_luna<10)?'0':'') . $data_start_luna . '-' .
				  (($data_start_zi)<10?'0':''). $data_start_zi;
	$data_sfarsit = $data_sfarsit_an . '-' . (($data_sfarsit_luna<10)?'0':'') . $data_sfarsit_luna . '-' .
				  (($data_sfarsit_zi)<10?'0':''). $data_sfarsit_zi;
		
	$query = "INSERT INTO `vacante` (`link_univ`,`data_start`,`data_stop`) VALUES
			 ('".$_SESSION['univ_id']."','".$data_start."','".$data_sfarsit."')";
	$res_add_vac = mysql_query($query);
	if($res_add_vac)
	{
		add($mesaj,'Perioada a fost adaugata cu succes.<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare : Perioada nu a putut fi adaugata</div>');
	
}//sfarsit tratare formular
else
if(isset($_POST['vacanta_modifica']))
{
	//add($mesaj,'modifica vacanta<br>');
	foreach($_POST as $index=>$val)
	{
		$$index = $val;
		//add($mesaj,$index . '  =>  '. $val . '<br>');
	}
	$data_start = $data_start_an . '-' . (($data_start_luna<10)?'0':'') . $data_start_luna . '-' .
				  (($data_start_zi)<10?'0':''). $data_start_zi;
	$data_sfarsit = $data_sfarsit_an . '-' . (($data_sfarsit_luna<10)?'0':'') . $data_sfarsit_luna . '-' .
				  (($data_sfarsit_zi)<10?'0':''). $data_sfarsit_zi;
	
	$query = "UPDATE `vacante` SET `data_start`='".$data_start."',`data_stop`='".$data_sfarsit."'
			 WHERE `vac_id`='".$vac_id."' LIMIT 1";
	if(mysql_query($query))
	{
		add($mesaj,'Modificarea a fost realizata cu succes.<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare aparuta la aplicarea modificarii.</div>');
}

//tratare pagina si parametri transmisi
if(isset($_GET['vacanta_sterge']))
{
	$query = "DELETE FROM `vacante` WHERE `vac_id`='".$_GET['vacanta_sterge']."' LIMIT 1";
	$result = mysql_query($query);
	if($result)
	{
		add($mesaj,'Perioada de vacanta a fost stearsa cu succes<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare : Perioada de vacanta nu a putut fi stearsa</div>');
}//sfarsit vacanta_sterge
else
if(isset($_GET['vacanta_modifica']))
{
	add($mesaj,'modifica vacanta');
	$flag_form = VACANTA_MODIFICA;
}

$layout->get_template('include/template.html');
$layout->replace('TITLE','Editare Vacante');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],2,0);
$layout->replace('SUBMENU',$submeniu);

$content = "";

if($flag_form == false)
{
add($content,'<div class="title" align="center">Editare Vacante</div>');
add($content,$mesaj);

add($content,'<br>
	<form action="0_vacanta.php" method="post">
	<table  width="500px" cellpadding="3" cellspacing="3" class="formular">
		<tr><td colspan="2">Adaugare perioada de vacanta</td></tr>
		<tr><td>Data inceput</td><td>'.write_select_data("data_start").'</td></tr>
	    <tr><td>Data sfarsit</td><td>'.write_select_data("data_sfarsit").'</td></tr>
		<tr><td colspan="2"><input type="submit" value="Adauga" name="vacanta_adauga"></td></tr>
	</table><br>
	</form>');

//add($content,'<tr><td colspan="4"><input type="submit" name="vacanta_salveaza" value="Salveaza"></tr>');
//selectare din baza de date si afisare perioade
$query = "SELECT * FROM `vacante` WHERE `link_univ`='".$_SESSION['univ_id']."'";
$res_vac = mysql_query($query);
$nr_vac = mysql_num_rows($res_vac);

if($nr_vac)
{
add($content,'<form action="0_vacanta.php" method="post"');
add($content,'<table class="special" cellpading="1" cellspacing="1" width="550px">
			<tr class="tr_head">
				<td>Nr</td>
				<td>Data inceput</td>
				<td>Data sfarsit</td>
				<td>Optiuni</td>
			</tr>');
$class =  array(0 => "tr_1",1=>"tr_2");//clasa pentru rand par respectiv impar

for($i=0;$i<$nr_vac;$i++)
{
	$vac_id = mysql_result($res_vac,$i,'vac_id');
	$data_start = mysql_result($res_vac,$i,'data_start');
	$data_stop = mysql_result($res_vac,$i,'data_stop');
	
	add($content,'<tr class="'.$class_select[$i%2].'">
					<td>'.($i+1).'</td>
					<td>'.translate_date($data_start).'</td>
					<td>'.translate_date($data_stop).'</td>
					<td><a href="0_vacanta.php?vacanta_sterge='.$vac_id.'">sterge</a> 
						<a href="0_vacanta.php?vacanta_modifica='.$vac_id.'">modifica</a>
					</td>
				</tr>');
}

add($content,'</table></form>');
}//end if($nr_vac)
}//sfasit flag_form
else
if($flag_form == VACANTA_MODIFICA)//afisez formularul de modificare al unei perioade
{
	add($content,'<br>
	<form action="0_vacanta.php" method="post">
	<input type="hidden" name="vac_id" value="'.$_GET['vacanta_modifica'].'">
	<table  width="500px" cellpadding="3" cellspacing="3" class="formular">
		<tr><td colspan="2">Modificare perioada de vacanta</td></tr>
		<tr><td>Data inceput</td><td>'.write_select_data("data_start").'</td></tr>
	    <tr><td>Data sfarsit</td><td>'.write_select_data("data_sfarsit").'</td></tr>
		<tr><td colspan="2"><input type="submit" value="Modifica" name="vacanta_modifica"></td></tr>
	</table><br>
	</form>');
}//sfarsit formular

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

