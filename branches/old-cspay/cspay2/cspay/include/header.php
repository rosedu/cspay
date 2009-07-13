<?
define("SECRETARA",0);
define("PROFESOR",1);
define("ADMINISTRATOR",2);

$color = array(0 => "#f6f2d8",1 => "#ede7c2");

function check_auth($tip)
{
	if($_SESSION['tip_cont'] != $tip)
		{
			header("Location: eroare.php");//redirectioneaza catre pagina principala
			die();
		}
	return 0;
}

class TEMPLATE  //template class
{
   var $template="";
   
   function get_template($filename) //citeste din fisier template-ul transmis ca parametru
   {
      $this->template = file_get_contents($filename);
   }
   
   function replace($var, $content) //inlocuieste in $template
   {
      $var = '{'.$var.'}';
	  $this->template = str_replace($var, $content, $this->template);
   }
   
   function print_template() //afiseaza fisierul
   {
      /*eval("?>".$this->template."<?");*/
	  echo $this->template;
   }
}

$layout = new TEMPLATE;

//vector de luni
$luna = array(0=>"Ianuarie",1=>"Februarie",2=>"Martie",3=>"Aprilie",4=>"Mai",5=>"Iunie",6=>"Iulie",
			  7=>"August",8=>"Septembrie",9=>"Octombrie",10=>"Noiembrie",11=>"Decembrie");

function add(&$content, $var) //concateneaza continutul variabilelor
{
	$content .= $var;
	
	return $var;
}

function equal($a,$b,$var)
{
	if($a == $b)
		return $var;
}

define("menu",0);
define("submenu",1);

function show_menu($tip_meniu,$tip_cont,$select,$cat=0)//MENU sau SUBMENU, 0,1,2 pt secretara, prof si admin; optiunea selectata
{
	$menu = array(3);
	$submenu = array(3);
	$var = '';
	for($i=0;$i<3;$i++)
		$submenu[$i] = array(8);
//meniu pentru fiecare tip de cont 
	$menu[0] = 	'
		<ul>
			<li'.equal(0,$select,' class="active"').'><a href="0_calendar.php">Calendar</a></li>
			<li'.equal(1,$select,' class="active"').'><a href="0_orar.php">Orar</a></li>
			<li'.equal(2,$select,' class="active"').'><a href="0_asistenta.php">Asistenta</a></li>
			<li'.equal(3,$select,' class="active"').'><a href="logout.php">Deconectare</a></li>
		</ul>
		';
	$menu[1] = 	'
		<ul>
			<li'.equal(0,$select,' class="active"').'><a href="1_vacanta.php">Vacante</a></li>
			<li'.equal(1,$select,' class="active"').'><a href="1_orar_viz.php">Orar</a></li>
			<li'.equal(2,$select,' class="active"').'><a href="1_asistenta.php">Asistenta</a></li>
			<li'.equal(3,$select,' class="active"').'><a href="logout.php">Deconectare</a></li>
		</ul>
		';
	$menu[2] = 	'
		<ul>
			<li'.equal(0,$select,' class="active"').'><a href="2_calendar.php">Conturi</a></li>
			<li'.equal(1,$select,' class="active"').'><a href="2_orar.php">Update</a></li>
			<li'.equal(2,$select,' class="active"').'><a href="logout.php">Deconectare</a></li>
		</ul>
		';
//submeniuri secretara
	$submenu[0][0] = '
		<ul>
			<li'.equal(0,$select,' class="subactive"').'><a href="0_universitate.php">Universitate</a></li>
			<li'.equal(1,$select,' class="subactive"').'><a href="0_facultate.php">Facultate</a></li>
			<li'.equal(2,$select,' class="subactive"').'><a href="0_vacanta.php">Vacante</a></li>
			<!--li'.equal(3,$select,' class="subactive"').'><a href="0_catedra.php">Catedra</a></li-->
		</ul>';
	$submenu[0][1] = '
		<ul>
			<li'.equal(0,$select,' class="subactive"').'><a href="0_orar_viz.php">Vizualizare</a></li>
			<li'.equal(1,$select,' class="subactive"').'><a href="0_salvare_xls.php">Salvare XLS</a></li>
			<!--li'.equal(2,$select,' class="subactive"').'><a href="#">Modifica</a></li>
			<li'.equal(3,$select,' class="subactive"').'><a href="#">printeaza</a></li>
			<li'.equal(4,$select,' class="subactive"').'><a href="#">cauta </a></li-->
		</ul>';
	$submenu[0][2] = '
		<ul>
			<li'.equal(0,$select,' class="subactive"').'><a href="#">Ajutor</a></li>
			<li'.equal(1,$select,' class="subactive"').'><a href="#">General</a></li>
			<li'.equal(2,$select,' class="subactive"').'><a href="#">etc</a></li>
			<li'.equal(3,$select,' class="subactive"').'><a href="#">etc</a></li>
			<li'.equal(4,$select,' class="subactive"').'><a href="#">etc</a></li>
		</ul>
	';
	$submenu[0][3] = '';

//submeniuri profesor
	$submenu[1][0] = '
		<!--ul>
			<li'.equal(0,$select,' class="subactive"').'><a href="0_universitate.php">Universitate</a></li>
			<li'.equal(1,$select,' class="subactive"').'><a href="0_facultate.php">Facultate</a></li>
			<li'.equal(2,$select,' class="subactive"').'><a href="0_vacanta.php">Vacanta</a></li>
			<li'.equal(3,$select,' class="subactive"').'><a href="0_catedra.php">Catedra</a></li>
		</ul-->';
	$submenu[1][1] = '
		<!--ul>
			<li'.equal(0,$select,' class="subactive"').'><a href="0_orar_viz.php">Vizualizare</a></li>
			<li'.equal(1,$select,' class="subactive"').'><a href="0_salvare_xls.php">Salvare XLS</a></li>
			<!--li'.equal(2,$select,' class="subactive"').'><a href="#">Modifica</a></li>
			<li'.equal(3,$select,' class="subactive"').'><a href="#">printeaza</a></li>
			<li'.equal(4,$select,' class="subactive"').'><a href="#">cauta </a></li-->
		</ul-->';
	$submenu[1][2] = '
		<ul>
			<li'.equal(0,$select,' class="subactive"').'><a href="#">Ajutor</a></li>
			<li'.equal(1,$select,' class="subactive"').'><a href="#">General</a></li>
			<li'.equal(2,$select,' class="subactive"').'><a href="#">etc</a></li>
			<li'.equal(3,$select,' class="subactive"').'><a href="#">etc</a></li>
			<li'.equal(4,$select,' class="subactive"').'><a href="#">etc</a></li>
		</ul>
	';
	$submenu[1][3] = '';
	
//submeniuri admin
	$submenu[2][0] = '
		<ul>
			<li'.equal(0,$select,' class="subactive"').'><a href="0_universitate.php">Universitate</a></li>
			<li'.equal(1,$select,' class="subactive"').'><a href="0_facultate.php">Facultate</a></li>
			<li'.equal(2,$select,' class="subactive"').'><a href="0_vacanta.php">Vacanta</a></li>
			<li'.equal(3,$select,' class="subactive"').'><a href="0_catedra.php">Catedra</a></li>
		</ul>';
	$submenu[2][1] = '
		<ul>
			<li'.equal(0,$select,' class="subactive"').'><a href="0_orar_viz.php">Vizualizare</a></li>
			<li'.equal(1,$select,' class="subactive"').'><a href="0_salvare_xls.php">Salvare XLS</a></li>
			<!--li'.equal(2,$select,' class="subactive"').'><a href="#">Modifica</a></li>
			<li'.equal(3,$select,' class="subactive"').'><a href="#">printeaza</a></li>
			<li'.equal(4,$select,' class="subactive"').'><a href="#">cauta </a></li-->
		</ul>';
	$submenu[2][2] = '
		<ul>
			<li'.equal(0,$select,' class="subactive"').'><a href="#">Ajutor</a></li>
			<li'.equal(1,$select,' class="subactive"').'><a href="#">General</a></li>
			<li'.equal(2,$select,' class="subactive"').'><a href="#">etc</a></li>
			<li'.equal(3,$select,' class="subactive"').'><a href="#">etc</a></li>
			<li'.equal(4,$select,' class="subactive"').'><a href="#">etc</a></li>
		</ul>
	';
	$submenu[2][3] = '';

	if($tip_meniu == menu)
	{
		$var = $menu[$tip_cont];
	}
	else
	if($tip_meniu == submenu)
	{
		$var = $submenu[$tip_cont][$cat];
	}
	
	return $var;
}

function get_facultate($id)//preia datele unei facultati care are id dat
{
	$query = "SELECT * FROM `facultate` WHERE `fac_id`='".$id."' LIMIT 1";
	$result = mysql_query($query);
	
	return $result;
}

function get_catedra($id)//preia datele unei facultati care are id dat
{
	$query = "SELECT * FROM `catedra` WHERE `cat_id`='".$id."' LIMIT 1";
	$result = mysql_query($query);
	
	return $result;
}

function write_select_data($data)//scrie optiune de selectie a datei
{
global $luna;

$output = '<select name="'.$data.'_zi">';

for($i=1;$i<=31;$i++)
	{
		add($output,'<option>'.$i.'</option>');
	}
add($output,'</select>&nbsp;');

add($output,'<select name="'.$data.'_luna">');

for($i=0;$i<12;$i++)
	add($output,'<option value='.($i+1).'>'.$luna[$i].'</option>');
add($output,'</select>&nbsp;');

add($output,'<select name="'.$data.'_an">');
for($i=0;$i<=4;$i++)
	{
		add($output,'<option '.'>'.($i+2008).'</option>');
	}

add($output,'</select>');

return $output;
}//sfarsit write_data()

function translate_date($data_explicit)//scrie in format de tipul 02 ianuarie 1998
{
$luna = array(0=>"Ianuarie",1=>"Februarie",2=>"Martie",3=>"Aprilie",4=>"Mai",5=>"Iunie",6=>"Iulie",
			  7=>"August",8=>"Septembrie",9=>"Octombrie",10=>"Noiembrie",11=>"Decembrie");

$aux = strstr($data_explicit,"-");
$an = substr($data_explicit,0,strlen($data_explicit)-strlen($aux));
$data_explicit = substr($aux,1,strlen($aux)-1);
$aux = strstr($data_explicit,"-");
$index_luna = substr($data_explicit,0,strlen($data_explicit)-strlen($aux));
$data_explicit = substr($aux,1,strlen($aux)-1);
$zi = $data_explicit;

$data_complet = $zi .' '. $luna[(int)$index_luna-1] .' ' .$an;

return $data_complet;
}

?>