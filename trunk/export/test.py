 # -*- coding: utf8 -*-
from db_gather import gather_data
	
if __name__ == "__main__":
        luni = range(2,7)
        an = "2008/2009"
        profi = [ "Rughinis Razvan", "Octavian Purdila", 
                  "Iulian Moraru", "Mihaela Dumitrache",
                  "Marius Zaharia", "Razvan Deaconescu"]
        univ="Universitatea Politehnica Bucuresti"
        facl="Facultatea de Automatica si Calculatoare"
        catd="Calculatoare"
        # ultimu argument e 0 pt plata cu ora, 1 pt orar xls, 2 pt orar ical
        for prof in profi:
                gather_data(prof, univ, facl, catd, "ciciolina", 2)
        
