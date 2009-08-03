<?php
//Lupescu Grigore
//July 2008 - July 2009
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");
$layout->get_template('include/template.html');
$layout->replace('TITLE','Salvare orar');

$meniu = show_menu(menu,$_SESSION['tip_cont'],1);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],2,1); 
$layout->replace('SUBMENU',$submeniu);

$content = '<script type="text/javascript">
			function selectallList() {
			var aselect = document.profesori["pers_acoperit[]"];
			var aselectLen = aselect.length;
			for(i = 0; i < aselectLen; i++) {
			aselect.options[i].selected = true;
			}
			}
			</script>';
add($content,'<div class="title" align="center">Verificare formular</div>');
add($content,'<br><div class="content_center">');
$layout->replace('CONTENT',$content);


<form action="http://www.rosedu.org/~cspay/2009/0_verificare_xls2.php" enctype="multipart/form-data" method="post">
<p>
Calea catre fisier (.XLS):<br>
<input type="file" name="datafile" size="40">
</p>
<div>
<input type="submit" value="Send">
</div>
</form>

$layout->print_template();
?>