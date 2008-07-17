<?php
	function extrage_date_BD(){
		echo "extragem date.\n";
	}
	function verifica_date_noi(){
		echo "verificam.\n";
	}
	function submite_modificari(){
		echo "submitem modificari.\n";
	}
	function afiseaza_legenda(){
		
		global $legenda;
		if(isset($legenda) and ($legenda=="Afiseaza")){
			echo "Legenda este:";
			echo "/*Va fi de forma COLOANA Explicatie<br> Valori posibile*/<br>";
			echo "<input type =\"submit\" name=\"legenda\" id=\"legenda\" value=\"Ascunde\" />";
		}
		else{
			echo "<input type =\"submit\" name=\"legenda\" id=\"legenda\" value=\"Afiseaza\" />";
		}
	}
	function afiseaza_criterii_selectie(){
		echo "afisam criterii selectie";
	}
	#extrage_date_BD();
	#verifica_date_noi();
	#submite_modificari();
	function afiseaza_zona_editare(){
		$query="";
		if(isset($sel_materie)){
		}
	# ... construiesc query
	echo "	
			<form> <!-- TODO -->
				<table border = \"0\" rules=\"cols\">
					<tr>
						<td>Titlu disciplina</td>
						<td>Forma</td>
						<td>Cod</td>
						<td>An</td>
						<td>Serie</td>
						<td>Nr st</td>
						<td>Nr g/sg</td>
						<td>g/sg</td>
						<td>C1</td>
						<td>A1</td>
						<td>Tip</td>
						<td>Post</td>
						<td>Grad</td>
						<td>Norma</td>
						<td>Tip</td>
						<td>Acoperit</td>
						<td>Acop. efectiv</td>
						<td>An/Gr.</td>
						<td>Zi</td>
						<td>Ora</td>
						<td>Sala</td>
					</tr>";
	$max=1;
	for ($i=0; $i<$max; $i++)	
		echo"
					<tr>
						<td><input type=\"text\" name=\"\" value=\"\"></td>
						<td><select></select></td>
						<td><select></select></td>
				<td><select></select></td>
				<td><select></select></td>
						<td><input type=\"text\" name=\"\" value=\"\"></td>
						<td><input type=\"text\" name=\"\" value=\"\"></td>
				<td><select></select></td>
						<td><input type=\"text\" name=\"\" value=\"\"></td>
						<td><input type=\"text\" name=\"\" value=\"\"></td>
				<td><select></select></td>
						<td><input type=\"text\" name=\"\" value=\"\"></td>
				<td><select></select></td>
						<td><input type=\"text\" name=\"\" value=\"\"></td>
				<td><select></select></td>
						<td><input type=\"text\" name=\"\" value=\"\"></td>
						<td><input type=\"text\" name=\"\" value=\"\"></td>
				<td><select></select></td>
				<td><select></select></td>
						<td><input type=\"text\" name=\"\" value=\"\"></td>
						<td><input type=\"text\" name=\"\" value=\"\"></td>
					</tr>";
	echo "</table></form>";
		
	}
	function tabel($lim){
		echo "<html>";
		echo "<html><table border=\"1\">\n";
		for ($i=0; $i<=$lim; $i++) {
			echo "<tr><td>randul ".$i."</td></tr>\n";
		}
		echo "</table>";
		echo "</html>";
	}
?>
<html>
	<head>
		<title>CSpay - Formular editare informatii complete</title>
		<link rel="stylesheet" type="text/css" href="stil.css" />
	</head>
	<body>
		
		<form action="mine.php" method="POST">
			<fieldset>
				<legend>Selecteaza criterii afisare!</legend>
				<table>
					<tr>
						<td><label for="sel_materie">Materia:</label></td>
						<td><select name="sel_materie" id="sel_materie" >
							<option value="1">-Oricare-</option>
							<option value="2">X</option></select></td>
					</tr>
					<tr>
						<td><label for="sel_an">Anul:</label></td>
						<td><select name="sel_an" id="sel_an" size="1">
							<option value="1">-Oricare-</option>
							<option value="2">X</option></select></td>
					</tr>
					<tr>
						<td><label for="sel_serie">Seria:</label></td>
						<td><select name="sel_serie" id="sel_serie"  size="1">
							<option value="1">-Oricare-</option>
							<option value="2">X</option></select></td>
					</tr>
					<tr>
						<td><label for="sel_profesor">Anul:</label></td>
						<td><select name="sel_profesor" id="sel_profesor"  size="1">
							<option value="1">-Oricare-</option>
							<option value="2">X</option></select></td>
					</tr>
				</table>
				<input type="submit" name="selectie" id="selectie"  value="OK!"/>
			</fieldset>
		</form>

		<form action="mine.php" method="POST">
			<fieldset>
				<legend>Zona Editare</legend>
				<?php 
					afiseaza_zona_editare();
				?>
				<table>
					<tr>
						<td><input type="submit" name="anulare" id="anulare" value="Anuleaza"></td>
						<td><input type="submit" name="salvare" id="salvare" value="Salveaza"></td>
					</tr>
				</table>
			</fieldset>
		</form>

		<form action="mine.php" method="POST">
			<fieldset>
				<legend>Legenda</legend>
				<?php 	afiseaza_legenda();	?>
			</fieldset>
		</form>
	</body>
</html>
