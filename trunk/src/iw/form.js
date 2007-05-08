var 	orar = null;

//adauga un nou camp cu date de orar
//foloseste variabila globala orar
//care retine cate astfel de campuri au fost adaugate
function do_more() {
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
	primul_input = document.getElementById('fel_'+orar);
	primul_input.focus();
	//TODO: copiez valori existente in ultimul div
	//incrementez orar pt o urmatoare adaugare (more)
	orar++;
}

function less( id ) {
	//obtin divul cadru si divul care trebuie sters
	cadru = document.getElementById('orar');
	continut = document.getElementById(id);
	
	//sterg continut din cadru
	cadru.removeChild( continut );
}

function get_orar() {
	i = 1;
	while(document.getElementById('orar_'+i)) 
		i++;
	orar = i;
}

function show_hide( id ) {
	//obtin elementul de afisat/ascuns
	element = document.getElementById(id);
	if(!element.style.display || element.style.display == 'none')
		element.style.display = 'block';
	else
		element.style.display = 'none';
}
