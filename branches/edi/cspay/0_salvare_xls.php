<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");

$layout->get_template('include/template.html');
$layout->replace('TITLE','Salvare formular');

$meniu = show_menu(menu,$_SESSION['tip_cont'],1);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],1,1);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Salvare formular</div>');
add($content,'<br><div class="content_center">');

$query = "SELECT `acoperit_efect` FROM `orar` WHERE `link_univ`='".$_SESSION['univ_id']."'
		  GROUP BY `acoperit_efect`";
$result = mysql_query($query);
$nr_reg = mysql_num_rows($result);

$pers = '<select size="10" multiple="multiple">';
for($i=0;$i<$nr_reg;$i++)
{
	$pers .= '<option>'.mysql_result($result,$i,'acoperit_efect').'</option>';
}

$form = '<form action="" method="post">
		<table width="800px" border="0">
		<tr>
			<td width="200px">
	';
add($form,'Lunile : <br>
						<select name="luna[]" id="luna" multiple="multiple" size="10" style="width:130px">
<!--				<option value="9">octombrie</option>
				<option value="10">noiembrie</option>
				<option value="11">decembrie</option>
				<option value="0">ianuarie</option>-->
				<option value="1">februarie</option>
				<option value="2">martie</option>
				<option value="3">aprilie</option>
				<option value="4">mai</option>
				<option value="5">iunie</option>

				<!--<option value="6">iulie</option>
				<option value="7">august</option>
				<option value="8">septembrie</option>
				-->
			</select>
			</td>
			<td valign="top">Persoana :<br>'
			.$pers.
			'</td>
			</tr>
			<tr>
			<td colspan="2"><br>
			<label>Format fisier iesire:</label>
			<select name="tip_fisier[]" id="tip_fisier" multiple="multiple">

				<option selected="selected" value="xls">Microsoft Excel(.xls)</option>
				<option value="ods">OpenDocumentSpreadsheet (.ods)</option>				
			</select>
			</tr>
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

