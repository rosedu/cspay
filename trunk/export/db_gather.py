# -*- coding: utf8 -*-
import MySQLdb
import sys
import datetime
import pickle
from logic_proto import output_table

days = ["lu", "ma", "mi", "jo", "vi", "sa", "du"]
levels = {'4a': "as", '3s': "sl", '2c': "conf", '1p': "prof"}

def gather_data(name, univ, facl, desk, path, months = 0):
        """ Gather data from a mySQL database for a certain name,
            and then write an Excel WorkBook for the given months,
            for the given universitary year
        """
        try:
                conn = MySQLdb.connect (host = "koala.cs.pub.ro", user = "rosedu_cspay",
                                        passwd = "huashaquou", db = "rosedu_cspay", charset = "utf8",
                                        use_unicode = True )
        except MySQLdb.Error, e:
                print "Error %d: %s" % (e.args[0], e.args[1])
                sys.exit (1)

        input = {'an': "", 'profesor': name, 'luni': [],
                 'facultate': facl, 'catedra': desk, 'functie_baza': "",
                 'ore': [], 'statut': "", 'titular_curs': "",
                 'sef_catedra': "", 'decan': "", 'universitate': univ,
                 'semestru':()}
				 
#load configuration file for parities
        config = open('parity.pkl', 'rb')
        parities = pickle.load(config)
        config.close()
        user_par={}
        
#fill the rest of the structure with datafrom  db
        cursor = conn.cursor (MySQLdb.cursors.DictCursor)        
        cursor.execute ("""SELECT univ_id, data_start, data_stop
                           FROM universitati WHERE nume=%s""",
                        univ)
        temp2 = cursor.fetchone()
        input['semestru'] = (temp2['data_start'], temp2['data_stop'])
        if temp2['data_start'].month < temp2['data_stop'].month :
                input['luni'] = range(temp2['data_start'].month,
                                      temp2['data_stop'].month + 1)
                input['an'] = (str(temp2['data_start'].year - 1) + "/" +
                               str(temp2['data_stop'].year))
        else:
                input['luni'] = ( range(temp2['data_start'].month, 13) +
                                  range(1, temp2['data_stop'].month + 1) )
                input['an'] = (str(temp2['data_start'].year) + "/" +
                               str(temp2['data_stop'].year))
        used_par = parities[temp2['univ_id']]

                
        cursor.execute ("""SELECT fac_id, decan
                           FROM facultati
                           WHERE nume=%s AND link_univ=%s """,
                        (facl, temp2['univ_id']))
        temp2 = cursor.fetchone()
        input['decan'] = temp2['decan']

        cursor.execute ("""SELECT sef
                           FROM catedre
                           WHERE nume=%s AND link_fac=%s""",
                        (desk, temp2['fac_id']))
        temp2 = cursor.fetchone()
        input['sef_catedra'] = temp2['sef']

                
        cursor.execute ("""SELECT link_disc, tip_ora, nr_post, an, serie,
                           grad_post, an_grupa, zi, ora, paritate,paritate_start
                           FROM ore
                           WHERE pers_acoperit=%s AND tip_ocupare='po'""",
                        name)
        result_set = cursor.fetchall ()

        i = 1
        for row in result_set:
                if i :
                        cursor.execute ("""SELECT nume
                                           FROM discipline
                                           WHERE disc_id=%s""",
                                        row['link_disc'])
                        temp1 = cursor.fetchone()
                        input['functie_baza'] = temp1['nume']

                        cursor.execute ("""SELECT nume
                                           FROM titulari
                                           WHERE link_disc=%s AND an=%s
                                                 AND serie=%s""",
					(row['link_disc'], row['an'],
                                         row['serie']))
                        temp2 = cursor.fetchone()
                        input['titular_curs'] = temp2['nume']

                        i = 0

                cursor.execute ("""SELECT link_fac, nume_scurt
				   FROM discipline
				   WHERE disc_id=%s""",
                                row['link_disc'])
                temp1 = cursor.fetchone()
                """
                cursor.execute ("SELECT link_fac
                                   FROM catedre
                                   WHERE cat_id=%s",
                                temp1['link_cat'])
                temp2 = cursor.fetchone()
                """
                cursor.execute ("""SELECT nume_scurt
                                   FROM facultati
                                   WHERE fac_id=%s""",
                                temp1['link_fac'])
                temp2 = cursor.fetchone()

                input['ore'].append(build_course(row,
                                                 temp1['nume_scurt'],
                                                 temp2['nume_scurt']))
                                  
        vacante=[]
        cursor.execute(""" SELECT data_start, data_stop FROM vacante""")
        result_set = cursor.fetchall()
        for row in result_set:
                vacante.append((row['data_start'], row['data_stop']))

        cursor.close ()
        conn.close()

        if i:
                print "No pay-per-hour found for", name
        else:
                output_table(input, vacante, used_par, path)
        print "okey"
	

def build_course(row, clas, fac):
        """Build a course structure from a database table row, the name
           of the course, and the name of the faculty
        """
        course = {'functie': "", 'nr_post': 0, 'facultate': "", 'zi': 0,
                  'tip': "", 'disciplina': "", 'grupa': "", 'ore': "",
                  'parit': 0, 'pari_st': 0}
        course['functie'] = conv_grade(row['grad_post'])
        course['nr_post'] = row['nr_post']
        course['zi'] = conv_day(row['zi'])
        course['tip'] = row['tip_ora']
        course['grupa'] = row['an_grupa']
        course['ore'] = row['ora']
        course['disciplina'] = clas
        course['facultate'] = fac
        course['parit'] = row['paritate']
        if row['paritate_start'] == course['parit']:
                course['pari_st'] = 0
        else:
                course['pari_st'] = row['paritate_start']
        return course

def conv_day(string):
        """Convert a weekday given as Romanian abreviation to order no."""
        return days.index(string)

def conv_grade(string):
        """Convert a grade from db format to logic_module format"""
        return levels[string]

#Server
#		conn = MySQLdb.connect (host = "localhost", user = "cspay",
#                                        passwd = "Chote3at", db = "rsoc_cspay")
