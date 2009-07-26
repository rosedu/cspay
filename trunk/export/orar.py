# -*- coding: utf-8 -*-
from datetime import date, timedelta
import dummy_writer as dw

ro_days = ["Luni", "Marţi", "Miercuri", "Joi", "Vineri"]
		
def output_orar(input, path):

	F = dw.initializeWB("utf8")
	months = input['luni']
	year1, year2 = input['an'].split('/')
	when = str(year1) + "-" + str (year2) 

	try:
		who = unicode(input['profesor'], "utf8")
	except TypeError:
		who = input['profesor']
	title = "Orar" + ' ' + who + ' ' + when
	title = path + title.replace(" ","_")

	
	S = dw.initializeWS(F, input['profesor'], 1)
	write_header(S, input['universitate'], input['facultate'],
                             input['catedra'], 
                             input['profesor'], input['statut'])
	for i in range(0, 5):
		courses = get_day(input['ore'], i)
		if courses:
			courses.sort(cmp=order_by_h)
			insert_course(S, i, courses)
		else:
			jump(S, 7, 21, 2*i+1)
	dw.finish(F, title)
	
def write_header(ws, univ, faculty, department, teacher, position):
	"""Insert the header of the table""" 
	ws.col(0).width = 6*256
	for i in range(1, 12):
		ws.col(i).width = 15*256
	dw.write(ws, 0, 0, univ)
	dw.write(ws, 0, 4, "Catedra " + department)
	dw.write(ws, 1, 0, faculty)
	dw.write(ws, 3, 2, "Orarul pentru")
	dw.write(ws, 3, 3, position + " " + teacher)
	dw.set_row_h(ws, 6, 3)
	dw.write(ws, 6, 0, "Ora", dw.boxed(2,1,1))
	
	for i in range(0,5):
		ws.write_merge(6, 6, 2*i+1, 2*i + 2, ro_days[i], dw.faktup(2))

	for i in range(0, 14):
		dw.write(ws, 7+i, 0, str(i+8)+":00", dw.boxed(1, 1, h_align = 2))
		dw.set_row_h(ws, 7 + i, 3)


def insert_course(ws, i, courses):
	j = 2*i + 1
	h0 = 7
	t = 0
	loc = ()
	for curs in courses:
		a, b = curs['ore'].split("-")
		h1 = int(a) - 1
		h2 = int(b) - 1

		if h0 < h1:
			jump(ws, h0, h1, j)
	
		if t > 1:
				put_corner(ws, loc, t)
				t = 0	
		
		if curs['parit'] == 2:
			if curs['pari_st'] == 1:
				ws.write(h1, j, curs['disciplina'] + " (" + curs ['tip'] + ") \n"
                       + curs['grupa'] + " - " + curs['sala'], dw.half("up"))
				t = 1
				loc = (h1, j)
			else:
				ws.write(h2 - 1, j + 1, curs['disciplina'] + " (" + curs ['tip'] + ") \n"
                       + curs['grupa'] + " - " + curs['sala'], dw.half("down"))
				t += 2
				loc = (h1, j)
		else:
			ws.write_merge(h1, h2 - 1, j, j + 1, curs['disciplina'] + " (" + curs ['tip'] + ") \n"
                       + curs['grupa'] + " - " + curs['sala'], dw.faktup(2))
		h0 = h2
	
	if t > 1:
				put_corner(ws, loc, t)
	if h0 < 21:
		jump(ws, h0, 21, j)

def put_corner(ws, loc, t):
	x, y = loc
	ws.write(x, y + 1, "", dw.corner(0, t))
	ws.write(x + 1, y , "", dw.corner(1, t))
	
def jump(ws, h0, h1, j):
	for i in range(h0, h1 - 1):
		ws.write(i, j, "", dw.sideboxed(left=1))
		ws.write(i, j + 1, "", dw.sideboxed(right=1))
	if h1 == 21:
		ws.write(20, j, "", dw.sideboxed(left=1, end=1))
		ws.write(20, j + 1, "", dw.sideboxed(right=1, end=1))
	else:
		ws.write(h1 - 1, j, "", dw.sideboxed(left=1))
		ws.write(h1 - 1, j + 1, "", dw.sideboxed(right=1))

def get_day(courses, i):
	result=[]
	for course in courses:
		if course['zi'] == i:
			result.append(course)
	return result
	
def order_by_h(x, y):
	a, b = x['ore'].split("-")
	h1 = int(a)
	a, b = y['ore'].split("-")
	h2 = int(a)
	if h1 > h2:
		return 1
	elif h1 < h2:
		return -1
	elif x['pari_st'] < y['pari_st']:
		return 1
	elif x['pari_st'] > y['pari_st']:
		return -1
	else:
		return 0
