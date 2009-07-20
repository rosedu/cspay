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

if(isset($_POST['salveaza']))//tratare formular export
{
	foreach($_POST as $index => $val)
	{
		$$index = $val;
		
		echo $index .' => '.$val .'<br>';
	}
	for($i=0;$i<count($norma);$i++)//pentru fiecare persoana din norma
	{
		//for($j=0;$j<count($luna);$j++)
		{
			//echo exec('include/cspay_bin -n '.$norma[$i].' -f a&c -c curs_nou -save xls -l 1');
			echo exec('include/cspay_bin -n "Zaharia M." -f "A&C" -c "curs" -s ods -l 6');
		}
	}
}

$query = "SELECT `norma` FROM `ore`
		  GROUP BY `norma`";
$result = mysql_query($query);
$nr_reg = mysql_num_rows($result);

$pers = '<select size="10" name="norma[]" multiple="multiple">';
for($i=0;$i<$nr_reg;$i++)
{
	$pers .= '<option>'.mysql_result($result,$i,'norma').'</option>';
}

$form = '<form action="" method="post">
		<table width="800px" border="0">
		<tr>
			<td width="200px">
	';
add($form,'Lunile : <br>
			<select name="luna[]" id="luna" multiple="multiple" size="10" style="width:130px">');

$query = "SELECT * FROM `universitati` WHERE `univ_id`='".$_SESSION['univ_id']."' LIMIT 1";
$res_data = mysql_query($query);

$data_start = date("d-m-Y",strtotime(mysql_result($res_data,0,'data_start')));
$data_stop = date("d-m-Y",strtotime(mysql_result($res_data,0,'data_stop')));

$luna_start = date("n",strtotime($data_start));
$luna_stop = date("n",strtotime($data_stop));

$luna = array(0=>"Ianuarie",1=>"Februarie",2=>"Martie",3=>"Aprilie",4=>"Mai",5=>"Iunie",6=>"Iulie",
			  7=>"August",8=>"Septembrie",9=>"Octombrie",10=>"Noiembrie",11=>"Decembrie");

if($luna_stop<$luna_start)
{
for($i=$luna_start;$i<12;$i++)
	add($form,'<option value="'.($i+1).'">'.$luna[$i].'</option>');
for($i=0;$i<$luna_start;$i++)
	add($form,'<option value="'.($i+1).'">'.$luna[$i].'</option>');
}
else
{
for($i=$luna_start;$i<$luna_stop;$i++)
	add($form,'<option value="'.$i.'">'.$luna[$i].'</option>');

}

add($form,'
			</select>
			</td>
			<td valign="top">Norma :<br>'
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

