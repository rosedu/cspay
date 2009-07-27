<?php
include("include/mysql_config.php");

function display_select_fac()
{
	$fac ='<div id="div_fac">Facultate:<select style="font-size:10pt;width: 100px;" name="facultate" >';
	$fac .='<option value="0"> </option>';
	$fac .= '</select></div>';
	return $fac;
}
		
function display_select_cat()
{
	$fac ='<div id="div_cat">Catedra:<select name="catedra" style="font-size:10pt;width: 100px;" >';
	$fac .='<option value="0"> </option>';
	$fac .= '</select></div>';
	return $fac;
}	

if (isset($_REQUEST['idCat']))
{
$ID=$_REQUEST['idCat'];
$query = "SELECT * FROM catedre WHERE link_fac=$ID";
$result = mysql_query($query);
$nr = mysql_num_rows($result);
echo 'Catedra:<select name="catedra" style="font-size:10pt;">';
echo '<option value="0"> </option>';
for ($i = 0; $i <$nr;$i++)
{
	 echo "<option value=".mysql_result($result,$i,'cat_id').">".
			mysql_result($result,$i,'nume')."</option>";
}
echo '</select>';
}

elseif (isset($_REQUEST['idFac']))
{
$ID=$_REQUEST['idFac'];
$query = "SELECT * FROM facultati WHERE link_univ=$ID";
$result = mysql_query($query);
$nr = mysql_num_rows($result);
echo 'Facultate:<select name="facultate" style="font-size:10pt;" onChange="CategoryGrab('."'".'get_fac.php?idCat='."'".'+this.value,'."'div_cat'".');">';
echo '<option value="0"> </option>';
for ($i = 0; $i <$nr;$i++)
{
	 echo "<option value=".mysql_result($result,$i,'fac_id').">".
			mysql_result($result,$i,'nume_scurt')."</option>";
}
echo '</select>';
}
elseif (isset($_REQUEST['idTip']))
{
$ID=$_REQUEST['idFac'];
if($ID != "A")
{
	$result = mysql_query("SELECT * FROM universitati");
	$nr = mysql_num_rows($result);
	$univ ='<tr><td>Universitate';
	$univ .= '<select style="font-size:10pt;" name="universitate" onChange="CategoryGrab('."'".'get_fac.php?idFac='."'".'+this.value,'."'div_fac'".');">';
	$univ .= '<option value="0"> </option>';
	for ($i = 0; $i <$nr;$i++)
		$univ .= "<option value=".'"'.mysql_result($result,$i,'univ_id').'" >'.mysql_result($result,$i,'nume')."</option>";
	$sel_fac = display_select_fac();
	$sel_cat = display_select_cat();
	$univ .= '<td>'.$sel_fac.'</td>';
	$univ .= '<td>'.$sel_cat.'</td></tr>';
	echo $univ;
}
}
mysql_close($conexiune);
?>