# -*- coding: cp1250 -*-
from datetime import date, timedelta
import dummy_writer as dw

C_col="E"
A_col="F"

def output_table(input, holidays):
	title = str(input['an']) + " " + input['functie_baza'] + " " + input['profesor']

	F= dw.initializeWB("cp1250")
	months = input['luni']
	for month in months:
		i = 0
		C_count = {'prof': [], 'conf': [], 'sl': [], 'as': [] }
		A_count = {'prof': [], 'conf': [], 'sl': [], 'as': [] }
		S = dw.initializeWS(F, ro_month(month).capitalize())
		write_header(S, input['facultate'], input['catedra'], input['functie_baza'],
                             input['profesor'], input['statut'], month)
		
		courses = input['ore']
		for curs in courses:
			i = insert_course(S, i, curs, input['an'], month, holidays, C_count,
                                          A_count)
		write_totals(S, i + 7, C_count, A_count)
		write_footer(S, i + 16, input['profesor'], input['titular_curs'], input['sef_catedra'], input['decan'])
	dw.finish(F, title)

	
def write_header(ws, faculty, department, basecourse, teacher, position, month):
	"""Insert the header of the table""" 
	ws.col(0).width = 4*256
	for i in range(1,8):
		ws.col(i).width = 12*256
	dw.write(ws, 0, 0, "Universitatea Politehnică Bucureşti")
	dw.write(ws, 0, 6, "Catedra " + department)
	dw.write(ws, 1, 0, "Facultatea de " + faculty)
	dw.write(ws, 1, 6, "Luna " + ro_month(month))
	dw.write(ws, 3, 2, "Situatia orelor efectuate de")
	dw.write(ws, 3, 5, position + " " + teacher)
	dw.write(ws, 4, 2, "cu functia de baza")
	dw.write(ws, 4, 5, "la " + basecourse)
	dw.write(ws, 6, 0, "Nr. crt.", dw.boxed(2,1,1))
	dw.write(ws, 6, 1, "Felul si nr. postului", dw.boxed(2,1,1))
	dw.write(ws, 6, 2, "Facultatea", dw.boxed(2,1))
	dw.write(ws, 6, 3, "Disciplina", dw.boxed(2,1))
	dw.write(ws, 6, 4, "Curs", dw.boxed(2,1))
	dw.write(ws, 6, 5, "Aplicatii", dw.boxed(2,1))
	dw.write(ws, 6, 6, "An/Grupa", dw.boxed(2,1))
	dw.write(ws, 6, 7, "Data", dw.boxed(2,1))
	dw.write(ws, 6, 8, "Orele", dw.boxed(2,1))


def insert_course(ws, i, curs, year, month, holidays, C_count, A_count):
	"""Insert a line for every application for a given month"""
	d = date(year, month, 1)
	format = dateform()
	format.update(dw.boxed(1))
	start, stop = curs['ore'].split('-')
	lenght = int(stop) - int(start)
	j = i

	if d.weekday()>curs['zi']:
		d = d + timedelta(days=(7 + curs['zi'] - d.weekday()))
	else:
		d = d +timedelta(days=(curs['zi'] - d.weekday()))

	if ws.col(3).width < (len(curs['disciplina']) * 256):
		ws.col(3).width = len(curs['disciplina']) * 256

	while d.month == month:
		if outside(d, holidays):
			dw.write(ws, 7 + i, 0, i + 1, dw.boxed(1,0,0,0))
			dw.write(ws, 7 + i, 1, curs['functie'] + str(curs['nr_post']), dw.boxed(1))
			dw.write(ws, 7 + i, 2, curs['facultate'], dw.boxed(1))
			dw.write(ws, 7 + i, 3, curs['disciplina'], dw.boxed(1))
			if curs['tip'] == "C":
				dw.write(ws, 7 + i, 4, lenght, dw.boxed(1))
				dw.write(ws, 7 + i, 5, "", dw.boxed(1))
			else:
				dw.write(ws, 7 + i, 5, lenght, dw.boxed(1))
				dw.write(ws, 7 + i, 4, "", dw.boxed(1))
			dw.write(ws, 7 + i, 6, curs['grupa'], dw.boxed(1))                                
			dw.write(ws, 7 + i, 7, d, format)
			dw.write(ws, 7 + i, 8, curs['ore'], dw.boxed(1))
			i= i + 1
		d=d + timedelta(days=7)

	if curs['tip'] == "C":
		add_sum(C_count, curs['functie'], 8 + j, 8 + i - 1)
	else:
		add_sum(A_count, curs['functie'], 8 + j, 8 + i - 1)
	return i


def write_totals(ws, i, C, A):
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
                         dw.formula(form_sum(C[internal[j]], C_col)),
                         dw.boxed(1))
		dw.write(ws, i + 3+ j, 5,
                         dw.formula(form_sum(A[internal[j]], A_col)),
                         dw.boxed(1))
						 
						 
def write_footer(ws, i, teacher, titular, dept_head, dean):
	dw.write(ws, i , 0, "Întocmit,")
	dw.write(ws, i , 3, "Titular curs,")
	dw.write(ws, i , 4, "Şef catedră,")
	dw.write(ws, i , 7, "Decan,")
	dw.write(ws, i + 1, 0, teacher)
	if ws.col(3).width < (len(titular) * 256):
		ws.col(3).width = len(titular) * 256
	dw.write(ws, i + 1, 3, titular)
	dw.write(ws, i + 1, 4, dept_head)
	dw.write(ws, i + 1, 7, dean)

	
						 
def ro_month(i):
	"""Get the Romanian name for a certain month"""
	luni = [ "ianuarie", "februarie", "martie", "aprilie", "mai", "iunie",
                 "iulie", "august", "septembrie", "octombrie", "noiembrie",
                 "decembrie" ]
	return luni.pop(i - 1)
						 
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

def outside(d, date_ranges):
	"""Check whether a date is inside a range of dates"""
	for d1, d2 in date_ranges:
		if (d1 <= d) and (d <= d2):
			return 0
	return 1
			