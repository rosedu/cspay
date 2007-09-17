<?php
/**
 * \ingroup iw
 * \file utils.php
 * \brief Function library for CSPay IW
 * \author Alex Eftimie
 */

/**
 * Sends an email message with arbitrary attachments
 * \param attachments: bidimensional array with 'file' and 'content_type' 
 */
function send_mail( $fromaddress, $emailaddress, $emailsubject, $body, $attachments=false)
{
	$eol="\n";	//\r\n doesn't work!
	$mime_boundary=md5(time());

	# Common Headers
	$headers .= 'From: <'.$fromaddress.'>'.$eol;
	//$headers .= 'Return-Path: <'.$fromaddress.'>'.$eol;    // these two to set reply address
	//$headers .= "Message-ID: <".$now." TheSystem@".$_SERVER['SERVER_NAME'].">".$eol;
	//$headers .= "X-Mailer: PHP v".phpversion().$eol;          // These two to help avoid spam-filters

	# Boundary for marking the split & Multitype Headers
	$headers .= 'MIME-Version: 1.0'.$eol;
	$headers .= "Content-Type: multipart/mixed; boundary=\"".$mime_boundary."\"".$eol;

	$msg = "";     

	# HTML Version
	$msg .= "--".$mime_boundary.$eol;
	$msg .= "Content-Type: text/html; charset=iso-8859-1".$eol;
	$msg .= "Content-Transfer-Encoding: 8bit".$eol;
	$msg .= "Content-Disposition: inline".$eol.$eol;
	$msg .= $body.$eol.$eol;
	
	# Attachments
	if ($attachments !== false) {
		for ($i=0; $i < count($attachments); $i++) {
			if (is_file($attachments[$i]["file"])) {  
			# File for Attachment
			$file_name = isset($attachments[$i]["name"])?$attachments[$i]["name"]:basename($attachments[$i]["file"]);

			$handle = fopen($attachments[$i]["file"], 'rb');
			$f_contents = fread($handle, filesize($attachments[$i]["file"]));
			$f_contents = chunk_split(base64_encode($f_contents));    //Encode The Data For Transition using base64_encode();
			fclose($handle);

			# Attachment
			$msg .= "--".$mime_boundary.$eol;
			$msg .= "Content-Type: ".$attachments[$i]["content_type"]."; name=\"".$file_name."\"".$eol;
			$msg .= "Content-Transfer-Encoding: base64".$eol;
			$msg .= "Content-Disposition: attachment; filename=\"".$file_name."\"".$eol.$eol; // !! This line needs TWO end of lines !! IMPORTANT !!
			$msg .= $f_contents.$eol.$eol;
			}
		}
	}
	
	# Finished
	$msg .= "--".$mime_boundary."--".$eol.$eol;  // finish with two eol's for better security. see Injection.

	# SEND THE EMAIL
	ini_set(sendmail_from,$fromaddress);  // the INI lines are to force the From Address to be used !
	mail($emailaddress, $emailsubject, $msg, $headers);
	ini_restore(sendmail_from);
	//echo "mail sent";
}

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
		        	   $result[$attr->name] = htmlspecialchars($attr->value);
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
?>
