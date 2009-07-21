<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

function addquot($str)
{
return "\"".$str."\"";
}
function dourjob()
{
global $_POST, $pers_acoperit;
$ics="";
if(isset($_POST['salveaza']))//tratare formular export
{
	if (count($_POST['pers_acoperit']) == 0)
		{
			$ics=$ics."Nu ati selectat nici un profesor";
			break;
		}

	$output=array();
	$command = "cd /home/cspay/web-exec-scripts/ && /usr/bin/python call.py ";
	$command = $command." ".addquot($_SESSION['univ_nume'])." ".addquot($_SESSION['fac_nume'])." ";
	$command = $command.addquot($_SESSION['cat_nume']);
	foreach ($_POST['pers_acoperit'] as $dobi)
		$command = $command." ".addquot($dobi);
	exec($command,&$output);
	
	foreach($output as $line)
		$ics=$ics.$line."<br>";
		
	$ics=$ics."<a href = \"file:///".$output[cout($output)-1]."\">Fisierul dvs</a>";

}
return $ics;
}

$layout->get_template('include/template.html');
$layout->replace('TITLE','Salvare formular');

$meniu = show_menu(menu,$_SESSION['tip_cont'],1);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],1,1);
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
add($content,'<div class="title" align="center">Salvare formular</div>');
add($content,'<br><div class="content_center">');

$query = "SELECT `pers_acoperit` FROM `ore`
		   WHERE `tip_ocupare`='po' GROUP BY `pers_acoperit`";
$result = mysql_query($query);
$nr_reg = mysql_num_rows($result);

$pers = '<select size="10" name="pers_acoperit[]" multiple="multiple">';
for($i=0;$i<$nr_reg;$i++)
{
	$pers .= '<option>'.mysql_result($result,$i,'pers_acoperit').'</option>';
}

$form = '<form name="profesori" action="" method="post">
		<table width="800px" border="0">
		<tr>
	';

add($form,'
			<td>Profesor :<br>'
			.$pers.
			'
			</select>
			<br/>
			<a href="#"
				onclick="javascript:selectallList();"
				onfocus="this.blur()">Selecteaza pe toti</a>
			</td>
			<td >Format iesire:<br>
				<select name="tip_fisier[]" id="tip_fisier" multiple="multiple">

				<option selected="selected" value="xls">Microsoft Excel (.xls)</option>
				<option value="ods">OpenDocumentSpreadsheet (.ods)</option>
				<option value="pdf">Adobe Acrobat (.pdf)</option>	
			</select>
			</td>
			<td>'.
			dourjob().
			'</tr>
			<tr>
			<td colspan="2">
				<br>Pentru selectie multipla<br>tineti apasata tasta Ctrl
			</td>
			</tr>
			<tr>
			<td colspan="2">
				<input type="submit" name="salveaza" value="Salveaza">
			</td>
			</tr>
			</table>
			');
add($content,$form);



$layout->replace('CONTENT',$content);

$layout->print_template();
?>

