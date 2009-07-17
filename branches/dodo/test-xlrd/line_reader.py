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
        cell_value=sheet.cell_value(line_nr,i_col)
        if type(cell_value)==type('string'):
            print cell_value," -->"
            ell_value.encode('ASCII',ignore)
            print cell_value
        list_el.append(cell_value) #appending data read to the list
        i_col=i_col+1
    return list_el
