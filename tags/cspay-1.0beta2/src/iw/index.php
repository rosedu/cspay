<?php
/*
 *	index.php
 *	Citeste cspay.xml si afiseaza formularul.
 *
 */
/**
 * \ingroup iw
 * \file index.php
 * \brief iw index file
 *
 * Obtains data from configuration cspay.xml, then loads the template and prints it in browser.
 * \author Alex Eftimie
 * \defgroup iw Web interface for CSPay
 */
 
# configuration data (vlad's ideea)
include('config.php');
include('utils.php');

# incarc sablonul html TODO it somehow else
$template = implode("", @file('index.tpl'));

# incarc cspay.xml TODO: error checking
$doc = new DOMDocument();
$doc->load( CSPAY_XML_URI );
$cspay = dom2array( $doc );

# TODO: verificari de corectitudine and so
$universitate = $cspay['cspay-config']['universitate'];

# pregatesc campurile care se inlocuiesc: facultati, catedre, sefi, anul
if(!isset($universitate['facultati']['facultate']['nume']))
	$universitate['facultati'] = $universitate['facultati']['facultate'];
	
foreach( $universitate['facultati'] as $facultate) {
	$facultati_info[$facultate['nume_scurt']] = array( 
		'nume'=>$facultate['nume'], 
		'nume_scurt'=>$facultate['nume_scurt'], 
		'decan'=>$facultate['decan']);
	# adaug si catedrele acestei facultati
	if( !isset($facultate['catedra']['nume']) ) {
		foreach( $facultate['catedra'] as $catedra) {
			$catedre_fac[$facultate['nume']][] = $catedra['nume'];
			$sefi_catedre[$facultate['nume'].'_'.$catedra['nume']] = $catedra['sef'];
		}
	}
	else {
		$catedra = $facultate['catedra'];
		$catedre_fac[$facultate['nume']][] = $catedra['nume'];
		$sefi_catedre[$facultate['nume'].'_'.$catedra['nume']] = $catedra['sef'];
	}
}

# scot anul din data semestrului !Atentie, aleg primul semestru din xml TODO: data cea mai recenta
if( isset($universitate['semestru']['nume']) )
	$data_start = $universitate['semestru']['data_start'];
else
	$data_start = $universitate['semestru'][0]['data_start'];
$anul = substr($data_start, strrpos($data_start, '.')+1);

# am obtinut facultati_info, catedre_fac si sefi_catedre; transform in HTML:
foreach( $facultati_info as $nume => $descriere ) {
	$facultati .= "<option value=\"$descriere[nume]\">$descriere[nume]</option>\n";
	$facultati_scurt .= "<option value=\"$nume\">$descriere[nume_scurt]</option>\n";
	$decani .= "<input type=\"hidden\" name=\"decan_".str_replace(' ', '_',$descriere['nume'])."\" value=\"".$descriere['decan']."\" />\n";
}

$i = 0;
foreach( $catedre_fac as $nume => $catedrele) {
	$catedre .= "<select name=\"".str_replace(' ', '_',$nume)."_catedre\" id=\"catedre_".$i."\" class=\"hidden\">\n";
	foreach( $catedrele as $c )
		$catedre .= "<option>$c</option>\n";
	$catedre .= "</select>";
	$i++;
}

foreach( $sefi_catedre as $catedra => $sef ) {
	$sefi .= "<input type=\"hidden\" name=\"".str_replace(' ', '_',$catedra)."\" value=\"".$sef."\" />\n";
}
# inlocuiesc in xhtmlu
$template = str_replace('{UNIVERSITATE}', $universitate['nume'], $template);
$template = str_replace('{FACULTATI}', $facultati, $template);
$template = str_replace('{FACULTATI_SCURT}', $facultati_scurt, $template);
$template = str_replace('{CATEDRE}', $catedre, $template);
$template = str_replace('{SEFI}', $sefi, $template);
$template = str_replace('{DECANI}', $decani, $template);
$template = str_replace('{ANUL}', $anul, $template);

# afisez
echo $template;
?>
