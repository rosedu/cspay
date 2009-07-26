<?php
include("include/config.php");
include("romunka.php");

echo "<body>

<center><h1>PHP ajax cascading drop down using MySql</h1></center><br>";
echo '<form method="post" action="">Universitate:';
$result = mysql_query("SELECT * FROM universitati");
$nr = mysql_num_rows($result);
echo '<select name="universitate" onChange="CategoryGrab('."'".'get_fac.php?idFac='."'".'+this.value,'."'div_fac'".');">';
echo '<option value="0"> </option>';
for ($i = 0; $i <$nr;$i++)
{
 echo "<option value=".'"'.mysql_result($result,$i,'univ_id').'" >'.mysql_result($result,$i,'nume')."</option>";
}
echo '</select>';

echo '<div id="div_fac">Facultate:<select name="facultate" width="100" >';
echo '<option value="0"> </option>';
echo '</select></div>';

echo '<div id="div_cat">Catedra:<select name="catedra" width="100" >';
echo '<option value="0"> </option>';
echo '</select></div>';

echo '</form>';
mysql_close($conexiune);
echo '</body></html>';
?>







