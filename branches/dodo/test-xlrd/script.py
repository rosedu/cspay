#script for xls to database insertion
import xlrd
import line_parser
import line_reader

#pointer to the xls file

################################################
#
# Verify if file exists and open it
#
################################################

file_name="model_small.xls"
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

index_sheet=0

################################################
#
# for each sheet ot the file get all line
#
################################################

while index_sheet<file_xls.nsheets:
    index_line=0
    #get sheet and pass it to read line
   
    sheet = file_xls.sheet_by_index(index_sheet)
    while index_line<sheet.nrows:
        line=line_reader.read_line(sheet,index_line)
        
        error_check=line_parser.parse(line,index_line+1) #if "OK" data is consistent
        if error_check!="Ok":
            print "Data missing on line :",
            print error_check," Sheet :",sheet.name
            print "Please correct XLS file"
            #index_sheet=file_xls.nsheets
            #break
        index_line=index_line+1
    index_sheet=index_sheet+1
#test read first line in xls file -> uncomment next line to test
#print "Line 0 : ",reader.read_line(xls_file,0,0)


