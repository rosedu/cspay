from datetime import date, timedelta
import simple as qq

C_col="E"
A_col="F"

def output_table(input, excluded):
	course_count = {'prof': [], 'conf': [], 'sl': [], 'as': [] }
	apps_count = {'prof': [], 'conf': [], 'sl': [], 'as': [] }
	i = 0
	
	F, S = qq.initialize()
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
	qq.finish(F, title)
	
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
	qq.write(ws, 0, 0, "Universitatea  Politehnica  Bucuresti")
	qq.write(ws, 0, 6, "Catedra " + location)
	qq.write(ws, 1, 0, "Facultatea Automatica si Calculatoare")
	qq.write(ws, 1, 6, "Luna " + ro_month(month))
	qq.write(ws, 3, 2, "Situatia orelor efectuate de")
	qq.write(ws, 3, 5, position + ' ' + teacher)
	qq.write(ws, 4, 2, "    cu functia de baza")
	qq.write(ws, 4, 5, "la")
	qq.write(ws, 4, 6, basecourse)
	qq.write(ws, 6, 0, "Nr. crt.", qq.boxed(2))
	qq.write(ws, 6, 1, "Felul si nr. postului", qq.boxed(2))
	qq.write(ws, 6, 2, "Facultatea", qq.boxed(2))
	qq.write(ws, 6, 3, "Disciplina", qq.boxed(2))
	qq.write(ws, 6, 4, "Curs", qq.boxed(2))
	qq.write(ws, 6, 5, "Aplicatii", qq.boxed(2))
	qq.write(ws, 6, 6, "An/Grupa", qq.boxed(2))
	qq.write(ws, 6, 7, "Data", qq.boxed(2))
	qq.write(ws, 6, 8, "Orele", qq.boxed(2))

	
def insert_course(ws, i, curs, year, month, excluded):
	"""Insert a line for every application for a given month"""
	d = date(year, month, 1)
	format = dateform()
	format.update(qq.boxed(1))

	if d.weekday() > curs.day():
		d = d + timedelta(days=(7 + curs.day() - d.weekday()))
	else:
		d = d + timedelta(days=(curs.day() - d.weekday()))

	if ws.col(3).width < (len(curs.name()) * 256):
		ws.col(3).width = len(curs.name()) * 256
		
	while d.month() == month:
		if d not in excluded:
			qq.write(ws, 7 + i, 0, i + 1)
			qq.write(ws, 7 + i, 1, curs.code())
			qq.write(ws, 7 + i, 2, curs.fac())
			qq.write(ws, 7 + i, 3, curs.duration())
			qq.write(ws, 7 + i, 5, curs.group())
			qq.write(ws, 7 + i, 6, d, {"format":'D-MMM'})
			qq.write(ws, 7 + i, 7, curs.hours())
			i = i + 1
		d=d+timedelta(days=7)
	return i
		
def insert_lab(ws, i, curs, year, month, excluded, count):
	"""Insert a line for every application for a given month"""
	d = date(year, month, 1)
	format = dateform()
	format.update(qq.boxed(1))
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
			qq.write(ws, 7 + i, 0, i + 1, qq.boxed(1))
			qq.write(ws, 7 + i, 1, curs['poz'] + str(curs['no']), qq.boxed(1))
			qq.write(ws, 7 + i, 2, curs['fac'], qq.boxed(1))
			qq.write(ws, 7 + i, 3, curs['name'], qq.boxed(1))
			qq.write(ws, 7 + i, 5, curs['duration'], qq.boxed(1))
			qq.write(ws, 7 + i, 4, "", qq.boxed(1))
			qq.write(ws, 7 + i, 6, curs['group'], qq.boxed(1))
			qq.write(ws, 7 + i, 7, d, format)
			qq.write(ws, 7 + i, 8, interval, qq.boxed(1))
			i= i + 1
		d=d + timedelta(days=7)
	
	add_sum(count, curs['poz'], 8 + j, 8 + i - 1)
	return i

def write_totals(S, i, C, A):
	"""Write the totals for a table"""
	qq.write(ws, i + 2, 1, "TOTAL ore:", qq.boxed(2))
	qq.write(ws, i + 2, 2, "Curs:", qq.boxed(2))
	qq.write(ws, i + 2, 3, "Nr. ore:", qq.boxed(2))
	qq.write(ws, i + 2, 4, "Aplicatii:", qq.boxed(2))
	qq.write(ws, i + 2, 5,"Nr. ore:", qq.boxed(2))
	posts=["Prof", "Conf", "S.L.", "AS"]
	internal=["prof", "conf", "sl", "as"]

	for j in [2, 4]:
		for post in posts:
			qq.write(ws, i + 3 + posts.index(post), j, post, qq.boxed(1))

	for j in range(0, 4):
		qq.write(ws, i+ 3 + j, 3,
                         qq.formula(form_sum(course_count[internal[j]], C_col)),
                         qq.boxed(1))
		qq.write(ws, i + 3+ j, 5,
                         qq.formula(form_sum(apps_count[internal[j]], A_col)),
                         qq.boxed(1))

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
        lab3={ 'poz': 'as', 'no':5 7, 'fac': 'A & C', 'day': 1,
               'name': 'Utiliz Sist Operare', 'duration': 2, 'group': '322CA',
               'hour': 11}

        wb, ws = qq.initialize()
        writeheader(ws, "Calculatoare", "Utilizarea Sitemelor de Operare",
			"Deaconescu Razvan", "ing.", 10)
        i=insert_lab(ws, 0, lab1, 2009, 10, exclusion, apps_count)
        i=insert_lab(ws, i, lab2, 2009, 10, exclusion, apps_count)
        i=insert_lab(ws, i, lab3, 2009, 10, exclusion, apps_count)
        write_totals(ws, i+7, 2, 3)
        qq.finish(wb, "ceva")
