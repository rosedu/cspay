from db_gather import gather_data
	
if __name__ == "__main__":
        luni = range(2,7)
        an = "2008/2009"
        profi = [ "Rughinis Razvan", "Octavian Purdila", "Razvan Deaconescu",
                  "Mihai Capota", "Iulian Moraru", "Mihaela Dumitrache",
                  "Marius Zaharia"]
        for prof in profi:
                gather_data(prof,an,luni)
        