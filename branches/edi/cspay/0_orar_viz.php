<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

$layout->get_template('include/template.html');
$layout->replace('TITLE','Formular detalii plata cu ora');

$meniu = show_menu(menu,$_SESSION['tip_cont'],1);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,1);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Formular detalii plata cu ora</div>');
add($content,'<br>Tipul Contului : '.$_SESSION['tip_cont']);


//afisare tabel
$query = "SELECT * FROM `orar`";
//$query = "SELECT `orar_id` , `facultate` , `tip_curs1` , `materie` , `forma` , `cod` , `an` , `serie` , `nr_stud` , `nr_grupa` , `tip_grupa` , `modul_c` , `modul_a` , `tip_curs2` , `post` , `grad` , `norma` , `ocupat` , `acoperit` , `acoperit_efect` , `an_grupa` , `zi` , `ora` , `sala` FROM `orar`";

$result = mysql_query($query);
$nr = mysql_num_rows($result);

add($content,'<form>
<table width="100%" border="0" cellpadding="1" cellspacing="1" style="border:1px solid #CCCCCC;">
  <tr align="center" bgcolor="#ece9d8">
    <td>Nr</td>
	<td>Facultate</td>
    <td>Tip</td>
    <td>Disciplina</td>
    <td>Forma</td>
    <td>Cod</td>
    <td>An</td>
    <td>Serie</td>
    <td>Nr Studenti  </td>
    <td>Nr Grupe </td>
    <td>Tip Grupa  </td>
    <td>C1</td>
    <td>A1</td>
    <td>Tip</td>
    <td>Post</td>
    <td>Grad</td>
    <td>Norma</td>
    <td>Ocupat</td>
    <td>Acoperit</td>
    <td>Efectiv</td>
    <td>An/Gr</td>
    <td>Zi</td>
    <td>Ora </td>
    <td>Sala</td>
    </tr>');

$color = array(0 => "#f6f2d8",1 => "#ede7c2");

for($i=0;$i<$nr;$i++)
{
	$rand = mysql_fetch_row($result);
	add($content,'<tr bgcolor="'.$color[$i%2].'">');
	
	add($content,'<td>'.($i+1).'</td>');
	for($j=1;$j<24;$j++)//fara id
	{
		add($content,'<td>'.$rand[$j].'</td>');
	}
	add($content,'</tr>');
}
add($content,'</table></form>');

$layout->replace('CONTENT',$content);

$query = "INSERT INTO `admin` (`materie`,`utilizator`) VALUES('uso','')";
mysql_query($query);

$layout->print_template();
?>

