<?php
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
	$query = "SELECT `nume_scurt` FROM `facultate` WHERE `link_univ`='".$univ_id."'";
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

//An  	Serie  	Nr Studenti  	Nr Grupe  	Tip Grupa  	C1  	A1  	Post  	Grad  	Norma  	Ocupat  	Acoperit  	Efectiv  	An/Gr  	Zi  	Ora  	Sala
$select_tip_ora = array("C","L");//tipul de ora
$select_forma = array("IZ","CZ");//forma de invatamant
$select_cod = array("LR");//codul asociat materiei sau facultatii ????
$select_an = array(1,2,3,4,5);//anul asociat materiei
$select_serie = array("CA","CB","CC","C1","C2","C3","A1","A2","A3");//seria la care se preda
//nr grupa - editabil
$select_nr_grupa = array(1,2,3,4,5,6,7,8,9,10);//nr de grupe
$select_tip_grupa = array("g","sg");//tipul grupei : grupa sau semigrupa
$select_a = array(1,2,3,4,5,6,7,8,9,10);
$select_c = $select_a;
//post - editabil
$select_grad = array("1p","2c","3s","4a");
//norma - editabil 
//ocupat po sau B ????
//acoperit
//efectiv
$select_grupa = array("serie","311","312","313","314","315","311a","312a","313a","314a","315a"
						,"311b","312b","313b","314b","315b");
$select_zi = array("Lu","Ma","Mi","Jo","Vi");
$select_ora = array(8,9,10,11,12,13,14,15,16,17,18,19,20);
$select_sala = array("EC105","EG405","EG106","EG407","ED011","EC004","EG306",
					 "EC002","ED011");


//preia campurile din vectorul $result returnat de mysql_query
//si afiseaza sub forma de tabel
function display_result_secr($result,$index)
{
	
	$hidden_id = 'modificat_'.$index;
	global $color;//culorile pentru linii pare sau impare
	global $select_ora,$select_forma,$select_cod,
		   $select_an,$select_serie,$select_tip_grupa,
		   $select_a,$select_c,$select_nr_grupa,
		   $select_grupa,$select_grad,$select_zi,$select_ora,
		   $select_sala,$select_tip_ora;
	
	$output = '';
	
	mysql_data_seek($result,$index);//trec la intrarea cu numarul $index
	$valori = list($orar_id,$link_univ, $facultate, $tip_curs1, $materie, $forma, $cod, $an, $serie, 
	  $nr_stud, $nr_grupa, $tip_grupa, $modul_c, $modul_a, $tip_curs2, $post,
	  $grad, $norma, $ocupat, $acoperit, $acoperit_efect, $an_grupa, $zi, 
	  $ora, $sala) = mysql_fetch_array($result); 
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
	add($output,'<td class="read_only">'.$facultate.'</td>');//scriu facultatea din care face parte - needitabil
	add($output,'<td>'.display_select('orar_tip_ora[]',$tip_curs1,$hidden_id,$index,$select_tip_ora).
				'</td>');//tipul cursului : C sau L
	add($output,'<td class="read_only">'.$materie.'</td>');//materia - needitabil
	add($output,'<td>'.display_select('orar_forma[]',$forma,$hidden_id,$index,$select_forma).'</td>');//tipul orei 
	add($output,'<td>'.display_select('orar_cod[]',$cod,$hidden_id,$index,$select_cod).'</td>');//codul asociat
	
	add($output,'<td>'.display_select('orar_an[]',$an,$hidden_id,$index,$select_an).'</td>');//anul
	add($output,'<td>'.display_select('orar_seria[]',$serie,$hidden_id,$index,$select_serie).'</td>');//seria
	add($output,'<td>'.display_input('orar_nr_stud[]',$nr_stud,$hidden_id,$index,40).'</td>');//nr studenti
	add($output,'<td>'.display_select('orar_nr_grupa[]',$nr_grupa,$hidden_id,$index,$select_nr_grupa).'</td>');//nr de grupe
	add($output,'<td>'.display_select('orar_tip_grupa[]',$tip_grupa,$hidden_id,$index,$select_tip_grupa).'</td>');//tipul grupei
	add($output,'<td>'.display_select('orar_modul_c[]',$modul_c,$hidden_id,$index,$select_c).'</td>');//modulul C1
	add($output,'<td>'.display_select('orar_modul_a[]',$modul_a,$hidden_id,$index,$select_a).'</td>');//modulul A1
	
	add($output,'<td>'.display_input('orar_post[]',$post,$hidden_id,$index,30).'</td>');//postul
	add($output,'<td>'.display_select('orar_grad[]',$grad,$hidden_id,$index,$select_grad).'</td>');//gradul
	add($output,'<td>'.display_input('orar_norma[]',$norma,$hidden_id,$index,95).'</td>');//norma
	add($output,'<td>'.display_input('orar_ocupat[]',$ocupat,$hidden_id,$index,35).'</td>');//ocupat po sau B
	
	add($output,'<td>'.display_input('orar_acoperit[]',$acoperit,$hidden_id,$index,95).'</td>');//acoperit
	add($output,'<td>'.display_input('orar_acoperit_efect[]',$acoperit_efect,$hidden_id,$index,95).'</td>');//acoperit efectiv
	//add($output,'<td>'.display_select('orar_an_grupa[]',$an_grupa,$hidden_id,$index,$select_grupa).'</td>');//grupa sau subgrupa
	add($output,'<td>'.display_input('orar_an_grupa[]',$an_grupa,$hidden_id,$index,40).'</td>');//grupa sau subgrupa
	add($output,'<td>'.display_select('orar_zi[]',$zi,$hidden_id,$index,$select_zi).'</td>');//ziua
	add($output,'<td>'.display_select('orar_ora_start[]',$ora_start,$hidden_id,$index,$select_ora).'</td>');//ora start
	add($output,'<td>'.display_select('orar_ora_stop[]',$ora_stop,$hidden_id,$index,$select_ora).'</td>');//ora stop
	add($output,'<td>'.display_select('orar_sala[]',$sala,$hidden_id,$index,$select_sala).'</td>');//sala

	add($output,'</tr>');//inchid randul
	
	return $output;
}

function display_thead_orar_secr()
{
$content='';
add($content,'
<table width="99%" border="0" class="special" cellpadding="0" cellspacing="1" >
  <tr align="center" class="tr_head">
    <td>Nr*</td>
	<td width="50px">Facultate*</td>
    <td>Tip</td>
    <td width="100px">Disciplina*</td>
    <td>Forma</td>
    <td>Cod</td>
    <td>An</td>
    <td>Serie</td>
    <td>Nr Stud</td>
    <td>Nr Grupe</td>
    <td>Tip Grupa</td>
    <td>C1</td>
    <td>A1</td>
    <td>Post</td>
    <td>Grad</td>
    <td>Norma</td>
    <td>Ocupat</td>
    <td>Acoperit</td>
    <td>Efectiv</td>
    <td>Grupa</td>
    <td>Zi</td>
    <td>Ora Start</td>
    <td>Ora Stop</td>
    <td>Sala</td>
    </tr>');
return $content;
}

function display_page_nav_secr($start,$nr_afisari,$nr)
{
$cont = $_SESSION['tip_cont'];
$content='';
$start1 = ( ($start-$nr_afisari<0)?0:($start-$nr_afisari));
$start2 =( ($start+$nr_afisari<$nr)?($start+$nr_afisari):$start);
if($start!=0)
	$link1 = '<a href="'.$cont.'_orar_viz.php?start='.$start1.'">Inapoi</a>';
	else $link1 = "Inapoi";
if($start+$nr_afisari<$nr)
	$link2 = '<a href="'.$cont.'_orar_viz.php?start='.$start2.'">Inainte</a>';
	else $link2 = "Inainte";

$nr_pag = ceil($nr/$nr_afisari);
$pag = $start/$nr_afisari;
$select = '';
add($select,'<form action="" method="post" >
			 <select class="small_select" name="pagina" onchange="this.form.submit();">');
for($i=0;$i<$nr_pag;$i++)
{
	add($select,'<option '.(($pag==$i)?'selected':'').
				' value="'.$cont.'_orar_viz.php?start='.($i*$nr_afisari).
				'">pag '.($i+1).'</option>');
}
add($select,'</select></form>');

$valori = array(5,10,15,20,25,"tot");

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

add($content,'<div align="center"><table border="0" cellpadding="0" cellspacing="0"><td><td valign="middle">');
add($content,$link1.'&nbsp;&nbsp;&nbsp;</td><td>'.
			 $select.
			 '</td><td valign="middle">&nbsp;&nbsp;&nbsp;'.$link2.'&nbsp;&nbsp;&nbsp;'.
			 '</td><td valign="top">'.$select2.'</tr></table></div>');	
return $content;
}

//afisare pentru profesor

//preia campurile din vectorul $result returnat de mysql_query
//si afiseaza sub forma de tabel
function display_result_prof($result,$index)
{
	global $color;//culorile pentru linii pare sau impare
	global $select_ora,$select_forma,$select_cod,
		   $select_an,$select_serie,$select_tip_grupa,
		   $select_a,$select_c,$select_nr_grupa,
		   $select_grupa,$select_grad,$select_zi,$select_ora,
		   $select_sala,$select_tip_ora;
	
	$output = '';
	
	mysql_data_seek($result,$index);//trec la intrarea cu numarul $index
	list($orar_id,$link_univ,  $facultate, $tip_curs1, $materie, $forma, $cod, $an, $serie, 
	  $nr_stud, $nr_grupa, $tip_grupa, $modul_c, $modul_a, $tip_curs2, $post,
	  $grad, $norma, $ocupat, $acoperit, $acoperit_efect, $an_grupa, $zi, 
	  $ora, $sala) = mysql_fetch_array($result); 
	  
	 $aux = strstr($ora,"-");
	 $ora_start = substr($ora,0,strlen($ora)-strlen($aux));
	 $ora_stop  = substr($aux,1,strlen($aux)-1);
	
	$hidden_id = 'modificat_'.$index;

	
	//campul care arata daca s-a schimbat ceva in linia curenta
	add($output,'<input type="hidden" name="modificat[]" value="0" id="'.$hidden_id.'">');
	add($output,'<input type="hidden" name="orar_id[]" value="'.$orar_id.'">');
	add($output,'<tr bgcolor="'.$color[$index%2].'">');//deschid un nou rand 
	add($output,'<td class="read_only">'.($index+1).'</td>');//scriu numarul liniei curente - needitabil
	add($output,'<td class="read_only">'.$facultate.'</td>');//scriu facultatea din care face parte - needitabil
	//add($output,'<td>'.display_select('orar_tip_ora[]',$tip_curs1,$hidden_id,$index,$select_tip_ora).
		//		'</td>');//tipul cursului : C sau L
		add($output,'<td class="read_only">'.$tip_curs1.'</td>');//tipul cursului : C sau L
	add($output,'<td class="read_only">'.$materie.'</td>');//materia - needitabil
	//add($output,'<td>'.display_select('orar_forma[]',$forma,$hidden_id,$index,$select_forma).'</td>');//tipul orei 
		add($output,'<td class="read_only">'.$forma.'</td>');//tipul orei 
	//add($output,'<td>'.display_select('orar_cod[]',$cod,$hidden_id,$index,$select_cod).'</td>');//codul asociat
		add($output,'<td class="read_only">'.$cod.'</td>');//codul asociat
		
	//add($output,'<td>'.display_select('orar_an[]',$an,$hidden_id,$index,$select_an).'</td>');//anul
		add($output,'<td class="read_only">'.$an.'</td>');//anul
	//add($output,'<td>'.display_select('orar_seria[]',$serie,$hidden_id,$index,$select_serie).'</td>');//seria
		add($output,'<td class="read_only">'.$serie.'</td>');//seria
	add($output,'<td>'.display_input('orar_nr_stud[]',$nr_stud,$hidden_id,$index,40).'</td>');//nr studenti
	//add($output,'<td>'.display_select('orar_nr_grupa[]',$nr_grupa,$hidden_id,$index,$select_nr_grupa).'</td>');//nr de grupe
		add($output,'<td class="read_only">'.$nr_grupa.'</td>');//nr de grupe
	//add($output,'<td>'.display_select('orar_tip_grupa[]',$tip_grupa,$hidden_id,$index,$select_tip_grupa).'</td>');//tipul grupei
		add($output,'<td class="read_only">'.$tip_grupa.'</td>');//tipul grupei
	//add($output,'<td>'.display_select('orar_modul_c[]',$modul_c,$hidden_id,$index,$select_c).'</td>');//modulul C1
		add($output,'<td class="read_only">'.$modul_c.'</td>');//modulul C1
	//add($output,'<td>'.display_select('orar_modul_a[]',$modul_a,$hidden_id,$index,$select_a).'</td>');//modulul A1
		add($output,'<td class="read_only">'.$modul_a.'</td>');//modulul A1
	
	//add($output,'<td>'.display_input('orar_post[]',$post,$hidden_id,$index,30).'</td>');//postul
		add($output,'<td class="read_only">'.$post.'</td>');//postul
	//add($output,'<td>'.display_select('orar_grad[]',$grad,$hidden_id,$index,$select_grad).'</td>');//gradul
		add($output,'<td class="read_only">'.$grad.'</td>');//gradul
	add($output,'<td>'.display_input('orar_norma[]',$norma,$hidden_id,$index,95).'</td>');//norma
	add($output,'<td>'.display_input('orar_ocupat[]',$ocupat,$hidden_id,$index,35).'</td>');//ocupat po sau B
	
	add($output,'<td>'.display_input('orar_acoperit[]',$acoperit,$hidden_id,$index,95).'</td>');//acoperit
	add($output,'<td>'.display_input('orar_acoperit_efect[]',$acoperit_efect,$hidden_id,$index,95).'</td>');//acoperit efectiv
	////add($output,'<td>'.display_select('orar_an_grupa[]',$an_grupa,$hidden_id,$index,$select_grupa).'</td>');//grupa sau subgrupa
	add($output,'<td>'.display_input('orar_an_grupa[]',$an_grupa,$hidden_id,$index,40).'</td>');//grupa sau subgrupa
	add($output,'<td>'.display_select('orar_zi[]',$zi,$hidden_id,$index,$select_zi).'</td>');//ziua
	add($output,'<td>'.display_select('orar_ora_start[]',$ora_start,$hidden_id,$index,$select_ora).'</td>');//ora start
	add($output,'<td>'.display_select('orar_ora_stop[]',$ora_stop,$hidden_id,$index,$select_ora).'</td>');//ora stop
	add($output,'<td>'.display_select('orar_sala[]',$sala,$hidden_id,$index,$select_sala).'</td>');//sala

	add($output,'</tr>');//inchid randul
	
	return $output;
}

function display_thead_orar_prof()
{
$content='';
add($content,'<form>
<table width="100%" border="0" cellpadding="1" cellspacing="1" style="border:1px solid #CCCCCC;">
  <tr align="center" bgcolor="#ece9d8">
     <td>Nr*</td>
	<td>Tip*</td>
    <td>An*</td>
    <td>Serie*</td>
    <td>Tip Grupa*</td>
    <td>Norma</td>
    <td>Ocupat</td>
    <td>Acoperit</td>
    <td>Efectiv</td>
    <td>Grupa</td>
    <td>Zi</td>
    <td>Ora </td>
    <td>Sala</td>
    </tr>');

return $content;
}

function display_thead_orar_prof2()
{
$content='';
add($content,'
<table width="100%" border="0" class="special" cellpadding="1" cellspacing="1" >
  <tr align="center" class="tr_head">
    <td>Nr*</td>
	<td width="50px">Facultate*</td>
    <td>Tip*</td>
    <td width="100px">Disciplina*</td>
    <td>Forma*</td>
    <td>Cod*</td>
    <td>An*</td>
    <td>Serie*</td>
    <td>Nr Stud</td>
    <td>Nr Grupe*</td>
    <td>Tip Grupa*</td>
    <td>C1*</td>
    <td>A1*</td>
    <td>Post*</td>
    <td>Grad*</td>
    <td>Norma</td>
    <td>Ocupat</td>
    <td>Acoperit</td>
    <td>Efectiv</td>
    <td>Grupa</td>
    <td>Zi</td>
    <td>Ora Start</td>
    <td>Ora Stop</td>
    <td>Sala</td>
    </tr>');
return $content;
}

function display_select_materie()
{
	$query = "SELECT * FROM `admin` WHERE `link_univ`='".$_SESSION['univ_id']."' ORDER BY `tip_cont` ASC";
	$result = mysql_query($query);

	$nr = mysql_num_rows($result);
	
	$select_materie = '<select name="materie">';

	for($i=0;$i<$nr;$i++)
	{
		$id = mysql_result($result,$i,'admin_id');//tre verificat
				$val =  mysql_result($result,$i,'materie');
				$select_materie .= '<option value="'.$id.'">'.$val.'</option>';	
	}
	$select_materie .= '</select>';
	
	mysql_free_result($result);
	
	return $select_materie;
}

?>