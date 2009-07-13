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
for($i=0;$i<=4;$i++)
	{
		add($output,'<option '.( (($i+2008)== $an)?'selected':'').'>'.($i+2008).'</option>');
	}

add($output,'</select></tr>');

return $output;
}//sfarsit write_data()

$mesaj = "";
//tratare formulare
if(isset($_POST['univ_modifica']))//formularul de modificare al universitatii
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
	$query = "UPDATE `universitate` SET `nume`='".$nume_univ."',`data_start`='".$data_start."',`data_stop`='".$data_sfarsit."' WHERE 
			  `univ_id`='".$_SESSION['univ_id']."' LIMIT 1";
	if(mysql_query($query))
	{
		add($mesaj,'Modificarea a fost realizata cu succes.<br>');
	}
	else
		add($mesaj,'<div class="eroare">Eroare aparuta la aplicarea modificarii.</div>');
}//sfarsit formular

$layout->get_template('include/template.html');
$layout->replace('TITLE','Universitate');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,0);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Universitate</div>');

add($content,$mesaj);

$query = "SELECT * FROM `universitate` WHERE `univ_id` = '".$_SESSION['univ_id']."'LIMIT 1";
$res_univ = mysql_query($query);

if(mysql_num_rows($res_univ))
{
$_SESSION['univ_nume'] = stripslashes(mysql_result($res_univ,0,'nume'));
$data_start = mysql_result($res_univ,0,'data_start');
$data_stop = mysql_result($res_univ,0,'data_stop');
}

//adaug formularul in care poate modifica numele universitatii, data de start si de sfarsit
add($content,'
	<form action="" method="post">
	<table width="500px" cellpadding="3" cellspacing="3" class="formular">
		<tr>
		<td>Nume Universitate</td>
			<td><input type="text" name="nume_univ" size="35" value="'.htmlspecialchars($_SESSION['univ_nume'],ENT_QUOTES).'">
		</tr>'.
		write_data("data_start","Data inceput",$data_start) . write_data("data_sfarsit","Data sfarsit",$data_stop)
		.'<tr>
		<td colspan="2"><input type="submit" name="univ_modifica" value="Salveaza">
		</tr>
	</table>');

$layout->replace('CONTENT',$content);



$layout->print_template();
?>

