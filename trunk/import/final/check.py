#!/usr/bin/env python
# -*- coding: UTF-8 -*-

#script for xls to database insertion
import xlrd
import lparser
import lreader
import db
import sys
import MySQLdb
#pointer to the xls file

# Each file is checked between line numbers (ex: all lines,line 6,lines 5-10)
# all files -> start_check=0 , end_check=0
# line a -> start_check=a end_check<start_check
# lines a-b -> start_check=a and end_check=b where a<b
print """"
<html>
    <head>
        <title>Errors XLS</title>
    <head>
    <body>
    <p>
"""
file_name=""
start_check=0   
end_check=0
nr_argv=0
for arg in sys.argv:
    nr_argv+=1
    
if nr_argv>1:
    file_name=sys.argv[1]
else :
    print """ ERROR  -> Possible parameters :
            \n python check.py filename.xls     ->go threw all lines
            \n python check.py filename.xls x   ->go only at line x
            \n python check.py filename.xls x y ->go only threw lines x->y
            """
    

if nr_argv>2:
    start_check=int(sys.argv[2])

if nr_argv>3:
     end_check=int(sys.argv[3])

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
        file_name=raw_input(" Enter the correct path to the xls file ")
        file_exists=0



################################################
#
# Connection database
#
################################################

try:
    conn = MySQLdb.connect (host = "koala.cs.pub.ro", user = "rosedu_cspay",
                                    passwd = "huashaquou", db = "rosedu_cspay")
except MySQLdb.Error, e:
    print "Error %d: %s" % (e.args[0], e.args[1])
    print "</p></body></html>"
    sys.exit (1)
cursor = conn.cursor (MySQLdb.cursors.DictCursor)

##################################################


################################################
#
# for each sheet ot the file get all line
#
################################################


col_list=["Facultate","C,L,P,S","Titlu disc","Forma","Cod","An","Serie","Nr Stuf",
          "Nr gr/sg","sg","C2","A2","Post","Grad","PERS","Tip","Acoperit",
          "Acop.efectiv","An/Gr","Zi","Ora","Sala"]

################################################
print "</p>"
print """<table border="1">"""
print "<tr><b>"
print "<td> Line Number </td>"
print "<td> Column Name </td>"
print "<td> Column Index </td>"
print "<td> Error Type </td>"
print "<td> Data Expected </td>"
print "<td> Other info </td>"
print "</b></tr>"
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
    line=lreader.read_line(sheet,index_line)
    error_check=prev_error
    prev_error=lparser.parse(line,index_line,prev_error) #if "OK" data is consistent
    if error_check==prev_error:
        prev_error=db.db_write_line(cursor,line,index_line,prev_error)  #pass the line and its number
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
print """</table></body></html>"""
cursor.close ()
conn.close()
