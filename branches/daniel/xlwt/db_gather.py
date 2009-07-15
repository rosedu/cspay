import MySQLdb
import sys
import datetime
from logic_proto import output_table

days = ["lu", "ma", "mi", "jo", "vi", "sa", "du"]
levels = {'4a': "as", '3s': "sl", '2c': "conf", '1p': "prof"}

def gather_data(name, year, months):
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
	
	input = {'an': year, 'profesor': name, 'luni': months,
                 'facultate': "", 'catedra': "", 'functie_baza': "",
                 'ore': [], 'statut': "", 'titular_curs': "",
                 'sef_catedra': "", 'decan': "", 'universitate':"",
                 'semestru':()}

	cursor = conn.cursor (MySQLdb.cursors.DictCursor)
	cursor.execute ("""SELECT link_disc, tip_ora, nr_post, an, serie,
                           grad_post, an_grupa, zi, ora, paritate,paritate_start
                           FROM ore
                           WHERE pers_acoperit=%s AND tip_ocupare='po'""",
                        name)
	result_set = cursor.fetchall ()
	i = 1
	
	for row in result_set:
		if i :
			cursor.execute ("""SELECT link_cat, nume
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

			cursor.execute ("""SELECT nume, link_fac, sef
                                           FROM catedre
                                           WHERE cat_id=%s""",
                                        temp1['link_cat'])
			temp2 = cursor.fetchone()
			input['catedra'] = temp2['nume']
			input['sef_catedra'] = temp2['sef']

			cursor.execute ("""SELECT nume, decan, link_univ
                                           FROM facultati
                                           WHERE fac_id=%s""",
                                        temp2['link_fac'])
			temp2 = cursor.fetchone()
			input['facultate'] = temp2['nume']
			input['decan'] = temp2 ['decan']
			
			cursor.execute ("""SELECT nume, data_start, data_stop
                                           FROM universitati
                                           WHERE univ_id=%s""",
                                        temp2['link_univ'])
			temp2 = cursor.fetchone()
			input['universitate'] = temp2['nume']
			input['semestru'] = (temp2['data_start'], temp2['data_stop'])
			
			i = 0
		
		cursor.execute ("""SELECT link_cat, nume_scurt
				   FROM discipline
				   WHERE disc_id=%s""",
                                row['link_disc'])
		temp1 = cursor.fetchone()

		cursor.execute ("""SELECT link_fac
                                   FROM catedre
                                   WHERE cat_id=%s""",
                                temp1['link_cat'])
		temp2 = cursor.fetchone()
		
		cursor.execute ("""SELECT nume_scurt
                                   FROM facultati
                                   WHERE fac_id=%s""",
                                temp2['link_fac'])
		temp2 = cursor.fetchone()
		
		input['ore'].append(build_course(row,
                                                 temp1['nume_scurt'],
                                                 temp2['nume_scurt']))
	
	vacante=[]
	
	cursor.execute(""" SELECT data_start, data_stop FROM vacante""")
	result_set = cursor.fetchall ()
	for row in result_set:
		vacante.append((row['data_start'], row['data_stop']))

	cursor.close ()
	conn.close()
	if i:
		print "No pay-per-hour found for",name
	else:
		output_table(input,vacante)
	

def build_course(row, clas, fac):
	course = {'functie': "", 'nr_post': 0, 'facultate': "", 'zi': 0,
                  'tip': "", 'disciplina': "", 'grupa': "", 'ore': ""}
	course['functie'] = conv_grade(row['grad_post'])
	course['nr_post'] = row['nr_post']
	course['zi'] = conv_day(row['zi'])
	course['tip'] = row['tip_ora']
	course['grupa'] = row['an_grupa']
	course['ore'] = row['ora']
	course['disciplina'] = clas
	course['facultate'] = fac
	return course
	
			   
def conv_day(string):
	return days.index(string)

def conv_grade(string):
	return levels[string]
