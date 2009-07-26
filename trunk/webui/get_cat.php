<?php
include("include/mysql_config.php");
$ID=$_REQUEST['idCat'];
echo $ID;
$query = "SELECT * FROM catedre WHERE link_fac=$ID";
$result = mysql_query($query);
$nr = mysql_num_rows($result);
echo 'Catedra:<select name="catedra" width="100">';
echo '<option value="0"> </option>';
for ($i = 0; $i <$nr;$i++)
{
	 echo "<option value=".mysql_result($result,$i,'cat_id').">".
			mysql_result($result,$i,'nume')."</option>";
}
echo '</select>';
mysql_close($conexiune);
?>