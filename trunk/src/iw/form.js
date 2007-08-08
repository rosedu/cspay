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

var 	orar_count = null;
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
	if( orar_count == null )
		get_orar();
	//modific template [1] -> [orar] , _1 -> _orar
	template = template.replace(/\[1\]/g, '['+orar_count+']');
	template = template.replace(/_1/g, '_'+orar_count);

	//creez un nou div, cu id orar2(3,4...)
	newdiv = document.createElement('div');
	newdiv.setAttribute('id', 'orar_'+orar_count);
	//pun continut
	newdiv.innerHTML = template;
	//adaug la cadru:
	cadru.appendChild(newdiv);
	//ascund campul complex si il afisez pe cel less:
	document.getElementById('complex_'+orar_count).style.display = 'none';
	document.getElementById('less_1').style.display = 'inline';
	//fac focus
	primul_input = document.getElementById('numarpost_'+orar_count);
	primul_input.focus();
	//copiez valori existente din div precedent in div nou: 
	document.getElementById('numarpost_'+orar_count).value = document.getElementById('numarpost_'+(orar_count-1)).value;
	document.getElementById('disciplina_'+orar_count).value = document.getElementById('disciplina_'+(orar_count-1)).value;	
	document.getElementById('grupa_'+orar_count).value = document.getElementById('grupa_'+(orar_count-1)).value;
	document.getElementById('orele_'+orar_count).value = document.getElementById('orele_'+(orar_count-1)).value;	
	
	document.getElementById('zi_'+orar_count).selectedIndex = document.getElementById('zi_'+(orar_count-1)).selectedIndex;
	document.getElementById('felpost_'+orar_count).selectedIndex = document.getElementById('felpost_'+(orar_count-1)).selectedIndex;
	document.getElementById('facultatea_'+orar_count).selectedIndex = document.getElementById('facultatea_'+(orar_count-1)).selectedIndex;
	document.getElementById('tipora_'+orar_count).selectedIndex = document.getElementById('tipora_'+(orar_count-1)).selectedIndex;
	//incrementez orar pt o urmatoare adaugare (more)
	orar_count++;
}

/**
 * Remove the last orar fieldset, given by global orar_count variable from the container element 'orar'
 * \todo error checking
 */
function less( ) 
{
	//obtin divul cadru si divul care trebuie sters
	cadru = document.getElementById('orar');
	continut = document.getElementById('orar_'+(orar_count-1));
	
	if( orar_count > 1 ) {
		//sterg continut din cadru
		cadru.removeChild( continut );
		
		//decrementez orar_count
		orar_count--;
	}
	//daca nu mai am decat un camp, ascund pe less:
	if( orar_count == 2 ) {
		document.getElementById('less_1').style.display = 'none';
	}
}

/**
 * Returns the number of consecutive orar fieldsets into global orar_count
 * \return orar_count int >= 1
 */
function get_orar() 
{
	i = 1;
	while(document.getElementById('orar_'+i)) 
		i++;
	orar_count = i;
	return orar_count;
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
