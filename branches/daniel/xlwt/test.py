from logic_proto import output_table
from datetime import date
	
if __name__ == "__main__":
        holidays = [ (date(2009, 10, 14), date(2009, 10, 14)),
                     (date(2009, 12, 24), date(2010, 01, 06))]
        lab1= {'functie': 'as', 'nr_post': 87, 'facultate': 'A & C', 'zi': 2,
               'tip':"L", 'disciplina': 'Utiliz Sist Operare', 'grupa': '324CA',
               'ore': "12-14"}
        lab2= {'functie': 'sl', 'nr_post': 67, 'facultate': 'A & C', 'zi': 4,
               'tip':"C", 'disciplina': 'Utiliz Sist Operare', 'grupa': '321CA',
               'ore': "10-13"}
        lab3={ 'functie': 'as', 'nr_post': 57, 'facultate': 'A & C', 'zi': 1,
               'tip':"L", 'disciplina': 'Utiliz Sist Operare', 'grupa': '322CA',
               'ore': "11-13"}

        labs = []
        labs.append(lab1)
        labs.append(lab2)
        labs.append(lab3)
        months = [10, 11, 12]
        intrare = {'an':2009, 'profesor':"Deaconescu Rãzvan", 'luni':months,
                   'facultate':"Automaticã ºi Calculatoare",
                   'catedra':"Calculatoare",
                   'functie_baza':"Utilizarea Sistemelor de Operare",
                   'ore':labs, 'statut':"as. dnd. ing.",
                   'titular_curs':"Octavian Purdilã",
                   'sef_catedra':"Valentin Cristea",
                   'decan':"Dumitru Popescu"}
        output_table(intrare, holidays)
