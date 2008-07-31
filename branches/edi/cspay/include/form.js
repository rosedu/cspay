function change_hidden(hidden_id,value)//modifica campul "value" al variabilei ascunse
{
	document.getElementById(hidden_id).value=value;
}

function less(id)
{
	for(i=0;i<10;i++)
		{			
		elem = document.getElementById(id+'_'+i);
		if(elem.style.display == 'none')
			elem.style.display = 'block';
		else
			elem.style.display = 'none';	
		}
	
	text1 = '--';
	text2 = '++';
	
	var elem = document.getElementById( 'but_'+id );
	if( elem.innerHTML != text2 )
		elem.innerHTML = text2;
	else
		elem.innerHTML = text1;
}