<?php
/*
 *	generate.php
 *	Primeste date prin _POST de la formularul din index.php. Creeaza personal.ini din datele introduse de utilizator, 
 *	apeleaza cspay (consola) si afiseaza rezultatul (calea catre fisierele spreadsheet pentru download).
 */
/**
 * \ingroup iw
 * \file generate.php
 * \brief iw personal.ini generator
 * \author Alex Eftimie
 */

# Initializare sesiune!
session_start();

# Captcha test (10x lucian)
if( md5($_POST['seccode']) != $_SESSION['security_code'] ) {
	die("Bad security code!");
	#TODO: redirect to index.php. ideas needed
}

# creez fisier personal.ini temporar
$tmpfname = tempnam("/tmp", "personalini");
$file = fopen($tmpfname, "w");

# mica initializare si adjustementuri
if( !is_array($_POST['tip_fisier']) )
	$_POST['tip_fisier'] = array( 'ods' );
$_POST['catedra'] = $_POST[str_replace(' ', '_', $_POST['facultate']).'_catedre'];
$_POST['sef_catedra'] = $_POST[str_replace(' ', '_', $_POST['facultate'].'_'.$_POST['catedra'])];
$_POST['decan'] = $_POST['decan_'.str_replace(' ', '_', $_POST['facultate'])];
//echo "<pre>";print_r($_POST);

# scriu un model personal.ini cu date reale
fwrite($file, "[antet]\n".
	"nume=$_POST[nume]\n".
	"intocmit=$_POST[intocmit]\n".
	"universitate=$_POST[universitate]\n".
	"facultate=$_POST[facultate]\n".
	"decan=$_POST[decan]\n".
	"catedra=$_POST[catedra]\n".
	"sef_catedra=$_POST[sef_catedra]\n".
	"luna=$_POST[luna]\n".
	"tip_fisier=".implode(',',$_POST['tip_fisier'])."\n\n");
// fwrite($file, "[ore]\n"); # nu mai este necesar
$i = 1;
foreach( $_POST[orar] as $o) {
	fwrite($file, "\n[ore/$i]\n".
	"facultate=$o[facultatea]\n".
	"disciplina=$o[disciplina]\n".
	"rol=$o[felpost]\n".   	
	"numar_post=$o[numarpost]\n".
	"tip_post=$o[tipora]\n".
	"grupa=$o[grupa]\n".
	"zi=$o[zi]\n".
	"ore=$o[ore]\n".
	"paritate=$o[paritate]\n".
	"paritate_start=$o[paritate_prima]\n");
	$i++;
}

# il inchid
fclose($file);

# listez continutul personal.ini obtinut
exec("cat $tmpfname", $continut);

# afisez TODO: formatare links
if($_POST['debug'] == 1)
	echo implode('<br />', $continut);

# Apelez cspay si afisez outputul
# exec("./cspay $tmpfname", $lista_fisiere);
echo "<h1>Fisiere spreadsheet obtinute:</h1>";
?>
