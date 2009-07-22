#!/usr/bin/env python
# -*- coding: UTF-8 -*-

#script for xls to database insertion
import xlrd
import line_parser
import line_reader
import db_writer
import sys
import MySQLdb
#pointer to the xls file

print sys.getdefaultencoding()
################################################
#
# Verify if file exists and open it
#
################################################

file_name="model.xls"
file_exists=0
while file_exists==0:
    
    try:
        file_xls=xlrd.open_workbook(file_name)
        file_exists=1
    except:
        print "Error! File (.xls) not found !"
        print "If problems persist contact your local administrator !"
        file_name=raw_input("Enter the correct path to the xls file !!! ")
        file_exists=0



################################################
#
# Connection database
#
################################################

try:
    conn = MySQLdb.connect (host = "localhost", user = "cspay",
                                    passwd = "Chote3at", db = "rsoc_cspay")
except MySQLdb.Error, e:
    print "Error %d: %s" % (e.args[0], e.args[1])
    sys.exit (1)
cursor = conn.cursor (MySQLdb.cursors.DictCursor)

##################################################


################################################
#
# for each sheet ot the file get all line
#
################################################

index_line=2
#get sheet and pass it to read line

sheet = file_xls.sheet_by_index(0)
while index_line<sheet.nrows:
    
    print index_line," -------------->"
    line=line_reader.read_line(sheet,index_line)
    error_check=line_parser.parse(line,index_line) #if "OK" data is consistent
    if error_check=="Ok":
        print "Line ",index_line+2," is candidate to be inserted into DB"
        db_writer.db_write_line(cursor,line,index_line)  #pass the line and its number
    elif error_check=="Ignore":
        print "Line ",index_line+2," has been ignored >>> "
    else:
        print "Data missing on line :",
        print error_check+1," Sheet :",sheet.name
        print "Please correct XLS file"
        #index_sheet=file_xls.nsheets
        #break
    index_line+=1
input "Enter y to truncate table or any other to keep variables" yes
if yes=='y'
    cursor.execute("TRUNCATE TABLE ore")
print "\n Table has been truncated >>>"
#test read first line in xls file -> uncomment next line to test
#print "Line 0 : ",reader.read_line(xls_file,0,0)
print " \n\n\n\n\n\n"
print " ------------------------------------------> "
print " Database ore "
print " ------------------------------------------> "
print "\n\n"
#cursor.execute("Select * FROM ore")
#ore_print=cursor.fetchall()
#iii=0
#comment if no need to pring ore table contents
#for value in ore_print:
 #   if(iii==24):
  #      print "\n\n"
  #      iii=0
  #  print value
  #  iii+=1
    
cursor.close ()
conn.close()
