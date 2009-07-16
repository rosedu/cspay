#For parsing lists of data

#Curently checks for
#1 empty lines
#2 incomplete lines
#3 data coruption

#Does not change attempt to correct data errors
#If the data is inconsistent it gives an error and halts execution

def parse(list_data,line_number):
    #all cells in a line are empty=>ignore the line, return Ok
    ii=0
    for empty in list_data:
        if not empty:
            ii=ii+1
        elif type(empty)==type('string'):
            empty=""
        elif type(empty)==type('int'):
            empty=0
            
    if ii>21:
        return "Ok"
    
    #common mandatory data 1(C+L, index 0->6)
    i=0
    while i<7:
        if not list_data[i]:
            print "Cell ",i,
            return line_number 
        i=i+1

    #common mandatory data 2(C+L, index 12->13)
    i=12
    while i<14:
        if not list_data[i]:
            print "Cell ",i,
            return line_number
        i=i+1
    #common mandatory data 3(C+L, index 15->21)
    i=15
    while i<22:
        if not list_data[i]:
            print "Cell ",i,
            return line_number
        i=i+1
    

    #mandatory data only for C
    if list_data[1]=="C":
        if not list_data[7]:
            print "Cell ",7,
            return line_number
        if not list_data[8]:
            print "Cell ",8,
            return line_number
        if not list_data[10]:
            print "Cell ",10,
            return line_number

    
    #mandatory data only for L
        #mandatory data only for C
    elif list_data[1]=="L" or list_data[1]=="P":
        if not list_data[9]:
            print "Cell ",9,
            return line_number
        if not list_data[11]:
            print "Cell ",11,
            return line_number
    else:
        return line_number
        #remove spacesa
    for value in list_data:
        if type(value)==type('string'): #check data type
            value.strip() #removes leading and trailing spaces from string types
        #to remove only leading (lstrip) and only trailing (rstrip)

    return "Ok"
        



