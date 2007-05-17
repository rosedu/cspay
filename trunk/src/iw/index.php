<?php
/*
 *	index.php
 *	Citeste cspay.xml si afiseaza formularul.
 *
 */
 
# incarc continutul html
$template = implode("",@file('model-form.html'));

# pregatesc campurile care se inlocuiesc: facultati, catedre
# TODO: read from xml
$facultati_info = array('ACS'=>'Automatica si Calculatoare','Electro'=>'Electrocutari');
foreach( $facultati_info as $nume => $descriere )
	$facultati .= "<option value=\"$nume\">$descriere</option>\n";
	
$catedre_fac = array('ACS' => array('USO', 'SDA'), 'Electro'=>array('Fizica','Bla blauri'));
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
