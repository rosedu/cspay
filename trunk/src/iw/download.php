<?php
/*
 * download.php
 * Script pentru download fisier care nu se afla in dosar apache, ci in tmp (?)
 * Alex Eftimie 08.08.07
 */

$file_location = '/tmp';

if( isset($_GET['f']) ) {
	# numele fisierului e primit parametru get ?f=<nume>
	$file_name = urldecode($_GET['f']);
	# fac curat, nu am voie cu subdirectoare (lucian: ../etc/passwd :))
	$file_name = basename( $file_name );
	# pot avea un alt nume cu care il servesc:
	if ( isset($_GET['n']) ) 
		$out_name = basename( urldecode( $_GET['n']) );
	else
		$out_name = $file_name;
	
	# citesc si scuip in browser
	$fp = @fopen($file_location."/".$file_name, "r");
	if($fp) {
		header("Content-Type: application/octet-stream");
		header("Accept-Ranges: bytes");
		header("Content-Disposition: ; attachment; Filename=$out_name");
		while ( !feof($fp) ) {
			echo( fgets($fp, 4096) );
		}
		fclose ($fp);
		die;
	}

}
# ceva a mers prost, trimit header 404
header("HTTP/1.0 404 Not Found");
die;
?>
