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
define('CSPAY_XML_URI', 'http://anaconda.cs.pub.ro/~cspay/cspay.xml'); //discutabila alegere...

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
		        for($i=0;$i<$children->length;$i++) {
		        	$child = $children->item($i);
		        	if($child->nodeName != '#text')
		        		if(!isset($result[$child->nodeName]))
		              			$result[$child->nodeName] = dom2array($child);
		              		else {
		              			$aux = $result[$child->nodeName];
		              			$result[$child->nodeName] = array( $aux );
		              			$result[$child->nodeName][] = dom2array($child);
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
	$facultati_info[$facultate['nume_scurt']] = $facultate['nume'];
	# adaug si catedrele acestei facultati
	if( !isset($facultate['catedra']['nume']) ) {
		foreach( $facultate['catedra'] as $catedra)
			$catedre_fac[$facultate['nume_scurt']][] = $catedra['nume'];
	}
	else
		$catedre_fac[$facultate['nume_scurt']][] = $facultate['catedra']['nume'];
}

# am obtinut facultati_info si catedre_fac; transform in HTML:
foreach( $facultati_info as $nume => $descriere )
	$facultati .= "<option value=\"$nume\">$descriere</option>\n";

foreach( $catedre_fac as $nume => $catedrele) {
	$catedre .= "<select name=\"".$nume."_catedre\" id=\"".$nume."_catedre\" class=\"hidden\">\n";
	foreach( $catedrele as $c )
		$catedre .= "<option>$c</option>\n";
	$catedre .= "</select>";
}

# inlocuiesc in xhtmlu
$template = str_replace('{FACULTATI}', $facultati, $template);
$template = str_replace('{CATEDRE}', $catedre, $template);

# afisez
echo $template;
?>
