<?php
//include headers
include("include/config.php");
include("include/header.php");
//include("include/check_login.php");

$mesaj = '';
/*

if(isset($_POST['send']))
{
	foreach($_POST as $index=>$var)
		{
		add($mesaj,$index.' => '.$var.'<br>');
		$$index = $var;
		}
	add($mesaj,'dim vector : '.count($nume).'<br><br>');
	
	foreach($nume as $index=>$var)
		add($mesaj,$index.' => '.$var.'<br>');
	add($mesaj,'<br>');
	foreach($modificat as $index=>$var)
		add($mesaj,$index.' => '.$var.'<br>');
}
*/
$layout->get_template('include/template.html');
$layout->replace('TITLE','prima pagina');

$meniu = show_menu(menu,$_SESSION['tip_cont'],-1);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,-1);
$layout->replace('SUBMENU',$submeniu);

/*
$content = "";
add($content,'<div class="title" align="center">Pagina principala</div>');
add($content,'<br>Tipul Contului : '.$_SESSION['tip_cont'].'<br><br>');

add($content,$mesaj);

// onchange="document.getElementById(\'camp_mod_'.$i.'\').value=\'1\';"

$aux = '<form action="" method="post"><table>';
for($i=0;$i<10;$i++)
	add($aux,'<tr><td><input type="hidden" value="0" name="modificat[]" id="camp_mod_'.$i.'">
			  <input  onchange="document.getElementById(\'camp_mod_'.$i.'\').value=\'1\';" type="text" value="nume_'.$i.'" name="nume[]" id="camp_'.$i.'"
			  > <input  onchange="document.getElementById(\'camp_mod_'.$i.'\').value=\'1\';" type="text" value="nume_'.$i.'" name="nume2[]" 
			  ><br></td></tr>');

add($content,$aux.'</table><input type="submit" name="send" value="test"></form>');
*/
##################################
$content = '';


$var = "edi are ' ' <br> mere ' ";

$b = html_entity_decode($var);

//$add = htmlentities($var);
$add = htmlspecialchars($var,ENT_QUOTES);

add($content,$add);
add($content,addslashes($var));

$x = "Nicolae Tapus \"";
$x = stripslashes($x);
$x = htmlspecialchars($x,ENT_QUOTES);
add($content,'<br><br><br>'.$x);


$tabel = '<table class="special">';
for($i=0;$i<10;$i++)
	{
		add($tabel,'<tr class="'.$class_select[$i%2].'">
					<td id="td_a_'.$i.'"><div id="a_'.$i.'">Pr.retelelor de calculatoare</td>
					<td id="td_b_'.$i.'"><div  id="b_'.$i.'">IZ</td>
					<td id="td_c_'.$i.'"><div id="c_'.$i.'">Rughinis R.</td>
					<td id="td_d_'.$i.'"><div  id="d_'.$i.'">po</td>
					<td id="td_e_'.$i.'"><div  id="e_'.$i.'">311CB</td>
					<td id="td_f_'.$i.'"><div  id="f_'.$i.'">Chiriac M.</td>
					<td id="td_g_'.$i.'"><div  id="g_'.$i.'">marti</td>
					<td id="td_h_'.$i.'"><div  id="h_'.$i.'">EC333</td>
					<td id="td_i_'.$i.'"><div  id="i_'.$i.'">A & C</td>
					<td id="td_j_'.$i.'"><div  id="j_'.$i.'">C</td>
					<td id="td_k_'.$i.'"><div id="k_'.$i.'">Pr.retelelor de calculatoare</td>
					<td id="td_l_'.$i.'"><div  id="l_'.$i.'">IZ</td>
					<td id="td_m_'.$i.'"><div id="m_'.$i.'">Rughinis R.</td>
					<td id="td_n_'.$i.'"><div  id="n_'.$i.'">po</td>
					<td id="td_o_'.$i.'"><div  id="o_'.$i.'">311CB</td>
					<td id="td_p_'.$i.'"><div  id="p_'.$i.'">Chiriac M.</td>
					<td id="td_q_'.$i.'"><div  id="q_'.$i.'">marti</td>
					<td id="td_r_'.$i.'"><div  id="r_'.$i.'">EC333</td>
					<td id="td_s_'.$i.'"><div  id="s_'.$i.'">A & C</td>
					<td id="td_t_'.$i.'"><div  id="t_'.$i.'">C</td>
					</tr>');
	}

add($tabel,'<tr align="center">');

$buton = '';
add($buton,'<td><a id="but_a"  onclick="less(\'a\');">--</a></td>');
add($buton,'<td><a id="but_b"  onclick="less(\'b\');">--</a></td>');
add($buton,'<td><a id="but_c"  onclick="less(\'c\');">--</a></td>');
add($buton,'<td><a id="but_d"  onclick="less(\'d\');">--</a></td>');
add($buton,'<td><a id="but_e"  onclick="less(\'e\');">--</a></td>');
add($buton,'<td><a id="but_f"  onclick="less(\'f\');">--</a></td>');
add($buton,'<td><a id="but_g"  onclick="less(\'g\');">--</a></td>');
add($buton,'<td><a id="but_h"  onclick="less(\'h\');">--</a></td>');
add($buton,'<td><a id="but_i"  onclick="less(\'i\');">--</a></td>');
add($buton,'<td><a id="but_j"  onclick="less(\'j\');">--</a></td>');

add($buton,'<td><a id="but_k"  onclick="less(\'k\');">--</a></td>');
add($buton,'<td><a id="but_l"  onclick="less(\'l\');">--</a></td>');
add($buton,'<td><a id="but_m"  onclick="less(\'m\');">--</a></td>');
add($buton,'<td><a id="but_n"  onclick="less(\'n\');">--</a></td>');
add($buton,'<td><a id="but_o"  onclick="less(\'o\');">--</a></td>');
add($buton,'<td><a id="but_p"  onclick="less(\'p\');">--</a></td>');
add($buton,'<td><a id="but_q"  onclick="less(\'q\');">--</a></td>');
add($buton,'<td><a id="but_r"  onclick="less(\'r\');">--</a></td>');
add($buton,'<td><a id="but_s"  onclick="less(\'s\');">--</a></td>');
add($buton,'<td><a id="but_t"  onclick="less(\'t\');">--</a></td>');

add($tabel,$buton);

add($tabel,'</tr></table>');

add($content,$tabel);


$layout->replace('CONTENT',$content);

$layout->print_template();
?>

