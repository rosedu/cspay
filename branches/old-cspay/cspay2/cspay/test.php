<?php
//include headers
include("include/config.php");
include("include/header.php");
//include("include/check_login.php");

$mesaj = '';

if(isset($_POST['send']))
{
	foreach($_POST as $index=>$var)
		{
		add($mesaj,$index.' => '.$var.'<br>');
		$$index = $var;
		}
	add($mesaj,'dim vector : '.count($nume).'<br><br>');
	
	foreach($nume as $index=>$var)
		add($mesaj,$index.' => '.$var.'<br>');
	add($mesaj,'<br>');
	foreach($modificat as $index=>$var)
		add($mesaj,$index.' => '.$var.'<br>');
}

$layout->get_template('include/template.html');
$layout->replace('TITLE','prima pagina');

$meniu = show_menu(menu,$_SESSION['tip_cont'],-1);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,-1);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Pagina principala</div>');
add($content,'<br>Tipul Contului : '.$_SESSION['tip_cont'].'<br><br>');

add($content,$mesaj);

// onchange="document.getElementById(\'camp_mod_'.$i.'\').value=\'1\';"

$aux = '<form action="" method="post"><table>';
for($i=0;$i<10;$i++)
	add($aux,'<tr><td><input type="hidden" value="0" name="modificat[]" id="camp_mod_'.$i.'">
			  <input  onchange="document.getElementById(\'camp_mod_'.$i.'\').value=\'1\';" type="text" value="nume_'.$i.'" name="nume[]" id="camp_'.$i.'"
			  > <input  onchange="document.getElementById(\'camp_mod_'.$i.'\').value=\'1\';" type="text" value="nume_'.$i.'" name="nume2[]" 
			  ><br></td></tr>');

add($content,$aux.'</table><input type="submit" name="send" value="test"></form>');

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

