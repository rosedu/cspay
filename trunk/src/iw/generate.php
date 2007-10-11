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

# Configuration data (vlad's ideea)
include('config.php');
include('utils.php');

# Initializare sesiune!
session_start();

# Captcha test (10x lucian)
if( md5($_POST['seccode']) != $_SESSION['security_code']) { #  !$_POST['debug']
	echo "<html><head><title>CSpay parser</title>".
		'<meta http-equiv="content-type" content="text/html; charset=UTF-8" />'.
		'<link rel="stylesheet" type="text/css" href="stil.css" />'.		
		"</head><body>";
	echo "<h1>Cod de securitate greşit</h1>";
	echo "<p>Codul introdus este greşit. Vă rugăm să îl introduceţi din nou.</p>";
	echo "<p>Se foloseşte un cod imagine pentru a se asigura interacţiunea cu un utilizator uman, nu robot</p>";
	echo "\n<br />\n<a href=index.php>&laquo;înapoi la formular</a>";
	echo "</body>";

	die;
	#TODO: redirect to index.php. ideas needed
}

# Creez fisier personal.ini temporar
$tmpfname = tempnam("/tmp", "personalini");
$file = fopen($tmpfname, "w");

# Mica initializare si adjustementuri
if( !is_array($_POST['tip_fisier']) )
	$_POST['tip_fisier'] = array( $_POST['tip_fisier'] );
if( !is_array($_POST['luna']) )
	$_POST['luna'] = array( $_POST['luna'] );
$_POST['catedra'] = $_POST[str_replace(' ', '_', $_POST['facultate']).'_catedre'];
$_POST['sef_catedra'] = $_POST[str_replace(' ', '_', $_POST['facultate'].'_'.$_POST['catedra'])];
$_POST['decan'] = $_POST['decan_'.str_replace(' ', '_', $_POST['facultate'])];

# scriu un model personal.ini cu date reale
fwrite($file, "[antet]\n".
	"nume=$_POST[nume]\n".
	"intocmit=$_POST[intocmit]\n".
	"universitate=$_POST[universitate]\n".
	"facultate=$_POST[facultate]\n".
	"nume_curs=$_POST[nume_curs]\n".
	"titular=$_POST[titular]\n".
	"decan=$_POST[decan]\n".
	"catedra=$_POST[catedra]\n".
	"sef_catedra=$_POST[sef_catedra]\n".
	"luna=".implode(',',$_POST['luna'])."\n".
	"tip_fisier=".implode(',',$_POST['tip_fisier'])."\n\n"
	);

$i = 1;
foreach( $_POST[orar] as $o) {
	fwrite($file, "\n[ore/$i]\n".
	"facultate=$o[facultatea]\n".
	"disciplina=$o[disciplina]\n".
	"rol=$o[felpost]\n".   	
	"numar_post=$o[numar_post]\n".
	"tip_post=$o[tipora]\n".
	"grupa=$o[grupa]\n".
	"zi=$o[zi]\n".
	"ore=$o[ore_start]-$o[ore_stop]\n".
	"paritate=$o[paritate]\n".
	"paritate_start=$o[paritate_prima]\n");
	$i++;
}

# il inchid
fclose($file);

# Apelez cspay si afisez outputul
exec(CSPAY_URI." ".CSPAY_XML_URI." ".$tmpfname, $lista_fisiere);

# Cojocar Lucian:
# vreau sa vad personalini creat, asa ca-i schimb grupul pentru a putea fi citit
exec("chgrp cspay ".$tmpfname, $_output);
exec("chmod g+r ".$tmpfname, $_output);

# Verific daca am primit output
if( !empty($lista_fisiere) ) {
	echo "<html><head><title>CSpay download</title>".
		'<meta http-equiv="content-type" content="text/html; charset=UTF-8" />'.
		'<link rel="stylesheet" type="text/css" href="stil.css" />'.
		"</head><body>";
	echo "<h1>Fişiere tabel de date obţinute:</h1><ul>";
	
	for($i = 0; $i<count($lista_fisiere); $i++) {
		# CL: am modificat pe aici
		$fn =  str_replace(array("/tmp/"), "", $lista_fisiere[$i]);
		echo "<li><a href=\"download.php?f=$fn\">$fn</a></li>\n";
	}
	echo "</ul>\n<br />\n";
	
	# Daca s-a comandat mail ma conformez:
	if( $_POST['send_mail'] == 1 ) {
		$attachments = array();
		for($i = 0; $i<count($lista_fisiere); $i++) {
			$fn =  str_replace(array("/tmp/"), "", $lista_fisiere[$i]);
			$attachments[] = array(	'file' =>'/tmp/'.$fn, 
								'content_type'=>'application/ods',
								'name' => $fn);
		}

		send_mail( 'CSPay <no-reply@anaconda.cs.pub.ro>', $_POST['email'],
			'CSPay Download', '<p>Fişiere rezultate:</p>',
			 $attachments );
		echo "Un mail a fost trimis la: <i>".$_POST['email']."</i>.<br />";
	} 
	echo "<a href=index.php>&laquo;înapoi la formular</a>";
	echo "</body></html>";
}
else {
	# Nu am output, o eroare s-a produs :-s
	echo "<html><head><title>CSpay download</title>".
		'<meta http-equiv="content-type" content="text/html; charset=UTF-8" />'.
		'<link rel="stylesheet" type="text/css" href="stil.css" />'.
		"</head><body>";
	echo "<h1>Eroare</h1>";
	echo "<p>O eroare necunoscută s-a produs. Verificaţi datele introduse şi încercaţi din nou.</p>";
	echo "<p>Puteţi raporta această eroare aici: <a href=http://www.rosedu.org/dev/cspay/newticket/>http://www.rosedu.org/dev/cspay/newticket/</a></p>";
	echo "</ul>\n<br />\n<a href=index.php>&laquo;înapoi la formular</a>";
	echo "</body></html>";
}

##debug:
/*if($_POST['debug'] == 1) {
	# listez continutul personal.ini obtinut
	exec("cat $tmpfname", $continut);
	echo "<hr>$tmpfname :<br/>";
	echo implode('<br />', $continut);
}*/
?>
