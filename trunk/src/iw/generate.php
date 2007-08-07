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

# creez fisier personal.ini temporar
$tmpfname = tempnam("/tmp", "personalini");
$file = fopen($tmpfname, "w");

# mica initializare
if( !is_array($_POST['tip_fisier']) )
	$_POST['tip_fisier'] = array( 'odt' );
$_POST['catedra'] = $_POST[$_POST['facultate'].'_catedre'];

##TODO: obtine sefcatedra!!! din cspay.xml sau transmite-l din model-form;)

# scriu un model personal.ini cu date reale
fwrite($file, "[antet]\n".
	"nume=$_POST[nume]\n".
	"intocmit=$_POST[intocmit]\n".
	"universitate=$_POST[universitate]\n".
	"facultate=$_POST[facultate]\n".
	"decan=$_POST[decan]\n".
	"catedra=$_POST[catedra]\n".
	"sef_catedra=\n".
	"luna=$_POST[luna]\n".
	"tip_fisier=".implode(',',$_POST['tip_fisier'])."\n\n");
// fwrite($file, "[ore]\n"); # nu mai este necesar
$i = 1;
foreach( $_POST[orar] as $o) {
	fwrite($file, "\n[ore/$i]\n".
	"facultate=$o[facultatea]\n".
	"disciplina=$o[disciplina]\n".
	"rol=$o[felpost]\n".   			#TODO: transform c,a,...in 1,2,3...
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

# apelez cspay
exec("cat $tmpfname", $lista_fisiere); # exec("./cspay $tmpfname"

# afisez TODO: formatare links
echo implode('<br />', $lista_fisiere);

# debug:
/*
unset($_POST['submit']);

echo "<pre>";
print_r($_POST);
echo "</pre>";*/
?>
