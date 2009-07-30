<?php
//Eduard Tuþescu & Daniel Urdã
//July 2008 - July 2009
include('conf_select.php');

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

$thead_secr = array('Nr', 'Disciplina', 'Facultate', 'Tip', 'Forma', 'Cod',
					'An', 'Serie', 'Nr Stud', 'Nr Grup&#259;', 'Tip Grup&#259;',
					'Ore curs', 'Ore aplica&#x21B;ii', 'Post', 'Grad', 'Norma',
					'Tip ocupare', 'Acoperit', 'Efectiv', 'Grupa', 'Zi',
					'Ora Start', 'Ora Stop', 'Sala', 'Tip paritate',
					'Început paritate');

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
	add($content,'<table width="99%" border="0" class="special" cellpadding="0" cellspacing="1" >
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
	else
		$link2 = "Inainte";

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

	add($content,'<div align="center"><table border="0" cellpadding="0" cellspacing="0">
					<td ><td valign="top">');
	add($content,$link1.'&nbsp;&nbsp;&nbsp;</td><td >'.
				$select.
				'</td><td valign="top">&nbsp;&nbsp;&nbsp;'.$link2.'&nbsp;&nbsp;&nbsp;'.
				'</td><td valign="top">'.$select2.'</tr></table></div>');	
	return $content;
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
	$result='<select style="font-size:10pt;" name="tip"
			onChange="CategoryGrab('."'".'get_fac.php?idTip='."'".'+this.value,'."'div_univ'".');">';
	$result .= '<option value="-"> </option>';
	$result .= '<option value="S">Secretara</option>';
	$result .= '<option value="P">Profesor</option>';
	$result .= '<option value="A">Administrator</option>';
	$result .= '</select>';
	
	return $result;
	}

function display_select_fac($univ, $facid)
	{
	$changeprone ="";
	if($facid && $univ)
		$changeprone = 'onChange="CategoryGrab('."'".'get_fac.php?idDisc=-1,'."'div_disc'".');
								  CategoryGrab('."'get_fac.php?idCat='+this.value,"."'div_cat'".');"';
	$fac ='<div id="div_fac"><select style="font-size:10pt;width: 100px;" name="facultate" '.$changeprone.' >';
	$fac .='<option value="0">&nbsp;</option>';
	if($univ && $facid)
		{
		$query = "SELECT * FROM facultati WHERE link_univ=".$univ;
		$result = mysql_query($query);
		$nr = mysql_num_rows($result);
		
		if($nr)
			for($i=0;$i<$nr;$i++)
				{
				if ($facid == mysql_result($result,$i,'fac_id'))
					$sel = "selected";
				else
					$sel = "";
				$fac .='<option '.$sel.' value='.mysql_result($result,$i,'fac_id').'>'.
							mysql_result($result,$i,'nume_scurt')
							.'</option>';
				}
		}
	$fac .= '</select></div>';
	return $fac;
	}
		
function display_select_cat($facid, $cat)
	{
	$changeprone ="";
	if($facid && $cat)
		$changeprone = 'onChange="CategoryGrab('."'get_fac.php?idDisc='+this.value,"."'div_disc'".');"';
	
	$fac ='<div id="div_cat"><select name="catedra" style="font-size:10pt;width: 100px;" '.$changeprone.' >';
	$fac .='<option value="0">&nbsp;</option>';
	if($facid && $cat)
		{
		$query = "SELECT * FROM catedre WHERE link_fac=".$facid;
		$result = mysql_query($query);
		$nr = mysql_num_rows($result);
		
		if($nr)
			for($i=0;$i<$nr;$i++)
				{
				if ($cat == mysql_result($result,$i,'cat_id'))
					$sel = "selected";
				else
					$sel = "";
				$fac .='<option '.$sel.' value='.mysql_result($result,$i,'cat_id').'>'.
							mysql_result($result,$i,'nume')
							.'</option>';
				}
		}	
	$fac .= '</select></div>';
	return $fac;
	}
	
function display_select_disc($cat, $disc)
	{
	
	$fac ='<div id="div_disc"><select style="font-size:10pt;width: 100px;" name="disciplina" >';
	$fac .='<option value="0">&nbsp;</option>';
	if($disc && $cat)
		{
		$query = "SELECT * FROM discipline WHERE link_cat=".$cat." ORDER BY nume_scurt";
		$result = mysql_query($query);
		$nr = mysql_num_rows($result);
		
		if($nr)
			for($i=0;$i<$nr;$i++)
				{
				if ($disc == mysql_result($result,$i,'disc_id'))
					$sel = "selected";
				else
					$sel = "";
				$fac .='<option '.$sel.' value='.mysql_result($result,$i,'disc_id').'>'.
							mysql_result($result,$i,'nume_scurt')
							.'</option>';
				}
		}	
	$fac .= '</select></div>';
	return $fac;
	}	
	
function display_select_herarch($level, $an_id)
	{
		if ($an_id)
			{
			if($level>2)
				{
				$query = "SELECT * FROM discipline WHERE disc_id=".$an_id;
				$result = mysql_query($query);
				$disc_id = $an_id;
				$cat_id = mysql_result($result,0,'link_cat');
				}
			else
				$cat_id = $an_id;

			if($level>1)
				{
				$query = "SELECT * FROM catedre WHERE cat_id=".$cat_id;
				$result = mysql_query($query);
				$fac_id = mysql_result($result,0,'link_fac');
				}
			else
				$fac_id = $cat_id;

			if($level>0)
				{
				$query = "SELECT * FROM facultati WHERE fac_id=".$fac_id;
				$result = mysql_query($query);
				$univ_id = mysql_result($result,0,'link_univ');
				}
			else
				$univ_id = 0;

			}				
		
		$result = mysql_query("SELECT * FROM universitati");
		$nr = mysql_num_rows($result);
		$univ ='<td>Universitatea:</td><td>';
		$univ .= '<select style="font-size:10pt;" name="universitate" 
				   onChange="CategoryGrab('."'".'get_fac.php?idDisc=-1,'."'div_disc'".');
							 CategoryGrab('."'".'get_fac.php?idCat=-1,'."'div_cat'".');
							 CategoryGrab('."'".'get_fac.php?idFac='."'".'+this.value,'."'div_fac'".');
							">';
		$univ .= '<option value="0">&nbsp;</option>';
		for ($i = 0; $i < $nr; $i++)
			{
			if($univ_id == mysql_result($result,$i,'univ_id'))
				$sel = "selected";
			else
				$sel = "";
			$univ .= "<option ".$sel." value=".'"'.mysql_result($result,$i,'univ_id').'" >'.
						mysql_result($result,$i,'nume')."</option></select></td>";
			}
			
		if(! $an_id)
			$sel_fac = display_select_fac(0,0);
		else
			$sel_fac = display_select_fac($univ_id, $fac_id);
		$univ .= '<br><td>Facultatea:</td><td>'.$sel_fac.'</td>';

		if($level > 2)
			{
			if(! $an_id)
				$sel_cat = display_select_cat(0,0);
			else
				$sel_cat = display_select_cat($fac_id, $cat_id);
			$univ .= '<br><td>Catedra:</td><td>'.$sel_cat.'</td>';
			}
			
		if($level > 3)
			{
			if(! $an_id)
				$sel_disc = display_select_disc(0,0);
			else
				$sel_disc = display_select_disc($cat_id, $disc_id);
			$univ .= '<br><td>Disciplina:</td><td>'.$sel_disc.'</td>';
			}
				
		return $univ;
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
			{
			$$index = $value;
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
	elseif($flag == 'nemodificat')
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
	add($output,'<td class="read_only"><div id="td_25_'.$count.'">s&#259;pt. '.$pari_st.'</div></td>');
	add($output,'</tr>');//inchid randul
	
	return $output;
}
?>