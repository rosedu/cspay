/*
 *	form.js - javascript functions
 *	26.07.2007 Alex Eftimie	
 */
/**
 * \ingroup iw
 * \file form.js
 * \brief iw script library
 * \author Alex Eftimie
 */

var 	orar = null;
var 	catedra_select = null;

/**
 * Adds a new empty fieldset
 * Uses global variable orar to retain the number of added fieldsets
 */
function more() 
{
	//obtin div-ul cadru, si div-ul continut
	cadru = document.getElementById('orar');
	continut = document.getElementById('orar_1');
	
	template = continut.innerHTML;
	//vad al catelea sunt orar = 2, 3, 4...
	if( orar == null )
		get_orar();
	//modific template [1] -> [orar] , _1 -> _orar
	template = template.replace(/\[1\]/g, '['+orar+']');
	template = template.replace(/_1/g, '_'+orar);

	//creez un nou div, cu id orar2(3,4...)
	newdiv = document.createElement('div');
	newdiv.setAttribute('id', 'orar_'+orar);
	//pun continut
	newdiv.innerHTML = template;
	//adaug la cadru:
	cadru.appendChild(newdiv);
	//ascund campul complex si afisez boton less
	document.getElementById('complex_'+orar).style.display = 'none';
	document.getElementById('less_'+orar).style.display = 'inline';
	//fac focus
	primul_input = document.getElementById('numarpost_'+orar);
	primul_input.focus();
	//copiez valori existente in ultimul div: 
	document.getElementById('numarpost_'+orar).value = document.getElementById('numarpost_'+(orar-1)).value;
	document.getElementById('disciplina_'+orar).value = document.getElementById('disciplina_'+(orar-1)).value;	
	document.getElementById('grupa_'+orar).value = document.getElementById('grupa_'+(orar-1)).value;
	document.getElementById('orele_'+orar).value = document.getElementById('orele_'+(orar-1)).value;	
	
	document.getElementById('zi_'+orar).selectedIndex = document.getElementById('zi_'+(orar-1)).selectedIndex;
	document.getElementById('felpost_'+orar).selectedIndex = document.getElementById('felpost_'+(orar-1)).selectedIndex;
	document.getElementById('facultatea_'+orar).selectedIndex = document.getElementById('facultatea_'+(orar-1)).selectedIndex;
	document.getElementById('tipora_'+orar).selectedIndex = document.getElementById('tipora_'+(orar-1)).selectedIndex;
	//incrementez orar pt o urmatoare adaugare (more)
	orar++;
}

/**
 * Remove the element specified by id from the container element 'orar'
 * \param id the to be removed element's id
 * \todo error checking
 */
function less( id ) 
{
	//obtin divul cadru si divul care trebuie sters
	cadru = document.getElementById('orar');
	continut = document.getElementById(id);
	
	//sterg continut din cadru
	cadru.removeChild( continut );
}

/**
 * Returns the number of consecutive orar fieldsets
 * \return orar int >= 1
 */
function get_orar() 
{
	i = 1;
	while(document.getElementById('orar_'+i)) 
		i++;
	orar = i;
}

/**
 * Switch the visibility state of an element, using the DOM style.display property
 * \param id element's id
 */
function show_hide( id ) 
{
	//obtin elementul de afisat/ascuns
	element = document.getElementById(id);
	if(!element.style.display || element.style.display == 'none')
		element.style.display = 'block';
	else
		element.style.display = 'none';
}

/**
 * Copies the value of src to dest, if dest is empty
 * \param src source element
 * \param dest destination element
 */
function input_copy( src, dest )
{
	src = document.getElementById(src);
	dest = document.getElementById(dest);
	if( dest.value == '')
		dest.value = src.value;
}

/**
 * Displays the coresponding catedra SELECT (initially hidden) for the chosen faculty.
 * It's called by an ONCLICK or ONCHANGE event.
 * Uses global catedra_select variable to keep the currently visible SELECT (catedra_select 
 * is initialy null, that means 'catedradisabled' is visible).
 * \param facultate_element the id of faculty's html SELECT
 */
function select_catedra( facultate_element)
{
	facultate = facultate_element.selectedIndex;
	//ascund selectul
	if( catedra_select ) 
		catedra_select.style.display = 'none';
	else
		document.getElementById('catedradisabled').style.display = 'none';
	//afisez pe cel nou
	catedra_select = document.getElementById('catedre_'+facultate);
	catedra_select.style.display = 'inline';
}
/**
 * Switches text using the innerHTML atribute of the element passed as parameter
 * \param label_element element to be changed's id
 * \param text1
 * \param text2
 */
function switch_text( label_element, text1, text2)
{
	var elem = document.getElementById( label_element );
	if( elem.innerHTML != text2 )
		elem.innerHTML = text2;
	else
		elem.innerHTML = text1;
}
