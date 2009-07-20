<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");
include("include/display.php");//functii pentru aratat materiile


 function xlsBOF() {
    return pack("ssssss", 0x809, 0x8, 0x0, 0x10, 0x0, 0x0);  
    
}

function xlsEOF() {
    return pack("ss", 0x0A, 0x00);
   
}

function xlsWriteNumber($Row, $Col, $Value) {
    return pack("sssss", 0x203, 14, $Row, $Col, 0x0) . pack("d", $Value);
    
}

function xlsWriteLabel($Row, $Col, $Value ) {
    $L = strlen($Value);
    return pack("ssssss", 0x204, 8 + $L, $Row, $Col, 0x0, $L) . $Value;

} 

//selecteaza inregistrarile din tabela orar
$query = "SELECT * FROM `ore`";
$result_orar = mysql_query($query);
$nr = mysql_num_rows($result_orar);//numarul de intrari


check_auth(SECRETARA);//verifica daca este cont de secretara

$layout->get_template('include/template.html');
$layout->replace('TITLE','Formular detalii plata cu ora');

$meniu = show_menu(menu,$_SESSION['tip_cont'],1);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,1);
$layout->replace('SUBMENU',$submeniu);

$content = '';
$mesaj   = '';

$aux = '';

/*
if(!isset($_SESSION['nr_afisari']))//seteaza numarul de afisari pe pagina
{
	$_SESSION['nr_afisari'] = 10;
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
			break;
			}	
		else if(!$flag)
			{
				$flag = 1;
				add($mesaj,'Modificarile au fost salvate<br>');			
			}		
		}
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
			$_SESSION['nr_afisari'] = $nr;
		else
			$_SESSION['nr_afisari'] = $_POST['nr_pe_pag'];
		$start = 0;
}

//tratare pagina si parametri
if(isset($_GET['start']) && !isset($_POST['nr_pe_pag']))
{
	$start = $_GET['start'];
	$stop = $start + $_SESSION['nr_afisari'];
	if($stop>$nr)
		$stop = $nr;
}
else
{
	$start = 0;
	$stop = $_SESSION['nr_afisari'];
	if($stop>$nr)
		$stop = $nr;
}	
$nr_afisari = $_SESSION['nr_afisari'];

add($content,'<div class="title" align="center">Formular detalii plata cu ora</div>');
add($content,$mesaj);
*
//selecteaza inregistrarile din tabela orar
$query = "SELECT * FROM `ore`";
$result_orar = mysql_query($query);
	

		
if($nr != 0)//daca exista inregistrari in tabela `orar`
{
	*/
	
	$query = "SELECT * FROM `ore`";
$result = mysql_query($query);
	
	$f = fopen("test.xls","w");	
	
	$aux = xlsBOF();
	
	for($i=0;$i<$nr;$i++)//pentru  fiecare intrare
	{
		//$output = display_result_secr($result_orar,$i);//scrie linia respectiva sub forma de tabel
		//add($content,$output);
		
		mysql_data_seek($result,$i);//trec la intrarea cu numarul $index
	$x = list($orar_id, $facultate, $tip_curs1, $materie, $forma, $cod, $an, $serie, 
	  $nr_stud, $nr_grupa, $tip_grupa, $modul_c, $modul_a, $tip_curs2, $post,
	  $grad, $norma, $ocupat, $acoperit, $acoperit_efect, $an_grupa, $zi, 
	  $ora, $sala) = mysql_fetch_array($result); 
	  
	  for($j=0;$j<count($x);$j++)
                $aux .= xlsWriteLabel($i,$j,$x[$j]);
                
	  for($j=0;$j<count($x);$j++)
	  	;//echo $x[$j].'<br>';

		
	}
	$aux .= xlsEOF();
	fwrite($f, $aux);
	fclose($f);

$content = '';
add($content,'<script language="text/javascript">alert("sss");</script>');

$layout->replace('CONTENT',$content);

$layout->print_template();
?>

