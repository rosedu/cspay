from datetime import date, timedelta
import simple as qq

def output_table(input, excluded):
	F,S=qq.initialize()
	write_header(S,input.get_where(),input.get_bcourse(),
				 input.get_teacher_name(),
				 input.get_teacher_position(),
				 input.get_month())
	i=0
	courses=input.get_courses()
	for curs in courses:
		i=insert_course(S,i,curs,input.get_year(),input.get_month(),excluded)
	labs=input.get_labs()
	for lab in labs:
		i=insert_lab(S,i,lab,input.get_year(),input,get_month(),excluded)
	write_totals(S,i,input.get_C_post(),get_A_post())
	write_footer(S,input.get_teacher(),input.get_where)
	qq.finish(F,title)
	
def ro_month(i):
	luni = [ "ianuarie", "februarie", "martie", "aprilie", "mai", "iunie",
			 "iulie", "august", "septembrie", "octombrie", "noiembrie",
			 "decembrie" ]
	return luni.pop(i-1)
			 
	

	
def writeheader(ws,location,basecourse,teacher,position,month):
	ws.col(0).width = 4*256
	for i in range(4,8):
		ws.col(i).width = 9*256
	qq.write(ws, 0, 0, "Universitatea  Politehnica  Bucuresti")
	qq.write(ws, 0, 6, "Catedra "+location)
	qq.write(ws, 1, 0, "Facultatea Automatica si Calculatoare")
	qq.write(ws, 1, 6, "Luna "+ ro_month(month))
	qq.write(ws, 3, 2, "Situatia orelor efectuate de")
	qq.write(ws, 3, 5, position+' '+teacher)
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
	
def insert_course(ws,i,curs,year,month,excluded):
	d= date(year,month,1)
	format=dateform()
	format.update(qq.boxed(1))

	if d.weekday()>curs.day():
		d=d+timedelta(days=(7+curs.day()-d.weekday()))
	else:
		d=d+timedelta(days=(curs.day()-d.weekday()))

	if ws.col(3).width < (len(curs,name())*256):
		ws.col(3).width = len(curs.name())*256
		
	while d.month()==month:
		if d not in excluded:
			qq.write(ws, 7+i, 0, i+1)
			qq.write(ws, 7+i, 1, curs.code())
			qq.write(ws, 7+i, 2, curs.fac())
			qq.write(ws, 7+i, 3, curs.duration())
			qq.write(ws, 7+i, 5, curs.group())
			qq.write(ws, 7+i, 6, d, {"format":'D-MMM'})
			qq.write(ws, 7+i, 7, curs.hours())
			i=i+1
		d=d+timedelta(days=7)
	return i
		
def insert_lab(ws,i,curs,year,month,excluded):
	d= date(year,month,1)
	format=dateform()
	format.update(qq.boxed(1))

	if d.weekday()>curs['day']:
		d=d+timedelta(days=(7+curs['day']-d.weekday()))
	else:
		d=d+timedelta(days=(curs['day']-d.weekday()))

	if ws.col(3).width < (len(curs['name'])*256):
		ws.col(3).width = len(curs['name'])*256

	while d.month==month:
		if d not in excluded:
			qq.write(ws, 7+i, 0, i+1, qq.boxed(1))
			qq.write(ws, 7+i, 1, curs['code'],qq.boxed(1))
			qq.write(ws, 7+i, 2, curs['fac'],qq.boxed(1))
			qq.write(ws, 7+i, 3, curs['name'],qq.boxed(1))
			qq.write(ws, 7+i, 5, curs['duration'],qq.boxed(1))
			qq.write(ws, 7+i, 4, "",qq.boxed(1))
			qq.write(ws, 7+i, 6, curs['group'],qq.boxed(1))
			qq.write(ws, 7+i, 7, d, format)
			qq.write(ws, 7+i, 8, curs['hours'],qq.boxed(1))
			i=i+1
		d=d+timedelta(days=7)
	return i

def write_totals(S,i,C,A):
	qq.write(ws, i+2, 1, "TOTAL ore:",qq.boxed(2))
	qq.write(ws, i+2, 2, "Curs:",qq.boxed(2))
	qq.write(ws, i+2, 3, "Nr. ore:",qq.boxed(2))
	qq.write(ws, i+2, 4, "Aplicatii:",qq.boxed(2))
	qq.write(ws, i+2, 5,"Nr. ore:",qq.boxed(2))
	posts=["Prof","Conf","S.L.","AS"]
	

	for j in [2,4]:
		for post in posts:
			qq.write(ws, i+3+posts.index(post), j, post,qq.boxed(1))

	course_totals="SUM(E8:E"+str(i)+")"
	apps_totals="SUM(F8:F"+str(i)+")"
	for j in range(0, 4):
		if C==j:
			qq.write(ws, i+3+j, 3, qq.formula(course_totals),qq.boxed(1))
		else:
			qq.write(ws, i+3+j, 3, "",qq.boxed(1))
	for j in range(0, 4):
		if A==j:
			qq.write(ws, i+3+j, 5, qq.formula(apps_totals),qq.boxed(1))
		else:
			qq.write(ws, i+3+j, 5, "",qq.boxed(1))
	

def dateform():
	return {"format":'D-MMM'}
	
if __name__ == "__main__":
	wb,ws=qq.initialize()
	writeheader(ws,"Calculatoare","Utilizarea Sitemelor de Operare",
			"Deaconescu Razvan","ing.",10)
	exclusion=[ date(2009,10,14),date(2009,10,15)]
	curs={ 'code': 'as87', 'fac':'A & C', 'day':2,'name':'Utiliz Sist Operare','duration':2,'group':'324CA','hours':'12-14'}
	i=insert_lab(ws,0,curs,2009,10,exclusion)
	write_totals(ws,i+7,2,3)
	qq.finish(wb,"ceva")
