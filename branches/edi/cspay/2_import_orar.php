<?php
//include headers
include("include/config.php");
include("include/header.php");
include("include/check_login.php");
include("include/display.php");

check_auth(ADMINISTRATOR);//verifica daca este cont de administrator

//verificare formulare
if(isset($_POST['populeaza']))
{	
	$tmp_name = $_FILES['xls']['tmp_name'];//fisierul temporar
	move_uploaded_file($tmp_name,"temp/fisier.xls");
	$populeaza = true;
}
		
$layout->get_template('include/template.html');
$layout->replace('TITLE','Administrare');

$meniu = show_menu(menu,$_SESSION['tip_cont'],1);
$layout->replace('MENU',$meniu);

$submeniu = show_menu(submenu,$_SESSION['tip_cont'],0,1);
$layout->replace('SUBMENU',$submeniu);

$content = "";
add($content,'<div class="title" align="center">Importare date "orar"</div>');
add($content,'<br>');

$form = '<form action="" method="post" enctype="multipart/form-data">';
add($form,'  <div class="eroare">Toate datele trebuie sa se gaseasca in "Sheet1" in cadrul fisierului
	   transmis ca parametru.
           <br>Vor fi importate datele incepand cu a treia linie inclusiv.<br>
	   Campurile importate vor fi in urmatoarea ordine :<br> 
	facultate, tip_curs1, materie, forma, cod, an, serie, nr_stud, nr_grupa, 
      tip_grupa, modul_c, modul_a, tip_curs2,post,grad<br>
	   Dupa importare conturile pentru fiecare materie vor fi create automat.
      <br>Acestea pot fi modificate in cadrul sectiunii <i>Conturi->asociere materie</i><br><br></div>
	Fisierul XLS : <br>   
	<input name="xls" type="file"><br>
	   <input type="submit" name="populeaza" value="Incarca">
	   </form>');
	   
if(isset($populeaza))
{
#########################################################
##popularea bazei de date

require_once 'convert/excel/reader.php';
$data = new Spreadsheet_Excel_Reader();
$data->setOutputEncoding('CP1251');
$data->read('temp/file.xls');


//Numele Coloanelor in ordinea din fisierul Excel si a db MySql
		$NumColumn="facultate, tip_curs1, materie, forma, cod, an, serie, nr_stud, nr_grupa, 
  tip_grupa, modul_c, modul_a, tip_curs2,post,grad";

#stergerea tabelelor
$query = "TRUNCATE TABLE `orar`";
$result = mysql_query($query);
add($mesaj,'Tabela orar a fost stearsa.<br>');
$query = "UPDATE `asociatie` SET `link_admin`='-1'";
$result = mysql_query($query);
add($mesaj,'Tabela asociatie a fost stearsa.<br>');
$query = "DELETE FROM `admin` WHERE `tip_cont`='".PROFESOR."'";
$result = mysql_query($query);
add($mesaj,'Conturile de administrator au fost sterse.<br>');

$cond = true;
$i=3;

$query = "INSERT INTO `orar` (".$NumColumn.") VALUES";

While($cond)
{
	add($query,'(');
	for($j=1;$j<=15;$j++)
	{
		$val = $data->get_cell($i,$j);
		add($query,"'".$val."',");	
	}
	
	$query = substr($query,0,strlen($query)-1);
	add($query,'),');
	
	$i++;
	$val = $data->get_cell($i,1);
	if( empty( $val ) )
		$cond = false;
}
$query = substr($query,0,strlen($query)-1);

$result = mysql_query($query);
if($result)
	add($mesaj,'<br>Fisierul XLS a fost parcurs iar inregistrarile au fost adaugate cu succes.<br>');
else
	add($mesaj,'<div class="eroara">Nu s-a populat.<br>');

add($content,$mesaj);
########################################################

	   
//adaugare conturi dupa ce am adaugat inregistrarile in tabela orar
$query = "SELECT * FROM `orar` WHERE `link_univ`='".$_SESSION['univ_id']."' GROUP BY `materie`";
$result = mysql_query($query);
$nr = mysql_num_rows($result);

if($nr)
{
$query = "INSERT INTO `admin` (`materie`,`utilizator`,`parola`,`tip_cont`,`link_univ`) VALUES";
for($i=0;$i<$nr;$i++)
{
	$materie = mysql_result($result,$i,'materie');
	$id = mysql_result($result,$i,'orar_id');
	
	if($i != 0)
		add($query,',');
	add($query,"('".$materie."','prof_".$i."','root','1','".$_SESSION['univ_id']."')");
}

$res_adauga = mysql_query($query);
if($res_adauga)
	{
	add($content,'Conturile au fost create.<br><br>');
	}
else 
	echo mysql_error($res_adauga);
}
}

add($content,$form);

$layout->replace('CONTENT',$content);

$layout->print_template();

?>

