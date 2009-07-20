<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

check_auth(ADMINISTRATOR);//verifica daca este cont de administrator

$mesaj = '';

//tratare formulare
if(isset($_POST['salveaza_admin']))//formular de modificare parola/utilizator administrator
{
	foreach($_POST as $index=>$val)
		$$index = addslashes($val);
	
	$query = "UPDATE `admin` SET `utilizator`='".$utilizator."',`parola`='".$parola."'
			  WHERE `admin_id`='".$hidden_id."' LIMIT 1";
	$result = mysql_query($query);
	
	if($result)
		add($mesaj,'Salvarea modificarilor a fost realizata cu succes.<br>');
	else
		add($mesaj,'<div class="eroare">Eroare : Modificarile nu au putut fi salvata.</div>');
}//sfarsit administrator
else
if(isset($_POST['salveaza_secretara']))//formualar de modificare parola/utilizator secretara
{
	
	foreach($_POST as $index=>$val)
		$$index = addslashes($val);
	
	$query = "UPDATE `admin` SET `utilizator`='".$utilizator."',`parola`='".$parola."'
			  WHERE `admin_id`='".$hidden_id."' LIMIT 1";
	$result = mysql_query($query);
	
	if($result)
		add($mesaj,'Salvarea modificarilor a fost realizata cu succes.<br>');
	else
		add($mesaj,'<div class="eroare">Eroare : Modificarile nu au putut fi salvata.</div>');
}//sfarsit secretara
else
if(isset($_POST['salveaza_materie']))//formular de modificare parola/utilizator conturi materie
{
	$saved = true;
	foreach($_POST as $index=>$val)
		$$index = $val;
	foreach($utilizator as $index=>$val)
		$utilizator[$index] = addslashes($val);
	foreach($parola as $index=>$val)
		$parola[$index] = addslashes($val);
		
	//echo sizeof($hidden_id);
	
	for($i=0;$i<count($hidden_id);$i++)
	{
		$query = "UPDATE `admin` SET `utilizator`='".$utilizator[$i]."',`parola`='".$parola[$i]."'
			  WHERE `admin_id`='".$hidden_id[$i]."' LIMIT 1";
		$result = mysql_query($query);
		
		if(!$result)
			$saved = false;
			//echo "da ";
	}
	if($saved)
		add($mesaj,'Salvarea modificarilor a fost realizata cu succes.<br>');
	else
		add($mesaj,'<div class="eroare">Eroare : Modificarile nu au putut fi salvata.</div>');
}//sfarsit materie

$layout->get_template('include/template.html');
$layout->replace('TITLE','Administrare');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],1,0);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Import</div>');
add($content,'<br>');

/* // creare conturi pentru fiecare materie
   // trebuie apelat la instalare
   // cand se incarca fisierul xls pentru orar
$materie = get_materie_from_univ($_SESSION['univ_id']);
$nr = count($materie);

$query = "INSERT INTO `admin` (`materie`,`utilizator`,`parola`,`tip_cont`,`link_univ`)
		  VALUES ";
			  
for($i=0;$i<$nr;$i++)
{
	$query .= "('".$materie[$i]."','profesor_".$i."','root','1','".$_SESSION['univ_id']."')";
	if($i!=$nr-1)
		$query .= ",";
}
mysql_query($query);
*/

$admin = '';
$secretara = '';
$materie = '';

$query = "SELECT * FROM `admin` WHERE `link_univ`='".$_SESSION['univ_id']."' AND
		  `tip_cont` = '".ADMINISTRATOR."' LIMIT 1";
$res_asoc = mysql_query($query);
$nr_asoc = mysql_num_rows($res_asoc);

if($nr_asoc)
{
$admin_utiliz = htmlspecialchars(mysql_result($res_asoc,0,'utilizator'));
$admin_parola = htmlspecialchars(mysql_result($res_asoc,0,'parola'));
$admin_id = htmlspecialchars(mysql_result($res_asoc,0,'admin_id'));

add($admin,'Cont administrator <br>
			<form action="" method="post">
			<input type="hidden" name="hidden_id" value="'.$admin_id.'">
			<table class="special">
			<tr>
				<td>Utilizator <input type="text" value="'.$admin_utiliz.'" name="utilizator"></td>
				<td>Parola <input type="text" value="'.$admin_parola.'" name="parola"></td>
				<td><input type="submit" name="salveaza_admin" value="Salveaza"></td>
			</tr>');
add($admin,'</table></form><br>');
}
else
	add($admin,'Contul pentru administrator nu a fost creat.<br>');

$query = "SELECT * FROM `admin` WHERE `link_univ`='".$_SESSION['univ_id']."' AND
		  `tip_cont` = '".SECRETARA."' LIMIT 1";
$res_asoc = mysql_query($query);
$nr_asoc = mysql_num_rows($res_asoc);

if($nr_asoc)//exista contul de secretara
{
$secretara_utiliz = htmlspecialchars(mysql_result($res_asoc,0,'utilizator'));
$secretara_parola = htmlspecialchars(mysql_result($res_asoc,0,'parola'));
$secretara_id = htmlspecialchars(mysql_result($res_asoc,0,'admin_id'));

add($secretara,'Cont secretara <br>
			<form action="" method="post">
			<input type="hidden" name="hidden_id" value="'.$secretara_id.'">
			<table class="special">
			<tr>
				<td>Utilizator <input type="text" value="'.$secretara_utiliz.'" name="utilizator"></td>
				<td>Parola <input type="text" value="'.$secretara_parola.'" name="parola"></td>
				<td><input type="submit" name="salveaza_secretara" value="Salveaza"></td>
			</tr>');
add($secretara,'</table></form><br>');
}
else
	add($secretara,'Contul pentru secretara nu a fost creat.<br>');

//afisare formular pentru administrare conturi pentru materii
$query = "SELECT * FROM `admin` WHERE `link_univ`='".$_SESSION['univ_id']."' AND
		  `tip_cont` = '".PROFESOR."' ORDER BY `materie` ASC";
$res_asoc = mysql_query($query);
$nr_asoc = mysql_num_rows($res_asoc);

if(!empty($nr_asoc))//daca exista inregistrari penrtu materii
{

add($materie,'Cont per materie <br>
			<form action="" method="post">
			<table class="special" width="450px">
			<tr class="tr_head">
				<td>Nr*</td>
				<td>Materie*</td>
				<td>Utilizator</td>
				<td>Parola</td>
			</tr>');

for($i=0;$i<$nr_asoc;$i++)
{
	$mat = htmlspecialchars(mysql_result($res_asoc,$i,'materie'));
	$mat_utilizator = stripslashes(mysql_result($res_asoc,$i,'utilizator'));
	$mat_utilizator = htmlspecialchars($mat_utilizator,ENT_QUOTES);
	$mat_parola = mysql_result($res_asoc,$i,'parola');
	$mat_parola = stripslashes($mat_parola);
	$mat_parola = htmlspecialchars($mat_parola,ENT_QUOTES);
	$tip = mysql_result($res_asoc,$i,'tip_cont');
	$admin_id = mysql_result($res_asoc,$i,'admin_id');
	
	add($materie,'<input type="hidden" name="hidden_id[]" value="'.$admin_id.'">
				<tr class="'.$class_select[$i%2].'">
					<td class="read_only">'.($i+1).'</td>
					<td class="read_only">'.$mat.'</td>
					<td><input class="'.$class_select[$i%2].'" type="text" value="'.$mat_utilizator.'" name="utilizator[]"></td>
				<td><input  class="'.$class_select[$i%2].'" type="text" value="'.$mat_parola.'" name="parola[]"></td>
				  </tr>');
}
			
add($materie,'<tr>
				<td colspan="4"><input type="submit" value="Salveaza" name="salveaza_materie"></td>
			</tr>');
add($materie,'</table></form><br>* Coloanele marcate cu acest simbol nu sunt editabile.');
}
else
	add($materie,'Conturile per materie nu au fost definite.<br>');

add($content,$mesaj);
add($content,'<br>'.$admin . $secretara . $materie);

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

