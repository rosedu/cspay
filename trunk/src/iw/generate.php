<?php
/*
 *	generate.php
 *	Primeste date prin _POST de la formularul din index.php. Creeaza personal.ini din datele introduse de utilizator, 
 *	apeleaza cspay (consola) si afiseaza rezultatul (calea catre fisierele spreadsheet pentru download).
 */

# creez fisier personal.ini temporar
$tmpfname = tempnam("/tmp", "personalini");
$file = fopen($tmpfname, "w");

# scriu un model personal.ini cu date reale
fwrite($file, "[antet]\n".
	"nume=$_POST[nume]\n".
	"intocmit=$_POST[intocmit]\n".
	"universitate=$_POST[universitate]\n".
	"facultate=$_POST[facultate]\n".
	"decan=$_POST[decan]\n".
	"catedra=$_POST[catedra]\n".
	"sefcatedra=$_POST[sefcatedra]\n\n");
fwrite($file, "[ore]\n");
$i = 1;
foreach( $_POST[orar] as $o) {
	fwrite($file, "$i=$o[zi]:$o[facultatea]:$o[disciplina]:$o[felpost]:$o[numarpost]:$o[tipora]:$o[grupa]:$o[ore]:$o[paritate]:$o[paritate_prima]\n");
	$i++;
}

# il inchid
fclose($file);

# apelez cspay
exec("cat $tmpfname", $lista_fisiere); # exec("./cspay $tmpfname"

# afisez TODO: formatare links
echo implode('<br />', $lista_fisiere);

# debug:
unset($_POST['submit']);

echo "<pre>";
print_r($_POST);
echo "</pre>";
?>
