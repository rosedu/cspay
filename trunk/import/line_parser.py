#For parsing lists of data

#Curently checks for
#1 empty lines
#2 incomplete lines
#3 data coruption

#Does not change attempt to correct data errors
#If the data is inconsistent it gives an error and halts execution

def parse(list_data,line_number,prev_error):
    #print "Parser called at line number :",line_number
    #all cells in a line are empty=>ignore the line, return Ok
    ii=0
    i=0
    
    for empty in list_data:
        if not empty:
            #common mandatory data
            if(i<7):
                print "Error NO DATA Line NR : line_number",line_number
                print "Cell ",i+1,"\n"
                prev_error+=1
                return prev_error
            #common mandatory data
            if((i>11)and(i<14)):
                print "Error NO DATA Line NR : line_number",line_number
                print "Cell ",i+1,"\n"
                prev_error+=1
                return prev_error
            #common mandatory data
            if((i>14)and(i<22)):
                print "Error NO DATA Line NR : line_number",line_number
                print "Cell ",i+1,"\n"
                prev_error+=1
                return prev_error
            if((i==10)and(str(list_data[1])=="C")):
                print "Error NO DATA Line NR : line_number",line_number
                print "Cell 11\n"
                prev_error+=1
                return prev_error
            if((str(list_data[1])=="L")or(str(list_data[1])=="P")or(str(list_data[1])=="C")):
                if(i==12):
                    print "Error NO DATA Line NR : line_number",line_number
                    print "Cell 13\n"
                    prev_error+=1
                    return prev_error
            ii=ii+1
            list_data[i]='0'
        i=i+1
               
    if(i>22):
        print "Line : ",line_number,"\nWarning : Number of columns exceeds 22 (=nr needed) "
    elif(i<22):
        print "Line : ",line_number,"\nWarning : Number of columns bellow 22 (=nr needed) "
    
    if ii>21:
        print "Line : ",line_number," has been ignored -> 22 fields missing"
        prev_error+=1
        return prev_error     # more than 21 lines that count
                            # should make the line be ignored

        #remove spacesa
    index=0
    for value in list_data:
        
        if isinstance(value, unicode): #check data type
            list_data[index]=value.encode("utf-8",'ignore')
             #removes leading and trailing spaces from string types
            
        elif isinstance(value, str): #check data type
            list_data[index]=value.strip()
        elif isinstance(value, float):
            list_data[index]=int(value)
        index=index+1
    
    return "Ok"
        



