#script for xls to database insertion
import xlrd
import line_parser
import line_reader
import db_writer
#pointer to the xls file

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
# for each sheet ot the file get all line
#
################################################

index_line=2
#get sheet and pass it to read line
   
sheet = file_xls.sheet_by_index(0)
while index_line<sheet.nrows:
    line=line_reader.read_line(sheet,index_line)
        
    error_check=line_parser.parse(line,index_line) #if "OK" data is consistent
    if error_check=="Ok":
        db_writer.db_write_line(line,index_line)  #pass the line and its number
    elif error_check=="Ignore":
        print "Line ",index_line+2," has been ignored >>> "
    else:
        print "Data missing on line :",
        print error_check+1," Sheet :",sheet.name
        print "Please correct XLS file"
        #index_sheet=file_xls.nsheets
        #break
    index_line=index_line+1
    
#test read first line in xls file -> uncomment next line to test
#print "Line 0 : ",reader.read_line(xls_file,0,0)


