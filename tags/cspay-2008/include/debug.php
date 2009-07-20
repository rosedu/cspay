<?php

function show_post()
{
	$x = '';
	foreach($_POST as $index=>$val)
		add($x,$index .' => '.$val . '<br>');
	add($x,'<br>');
	return $x;
}

function debug_result($result)
{
	if(!$result)
		echo mysql_errno($result).' => '.mysql_error($result);
}

?>