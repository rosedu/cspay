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

$i=10;

$tabel = '<table class="special">';
add($tabel,'<tr class="tr_head">
					<td id="td_a_'.$i.'"><div class="over" id="ta_'.$i.'">c<br>o<br>l<br>1</div><div id="a_'.$i.'">col 1</td>
					<td id="td_b_'.$i.'"><div  class="over"  id="tb_'.$i.'">c<br>o<br>l<br>2</div><div  id="b_'.$i.'">col 2</td>
					<td id="td_c_'.$i.'"><div  class="over"  id="tc_'.$i.'">c<br>o<br>l<br>3</div><div id="c_'.$i.'">col 3</td>
					<td id="td_d_'.$i.'"><div  class="over"  id="td_'.$i.'">c<br>o<br>l<br>4</div><div  id="d_'.$i.'">col 4</td>
					<td id="td_e_'.$i.'"><div  class="over"   id="te_'.$i.'">c<br>o<br>l<br>5</div><div  id="e_'.$i.'">col 5</td>
					<td id="td_f_'.$i.'"><div  class="over"  id="tf_'.$i.'">c<br>o<br>l<br>6</div><div  id="f_'.$i.'">col 6</td>
					<td id="td_g_'.$i.'"><div  class="over"  id="tg_'.$i.'">c<br>o<br>l<br>7</div><div  id="g_'.$i.'">col 7</td>
					<td id="td_h_'.$i.'"><div  class="over"  id="th_'.$i.'">c<br>o<br>l<br>8</div><div  id="h_'.$i.'">col 8</td>
					<td id="td_i_'.$i.'"><div  class="over"  id="ti_'.$i.'">c<br>o<br>l<br>9</div><div  id="i_'.$i.'">col 9</td>
					<td id="td_j_'.$i.'"><div  class="over"  id="tj_'.$i.'">c<br>o<br>l<br>10</div><div  id="j_'.$i.'">col 10</td>
					<td id="td_k_'.$i.'"><div  class="over"  id="tk_'.$i.'">c<br>o<br>l<br>11</div><div id="k_'.$i.'">col 11</td>
					<td id="td_l_'.$i.'"><div  class="over"  id="tl_'.$i.'">c<br>o<br>l<br>12</div><div  id="l_'.$i.'">col 12</td>
					<td id="td_m_'.$i.'"><div  class="over"  id="tm_'.$i.'">c<br>o<br>l<br>13</div><div id="m_'.$i.'">col 13</td>
					<td id="td_n_'.$i.'"><div  class="over"  id="tn_'.$i.'">c<br>o<br>l<br>14</div><div  id="n_'.$i.'">col 14</td>
					<td id="td_o_'.$i.'"><div class="over"   id="to_'.$i.'">c<br>o<br>l<br>15</div><div  id="o_'.$i.'">col 15</td>
					<td id="td_p_'.$i.'"><div  class="over"  id="tp_'.$i.'">c<br>o<br>l<br>16</div><div  id="p_'.$i.'">col 16</td>
					<td id="td_q_'.$i.'"><div  class="over"  id="tq_'.$i.'">c<br>o<br>l<br>17</div><div  id="q_'.$i.'">col 17</td>
					<td id="td_r_'.$i.'"><div  class="over"  id="tr_'.$i.'">c<br>o<br>l<br>18</div><div  id="r_'.$i.'">col 18</td>
					<td id="td_s_'.$i.'"><div  class="over"  id="ts_'.$i.'">c<br>o<br>l<br>19</div><div  id="s_'.$i.'">col 19</td>
					<td id="td_t_'.$i.'"><div  class="over"  id="tt_'.$i.'">c<br>o<br>l<br>20</div><div  id="t_'.$i.'">col 20</td>
					</tr>');

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
add($buton,'<td><a class="resize"  id="but_a"  onclick="less(\'a\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_b"  onclick="less(\'b\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_c"  onclick="less(\'c\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_d"  onclick="less(\'d\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_e"  onclick="less(\'e\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_f"  onclick="less(\'f\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_g"  onclick="less(\'g\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_h"  onclick="less(\'h\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_i"  onclick="less(\'i\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_j"  onclick="less(\'j\');">&gt;-&lt;</a></td>');

add($buton,'<td><a class="resize"  id="but_k"  onclick="less(\'k\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_l"  onclick="less(\'l\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_m"  onclick="less(\'m\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_n"  onclick="less(\'n\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_o"  onclick="less(\'o\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_p"  onclick="less(\'p\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_q"  onclick="less(\'q\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_r"  onclick="less(\'r\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_s"  onclick="less(\'s\');">&gt;-&lt;</a></td>');
add($buton,'<td><a class="resize"  id="but_t"  onclick="less(\'t\');">&gt;-&lt;</a></td>');

add($tabel,$buton);

add($tabel,'</tr></table>');

//add($content,$tabel);

$new = '<table border="0">';

$i='head';
add($new,'<tr class="tr_head">
	<td><div id="test_0_but" onclick="hide(\'test_0\')" class="resize">++</div>
	    <div id="test_0_'.$i.'" onclick="hide(\'test_0\')">test</div></td>
<td><div id="test_1_but" onclick="hide(\'test_1\')" class="resize">++</div>
	    <div id="test_1_'.$i.'" onclick="hide(\'test_1\')">test</div></td>
	    <td><div id="test_2_but" onclick="hide(\'test_2\')" class="resize">++</div>
	    <div id="test_2_'.$i.'" onclick="hide(\'test_2\')">test</div></td>
	    <td><div id="test_3_but" onclick="hide(\'test_3\')" class="resize">++</div>
	    <div id="test_3_'.$i.'" onclick="hide(\'test_3\')">test</div></td>
	    <td><div id="test_4_but" onclick="hide(\'test_4\')" class="resize">++</div>
	    <div id="test_4_'.$i.'" onclick="hide(\'test_4\')">test</div></td>	
		    </tr>');

for($i=0;$i<10;$i++)
{
	add($new,'<tr class="'.$class_select[$i%2].'">
	<td><div id="test_0_'.$i.'">test</div></td>
	<td><div id="test_1_'.$i.'">test</div></td>
	<td><div id="test_2_'.$i.'">test</div></td>
	<td><div id="test_3_'.$i.'">test</div></td>
	<td><div id="test_4_'.$i.'">test</div></td>
			
		    </tr>');
}
add($new,'</table>');

add($content,$new);

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

