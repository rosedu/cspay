#For parsing lists of data

#Curently checks for
#1 empty lines
#2 incomplete lines
#3 data coruption

#Does not change attempt to correct data errors
#If the data is inconsistent it gives an error and halts execution

def parse(list_data,line_number):
    #print "Parser called at line number :",line_number
    #all cells in a line are empty=>ignore the line, return Ok
    ii=0
    i=0
    for empty in list_data:
        if not empty:
            ii=ii+1
            list_data[i]='0'
           
        i=i+1        
    if ii>21:
        return "Ignore"     # more than 21 lines that count
                            # should make the line be ignored
    
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
            print "Cell 7",
            return line_number
        if not list_data[8]:
            print "Cell 8",
            return line_number
        if not list_data[10]:
            print "Cell 10",
            return line_number

    
    #mandatory data only for L
    elif list_data[1]=="L" or list_data[1]=="P" or list_data[1]=="S":
        if not list_data[9]:
            print "Cell 9",
            return line_number
        if not list_data[11]:
            print "Cell 11",
            return line_number
    else:
        return line_number
        #remove spacesa
    index=0
    for value in list_data:
        
        if isinstance(value, unicode): #check data type
            list_data[index]=value.encode("utf-8",'ignore')
             #removes leading and trailing spaces from string types
            #print value
        elif isinstance(value, str): #check data type
            list_data[index]=value.strip()
        elif isinstance(value, float):
            list_data[index]=int(value)
        index=index+1
    
    return "Ok"
        



