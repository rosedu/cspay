<?php
include('conf_select.php');
//functii pentru afisarea optiunilor de selectie pentru diferite campuri

//afiseaza caseta de selectie pentru tipul de ora curs(C) sau laborator(L)
function display_select($name,$value,$hidden_id,$index_class,$vector)
{
	global $class_select;
	
	$output = '';
	$write_id = ($hidden_id != -1) ? 'onchange="change_hidden(\''.$hidden_id.'\',\'1\');"' : '';
	$class = ($index_class != -1) ? $class_select[$index_class%2] : '';
	
	add($output,'<select  class="'.$class.'" '.' name="'.$name.'" '.$write_id.'>');
	for($i=0;$i<sizeof($vector);$i++)
		add($output,'<option value="'.$vector[$i].'" '.
				( ($value==$vector[$i])?'selected':'').
				'>'.html_entity_decode($vector[$i]).'</option>');
	add($output,'</select>');
	
	return $output;
}

function display_input($name,$value,$hidden_id,$index_class,$width)
{
	global $class_select;
	
	$output = '';
	add($output,'<input type="text" name="'.$name.'" value="'.$value.'"'.'  class="'.$class_select[$index_class%2].'" '.
				'onchange="change_hidden(\''.$hidden_id.'\',\'1\');" style="width:'.$width.'px">');
	
	// class="'.$class_select[$index_class%2].'" ---->de adaugat pentru a modifica afisajul
	
	return $output;
}

function get_nume_scurt($univ_id)
{
	$output = '';
	$query = "SELECT `nume_scurt` FROM `facultati` WHERE `link_univ`='".$univ_id."'";
	$result = mysql_query($query);
	$nr = mysql_num_rows($result);
	
	add($output,'<select>');
	
	for($i=0;$i<$nr;$i++)
	{
		$nume_scurt = mysql_result($result,$i,'nume_scurt');
		add($output,'<option>'.$nume_scurt.'</option>');
	}
	
	add($output,'</select>');
	
	return $output;
}

function get_disc_fac($disc_id)
{
	$query = "SELECT `nume_scurt`, `link_fac` FROM `discipline` WHERE `disc_id`='".$disc_id."'";
	$result = mysql_query($query);
	$output = array();
	$output[] = mysql_result($result,0,'nume_scurt');
	$link_fac = mysql_result($result,0,'link_fac');
	$query = "SELECT `nume_scurt` FROM `facultati` WHERE `fac_id`='".$link_fac."'";
	$result = mysql_query($query);
	$output[] = mysql_result($result,0,'nume_scurt');
	return $output;
}
//preia campurile din vectorul $result returnat de mysql_query
//si afiseaza sub forma de tabel
function display_result_secr($result,$index,$count)
{
	 
	$hidden_id = 'modificat_'.$index;
	global $color;//culorile pentru linii pare sau impare
	global $select_ora, $select_forma, $select_cod,
		   $select_an, $select_serie, $select_tip_grupa,
		   $select_a, $select_c, $select_nr_grupa,
		   $select_grupa, $select_grad, $select_zi, $select_ora,
		   $select_sala, $select_tip_ora;
	
	$output = '';
	
	mysql_data_seek($result,$index);//trec la intrarea cu numarul $index
	$valori = list(	  $ora_id, $link_disc, $tip_ora, $forma, $cod, $an, $serie, 
	  $nr_stud, $nr_grupa, $tip_grupa, $modul_c, $modul_a, $post,
	  $grad, $norma, $tip_ocup, $acoperit, $acoperit_efect, $an_grupa, $zi, 
	  $ora, $sala, $parit, $parit_st) = mysql_fetch_array($result); 

	foreach($valori as $i=>$val)
		$valori[$i] = htmlspecialchars(stripslashes($val),ENT_QUOTES);

	$aux = strstr($ora,"-");
	$ora_start = substr($ora,0,strlen($ora)-strlen($aux));
	$ora_stop  = substr($aux,1,strlen($aux)-1);
	
	//campul care arata daca s-a schimbat ceva in linia curenta
	add($output,'<input type="hidden" name="modificat[]" value="0" id="'.$hidden_id.'">');
	add($output,'<input type="hidden" name="orar_id[]" value="'.$orar_id.'">');
	add($output,'<tr bgcolor="'.$color[$index%2].'">');//deschid un nou rand 
	add($output,'<td class="read_only">'.($index+1).'</td>');//scriu numarul liniei curente - needitabil
	add($output,'<td class="read_only"><div id="td_1_'.$count.'">'.$facultate.'</div></td>');//scriu facultatea din care face parte - needitabil
	add($output,'<td ><div id="td_2_'.$count.'">'.display_select('orar_tip_ora[]',$tip_curs1,$hidden_id,$index,$select_tip_ora).
				'</div></td>');//tipul cursului : C sau L
	add($output,'<td class="read_only"><div id="td_3_'.$count.'">'.$link_disc.'</div></td>');//materia - needitabil
	add($output,'<td ><div id="td_4_'.$count.'">'.display_select('orar_forma[]',$forma,$hidden_id,$index,$select_forma).'</div></td>');//tipul orei 
	add($output,'<td ><div id="td_5_'.$count.'">'.display_select('orar_cod[]',$cod,$hidden_id,$index,$select_cod).'</div></td>');//codul asociat
	
	add($output,'<td ><div id="td_6_'.$count.'">'.display_select('orar_an[]',$an,$hidden_id,$index,$select_an).'</div></td>');//anul
	add($output,'<td ><div id="td_7_'.$count.'">'.display_select('orar_seria[]',$serie,$hidden_id,$index,$select_serie).'</div></td>');//seria
	add($output,'<td ><div id="td_8_'.$count.'">'.display_input('orar_nr_stud[]',$nr_stud,$hidden_id,$index,40).'</div></td>');//nr studenti
	add($output,'<td ><div id="td_9_'.$count.'">'.display_select('orar_nr_grupa[]',$nr_grupa,$hidden_id,$index,$select_nr_grupa).'</div></td>');//nr de grupe
	add($output,'<td ><div id="td_10_'.$count.'">'.display_select('orar_tip_grupa[]',$tip_grupa,$hidden_id,$index,$select_tip_grupa).'</div></td>');//tipul grupei
	add($output,'<td ><div id="td_11_'.$count.'">'.display_select('orar_modul_c[]',$modul_c,$hidden_id,$index,$select_c).'</div></td>');//modulul C1
	add($output,'<td ><div id="td_12_'.$count.'">'.display_select('orar_modul_a[]',$modul_a,$hidden_id,$index,$select_a).'</div></td>');//modulul A1
	
	add($output,'<td ><div id="td_13_'.$count.'">'.display_input('orar_post[]',$post,$hidden_id,$index,30).'</div></td>');//postul
	add($output,'<td ><div id="td_14_'.$count.'">'.display_select('orar_grad[]',$grad,$hidden_id,$index,$select_grad).'</div></td>');//gradul
	add($output,'<td ><div id="td_15_'.$count.'">'.display_input('orar_norma[]',$norma,$hidden_id,$index,95).'</div></td>');//norma
	add($output,'<td ><div id="td_16_'.$count.'">'.display_input('orar_ocupat[]',$ocupat,$hidden_id,$index,35).'</div></td>');//ocupat po sau B
	
	add($output,'<td ><div id="td_17_'.$count.'">'.display_input('orar_acoperit[]',$acoperit,$hidden_id,$index,95).'</div></td>');//acoperit
	add($output,'<td ><div id="td_18_'.$count.'">'.display_input('orar_acoperit_efect[]',$acoperit_efect,$hidden_id,$index,95).'</div></td>');//acoperit efectiv
	//add($output,'<td >'.display_select('orar_an_grupa[]',$an_grupa,$hidden_id,$index,$select_grupa).'</td>');//grupa sau subgrupa
	add($output,'<td ><div id="td_19_'.$count.'">'.display_input('orar_an_grupa[]',$an_grupa,$hidden_id,$index,40).'</div></td>');//grupa sau subgrupa
	add($output,'<td ><div id="td_20_'.$count.'">'.display_select('orar_zi[]',$zi,$hidden_id,$index,$select_zi).'</div></td>');//ziua
	add($output,'<td ><div id="td_21_'.$count.'">'.display_select('orar_ora_start[]',$ora_start,$hidden_id,$index,$select_ora).'</div></td>');//ora start
	add($output,'<td ><div id="td_22_'.$count.'">'.display_select('orar_ora_stop[]',$ora_stop,$hidden_id,$index,$select_ora).'</div></td>');//ora stop
	add($output,'<td ><div id="td_23_'.$count.'">'.display_select('orar_sala[]',$sala,$hidden_id,$index,$select_sala).'</div></td>');//sala

	add($output,'</tr>');//inchid randul
	
	return $output;
}

$thead_secr = array('Nr', 'Disciplina', 'Facultate', 'Tip', 'Forma', 'Cod',
					'An', 'Serie', 'Nr Stud', 'Nr Grupa', 'Tip Grupa',
					'Ore curs', 'Ore aplicatii', 'Post', 'Grad', 'Norma',
					'Tip ocupare', 'Acoperit', 'Efectiv', 'Grupa', 'Zi',
					'Ora Start', 'Ora Stop', 'Sala', 'Tip paritate',
					'Inceput paritate');

function write_vert($text)
{
	$val = '<div title="'.$text.'"></div>';
	
	$val = '';
	for($i=0;$i<strlen($text);$i++)
		$val .= $text[$i].'<br>';
	
	return $val;
}

function display_thead_orar_secr($lista_col=null)
{

if($lista_col == null)
	{
	global $thead_secr;
	}
else
	$thead_secr = $lista_col;

$content='';
add($content,'
<table width="99%" border="0" class="special" cellpadding="0" cellspacing="1" >
  <tr align="center" class="tr_head">');
  
/*
<td>
	<div id="td_0_but" onclick="hide(\'td_0\')" class="resize">++</div>
	<div id="td_0_'.$i.'" onclick="hide(\'td_0\')">'.$thead_secr[$i].'</div>
</td>
*/
add($content,
	'<td title="'.$thead_secr[0].'">
	'.$thead_secr[0].'
	</td>');
for($i=1;$i<count($thead_secr);$i++)//adaug coloanele
{
	add($content,
	'<td title="'.$thead_secr[$i].'" class="mouse_res">
	<div id="td_'.$i.'_but" onclick="hide(\'td_'.$i.'\');marcheaza_coloana('.$i.',1);" class="resize">
	<img src="images/arrow.png" width="13px">
	</div><div id="td_'.$i.'_head" onclick="hide(\'td_'.$i.'\');marcheaza_coloana('.$i.',0);">'.$thead_secr[$i].'</div>
	</td>');
}
add($content,'</tr>');
return $content;
}

function display_page_nav_secr($start,$nr_afisari,$nr,$pag)
{
$cont = $_SESSION['tip_cont'];
$content='';
$start1 = ( ($start-$nr_afisari<0)?0:($start-$nr_afisari));
$start2 =( ($start+$nr_afisari<$nr)?($start+$nr_afisari):$start);
if($start!=0)
	$link1 = '<a href="'.$pag.'?start='.$start1.'">Inapoi</a>';
	else $link1 = "Inapoi";
if($start+$nr_afisari<$nr)
	$link2 = '<a href="'.$pag.'?start='.$start2.'">Inainte</a>';
	else $link2 = "Inainte";

$nr_pag = ceil($nr/$nr_afisari);
$afis_pe_pag = $start/$nr_afisari;
$select = '';
add($select,'<form action="" method="post" >
			 <select class="small_select" name="pagina" onchange="this.form.submit();">');
for($i=0;$i<$nr_pag;$i++)
{
	add($select,'<option '.(($afis_pe_pag==$i)?'selected':'').
				' value="'.$pag.'?start='.($i*$nr_afisari).
				'">pag '.($i+1).'</option>');
}
add($select,'</select></form>');

$valori = array(1,5,10,15,20,25,"tot");

$select2 = '';
add($select2,'<form action="" method="post" >
			 <select class="small_select" name="nr_pe_pag" onchange="this.form.submit();">');
for($i=0;$i<sizeof($valori)-1;$i++)
{
	add($select2,'<option '.(($valori[$i]==$nr_afisari)?'selected':'').
				' value="'.$valori[$i].
				'">'.$valori[$i].' / pag</option>');
}
add($select2,'<option '.(($nr==$nr_afisari)?'selected':'').
				' value="tot">tot / pag</option>');
				
add($select2,'</select></form>');

add($content,'<div align="center"><table border="0" cellpadding="0" cellspacing="0"><td ><td valign="top">');
add($content,$link1.'&nbsp;&nbsp;&nbsp;</td><td >'.
			 $select.
			 '</td><td valign="top">&nbsp;&nbsp;&nbsp;'.$link2.'&nbsp;&nbsp;&nbsp;'.
			 '</td><td valign="top">'.$select2.'</tr></table></div>');	
return $content;
}

//afisare pentru profesor

//preia campurile din vectorul $result returnat de mysql_query
//si afiseaza sub forma de tabel
function display_result_prof($result,$index,$count)
{
	global $color;//culorile pentru linii pare sau impare
	global $select_ora,$select_forma,$select_cod,
		   $select_an,$select_serie,$select_tip_grupa,
		   $select_a,$select_c,$select_nr_grupa,
		   $select_grupa,$select_grad,$select_zi,$select_ora,
		   $select_sala,$select_tip_ora;
	
	$output = '';
	
	mysql_data_seek($result,$index);//trec la intrarea cu numarul $index
	list(	  $orar_id, $link_disc, $tip_ora, $forma, $cod, $an, $serie, 
	  $nr_stud, $nr_grupa, $tip_grupa, $modul_c, $modul_a, $post,
	  $grad, $norma, $tip_ocup, $ocupat, $acoperit, $acoperit_efect, $an_grupa, $zi, 
	  $ora, $sala) = mysql_fetch_array($result); 
	  
	 $aux = strstr($ora,"-");
	 $ora_start = substr($ora,0,strlen($ora)-strlen($aux));
	 $ora_stop  = substr($aux,1,strlen($aux)-1);
	
	$hidden_id = 'modificat_'.$index;

	
	//campul care arata daca s-a schimbat ceva in linia curenta
	add($output,'<input type="hidden" name="modificat[]" value="0" id="'.$hidden_id.'">');
	add($output,'<input type="hidden" name="orar_id[]" value="'.$orar_id.'">');
	add($output,'<tr bgcolor="'.$color[$index%2].'">');//deschid un nou rand 
	add($output,'<td class="read_only"><div id="td_0_'.$count.'">'.($index+1).'</div></td>');//scriu numarul liniei curente - needitabil
	add($output,'<td class="read_only"><div id="td_1_'.$count.'">'.$link_disc.'</div></td>');//scriu facultatea din care face parte - needitabil
	add($output,'<td class="read_only"><div id="td_2_'.$count.'">'.$tip_curs1.'</div></td>');//tipul cursului : C sau L
	add($output,'<td class="read_only"><div id="td_3_'.$count.'">'.$materie.'</div></td>');//materia - needitabil
	add($output,'<td class="read_only"><div id="td_4_'.$count.'">'.$forma.'</div></td>');//tipul orei 
	add($output,'<td class="read_only"><div id="td_5_'.$count.'">'.$cod.'</div></td>');//codul asociat
	add($output,'<td class="read_only"><div id="td_6_'.$count.'">'.$an.'</div></td>');//anul
	add($output,'<td class="read_only"><div id="td_7_'.$count.'">'.$serie.'</div></td>');//seria
	add($output,'<td ><div id="td_8_'.$count.'">'.display_input('orar_nr_stud[]',$nr_stud,$hidden_id,$index,40).'</div></td>');//nr studenti
	add($output,'<td class="read_only"><div id="td_9_'.$count.'">'.$nr_grupa.'</div></td>');//nr de grupe
	add($output,'<td class="read_only"><div id="td_10_'.$count.'">'.$tip_grupa.'</div></td>');//tipul grupei
	add($output,'<td class="read_only"><div id="td_11_'.$count.'">'.$modul_c.'</div></td>');//modulul C1
	add($output,'<td class="read_only"><div id="td_12_'.$count.'">'.$modul_a.'</div></td>');//modulul A1
	add($output,'<td class="read_only"><div id="td_13_'.$count.'">'.$post.'</div></td>');//postul
	add($output,'<td class="read_only"><div id="td_14_'.$count.'">'.$grad.'</div></td>');//gradul
	add($output,'<td ><div id="td_15_'.$count.'">'.display_input('orar_norma[]',$norma,$hidden_id,$index,95).'</div></td>');//norma
	add($output,'<td ><div id="td_16_'.$count.'">'.display_input('orar_ocupat[]',$ocupat,$hidden_id,$index,35).'</div></td>');//ocupat po sau B
	
	add($output,'<td ><div id="td_17_'.$count.'">'.display_input('orar_acoperit[]',$acoperit,$hidden_id,$index,95).'</div></td>');//acoperit
	add($output,'<td ><div id="td_18_'.$count.'">' .display_input('orar_acoperit_efect[]',$acoperit_efect,$hidden_id,$index,95).'</div></td>');//acoperit efectiv
	add($output,'<td ><div id="td_119_'.$count.'">'.display_input('orar_an_grupa[]',$an_grupa,$hidden_id,$index,40).'</div></td>');//grupa sau subgrupa
	add($output,'<td ><div id="td_20_'.$count.'">'.display_select('orar_zi[]',$zi,$hidden_id,$index,$select_zi).'</div></td>');//ziua
	add($output,'<td ><div id="td_21_'.$count.'">'.display_select('orar_ora_start[]',$ora_start,$hidden_id,$index,$select_ora).'</div></td>');//ora start
	add($output,'<td ><div id="td_22_'.$count.'">'.display_select('orar_ora_stop[]',$ora_stop,$hidden_id,$index,$select_ora).'</div></td>');//ora stop
	add($output,'<td ><div id="td_23_'.$count.'">'.display_select('orar_sala[]',$sala,$hidden_id,$index,$select_sala).'</div></td>');//sala

	add($output,'</tr>');//inchid randul
	
	return $output;
}
function display_select_materie()
{
	$query = "SELECT * FROM `utilizatori` ORDER BY `tip_cont` ASC";
	$result = mysql_query($query);

	$nr = mysql_num_rows($result);
	
	$select_materie = '<select style="font-size:10pt;" name="materie">';

	for($i=0;$i<$nr;$i++)
	if ( mysql_result($result,$i,'tip_cont') != "A")
	{
		$id = mysql_result($result,$i,'utilizator_id');//tre verificat
		$cat_id =  mysql_result($result,$i,'link_cat');
		$query = "SELECT * FROM `catedre` WHERE `cat_id` = '$cat_id'";
		$cat_result = mysql_query($query);
		$val = mysql_result($cat_result,0,'nume');
		$select_materie .= '<option value="'.$id.'">'.$val.'</option>';	
	}
	$select_materie .= '</select>';

	
	return $select_materie;
}

function display_select_tip()
{
	$result='<select style="font-size:10pt;" name="tip" onChange="CategoryGrab('."'".'get_fac.php?idTip='."'".'+this.value,'."'div_univ'".');">';
	$result .= '<option value="-"> </option>';
	$result .= '<option value="S">Secretara</option>';
	$result .= '<option value="P">Profesor</option>';
	$result .= '<option value="A">Administrator</option>';
	$result .= '</select>';
	
	return $result;
}
	
function criterii_norma()
{
	$content = '';
	
	$query = "SELECT * FROM `ore` GROUP BY `pers_norma` ASC";
	$result = mysql_query($query);

	$nr = mysql_num_rows($result);
	
	$select_an = '<select name="norma" style="font-size:10pt;" >
					<option value="non_select">Alege Persoana Norma</option>';

	for($i=0;$i<$nr;$i++)
	{
		$id = mysql_result($result,$i,'pers_norma');//tre verificat
				$val =  mysql_result($result,$i,'pers_norma');
				$select_an .= '<option value="'.$val.'">'.$val.'</option>';	
	}
	$select_an .='<option value="all">Toti</option>';
	$select_an .= '</select>';
	
	return $select_an;
}

function criterii_serie()
{
	$content = '';
	
	$query = "SELECT * FROM `ore` GROUP BY `serie` ASC";
	$result = mysql_query($query);

	$nr = mysql_num_rows($result);
	
	$select_an = '<select style="font-size:10pt;" name="serie">
					<option value="non_select">Alege Seria</option>';

	for($i=0;$i<$nr;$i++)
	{
		$id = mysql_result($result,$i,'serie');//tre verificat
				$val =  mysql_result($result,$i,'serie');
				$select_an .= '<option value="'.$val.'">'.$val.'</option>';	
	}
	$select_an .='<option value="all">Toate</option>';
	$select_an .= '</select>';
	
	return $select_an;
}

function criterii_an()
{
	$content = '';
	
	$query = "SELECT * FROM `ore` GROUP BY `an` ASC";
	$result = mysql_query($query);

	$nr = mysql_num_rows($result);
	
	$select_an = '<select style="font-size:10pt;" name="an">
					<option value="non_select">Alege Anul</option>';

	for($i=0;$i<$nr;$i++)
	{
		$id = mysql_result($result,$i,'an');//tre verificat
				$val =  mysql_result($result,$i,'an');
				$select_an .= '<option value="'.$val.'">'.$val.'</option>';	
	}
	$select_an .='<option value="all">Toti</option>';
	$select_an .= '</select>';
	
	return $select_an;
}

function criterii_materie()
{
$query = "SELECT * FROM `utilizatori` WHERE `tip_cont`='".PROFESOR."' ORDER BY `tip_cont` ASC";
	$result = mysql_query($query);

	$nr = mysql_num_rows($result);
	
	$select_materie = '<select style="font-size:10pt;" name="materie">
						<option value="non_select">Alege Materia</option>';

	for($i=0;$i<$nr;$i++)
	{
		$id = mysql_result($result,$i,'admin_id');//tre verificat
				$val =  mysql_result($result,$i,'materie');
				$select_materie .= '<option value="'.$val.'">'.$val.'</option>';	
	}
	$select_materie .= '</select>';
	return $select_materie;
}

function criterii_selectie()
{
	$content = '';
	
	add($content,'<div align="center">');
	add($content,'<form method="post" action="">Criterii ');
	add($content,criterii_norma().criterii_serie().criterii_an().criterii_materie());
	add($content,'<input type="submit" name="criterii_selectie" value="Selecteaza"></form></div>');
	
	return $content;
}

function formular_criterii_selectie()
{
	$result = ' WHERE ';
	$flag = 0;
	
	if(isset($_POST['criterii_selectie']))
	{
		foreach($_POST as $index=>$value)
		{	$$index = $value;
		}
		if($norma != 'non_select' && $norma != 'all')
			{
			add($result,'`pers_norma`=\''.$norma.'\'');
			$flag=1;			
			}
		if($serie != 'non_select' && $serie != 'all')
			{
			if($flag == 1)
				add($result,' AND ');
			add($result,'`serie`=\''.$serie.'\'');
			$flag=1;
			}
		if($an != 'non_select' && $an != 'all')
			{
			if($flag == 1)
				add($result,' AND ');
			add($result,'`an`=\''.$an.'\'');
			$flag=1;
			}
		if($materie != 'non_select')
			{
			if($flag == 1)
				add($result,' AND ');
			add($result,'`materie`=\''.$materie.'\'');
			$flag = 1;
			}		
	}	
	else	
		$flag = 'nemodificat';
		
	if($flag == 0)
		$result = '';
	else
	if($flag == 'nemodificat')
		$result = -1;

	return $result;
}

//preia campurile din vectorul $result returnat de mysql_query
//si afiseaza sub forma de tabel
function display_result_read_only($result,$index,$count)
{
	global $color;//culorile pentru linii pare sau impare
	global $select_ora,$select_forma,$select_cod,
		   $select_an,$select_serie,$select_tip_grupa,
		   $select_a,$select_c,$select_nr_grupa,
		   $select_grupa,$select_grad,$select_zi,$select_ora,
		   $select_sala,$select_tip_ora;
	
	$output = '';
					
	mysql_data_seek($result,$index);//trec la intrarea cu numarul $index
	list($orar_id, $link_disc, $tip_ora, $forma, $cod, $an, $serie, 
	  $nr_stud, $nr_grupa, $tip_grupa, $modul_c, $modul_a, $post,
	  $grad, $norma, $tip_ocup, $acoperit, $acoperit_efect, $an_grupa, $zi, 
	  $ora, $sala, $parit, $pari_st) = mysql_fetch_array($result); 
	  
	 $aux = strstr($ora,"-");
	 $ora_start = substr($ora,0,strlen($ora)-strlen($aux));
	 $ora_stop  = substr($aux,1,strlen($aux)-1);
	 $info = get_disc_fac($link_disc);
	add($output,'<tr bgcolor="'.$color[$index%2].'">');//deschid un nou rand 
	add($output,'<td class="read_only">'.($index+1).'</td>');//scriu numarul liniei curente - needitabil
	add($output,'<td class="read_only"><div id="td_3_'.$count.'">'.$info[0].'</div></td>');//materia - needitabil
	add($output,'<td class="read_only"><div id="td_3b_'.$count.'">'.$info[1].'</div></td>');//materia - needitabil
	add($output,'<td class="read_only"><div id="td_2_'.$count.'">'.$tip_ora.'</div></td>');//tipul cursului : C sau L
	add($output,'<td class="read_only"><div id="td_4_'.$count.'">'.$forma.'</div></td>');//tipul orei 
	add($output,'<td class="read_only"><div id="td_5_'.$count.'">'.$cod.'</div></td>');//codul asociat
	add($output,'<td class="read_only"><div id="td_6_'.$count.'">'.$an.'</div></td>');//anul
	add($output,'<td class="read_only"><div id="td_7_'.$count.'">'.$serie.'</div></td>');//seria
	add($output,'<td class="read_only"><div id="td_8_'.$count.'">'.$nr_stud.'</div></td>');//nr studenti
	add($output,'<td class="read_only"><div id="td_9_'.$count.'">'.$nr_grupa.'</div></td>');//nr de grupe
	add($output,'<td class="read_only"><div id="td_10_'.$count.'">'.$tip_grupa.'</div></td>');//tipul grupei
	add($output,'<td class="read_only"><div id="td_11_'.$count.'">'.$modul_c.'</div></td>');//modulul C1
	add($output,'<td class="read_only"><div id="td_12_'.$count.'">'.$modul_a.'</div></td>');//modulul A1
	add($output,'<td class="read_only"><div id="td_13_'.$count.'">'.$post.'</div></td>');//postul
	add($output,'<td class="read_only"><div id="td_14_'.$count.'">'.$grad.'</div></td>');//gradul
	add($output,'<td class="read_only"><div id="td_15_'.$count.'">'.$norma.'</div></td>');
	add($output,'<td class="read_only"><div id="td_16_'.$count.'">'.$tip_ocup.'</div></td>');	
	add($output,'<td class="read_only"><div id="td_17_'.$count.'">'.$acoperit.'</div></td>');
	add($output,'<td class="read_only"><div id="td_18_'.$count.'">'.$acoperit_efect.'</div></td>');
	add($output,'<td class="read_only"><div id="td_19_'.$count.'">'.$an_grupa.'</div></td>');
	add($output,'<td class="read_only"><div id="td_20_'.$count.'">'.$zi.'</div></td>');
	add($output,'<td class="read_only"><div id="td_21_'.$count.'">'.$ora_start.'</div></td>');
	add($output,'<td class="read_only"><div id="td_22_'.$count.'">'.$ora_stop.'</div></td>');
	add($output,'<td class="read_only"><div id="td_23_'.$count.'">'.$sala.'</div></td>');
	add($output,'<td class="read_only"><div id="td_24_'.$count.'">'.$parit.'</div></td>');
	add($output,'<td class="read_only"><div id="td_25_'.$count.'">sapt. '.$pari_st.'</div></td>');

	add($output,'</tr>');//inchid randul
	
	return $output;
}
?>