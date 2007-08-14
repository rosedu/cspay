<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
			  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<!-- 14.05.2007		Razvan Deaconescu, indentare -->
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
	<head>
		<title>CSPay Formular</title>
		<!-- RD: nu ar putea sa fie un " />" la sfasit in loc de "</script>"-->
		<!-- AE: din pacate, nu merge, tag-ul script cerand inchidere deschidere... -->
		<script type="text/javascript" src="form.js"></script>
		<link rel="stylesheet" type="text/css" href="stil.css" />
		<meta http-equiv="content-type" content="text/html; charset=UTF-8" />
	</head>

	<body>
		<h1>Formular detalii plata cu ora</h1>
		<form action="generate.php" method="post" onsubmit="input_copy('nume','intocmit');">
		<fieldset>
			<legend>General</legend>
			<label for="nume">Nume:</label>
			<input type="text" name="nume" id="nume"/>&nbsp;
			<a href="#" id="a_more"
			onclick="input_copy('nume','intocmit');show_hide('intocmit_div');switch_text('a_more','mai mult','mai puţin');">mai mult</a>
			<div class="hidden" id="intocmit_div">
				<label for="intocmit">Intocmit:</label>
				<input type="text" name="intocmit" id="intocmit" value="" />
			</div>
			<br />
			<label for="luna">Luna:</label>
			<select name="luna" id="luna">
				<option value="9">octombrie</option>
				<option value="10">noiembrie</option>
				<option value="11">decembrie</option>
				<option value="0">ianuarie</option>
				<option value="1">februarie</option>
				<option value="2">martie</option>
				<option value="3">aprilie</option>
				<option value="4">mai</option>
				<option value="5">iunie</option>
				<option value="6">iulie</option>
				<option value="7">august</option>
				<option value="8">septembrie</option>
			</select>
			<!-- anul de inceput pentru octombrie -->
			<input type="hidden" name="anul" value="{ANUL}" />
		</fieldset>
		<fieldset>
			<legend>Detalii curs</legend>
			<label for="universitate">Universitatea:</label>
			<select id="universitate" name="universitate">
				<option value="{UNIVERSITATE}">{UNIVERSITATE}</option>
			</select>
			<br />
			<label for="facultate">Facultatea:</label>
			<select name="facultate" id="facultate" onchange="select_catedra( this )" onfocus="select_catedra( this )">
				{FACULTATI}
			</select>
			<br />
			<label for="catedradisabled">Catedra:</label>
			<select disabled="disabled" id="catedradisabled">
				<option>selectati mai intai facultatea</option>
			</select>
			<!-- urmeaza select-uri ascunse, catedre pentru fiecare facultate -->
				{CATEDRE}
			<br/>
			<!-- aici apar sefii de catedra si decanii ca inputuri hidden, pentru a nu mai citi din nou cspay.xml in generate.php -->
				{SEFI}
				{DECANI}
			<!-- -->
			<label for="curs">Nume curs:</label>
			<input type="text" name="curs" id="curs"/> <i>la care aveti functia de baza</i><br/>
			<label for="titular">Titular:</label>
			<input type="text" name="titular" id="titular"/><br/>
		</fieldset>
		<fieldset>
			<legend>Orar</legend>
			<div id="orar">
			<div id="orar_1">
			<!-- ziua, fel si nr. post, an/grupa, ora start-ora stop, MORE-->
				ziua: 
				<select name="orar[1][zi]" id="zi_1">
					<option value="1">luni</option>
					<option value="2">marti</option>
					<option value="3">miercuri</option>
					<option value="4">joi</option>
					<option value="5">vineri</option>
				</select>
				post:<input name="orar[1][numarpost]" id="numarpost_1" type="text" size="2" value=""/>
				fel:
				<select name="orar[1][felpost]" id="felpost_1">
					<option value="3">profesor</option>
					<option value="0">asistent</option>
					<option value="1">conferentiar</option>
					<option value="2">sef lucrari</option>
				</select>
				fac.:
				<select name="orar[1][facultatea]" id="facultatea_1">
					{FACULTATI_SCURT}
				</select>
				disc.:<input name="orar[1][disciplina]" id="disciplina_1" type="text" size="4" />
				curs:
				<select name="orar[1][tipora]" id="tipora_1">
					<option value="0">curs</option>
					<option value="1">aplicatii</option>
				</select>
				seria:<input name="orar[1][grupa]" id="grupa_1" type="text" size="5" value=""/>
				orele: <input name="orar[1][ore]" id="orele_1" type="text" size="5" value=""/>
				
				<a href="#" onclick="show_hide('complex_1')">complex</a>
				<!-- setari complexe ascunse implicit -->
				<div id="complex_1" class="complex_div">
					paritate: 
					<select name="orar[1][paritate]">
						<option>1</option>
						<option>2</option>
						<option>3</option>
						<option>4</option>
					</select>
					prima saptamana: 
					<select name="orar[1][paritate_prima]">
						<option>1</option>
						<option>2</option>
						<option>3</option>
						<option>4</option>
					</select>
				</div>
			</div>
			</div>
			<!-- butoane de less si de more -->
			<button id="more_1" onclick="more(); return false;">mai mult</button>
			<button id="less_1" onclick="less(); return false;" class="hidden">mai puţin</button>
		</fieldset>
		<fieldset>
			<legend>Diverse</legend>
			<label for="tip_fisier">Tip fisier:</label>
			<select name="tip_fisier[]" id="tip_fisier" multiple="multiple">
				<option selected="selected">ods</option>
				<option disabled="disabled">xls</option>				
			</select>
			<br />
			<label for="send_mail">Trimite prin e-mail</label>
			<input name="send_mail" id="send_mail" type="checkbox" value="1">&nbsp;
			<input name="email" value="" size=20>
			<br />
			<label for="seccode">Cod securitate:</label>
			<input name="seccode" value="" id="seccode" size="5" maxlength="5" />
			<img src="seccode.php" alt="security code" style="vertical-align:middle" />
			<br />
			<!-- TODO: not for final release -->
			<i>debug: </i><input type="checkbox" name="debug" value="1" />
		</fieldset>
		<p style="text-align:center">
			<input type="submit" value="Trimite" name="submit" />
		</p>
		</form>
	<hr />
	<p>&copy; 2007 <a href="http://anaconda.cs.pub.ro/~cspay/">The CSpay Team</a>.
	</p>
	</body>
</html>
