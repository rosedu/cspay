import MySQLdb
import sys
from datetime import date, timedelta
from logic_proto import outside
import pickle

def make_parity():
        """ Gather data from a mySQL database for a certain name,
            and then write an Excel WorkBook for the given months,
            for the given universitary year
        """
        try:
#Daniel-laptop
                conn = MySQLdb.connect (host = "localhost", user = "root",
                                        passwd = "myraki", db = "cspay")
#Server
#		conn = MySQLdb.connect (host = "localhost", user = "cspay",
#                                        passwd = "Chote3at", db = "rsoc_cspay")
        except MySQLdb.Error, e:
                print "Error %d: %s" % (e.args[0], e.args[1])
                sys.exit (1)

        vacante=[]

        cursor = conn.cursor (MySQLdb.cursors.DictCursor)
        cursor.execute(""" SELECT data_start, data_stop FROM vacante""")
        result_set = cursor.fetchall()
        for row in result_set:
                vacante.append((row['data_start'], row['data_stop']))

        cursor.execute ("""SELECT univ_id, data_start, data_stop
						   FROM universitati""")
        univ_set = cursor.fetchall()
        parities={}
        for univ in univ_set:
                uid=univ['univ_id']
                parities.update({uid:{}})
                for i in range (1,53):
                        parities[uid].update({i:0})
        cursor.close ()
        conn.close()

        for univ in univ_set:
                uid=univ['univ_id']
                set_parities(parities[uid], univ['data_start'],
                             univ['data_stop'], vacante)
        print str(parities)
        output = open('parity.pkl', 'wb')
        pickle.dump(parities, output, -1)
        output.close()


def set_parities(parity, start, stop, vacante):
        d = start
        w = d.isocalendar()[1]
        k = 1

        while  d <= stop:
                w = d.isocalendar()[1]
                p = 1
                while p and d.weekday() < 5:
                        if outside(d, vacante):
                                p = 0
                        d += timedelta(days = 1)
                        
                if not p:
                        parity[w] = k
                        k += 1
                        d += timedelta(days = 7 - d.weekday())
                else:
                        d += timedelta(days = 2)
                        

		
if __name__ == "__main__":
	make_parity()
			
