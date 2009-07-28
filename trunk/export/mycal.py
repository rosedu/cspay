# -*- coding: utf8 -*-
from icalendar import Calendar, Event, Timezone, vRecur, vDatetime, StandardT
from icalendar import DaylightT, UTC, UIDGenerator
from datetime import datetime, timedelta

days = ['MO','TU','WE','TH', 'FR', 'SA','SU']

class Entry():
        def __init__(self, course):
                day = days[course['zi']]
                interv = str(course['parit'])
                what = course['disciplina'] + " (" + course['tip'] + ") - "
                what += course['grupa']
                where = course['sala']
                x, y = course['ore'].split('-')
                self.str = int(x)
                far = int(y) - int(x)
                g = UIDGenerator()
                uid = g.uid('CSPay')
        
                self.ev = Event()
                self.ev.add('summary', what)
                self.ev.add('location', where)
                self.ev.add('tzid', 'Europe/Bucharest')
                self.ev.add('duration', timedelta(hours = far))
                self.ev.add('uid',uid)
                self.rpt = ('FREQ=WEEKLY;INTERVAL='+interv+';BYDAY='+day+
                            ';UNTIL=')
                self.exdate = []

        def add_start(self, d):
                self.ev.add('dtstart', datetime(d.year, d. month, d.day,
                                                self.str, 0, 0))

        def add_end(self, d):
                end = vDatetime(datetime(d.year, d. month, d.day,
                                         12, 0, 0, tzinfo = UTC))
                self.rpt += end.ical()
                self.ev.add('rrule',vRecur.from_ical(self.rpt))

        def add_except(self, ds, de):
                self.exdate.append(datetime(ds.year, ds.month, ds.day,
                                       self.str, 0, 0))
                d = ds + timedelta(days=7)
                while d < de:
                        self.exdate.append(datetime(d.year, d.month, d.day,
                                               self.str, 0, 0))
                        d = d + timedelta(days=7)
                print str(self.exdate)
                
		
        def get_entry(self):
                if self.exdate:
                        self.ev.add('exdate', [self.exdate])
                return self.ev


def output_ical(input, holidays, parities, path):

	C = ical_init()

	year1, year2 = input['an'].split('/')
	when = str(year1) + "-" + str (year2) 
	try:
		who = unicode(input['profesor'], "utf8")
	except TypeError:
		who = input['profesor']
	title = "Orar " + who + ' ' + when
	title = path+title.replace(" ","_")
	
	start, stop = input['semestru']
	
	courses = input['ore']

	for curs in courses:
		i = insert_course(C, curs, holidays, parities, start, stop)

	ical_write(C, title)



def insert_course(C, curs, holidays, parit, start, stop):
	"""Insert a line for every application for a given month"""
	d = start
	h1, h2 = curs['ore'].split('-')
	H = int(h1)
	p = 0

	while True:
		if d > stop:
			break
		t = 1
		d = get_next_wday(curs['zi'], d)
		while True:
			if d > stop:
				t = 2
				break
			else:
				d1 = in_holiday(d, holidays)
				if d1:
					break
				elif ((not p) and
                                      (curs['parit'] == 1 or
                                       (parit[d.isocalendar()[1]]
                                        % curs['parit'] == curs['pari_st'])
                                       )
                                      ):
					t = 0
					break
				d = d + timedelta( days = 7)
		
		if t == 1 and p:
			E.add_except(d, d1)
			d = d1
		elif t == 2 and p:
			if stop.weekday() == curs['zi']:
				E.add_end(stop + timedelta(days=1))
			else:
				E.add_end(stop)
			C.add_component(E.get_entry())
		else:
			E = Entry(curs)
			E.add_start(d)
			p = 1

					
def get_next_wday(which, after):
	if after.weekday() == which:
		return after
	else:
		if after.weekday() > which:
			d = after + timedelta(days=
                                              (7 + which - after.weekday()))
		else:
			d = after +timedelta(days=(which - after.weekday()))
		return d
		
def ical_init():
	cal = Calendar()
	cal.add('prodid', '-//CSPay calendar//EN/')
	cal.add('version', '2.0')
	TZ = Timezone()
	TZ.add('tzid','Europe/Bucharest')
	
	TZS = StandardT()
	TZS.add('TZOFFSETFROM',timedelta(hours=3))
	TZS.add('TZOFFSETTO',timedelta(hours=2))
	TZS.add('TZNAME','EET')
	TZS.add('DTSTART',datetime(1997,10,26))
	TZS.add('rrule',vRecur.from_ical('FREQ=YEARLY;BYMONTH=10;BYDAY=-1SU'))
	TZ.add_component(TZS)

	TZS = DaylightT()
	TZS.add('TZOFFSETFROM',timedelta(hours=2))
	TZS.add('TZOFFSETTO',timedelta(hours=3))
	TZS.add('TZNAME','EEST')
	TZS.add('DTSTART',datetime(1997,03,30))
	TZS.add('rrule',vRecur.from_ical('FREQ=YEARLY;BYMONTH=3;BYDAY=-1SU'))
	TZ.add_component(TZS)
	
	cal.add_component(TZ)
	
	return cal

def ical_write(C, title):
	f = open(title+'.ics', 'wb')
	f.write(C.as_string())
	f.close()
						 
def in_holiday(d, date_ranges):
	"""Check whether a date is inside a range of dates"""
	for d1, d2 in date_ranges:
		if (d1 <= d) and (d <= d2):
			return d2 + timedelta(days = 1)
	return 0
	
def inside(d, range):
	"""Check whether a date is inside a range"""
	d1, d2 = range
	if (d1 <= d) and (d <= d2):
		return 1
	return 0
			
