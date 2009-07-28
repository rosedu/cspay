<?php
include("include/mysql_config.php");

function display_select_fac()
{
	$fac ='<div id="div_fac"><select style="font-size:10pt;width: 100px;" name="facultate" >';
	$fac .='<option value="0">&nbsp;</option>';
	$fac .= '</select></div>';
	return $fac;
}
		
function display_select_cat()
{
	$fac ='<div id="div_cat"><select name="catedra" style="font-size:10pt;width: 100px;" >';
	$fac .='<option value="0">&nbsp;</option>';
	$fac .= '</select></div>';
	return $fac;
}	

if (isset($_REQUEST['idCat']))
{
$ID=$_REQUEST['idCat'];
$query = "SELECT * FROM catedre WHERE link_fac=$ID";
echo '<select name="catedra" style="font-size:10pt;">';
echo '<option value="0">&nbsp;</option>';

$result = mysql_query($query);
if ($result)
	{
	$nr = mysql_num_rows($result);
	for ($i = 0; $i <$nr;$i++)
		{
			echo "<option value=".mysql_result($result,$i,'cat_id').">".
					mysql_result($result,$i,'nume')."</option>";
		}
	}
echo '</select>';
}

elseif (isset($_REQUEST['idFac']))
{
$ID=$_REQUEST['idFac'];
$query = "SELECT * FROM facultati WHERE link_univ=$ID";
$result = mysql_query($query);
$nr = mysql_num_rows($result);
echo '<select name="facultate" style="font-size:10pt;" onChange="CategoryGrab('."'".'get_fac.php?idCat='."'".'+this.value,'."'div_cat'".');">';
echo '<option value="0">&nbsp;</option>';
for ($i = 0; $i <$nr;$i++)
{
	 echo "<option value=".mysql_result($result,$i,'fac_id').">".
			mysql_result($result,$i,'nume_scurt')."</option>";
}
echo '</select>';
}
elseif (isset($_REQUEST['idTip']))
{
$ID=$_REQUEST['idTip'];
if($ID != "A")
{
	$result = mysql_query("SELECT * FROM universitati");
	$nr = mysql_num_rows($result);
	$univ ='<td>Universitatea:</td><td>';
	$univ .= '<select style="font-size:10pt;" name="universitate" onChange="CategoryGrab('."'".'get_fac.php?idFac='."'".'+this.value,'."'div_fac'".');">';
	$univ .= '<option value="0">&nbsp;</option>';
	for ($i = 0; $i <$nr;$i++)
		$univ .= "<option value=".'"'.mysql_result($result,$i,'univ_id').'" >'.mysql_result($result,$i,'nume')."</option></select></td>";
	$sel_fac = display_select_fac();
	$sel_cat = display_select_cat();
	$univ .= '<br><td>Facultatea:</td><td>'.$sel_fac.'</td>';
	$univ .= '<br><td>Catedra:</td><td>'.$sel_cat.'</td>';
	echo $univ;
}
else
{
	echo '&nbsp;';
}
}
mysql_close($conexiune);
?>