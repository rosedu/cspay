import MySQLdb
import sys
import datetime

days = ["lu","ma","mi","jo","vi","sa","du"]
levels = {'4a':"as",'3s':"sl",'2c':"conf",'1p':"prof"}

def gather_data(name, year, months):
	try:
		conn = MySQLdb.connect (host = "anaconda.cs.pub.ro", user = "rsoc_cspay",
                                        passwd = "Chote3at", db = "rsoc_cspay")
	except MySQLdb.Error, e:
		print "Error %d: %s" % (e.args[0], e.args[1])
		sys.exit (1)
	
	iput = {'an':year, 'profesor':name, 'luni':months,
                   'facultate':"",
                   'catedra':"",
                   'functie_baza':"",
                   'ore':[], 'statut':"",
                   'titular_curs':"",
                   'sef_catedra':"",
                   'decan':""}

	cursor = conn.cursor (MySQLdb.cursors.DictCursor)
	cursor.execute ("""SELECT link_disc, tip_ora, nr_post, an, serie, grad_post,
                                  an_grupa, zi, ora, paritate, paritate_start
                                FROM ore WHERE pers_acoperit=%s AND tip_ocupare='po'""",name)
	result_set = cursor.fetchall ()
	i = 1
	
	for row in result_set:
		if i :
			cursor.execute ("""SELECT link_cat, nume
								FROM discipline WHERE disc_id=%s""",row['link_disc'])
			temp1=cursor.fetchone()
			input['functie_baza'] = temp1['nume']
			cursor.execute ("""SELECT nume FROM titulari
								WHERE link_disc=%s AND an=%s AND serie=%s""",
								temp1['link_disc'],row['an'],row['serie'])
			temp2=cursor.fetchone()
			input['titular_curs'] = temp2['nume']
			cursor.execute ("""SELECT nume, link_fac, sef FROM catedre
								WHERE cat_id=%s""", temp1['link_cat'])
			temp2=cursor.fetchone()
			input['catedra'] = temp2['nume']
			input['sef_catedra'] = temp2['sef']
			cursor.execute ("""SELECT nume, decan FROM facultati
								WHERE fac_id=%s""", temp2['link_fac'])
			temp2=cursor.fetchone()
			input['facultate'] = temp2['nume']
			input['decan'] = temp2 ['decan']
			i = 0
		
		cursor.execute ("""SELECT link_cat, nume_scurt
						FROM discipline WHERE disc_id=%s""",row['link_disc'])
		temp1=cursor.fetchone()
		cursor.execute ("""SELECT link_fac
								WHERE cat_id=%s""", temp1['link_cat'])
		temp2=cursor.fetchone()
		cursor.execute ("""SELECT nume_scurt FROM facultati
								WHERE fac_id=%s""", temp2['link_fac'])
		temp2=cursor.fetchone()
		input['ore'].append(build_course(row),temp1['nume_scurt'],temp2['nume_scurt'])
	
	vacante=[]
	cursor.execute(""" SELECT data_start, data_stop FROM vacante""")
	result_set = cursor.fetchall ()
	for row in result_set:
		vacante.append(row['data_start'],row['data_stop'])
	cursor.close ()
	conn.close ()
	
	print str(input)
	print "-"*30
	print str(vacante)
	
	
	
	
def build_course(row, clas, fac):
	course = {'functie': "", 'nr_post': 0, 'facultate': "", 'zi': 0,
               'tip':"", 'disciplina': "", 'grupa': "",
               'ore': ""}
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
	

if __name__ == "__main__":
        gather_data('Mihai Capota',2000,11)

        
        
