#library for reading xls files
import xlrd

#module for reading a line or column from a excel file given the number

######################################################
## Reading columns by line number
######################################################

def read_line(file_name,sheet_nr,line_nr):
    book = xlrd.open_workbook(file_name)
    list_el=[]
    sh = book.sheet_by_index(sheet_nr)
    #reading each column in that specific line
    i_col=0
    while i_col<sh.ncols:           
        list_el.append(sh.cell_value(line_nr,i_col)) #appending data read to the list
        i_col=i_col+1
    return list_el

#######################################################
## Reading lines by column index
#######################################################

def read_col(file_name,sheet_nr,col_nr):    #read by column NUMBER
    book = xlrd.open_workbook(file_name)
    list_col=[]
    sh = book.sheet_by_index(sheet_nr)
    #reading each column in that specific column
    i_row=1
    while i_row<sh.nrows:           
        list_col.append(sh.cell_value(i_row,col_nr)) #appending data read to the list
        i_row=i_row+1
    return list_col

######################################################
## Reading lines by column name
######################################################


def read_coln(file_name,sheet_nr,col_name):     #read by column NAME
    book = xlrd.open_workbook(file_name)
    list_col=[]
    sh = book.sheet_by_index(sheet_nr)
    #reading each column in that specific column
    i_col=0
    col_nr=-1
    #find out the column index by comparing each column with the col_name arg
    while i_col<sh.ncols:           
        if sh.cell_value(0,i_col)==col_name:
            col_nr=i_col
            i_col=sh.ncols
        i_col=i_col+1
    #reading data and adding it to the list
    i_row=1
    while i_row<sh.nrows:           
        list_col.append(sh.cell_value(i_row,col_nr)) #appending data read to the list
        i_row=i_row+1
    return list_col      
