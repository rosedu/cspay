#import library
import xlrd

#open file
book = xlrd.open_workbook("model.xls")

#number and name of worksheets
print "The number of worksheets is ", book.nsheets
print "Worksheet name(s): ", book.sheet_names()

#use the number of worksheets to loop between them
print "\nInformation Worksheets :"
i=0
while i<book.nsheets:
    sh = book.sheet_by_index(i)
    print "\n"
    print "WName[",i,"]: ",sh.name, "\nRows: ",sh.nrows,"\nColumns: ",sh.ncols
    
    #now that we now number of rows and cols
    #we can read each row at a time and then pass to the next col
    i_row=0
    while i_row<sh.nrows:               #pass threw columns
        i_col=0
        while i_col<sh.ncols:           #pass threw rows
            print sh.cell_value(i_row,i_col),
            i_col=i_col+1
        print "\n--------------------------------------> "
        i_row=i_row+1
    i=i+1


#print "Cell D30 is", sh.cell_value(rowx=29, colx=3)

