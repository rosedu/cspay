<html>
<head>
<title>PHP ajax cascading drop down using MySql</title>
<script>
function getXMLHTTP() { //fuction to return the xml http object
		var xmlhttp=false;	
		try{
			xmlhttp=new XMLHttpRequest();
		}
		catch(e)	{		
			try{			
				xmlhttp= new ActiveXObject("Microsoft.XMLHTTP");
			}
			catch(e){
				try{
				req = new ActiveXObject("Msxml2.XMLHTTP");
				}
				catch(e1){
					xmlhttp=false;
				}
			}
		}
		 	
		return xmlhttp;
	}
	

  function CategoryGrab(strURL, div_id)
    {         
     var req = getXMLHTTP(); // fuction to get xmlhttp object
     if (req)
     {
      req.onreadystatechange = function()
     {
      if (req.readyState == 4) { //data is retrieved from server
       if (req.status == 200) { // which reprents ok status                    
         document.getElementById(div_id).innerHTML=req.responseText;
      }
      else
      { 
         alert("There was a problem while using XMLHTTP:\n");
      }
      }            
      }        
    req.open("GET", strURL, true); //open url using get method
    req.send(null);
     }
    }
</script>
</head>

