 # -*- coding: utf8 -*-
from db_gather import gather_data
	
if __name__ == "__main__":
        luni = range(2,7)
        an = "2008/2009"
        profi = [ "Rughinis Razvan", "Octavian Purdila", "Razvan Deaconescu",
                  "Mihai Capotă", "Iulian Moraru", "Mihaela Dumitrache",
                  "Marius Zaharia"]
        univ="Universitatea Politehnica Bucuresti"
        facl="Facultatea de Automatica si Calculatoare"
        catd="Calculatoare"
        for prof in profi:
                gather_data(prof, an, univ, facl, catd)
        
