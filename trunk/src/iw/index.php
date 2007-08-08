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

/** Configuration's file (`cspay.xml') address */
define('CSPAY_XML_URI', 'cspay.xml'); //discutabila alegere...

/**
 * Metoda recursiva de transformat XML DOM in dictionar
 * source: php.net manual (modified)
 * \param node the root node of the xml tree
 */
function dom2array( $node ) 
{
	$result = array();
	if($node->nodeType == XML_TEXT_NODE) {
		$result = $node->nodeValue;
	}
	else {
		if( $node->hasAttributes() ) {
		        $attributes = $node->attributes;
		        if(!is_null($attributes)) 
			        foreach ($attributes as $index=>$attr) 
		        	   $result[$attr->name] = $attr->value;
		}
		if($node->hasChildNodes()){
		        $children = $node->childNodes;
		        for($i=0; $i<$children->length; $i++) {
		        	$child = $children->item($i);
		        	if($child->nodeName != '#text') {
		        		if(!isset($result[$child->nodeName]))
		              			$result[$child->nodeName] = dom2array($child);
		              		else {
		              			if(!isset( $result[$child->nodeName][0] ) ) {
		              				$aux = $result[$child->nodeName];
		              				$result[$child->nodeName] = array( $aux );
		              			}
		              			$result[$child->nodeName][] = dom2array($child);
		              		}
		             	}
		        }
		}
	}
	return $result;
}

# incarc sablonul html TODO it somehow else
$template = implode("",@file('model-form.html'));

# incarc cspay.xml TODO: error checking
$doc = new DOMDocument();
$doc->load(CSPAY_XML_URI);
$cspay = dom2array( $doc );

# TODO: verificari de corectitudine and so
$universitate = $cspay['cspay-config']['universitate'];

# pregatesc campurile care se inlocuiesc: facultati, catedre
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

# am obtinut facultati_info, catedre_fac si sefi_catedre; transform in HTML:
foreach( $facultati_info as $nume => $descriere ) {
	$facultati .= "<option value=\"$descriere[nume]\">$descriere[nume]</option>\n";
	$facultati_scurt .= "<option value=\"$nume\">$descriere[nume_scurt]</option>\n";
	$decani .= "<input type=hidden name=\"decan_".str_replace(' ', '_',$descriere['nume'])."\" value=\"".$descriere['decan']."\">\n";
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
	$sefi .= "<input type=hidden name=\"".str_replace(' ', '_',$catedra)."\" value=\"".$sef."\">\n";
}
# inlocuiesc in xhtmlu
$template = str_replace('{UNIVERSITATE}', $universitate['nume'], $template);
$template = str_replace('{FACULTATI}', $facultati, $template);
$template = str_replace('{FACULTATI_SCURT}', $facultati_scurt, $template);
$template = str_replace('{CATEDRE}', $catedre, $template);
$template = str_replace('{SEFI}', $sefi, $template);
$template = str_replace('{DECANI}', $decani, $template);

# afisez
echo $template;
?>
