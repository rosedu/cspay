#library for reading xls files
import xlrd

#module for reading a line from a excel file given the number

######################################################
## Reading columns by line number
######################################################

def read_line(sheet,line_nr):
    list_el=[]
    #reading each column in that specific line
    i_col=0
    while i_col<sheet.ncols:
        item=sheet.cell_value(line_nr,i_col)
        list_el.append(item)
        i_col=i_col+1
    if(len(list_el)>22):
        print "Line : ",line_nr,"\nWarning : Number of columns exceeds 21 (=nr needed) "
    elif(len(list_el)<22):
        print "Line : ",line_nr,"\nWarning : Number of columns bellow 21 (=nr needed) "
    
    return list_el
