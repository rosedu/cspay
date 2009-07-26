<?php
include("include/mysql_config.php");
$ID=$_REQUEST['idFac'];
echo $ID;
$query = "SELECT * FROM facultati WHERE link_univ=$ID";
$result = mysql_query($query);
$nr = mysql_num_rows($result);
echo 'Facultate:<select name="facultate" width="100" onChange="CategoryGrab('."'".'get_cat.php?idCat='."'".'+this.value,'."'div_cat'".');">';
echo '<option value="0"> </option>';
for ($i = 0; $i <$nr;$i++)
{
	 echo "<option value=".mysql_result($result,$i,'fac_id').">".
			mysql_result($result,$i,'nume_scurt')."</option>";
}
echo '</select>';
mysql_close($conexiune);
?>