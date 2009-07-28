#For parsing lists of data

#Curently checks for
#1 empty lines
#2 incomplete lines
#3 data coruption

#Does not change attempt to correct data errors
#If the data is inconsistent it gives an error and halts execution

################################################


col_list=["Facultate","C,L,P,S","Titlu disc","Forma","Cod","An","Serie","Nr Stud",
          "Nr gr/sg","sg","C2","A2","Post","Grad","PERS","Tip","Acoperit",
          "Acop.efectiv","An/Gr","Zi","Ora","Sala"]
# mark with a '-' the ones that are not mandatory
oblig=[0, # Facultate
       1, # C,L,P,S
       2, # Titlu disc
       3, # Forma
       -4, # Cod
       -5, # An
       -6, # Serie
       -7, # Nr Stud
       -8, # Nr gr/sg
       -9, # sg
       10,#C2
       11,#A2
       12,#Post
       13,#Grad
       -14,#PERS
       15,#Tip
       16,#Acoperit
       17,#Acop.efectiv
       18,#An/Gr
       19,#Zi
       20,#Ora
       21]#Sala

################################################

def parse(list_data,line_number,prev_error):
    #print "Parser called at line number :",line_number
    #all cells in a line are empty=>ignore the line, return Ok
    ii=0
    i=0
    
    for empty in list_data:
        if not empty:
            #common mandatory data
            if(i==10)and(list_data[1]=="C"):
                if(prev_error<5):
                    print "Line: ",line_number+1
                    print " Column : ",col_list[i]," Column index :",i+1," Error MISSING DATA \n"
                prev_error=prev_error+1
                return prev_error
            if(i==11)and(list_data[1]=="L"):
                if(prev_error<5):
                    print "Line: ",line_number+1
                    print " Column : ",col_list[i]," Column index :",i+1," Error MISSING DATA \n"
                prev_error=prev_error+1
                return prev_error
            if(i==11)and(list_data[1]=="P"):
                if(prev_error<5):
                    print "Line: ",line_number+1
                    print " Column : ",col_list[i]," Column index :",i+1," Error MISSING DATA \n"
                prev_error=prev_error+1
                return prev_error
            if(i==11)and(list_data[1]=="S"):
                if(prev_error<5):
                    print "Line: ",line_number+1
                    print " Column : ",col_list[i]," Column index :",i+1," Error MISSING DATA \n"
                prev_error=prev_error+1
                return prev_error
            if(i==oblig[i])and(i!=10)and(i!=11):
                if(prev_error<5):
                    print "Line: ",line_number+1
                    print " Column : ",col_list[i]," Column index :",i+1," Error MISSING DATA \n"
                prev_error=prev_error+1
                return prev_error
            
            ii=ii+1
            list_data[i]='0'
        elif(list_data[15]!="po")and(list_data[15]!="pa")and(list_data[15]!="B"):
            if(prev_error<5):
                print "Error BAD DATA Line NR : ",line_number+1
                print "Column : ",col_list[15],"\nCell index :",15
                print "Options: po,pa,B - Read :",list_data[15],"\n"
            prev_error=prev_error+1
            return prev_error
        i=i+1
    if(i>22):
        if(prev_error<5):
            print "Line : ",line_number+1,"\nError : Number of columns exceeds 22 (=nr needed) "
        prev_error=prev_error+1
        return prev_error
    elif(i<22):
        if(prev_error<5):
            print "Line : ",line_number+1,"\nError : Number of columns bellow 22 (=nr needed) "
        prev_error=prev_error+1
        return prev_error
    if ii>21:
        if(prev_error<5):
            print "Line : ",line_number+1," has been ignored -> 22 fields missing"
        prev_error=prev_error+1
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
    
    return prev_error
        



