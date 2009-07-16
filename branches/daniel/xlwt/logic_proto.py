# -*- coding: cp1250 -*-
from datetime import date, timedelta
import dummy_writer as dw

C_col="E"
A_col="F"
luni_ro = [ "ianuarie", "februarie", "martie", "aprilie", "mai", "iunie",
         "iulie", "august", "septembrie", "octombrie", "noiembrie",
         "decembrie" ]

def output_table(input, holidays, parities):

	F= dw.initializeWB("cp1250")
	months = input['luni']
	year1, year2 = input['an'].split('/')
	title = year1 + "-" + year2 + " " + input['functie_baza'] + " "
	title += input['profesor']
	for month in months:
		if month < 8:
			year = int(year2)
		else:
			year = int(year1)
		i = 0
		C_count = {'prof': [], 'conf': [], 'sl': [], 'as': [] }
		A_count = {'prof': [], 'conf': [], 'sl': [], 'as': [] }
		S = dw.initializeWS(F, ro_month(month).capitalize())
		write_header(S, input['universitate'], input['facultate'],
                             input['catedra'], input['functie_baza'],
                             input['profesor'], input['statut'], month)
		
		courses = input['ore']
		for curs in courses:
			i = insert_course(S, i, curs, year, month,
                                          input['semestru'], holidays, 
										  parities, C_count, A_count)
		write_totals(S, i + 7, C_count, A_count)
		write_footer(S, i + 16, input['profesor'],
                             input['titular_curs'], input['sef_catedra'],
                             input['decan'])
	dw.finish(F, title)

	
def write_header(ws, univ, faculty, department, basecourse, teacher, position,
                 month):
	"""Insert the header of the table""" 
	ws.col(0).width = 4*256
	for i in [1,2,3,6,7,8]:
		ws.col(i).width = 10*256
	ws.col(4).width=9*256
	ws.col(5).width=9*256
	dw.write(ws, 0, 0, univ)
	dw.write(ws, 0, 6, "Catedra " + department)
	dw.write(ws, 1, 0, faculty)
	dw.write(ws, 1, 6, "Luna " + ro_month(month))
	dw.write(ws, 3, 3, "Situaţia orelor efectuate de")
	dw.write(ws, 3, 5, position + " " + teacher)
	dw.write(ws, 4, 3, "cu funcţia de bază")
	dw.write(ws, 4, 5, "la " + basecourse)
	dw.write(ws, 6, 0, "Nr. crt.", dw.boxed(2,1,1))
	dw.write(ws, 6, 1, "Felul şi nr. postului", dw.boxed(2,1,1))
	dw.write(ws, 6, 2, "Facultatea", dw.boxed(2,1))
	dw.write(ws, 6, 3, "Disciplina", dw.boxed(2,1))
	dw.write(ws, 6, 4, "Curs", dw.boxed(2,1))
	dw.write(ws, 6, 5, "Aplicaţii", dw.boxed(2,1))
	dw.write(ws, 6, 6, "An/Grupa", dw.boxed(2,1))
	dw.write(ws, 6, 7, "Data", dw.boxed(2,1))
	dw.write(ws, 6, 8, "Orele", dw.boxed(2,1))


def insert_course(ws, i, curs, year, month, semester, holidays, parit,
                  C_count, A_count):
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
		if (inside(d, semester) and
                    parit[d.isocalendar()[1]] and
                    ( curs['parit'] == 1 or
                      ( parit[d.isocalendar()[1]] % curs['parit']
                        == curs['pari_st']))
                    and outside(d, holidays)) :
			dw.write(ws, 7 + i, 0, i + 1, dw.boxed(1,0,0,0))
			dw.write(ws, 7 + i, 1,
                                 curs['functie'] + str(curs['nr_post']),
                                 dw.boxed(1))
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
	dw.write(ws, i, 7, "Total:")
	dw.write(ws, i, 8, dw.formula("SUM(E8:F" + str(i) + ")"))
	dw.write(ws, i + 2, 3, "TOTAL ore:", dw.boxed(2))
	dw.write(ws, i + 2, 4, "Curs:", dw.boxed(2))
	dw.write(ws, i + 2, 5, "Aplicaţii:", dw.boxed(2))
	posts=["Prof", "Conf", "S.l.", "As"]
	internal=["prof", "conf", "sl", "as"]

	for post in posts:
		dw.write(ws, i + 3 + posts.index(post), 3, post, dw.boxed(1))

	for j in range(0, 4):
		dw.write(ws, i+ 3 + j, 4,
                         dw.formula(form_sum(C[internal[j]], C_col)),
                         dw.boxed(1))
		dw.write(ws, i + 3+ j, 5,
                         dw.formula(form_sum(A[internal[j]], A_col)),
                         dw.boxed(1))
						 
						 
def write_footer(ws, i, teacher, titular, dept_head, dean):
	"""Write the footer for the table"""
	dw.write(ws, i , 0, "Întocmit,")
	dw.write(ws, i , 3, "Titular curs,")
	dw.write(ws, i , 5, "Şef catedră,")
	dw.write(ws, i , 7, "Decan,")
	dw.write(ws, i + 1, 0, teacher)
	if ws.col(3).width < (len(titular) * 256):
		ws.col(3).width = len(titular) * 256
	dw.write(ws, i + 1, 3, titular)
	dw.write(ws, i + 1, 5, dept_head)
	dw.write(ws, i + 1, 7, dean)

	
						 
def ro_month(i):
	"""Get the Romanian name for a certain month"""
	return luni_ro[i - 1]
						 
def dateform():
	"""Return desired date format."""
	return {"format": 'D-MMM'}

	
def add_sum(count, poz, i, j):
	"""Calculate the ranges for the sum of a given teaching position"""
	z = count[poz]
	if z and z[-1].count(i - 1):
		t = count[poz].pop()
		count[poz].append([t[0], j])
	else:
		count[poz].append([i, j])

		
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
	
def inside(d, range):
	"""Check whether a date is inside a range"""
	d1, d2 = range
	if (d1 <= d) and (d <= d2):
		return 1
	return 0
			
