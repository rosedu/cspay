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

# Each file is checked between line numbers (ex: all lines,line 6,lines 5-10)
# all files -> start_check=0 , end_check=0
# line a -> start_check=a end_check<start_check
# lines a-b -> start_check=a and end_check=b where a<b
file_name="model.xls"
start_check=0   
end_check=0
nr_argv=0
for arg in sys.argv:
    nr_argv+=1
    
if nr_argv>1:
    file_name=sys.argv[1]
else :
    print """No filename entered !\n
            Default 'model.xls' will be used\n"""

if nr_argv>2:
    start_check=int(sys.argv[2])
else :
    print """Parameter passed is not integer ! \n
        Default settings will be used (all lines) !\n"""
if nr_argv>3:
     end_check=int(sys.argv[3])
else :
    print """Parameter passed is not integer ! \n
        Default settings will be used !\n"""

print " Parameters ------------------------ >>> "
print " FILENAME ",file_name
print " START CHECK ",start_check
print " END CHECK ",end_check
print " CHECK WHOLE FILE : ",
if start_check==0:
    print "yes"
else:
    print "no"
print " --------------------------------->>>"
        
################################################
#
# Verify if file exists and open it
#
################################################
prev_error=0 #number of errors
file_exists=0
while file_exists==0:
    
    try:
        file_xls=xlrd.open_workbook(file_name)
        file_exists=1
    except:
        print "Error! File (.xls) not found "
        file_name=raw_input("Enter the correct path to the xls file ")
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
cursor.execute("TRUNCATE TABLE ore")
sheet = file_xls.sheet_by_index(0)
#get sheet and pass it to read line
if(end_check==0):
    end_check=start_check
if(start_check==0):
    start_check=3
    end_check=sheet.nrows
index_line=start_check-1

while index_line<end_check:
    line=line_reader.read_line(sheet,index_line)
    error_check=prev_error
    error_check=line_parser.parse(line,index_line,prev_error) #if "OK" data is consistent
    if(error_check<prev_error):
            print "[ LINE PARSE ] Error occured on line ",index_line
    if error_check==prev_error:
        prev_error=db_writer.db_write_line(cursor,line,index_line,prev_error)  #pass the line and its number
        if(error_check<prev_error):
            print "[ DB INSERT ] Error occured on line ",index_line
    #elif error_check=="Ignore":
   #     print "Line ",index_line+2," has been ignored "
   # elif (prev_error<3):
   #     print "\n\n   --------------------------------- \n    "
   #     print "Data missing on line :",
    #    print error_check+1," Sheet :",sheet.name
    #    prev_error=prev_error+1
    #else:
     #   prev_error=prev_error+1
    index_line+=1

cursor.execute("TRUNCATE TABLE ore")
print "\n Only first 5 errors are displayed (if any) !!! "
print " There have been found a total of",prev_error,"errors in the .xls file or in the database"

cursor.close ()
conn.close()
