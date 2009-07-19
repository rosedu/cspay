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
        if not item:
            list_el.append(">>>") #appending data read to the list
        else:
            list_el.append(item)
        i_col=i_col+1
    return list_el
