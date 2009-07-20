function change_hidden(hidden_id,value)//modifica campul "value" al variabilei ascunse
{
	document.getElementById(hidden_id).value=value;
}

function hide(id)
{
	var nr = document.getElementById('nr_inreg').value;
	for(i=0;i<nr;i++)
	{
	var elem = document.getElementById(id+'_'+i);
	
	if(elem.style.display == 'none')
		elem.style.display = 'block';
	else
		elem.style.display = 'none';	
	}
	var but = document.getElementById(id+'_but');
	var head = document.getElementById(id+'_head');
	var info = document.getElementById(id+'_info');
		
	if(but.style.display == 'block')
		{
		head.style.display = 'block';
		but.style.display = 'none';
		}
	else
		{
		head.style.display = 'none';
		but.style.display = 'block';
		}
}

function toggle_col(id,nr)
{
//alert(id+'_');
	for(i=0;i<nr;i++)
		{			
		elem = document.getElementById(id+'_'+i);
		if(elem.style.display == 'none')
			elem.style.display = 'block';
		else
			elem.style.display = 'none';	
		}
	
	text2 = '&lt;-&gt;';
	text1 = '&gt;-&lt;';
	
	var elem = document.getElementById( 'but_'+nr );
	if( elem.innerHTML != text2 )
		elem.innerHTML = text2;
	else
		elem.innerHTML = text1; 
	/*	
	elem = document.getElementById('over'+'_'+nr);
	if(elem.style.display == 'block')
		elem.style.display = 'none';
	else
		elem.style.display = 'block';	
	*/
}
var stil_vect = new Array('stil1.css','stil2.css','stil3.css','stil4.css');
var index_style = 0;

function schimba_stil(val)
{
	val = parseInt(val);
	index_style = parseInt(index_style);
	
	var elem = document.getElementById('stylesheet');
	var new_index = parseInt(index_style) + val;
	if(new_index < stil_vect.length && new_index >=0)
	{
		index_style = new_index;
		createCookie('cspay_stylesheet',index_style,'100');
	}
	elem.href = stil_vect[index_style];
}

function schimba_font()
{
	var aux = readCookie('cspay_stylesheet');
	
	if(aux == null)
		{
		index_style = 0;
		createCookie('cspay_stylesheet',index_style,'100');
		}
	else
		{
		if(index_style != aux)
			index_style = aux;
		schimba_stil(0);
		}
}

function ascunde_coloane()
{
	var min = 1, max = 23;
	
	//parcurg toate coloanele
	for(i=min;i<=max;i++)
	{
		var elem = readCookie('cspay_td_'+i);
		//alert('cspay_td_'+i+'\nelem : '+elem);
		if(elem != null)//este setat
		{
			if(elem == 0)//ascund coloana
			{
				var col = 'td_'+i+'_';
				var but = document.getElementById(col+'but');
				var head = document.getElementById(col+'head');
				
				//alert(col+'head');
				
				if(head != null)
					head.style.display = 'none';
				
				if(but != null)
					but.style.display = 'block';
				
				var curent = 0;
				var flag = true;
				do
				{
					var celula = document.getElementById(col+curent);
					if(celula == null)
						{
						flag = false;
						break;
						}
					
					celula.style.display = 'none';
					
					curent++;
				}
				while(flag == true)
				
			}//altfel nu fac nimic
		}
	}
}

function marcheaza_coloana(nr,val)//seteaza cookie pentru a arata/ascunde coloana
{
	createCookie('cspay_td_'+parseInt(nr),parseInt(val),100);	
}

/*functii pentru lucrul cu cookie-uri
http://www.quirksmode.org/js/cookies.html
*/
function createCookie(name,value,days) {
	if (days) {
		var date = new Date();
		date.setTime(date.getTime()+(days*24*60*60*1000));
		var expires = "; expires="+date.toGMTString();
	}
	else var expires = "";
	document.cookie = name+"="+value+expires+"; path=/";
}

function readCookie(name) {
	var nameEQ = name + "=";
	var ca = document.cookie.split(';');
	for(var i=0;i < ca.length;i++) {
		var c = ca[i];
		while (c.charAt(0)==' ') c = c.substring(1,c.length);
		if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
	}
	return null;
}

function eraseCookie(name) {
	createCookie(name,"",-1);
}



/*************************************************************************/