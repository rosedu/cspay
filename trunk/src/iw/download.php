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

	$fp = @fopen($file_location."/".$file_name, "r");
	if($fp) {
		header("Content-Type: application/octet-stream");
		header("Accept-Ranges: bytes");
		header("Content-Disposition: ; attachment; Filename=$file_name");
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
