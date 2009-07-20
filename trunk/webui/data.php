

//afisare tabel
$query = "SELECT * FROM `ore` WHERE `materie` = '".$_SESSION['materie']."'";
//$query = "SELECT `orar_id` , `facultate` , `tip_curs1` , `materie` , `forma` , `cod` , `an` , `serie` , `nr_stud` , `nr_grupa` , `tip_grupa` , `modul_c` , `modul_a` , `tip_curs2` , `post` , `grad` , `norma` , `ocupat` , `acoperit` , `acoperit_efect` , `an_grupa` , `zi` , `ora` , `sala` FROM `ore`";

$result = mysql_query($query);
$nr = mysql_num_rows($result);

add($content,'<br>Materia :'. $_SESSION['materie'].'<br><br>');

add($content,'<form>
<table width="100%" border="0" cellpadding="1" cellspacing="1" style="border:1px solid #CCCCCC;">
  <tr align="center" bgcolor="#ece9d8">
     <td>Nr</td>
	<td>Tip</td>
    <td>An</td>
    <td>Serie</td>
    <td>Tip Grupa  </td>
    <td>Norma</td>
    <td>Ocupat</td>
    <td>Acoperit</td>
    <td>Efectiv</td>
    <td>An/Gr</td>
    <td>Zi</td>
    <td>Ora </td>
    <td>Sala</td>
    </tr>');

$color = array(0 => "#f6f2d8",1 => "#ede7c2");
$camp = array('1','5','6','9','15','16','17','18','19','20','21','22');

for($i=0;$i<$nr;$i++)
{
	$rand = mysql_fetch_row($result);
	add($content,'<tr bgcolor="'.$color[$i%2].'">');
	
	add($content,'<td>'.($i+1).'</td>');
	for($j=0;$j<12;$j++)//fara id
	{
		add($content,'<td>'.$rand[$camp[$j]+1].'</td>');
	}
	add($content,'</tr>');
}
add($content,'</table></form>');

$layout->replace('CONTENT',$content);



$layout->print_template();
?>













//////menu

<div id="menu">
		<ul>
			<li  class="active"><a href="#">Home</a></li>
			<li><a href="#">Despre</a></li>
			<li><a href="#">Modifica</a></li>
			<li><a href="#">Log Out </a></li>
			<li><a href="#">Contact </a></li>
		</ul>
</div>

////submenu

<div id="submenu">
		<ul>
			<li class="subactive"><a href="#">adauga</a></li>
			<li><a href="#">sterge</a></li>
			<li><a href="#">Modifica</a></li>
			<li><a href="#">printeaza</a></li>
			<li><a href="#">cauta </a></li>
		</ul>
	</div>

	
///////content
<div class="title">Titlu</div>
				
		<!-- de sters-->
				
<form>
					
<table width="100%" border="0" cellpadding="1" cellspacing="1" style="border:1px solid #CCCCCC;">
  <tr align="center" bgcolor="#ece9d8">
  `ora_id` , `link_disc` , `tip_ora` , `forma` , `cod` , `an` , `serie` , `nr_stud` , `nr_grupa` , 
					`tip_grupa_aplicatii` , `nr_ore_curs` , `nr_ore_aplicatii`, `nr_post`, `grad_post`,
					`pers_norma` , `tip_ocupare` , `pers_acoperit` , `pers_acoperit_efect` , `an_grupa` ,
					`zi` , `ora` , `sala`, `paritate`, `paritate_start`
    <td>Disciplina</td>
	<td>Tip</td>
    <td>Forma</td>
    <td>Cod</td>
    <td>An</td>
    <td>Serie</td>
    <td>Nr Studenti  </td>
    <td>Nr Grupe </td>
    <td>Tip Grupa  </td>
    <td>Nr ore curs</td>
    <td>Nr ore aplicatii</td>
    <td>Post</td>
    <td>Grad</td>
    <td>Norma</td>
    <td>Ocupat</td>
    <td>Acoperit</td>
    <td>Efectiv</td>
    <td>An/Gr</td>
    <td>Zi</td>
    <td>Ora </td>
    <td>Sala</td>
    </tr>
  <tr bgcolor="#f6f2d8">
    <td height="17" width="75pt" align="center"><input type="text" id="in_1" class="form1" value="A&C" onfocus="javascript:this.setAttribute('class','selected');" onblur="javascript:this.setAttribute('class','form1');" style="width:75pt;"/></td>
    <td align="center"><input type="text" class="form1" value="C" style="max-width:20pt;"/></td>
    <td align="left"><input type="text" class="form1" value="Sisteme de operare" /></td>
    <td align="left">IZ</td>
    <td align="left">LR</td>
    <td align="left">&nbsp;</td>
    <td align="center" sdval="4" sdnum="1033;0;0">4</td>
    <td align="left">CA</td>
    <td align="right" sdval="150" sdnum="1033;">&nbsp;</td>
    <td align="right" sdval="150" sdnum="1033;">150</td>
    <td align="center" sdval="6" sdnum="1033;">6</td>
    <td align="center">g</td>
    <td align="center">&nbsp;</td>
    <td align="right" sdval="3" sdnum="1033;">3</td>
    <td align="right"><strong><br />
    </strong></td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    </tr>
  <tr bgcolor="#ede7c2">
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    </tr>
</table>
</form>






/*
//tratare pagina si parametri
if(isset($_GET['start']) && isset($_GET['nr_afisari']))
{
	$start = $_GET['start'];
	$nr_afisari = $_GET['nr_afisari'];
	$stop = $start + $nr_afisari;
}
else
{
	$start = 0;
	$stop = $nr_afisari = 20;
}	

//selecteaza inregistrarile din tabela orar
$query = "SELECT * FROM `ore`";
$result = mysql_query($query);
$nr = mysql_num_rows($result);//numarul de intrari

add($content,'<div class="title" align="center">Formular detalii plata cu ora</div>');
add($content,'<br>Tipul Contului : '.$_SESSION['tip_cont']);

/*
add($content,'<a href="0_orar_viz.php?start='.( ($start-$nr_afisari<0)?0:($start-$nr_afisari<0)).'&nr_afisari=20>Inapoi</a>&nbsp'.
			 '<a href="0_orar_viz.php?start='.( ($start+$nr_afisari<$nr)?($start+$nr_afisari<0):$nr).'&nr_afisari=20>Inainte</a>');	

*/				
if($nr != 0)//daca exista inregistrari in tabela `orar`
{

	add($content,display_thead_orar_secr());//scrie capul de tabel

	for($i=0;$i<$nr;$i++)//pentru  fiecare intrare
	{
		$output = display_result($result,$i);//scrie linia respectiva sub forma de tabel
		add($content,$output);
	}
	add(content,'</table><input type="submit" name="salveaza" value="Salveaza">');
	add($content,'</form>');
}
else
	{
	add($content,"Fisierul XlS nu a fost importat.");	
	}


$layout->replace('CONTENT',$content);


//echo $_SESSION['univ_id'];

$layout->print_template();
?>

