from datetime import date, timedelta
import dummy_writer as dw

C_col="E"
A_col="F"

def output_table(input, excluded):
	course_count = {'prof': [], 'conf': [], 'sl': [], 'as': [] }
	apps_count = {'prof': [], 'conf': [], 'sl': [], 'as': [] }
	i = 0
	
	F, S = dw.initialize()
	write_header(S, input.get_where(), input.get_bcourse(),
                     input.get_teacher_name(), input.get_teacher_position(),
                     input.get_month())
	
	courses = input.get_courses()
	for curs in courses:
		i = insert_course(S, i, curs, input.get_year(), input.get_month(), excluded,
                                  course_count)
	labs = input.get_labs()
	for lab in labs:
		i = insert_lab(S, i, lab, input.get_year(), input.get_month(), excluded,
                               apps_count)
	write_totals(S, i, input.get_C_post(), get_A_post(), course_count, apps_count)
	write_footer(S, input.get_teacher(), input.get_where())
	dw.finish(F, title)
	
def ro_month(i):
	"""Get the Romanian name for a certain month"""
	luni = [ "ianuarie", "februarie", "martie", "aprilie", "mai", "iunie",
                 "iulie", "august", "septembrie", "octombrie", "noiembrie",
                 "decembrie" ]
	return luni.pop(i - 1)
			 
	

	
def writeheader(ws, location, basecourse, teacher, position, month):
	"""Insert the header of the table""" 
	ws.col(0).width = 4*256
	for i in range(4,8):
		ws.col(i).width = 9*256
	dw.write(ws, 0, 0, "Universitatea  Politehnica  Bucuresti")
	dw.write(ws, 0, 6, "Catedra " + location)
	dw.write(ws, 1, 0, "Facultatea Automatica si Calculatoare")
	dw.write(ws, 1, 6, "Luna " + ro_month(month))
	dw.write(ws, 3, 2, "Situatia orelor efectuate de")
	dw.write(ws, 3, 5, position + ' ' + teacher)
	dw.write(ws, 4, 2, "    cu functia de baza")
	dw.write(ws, 4, 5, "la")
	dw.write(ws, 4, 6, basecourse)
	dw.write(ws, 6, 0, "Nr. crt.", dw.boxed(2))
	dw.write(ws, 6, 1, "Felul si nr. postului", dw.boxed(2))
	dw.write(ws, 6, 2, "Facultatea", dw.boxed(2))
	dw.write(ws, 6, 3, "Disciplina", dw.boxed(2))
	dw.write(ws, 6, 4, "Curs", dw.boxed(2))
	dw.write(ws, 6, 5, "Aplicatii", dw.boxed(2))
	dw.write(ws, 6, 6, "An/Grupa", dw.boxed(2))
	dw.write(ws, 6, 7, "Data", dw.boxed(2))
	dw.write(ws, 6, 8, "Orele", dw.boxed(2))

	
def insert_course(ws, i, curs, year, month, excluded):
	"""Insert a line for every application for a given month"""
	d = date(year, month, 1)
	format = dateform()
	format.update(dw.boxed(1))

	if d.weekday() > curs.day():
		d = d + timedelta(days=(7 + curs.day() - d.weekday()))
	else:
		d = d + timedelta(days=(curs.day() - d.weekday()))

	if ws.col(3).width < (len(curs.name()) * 256):
		ws.col(3).width = len(curs.name()) * 256
		
	while d.month() == month:
		if d not in excluded:
			dw.write(ws, 7 + i, 0, i + 1)
			dw.write(ws, 7 + i, 1, curs.code())
			dw.write(ws, 7 + i, 2, curs.fac())
			dw.write(ws, 7 + i, 3, curs.duration())
			dw.write(ws, 7 + i, 5, curs.group())
			dw.write(ws, 7 + i, 6, d, {"format":'D-MMM'})
			dw.write(ws, 7 + i, 7, curs.hours())
			i = i + 1
		d=d+timedelta(days=7)
	return i
		
def insert_lab(ws, i, curs, year, month, excluded, count):
	"""Insert a line for every application for a given month"""
	d = date(year, month, 1)
	format = dateform()
	format.update(dw.boxed(1))
	interval = str(curs['hour']) + "-" + str(curs['hour'] + curs['duration'])	
	j = i

	if d.weekday()>curs['day']:
		d = d + timedelta(days=(7 + curs['day'] - d.weekday()))
	else:
		d = d +timedelta(days=(curs['day'] - d.weekday()))

	if ws.col(3).width < (len(curs['name']) * 256):
		ws.col(3).width = len(curs['name']) * 256
	

	while d.month == month:
		if d not in excluded:
			dw.write(ws, 7 + i, 0, i + 1, dw.boxed(1))
			dw.write(ws, 7 + i, 1, curs['poz'] + str(curs['no']), dw.boxed(1))
			dw.write(ws, 7 + i, 2, curs['fac'], dw.boxed(1))
			dw.write(ws, 7 + i, 3, curs['name'], dw.boxed(1))
			dw.write(ws, 7 + i, 5, curs['duration'], dw.boxed(1))
			dw.write(ws, 7 + i, 4, "", dw.boxed(1))
			dw.write(ws, 7 + i, 6, curs['group'], dw.boxed(1))
			dw.write(ws, 7 + i, 7, d, format)
			dw.write(ws, 7 + i, 8, interval, dw.boxed(1))
			i= i + 1
		d=d + timedelta(days=7)
	
	add_sum(count, curs['poz'], 8 + j, 8 + i - 1)
	return i

def write_totals(S, i, C, A):
	"""Write the totals for a table"""
	dw.write(ws, i + 2, 1, "TOTAL ore:", dw.boxed(2))
	dw.write(ws, i + 2, 2, "Curs:", dw.boxed(2))
	dw.write(ws, i + 2, 3, "Nr. ore:", dw.boxed(2))
	dw.write(ws, i + 2, 4, "Aplicatii:", dw.boxed(2))
	dw.write(ws, i + 2, 5,"Nr. ore:", dw.boxed(2))
	posts=["Prof", "Conf", "S.L.", "AS"]
	internal=["prof", "conf", "sl", "as"]

	for j in [2, 4]:
		for post in posts:
			dw.write(ws, i + 3 + posts.index(post), j, post, dw.boxed(1))

	for j in range(0, 4):
		dw.write(ws, i+ 3 + j, 3,
                         dw.formula(form_sum(course_count[internal[j]], C_col)),
                         dw.boxed(1))
		dw.write(ws, i + 3+ j, 5,
                         dw.formula(form_sum(apps_count[internal[j]], A_col)),
                         dw.boxed(1))

def dateform():
	"""Return desired date format."""
	return {"format": 'D-MMM'}
	
def add_sum(count, poz, i, j):
	"""Calculate the ranges for the sum of a given teaching position"""
	z = count[poz]
	if z and z[-1].count(i-1):
		t = count[poz].pop()
		count[poz].append([t[0],j])
	else:
		count[poz].append([i,j])

		
def form_sum(list, col):
	"""Transform a sum given as a series of ranges in an Excel formula"""
	result = ""
	if list:
		result += "SUM("
		for x in list:
			if result[-1] != '(':
				result += ";"
			result += col + str(x[0]) + ":" + col + str(x[1])
		result += ")"
	else:
		result = "0"
	
	return result

	
if __name__ == "__main__":
        course_count = {'prof': [], 'conf': [], 'sl': [], 'as': []}
        apps_count = {'prof': [], 'conf': [], 'sl': [], 'as': []}
        exclusion=[date(2009, 10, 14), date(2009, 10,15)]
        lab1= {'poz': 'as', 'no': 87, 'fac': 'A & C', 'day': 2,
               'name': 'Utiliz Sist Operare', 'duration': 2, 'group': '324CA',
               'hour': 12}
        lab2= {'poz': 'sl', 'no': 67, 'fac': 'A & C', 'day': 4,
               'name': 'Utiliz Sist Operare', 'duration': 3, 'group': '321CA',
               'hour': 10}
        lab3={ 'poz': 'as', 'no': 57, 'fac': 'A & C', 'day': 1,
               'name': 'Utiliz Sist Operare', 'duration': 2, 'group': '322CA',
               'hour': 11}

        wb, ws = dw.initialize()
        writeheader(ws, "Calculatoare", "Utilizarea Sitemelor de Operare",
			"Deaconescu Razvan", "ing.", 10)
        i=insert_lab(ws, 0, lab1, 2009, 10, exclusion, apps_count)
        i=insert_lab(ws, i, lab2, 2009, 10, exclusion, apps_count)
        i=insert_lab(ws, i, lab3, 2009, 10, exclusion, apps_count)
        write_totals(ws, i+7, 2, 3)
        dw.finish(wb, "ceva")
