<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(PROFESOR);//verifica daca este cont de profesor

function write_table_head()//scrie capul de tabel pentru perioadele de vacanta
{
	$output = "";
	add($output,'<table width="500px" cellpadding="1" cellspacing="1" class="special">');
	add($output,'<tr class="tr_head"><td>Nr</td><td>Data inceput</td><td>Data sfarsit</td></tr>');
	
	return $output;
}

$layout->get_template('include/template.html');
$layout->replace('TITLE','prima pagina');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,0);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Perioade de Vacanta</div><br><br>');

//selectare din baza de date si afisare perioade
$query = "SELECT * FROM `vacante` WHERE `link_univ`='".$_SESSION['univ_id']."'";
$res_vac = mysql_query($query);
$nr_vac = mysql_num_rows($res_vac);

if($nr_vac)
{
add($content,write_table_head());
$class =  array(0 => "tr_1",1=>"tr_2");//clasa pentru rand par respectiv impar

for($i=0;$i<$nr_vac;$i++)
{
	$vac_id = mysql_result($res_vac,$i,'vac_id');
	$data_start = mysql_result($res_vac,$i,'data_start');
	$data_stop = mysql_result($res_vac,$i,'data_stop');
	
	add($content,'<tr class="'.$class[$i%2].'"><td>'.($i+1).'</td><td>'.translate_date($data_start).
				 '</td><td>'.translate_date($data_stop).'</td></tr>');
}

add($content,'</table></form>');
}//end if($nr_vac)
else
{
	add($content,'Perioadele de vacanta nu au fost inca stabilite.<br>');
}

$layout->replace('CONTENT',$content);


$layout->print_template();
?>

