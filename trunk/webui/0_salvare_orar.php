<?php
//Eduard Tuþescu & Daniel Urdã
//July 2008 - July 2009
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
			$ics=$ics."Nu a&#x163;i selectat nici un profesor";
			return $ics;
			}
		$command = "cd /home/cspay/web-exec-scripts/ && /usr/bin/python call.py ";
		$command .= $_POST['tip_cal']." ";
		$command = $command." ".addquot($_SESSION['univ_nume'])." ".addquot($_SESSION['fac_nume'])." ";
		$command = $command.addquot($_SESSION['cat_nume']);
		foreach ($_POST['pers_acoperit'] as $dobi)
			$command = $command." ".addquot($dobi);

		$output=array();
		exec($command,&$output);
		foreach($output as $line)
			$ics=$ics.$line."<br>";
		$ics=$ics."<a href = \"".$output[count($output)-1]."\">Fi&#x15F;ierul dvs</a>";
		}
	return $ics;
	}

$layout->get_template('include/template.html');
$layout->replace('TITLE','Salvare orar');

$meniu = show_menu(menu,$_SESSION['tip_cont'],0);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],2,0); 
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

$query = "SELECT `pers_acoperit_efect` FROM `ore`
		   GROUP BY `pers_acoperit_efect`";
$result = mysql_query($query);
$nr_reg = mysql_num_rows($result);
$pers = '<select size="10" style="font-size:10pt;" name="pers_acoperit[]" multiple="multiple">';
for($i=0;$i<$nr_reg;$i++)
	{
	$pers .= '<option>'.mysql_result($result,$i,'pers_acoperit_efect').'</option>';
	}

$form = '<form name="profesori" action="" method="post">
		<table style="width:100%;" border="0">
		<tr>
		';

add($form,'
			<td style="width:30%;" valign="top">Profesor :<br>'
			.$pers.
			'
			</select>
			<br/>
			<a href="#"
				onclick="javascript:selectallList();"
				onfocus="this.blur()">Selecteaz&#x103; pe to&#x163;i</a>
			</td>
			<td valign="top" style="width:30%;">Format ie&#x15F;ire:<br>
				<select style="font-size:10pt;" name="tip_cal" id="tip_fisier">
				<option value="1">Microsoft Excel (.xls)</option>
				<option value="2">iCalendar (.ics)</option>	
			</select>
			</td>
			<td style="width:30%;" valign="top">'.
			dourjob().
			'</tr>
			<tr>
			<td colspan="2">
				<br>Pentru selec&#x163;ie multiplp<br>&#x163;ine&#x163;i ap&#x103;sat&#x103; tasta Ctrl
			</td>
			</tr>
			<tr>
			<td colspan="2">
				<input type="submit" name="salveaza" value="Salveaz&#x103;">
			</td>
			</tr>
			</table>
			');
add($content,$form);

$layout->replace('CONTENT',$content);
$layout->print_template();
?>

