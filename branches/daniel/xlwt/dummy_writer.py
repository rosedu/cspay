#! /usr/bin/env python
#uses xlwt module

import xlwt as pycel
STYLE_FACTORY = {}
FONT_FACTORY = {}
	
def write(ws, row, col, data, style=None):
	"""
	Write data to row, col of worksheet (ws) using the style
	information.
	Again, I'm wrapping this because you'll have to do it if you
	create large amounts of formatted entries in your spreadsheet
	(else Excel, but probably not OOo will crash).
	"""
	if style:
		s = get_style(style)
		ws.write(row, col, data, s)
	else:
		ws.write(row, col, data)
		
def get_style(style):
	"""
	Style is a dict maping key to values.
	Valid keys are: background, format, alignment, border
	The values for keys are lists of tuples containing (attribute,
	value) pairs to set on model instances...
	"""
	#print "KEY", style
	style_key = tuple(style.items())
	s = STYLE_FACTORY.get(style_key, None)
	if s is None:
		s = pycel.XFStyle()
		for key, values in style.items():
			if key == "background":
				p = pycel.Pattern()
				for attr, value in values:
					p.__setattr__(attr, value)
				s.pattern = p
			elif key == "format":
				s.num_format_str = values
			elif key == "alignment":
				a = pycel.Alignment()
				for attr, value in values:
					a.__setattr__(attr, value)
				s.alignment = a
			elif key == "border":
				b = pycel.Formatting.Borders()
				for attr, value in values:
					b.__setattr__(attr, value)
				s.borders = b
			elif key == "font":
				f = get_font(values)
				s.font = f
		STYLE_FACTORY[style_key] = s
	return s
	
def get_font(values):
	"""
	'height' 10pt = 200, 8pt = 160
	"""
	font_key = values
	f = FONT_FACTORY.get(font_key, None)
	if f is None:
		f = pycel.Font()
		for attr, value in values:
			f.__setattr__(attr, value)
		FONT_FACTORY[font_key] = f
	return f
	
def initializeWB(coding):
	"""Initialize a WorkBook with a certain encoding"""
	wb = pycel.Workbook(encoding=coding)
	return wb
	
def initializeWS(wb,name):
	"""Initialize a WorkSheet with a certain name"""
	ws = wb.add_sheet(name)
	ws.fit_num_pages = 1
	ws.fit_height_to_pages = 1
	ws.fit_width_to_pages = 1
	ws.paper_size_code = 9
	ws.footer_str= ""
	ws.header_str='&F'
	ws.portrait = 1
	return ws
	
def finish(wb, title):
	"""Export the WorkBook"""
	wb.save(title+".xls")
	
def formula(str):
	"""Conver formula from string to internal format"""
	return pycel.Formula(str)
	
def boxed(i, v_center=0, wrap=0, h_center=1):
	"""Set borders: 
	    i=1 for thin border
		i=2 for thick border
		Set other values to 1 to enable
	"""
	result = {"border": (("bottom",i),("top",i),("left",i),("right",i))}
	lista=[]
	if wrap:
		lista.append(("wrap", pycel.Alignment.WRAP_AT_RIGHT))
	if v_center:
		lista.append(("vert", pycel.Alignment.VERT_CENTER))
	if h_center:
		lista.append(("horz", pycel.Alignment.HORZ_CENTER))
	if list:
		result.update({"alignment":tuple(lista)})
	return result
