<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");
include("include/display.php");//functii pentru aratat materiile

//selecteaza inregistrarile din tabela orar
//selecteaza inregistrarile din tabela orar
$opt_selectie = formular_criterii_selectie();

if($opt_selectie != -1)
	{
	$_SESSION['opt_selectie'] = $opt_selectie;
	}
else 
	$_SESSION['opt_selectie'] = '';

$query = "SELECT  `ora_id` , `link_disc` , `tip_ora` , `forma` , `cod` , `an` , `serie` , `nr_stud` , `nr_grupa` , 
					`tip_grupa_aplicatii` , `nr_ore_curs` , `nr_ore_aplicatii`, `nr_post`, `grad_post`,
					`pers_norma` , `tip_ocupare` , `pers_acoperit` , `pers_acoperit_efect` , `an_grupa` ,
					`zi` , `ora` , `sala`, `paritate`, `paritate_start` FROM `ore` ".$_SESSION['opt_selectie']."";

$result_orar = mysql_query($query);
$nr = mysql_num_rows($result_orar);

//check_auth(SECRETARA);//verifica daca este cont de secretara

$layout->get_template('include/template.html');
$layout->replace('TITLE','Formular detalii plata cu ora');

$meniu = show_menu(menu,$_SESSION['tip_cont'],1);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,1);
$layout->replace('SUBMENU',$submeniu);

$content = '';
$mesaj   = '';

if(!isset($_SESSION['nr_afisari_viz']))//seteaza numarul de afisari pe pagina
{
	$_SESSION['nr_afisari_viz'] = 10;
}

if(isset($_POST['salveaza']))//formularul tabelului
{
	foreach($_POST as $index=>$val)
	{
		$$index = $val;
		//add($content,$index.' => '.$val .'<br>');
	}	
	//add($content,'linii modificate <br>');
	foreach($modificat as $index=>$val)//verific campul ascuns modificat
	{		
	if($val == '1')//daca a fost modificata o valoare pe linia curenta
		{
		$ora = $orar_ora_start .'-'.$orar_ora_stop;
		$query = "UPDATE `ore` SET `tip_curs1`='".$orar_tip_ora[$index]."' ,
			 `forma`='".$orar_forma[$index]."' , `cod`='".$orar_cod[$index]."' , `an`='".$orar_an[$index]."',
			 `serie`='".$orar_seria[$index]."' , `nr_stud`='".$orar_nr_stud[$index]."' ,
			 `nr_grupa`='".$orar_nr_grupa[$index]."' , `tip_grupa`='".$orar_tip_grupa[$index]."' , 
			 `modul_c`='".$orar_modul_c[$index]."' , `modul_a`='".$orar_modul_a[$index]."' , 
			 `post`='".$orar_post[$index]."' , `grad`='".$orar_grad[$index]."' , `norma`='".$orar_norma[$index]."' , 
			 `ocupat`='".$orar_ocupat[$index]."' , `acoperit`='".$orar_acoperit[$index]."' ,
			 `acoperit_efect`='".$orar_acoperit_efect[$index]."' , `an_grupa`='".$orar_an_grupa[$index]."',
			 `zi`='".$orar_zi[$index]."' , `ora`='".$ora."' , `sala`='".$orar_sala[$index]."'		
			  WHERE `orar_id`='".$orar_id[$index]."' LIMIT 1";
		$result = mysql_query($query);
		if(!result)
			{
			add($mesaj,'<div class="eroare">Eroare : modificarile nu au fost salvate</div>');
			//break;
			}	
		else if(!$flag)
			{
				$flag = 1;
				add($mesaj,'Modificarile au fost salvate<br>');			
			}
		
		}
		
		//mysql_free_result($result);
	}
}

if(isset($_POST['pagina']))//redirectare la pagina
{
		header("Location: ".$_POST['pagina']);//redirectioneaza catre pagina principala
		die();
}

if(isset($_POST['nr_pe_pag']))//modificare afisari pe pagina
{
		if($_POST['nr_pe_pag'] == "tot")
			$_SESSION['nr_afisari_viz'] = $nr;
		else
			$_SESSION['nr_afisari_viz'] = $_POST['nr_pe_pag'];
		$start = 0;
}

//tratare pagina si parametri
if(isset($_GET['start']) && !isset($_POST['nr_pe_pag']))
{
	$start = $_GET['start'];
	$stop = $start + $_SESSION['nr_afisari_viz'];
	if($stop>$nr)
		$stop = $nr;
}
else
{
	$start = 0;
	$stop = $_SESSION['nr_afisari_viz'];
	if($stop>$nr)
		$stop = $nr;
}	
$nr_afisari = $_SESSION['nr_afisari_viz'];

add($content,'<div class="title" align="center">Formular detalii plata cu ora</div>');
add($content,$mesaj);

if($opt_selectie)
{
$start = 0;
	$stop = $_SESSION['nr_afisari_viz'];
	if($stop>$nr)
		$stop = $nr;
}
		

if($nr != 0)//daca exista inregistrari in tabela `orar`
{
	add($content,display_page_nav_secr($start,$nr_afisari,$nr,"1_orar_viz.php"));//butoanele inainte si inapoi
	
	add($content,display_thead_orar_secr($thead_secr_ro));//scrie capul de tabel

	for($i=$start;$i<$stop;$i++)//pentru  fiecare intrare
	{
		$output = display_result_read_only($result_orar,$i,$i-$start);//scrie linia respectiva sub forma de tabel
		add($content,$output);
	}	
	
	add($content,'</table>');
	add($content,'<input type="hidden" id="nr_inreg" value="'.($stop-$start).'">');
	add($content,'<input type="submit" name="salveaza" value="Salveaza">');
	add($content,'</form>');
	add($content,display_page_nav_secr($start,$nr_afisari,$nr,"1_orar_viz.php"));
	add($content,criterii_selectie());
	
	add($content,'* Coloanele marcate cu acest simbol nu sunt editabile.<br>
		      ** Pentru vizulizarea intregului tabel folositi scroll-ul orizontal sau ascundeti<br>
		      &nbsp;&nbsp;colonele dorite prin efectuarea unui click pe capul de tabel.');
	}
else
	{
	if($opt_selectie != -1)//daca s-au marcat criterii de selectie
		add($content,'Nu s-au gasit inregistrari.');
	else
	add($content,"Fisierul XlS nu a fost importat.");	
	}

	
$layout->replace('CONTENT',$content);

$layout->print_template();

//mysql_free_result($result_orar);
?>


		