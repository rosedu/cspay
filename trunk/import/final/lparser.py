#For parsing lists of data

#Curently checks for
#1 empty lines
#2 incomplete lines
#3 data coruption

#Does not change attempt to correct data errors
#If the data is inconsistent it gives an error and halts execution

DEBUG = True

################################################

error_color="""><FONT COLOR="#CC0000">"""
error_color2="""</FONT>"""
col_list = ["Facultate", "Tip", "Titlu discicplina", "Forma", "Cod", "Serie", "Nr gr/sg", "gr/sg" , "Tip", "An", "C1", "A1", "Post", "Grad", "Persoana", "Tip", "Acoperit", "Acoperit efectiv", "An/Grupa", "Zi", "Ora", "Sala"]

# mark with a '-' the ones that are not mandatory
oblig = [0, # Facultate
       1, # Tip (C, L, P, S)
       2, # Titlu disciplina
       -3, # Forma
       -4, # Cod
       -5, # Serie
       -6, # Nr gr/sg
       -7, # gr/sg
        8, # Tip (C, L, P, S)
       -9, # An
       10, # C1
       11, # A1
       12, # Post
       13, # Grad
       -14, # Persoana
       15, # Tip
       16, # Acoperit
       17, # Acoperit efectiv
       18, # An/Grupa
       19, # Zi
       20, # Ora
       21] # Sala

NUM_ITEMS = 22

################################################

def parse(list_data,line_number,prev_error):
    #print "Parser called at line number:",line_number
    #all celule in a line are empty=>ignore the line, return Ok
    ii=0
    i=0

    if DEBUG == True:
	    print "parse line number ", line_number

    for check in list_data:
        if not check:
            ii=ii+1
        i=i+1

    if DEBUG == True:
        print i, " elements on line ", line_number

    if(i > NUM_ITEMS):
        if(prev_error<1000):
            error="<tr>"
            error+="<td>"+str(line_number+1)+"</td>"
            error+="<td>"+"N/A"+"</td>"
            error+="<td>"+"N/A"+"</td>"
            error+="<td>"+str(i),"celule"+"</td>"
            error+="<td>" + NUM_ITEMS + "celule"+"</td>"
            error+="<td>"+"Prea multe"+"</td>"
            error+="</tr>"
            print error
        prev_error=prev_error+1
        return prev_error
    elif (i < NUM_ITEMS):
        if(prev_error<1000):
            error="<tr>"
            error+="<td>"+line_number+1+"</td>"
            error+="<td>"+"N/A"+"</td>"
            error+="<td>"+"N/A"+"</td>"
            error+="<td"+error_color+str(i)+error_color2+"celule"+"</td>"
            error+="<td>" + NUM_ITEMS + "celule"+"</td>"
            error+="<td>"+"Prea putine"+"</td>"
            error+="</tr>"
            print error
        prev_error=prev_error+1
        return prev_error
    if ii > (NUM_ITEMS - 1):
        if(prev_error<1000):
            error="<tr>"
            error+="<td>"+str(line_number+1)+"</td>"
            error+="<td>"+"N/A"+"</td>"
            error+="<td>"+"N/A"+"</td>"
            error+="<td"+error_color+"0 celule"+error_color2+"</td>"
            error+="<td>" + NUM_ITEMS + "celule"+"</td>"
            error+="<td>"+"Linie ignorata"+"</td>"
            error+="</tr>"
            print error
        prev_error=prev_error+1
        return prev_error     # more than 21 lines that count
                            # should make the line be ignored

    ii=0
    i=0
    for empty in list_data:
        if not empty:
            #common mandatory data
            if(i==10)and(list_data[1]=="C"):
                if(prev_error<1000):
                    error="<tr>"
                    error+="<td>"+str(line_number+1)+"</td>"
                    error+="<td>"+str(col_list[i])+"</td>"
                    error+="<td>"+str(i+1)+"</td>"
                    error+="<td"+error_color+"Date lipsa"+error_color2+"</td>"
                    error+="<td>"+"C/L/P/S"+"</td>"
                    error+="<td>"+""+"</td>"
                    error+="</tr>"
                    print error
                prev_error=prev_error+1
                return prev_error
            if(i==11)and(list_data[1]=="L"):
                if(prev_error<1000):
                    error="<tr>"
                    error+="<td>"+str(line_number+1)+"</td>"
                    error+="<td>"+str(col_list[i])+"</td>"
                    error+="<td>"+str(i+1)+"</td>"
                    error+="<td"+error_color+"Date lipsa"+error_color2+"</td>"
                    error+="<td>"+"C/L/P/S"+"</td>"
                    error+="<td>"+"Gasit: N/A"+"</td>"
                    error+="</tr>"
                    print error
                prev_error=prev_error+1
                return prev_error
            if(i==11)and(list_data[1]=="P"):
                if(prev_error<1000):
                    error="<tr>"
                    error+="<td>"+str(line_number+1)+"</td>"
                    error+="<td>"+str(col_list[i])+"</td>"
                    error+="<td>"+str(i+1)+"</td>"
                    error+="<td"+error_color+"Date lipsa"+error_color2+"</td>"
                    error+="<td>"+"C/L/P/S"+"</td>"
                    error+="<td>"+"Gasit: N/A"+"</td>"
                    error+="</tr>"
                    print error
                prev_error=prev_error+1
                return prev_error
            if(i==11)and(list_data[1]=="S"):
                if(prev_error<1000):
                    error="<tr>"
                    error+="<td>"+str(line_number+1)+"</td>"
                    error+="<td>"+str(col_list[i])+"</td>"
                    error+="<td>"+str(i+1)+"</td>"
                    error+="<td"+error_color+"Date lipsa"+error_color2+"</td>"
                    error+="<td>"+"C/L/P/S"+"</td>"
                    error+="<td>"+"Gasit: N/A"+"</td>"
                    error+="</tr>"
                    print error
                prev_error=prev_error+1
                return prev_error
            if(i==oblig[i])and(i!=10)and(i!=11):
                if(prev_error<1000):
                    error="<tr>"
                    error+="<td>"+str(line_number+1)+"</td>"
                    error+="<td>"+str(col_list[i])+"</td>"
                    error+="<td>"+str(i+1)+"</td>"
                    error+="<td"+error_color+"Date lipsa"+error_color2+"</td>"
                    error+="<td>"+"C/L/P/S"+"</td>"
                    error+="<td>"+"Gasit: N/A"+"</td>"
                    error+="</tr>"
                    print error
                prev_error=prev_error+1
                return prev_error

            ii=ii+1
            list_data[i]='0'
        elif(list_data[15]!="po")and(list_data[15]!="pa")and(list_data[15]!="B"):
            if(prev_error<1000):
                error="<tr>"
                error+="<td>"+str(line_number+1)+"</td>"
                error+="<td>"+str(col_list[15])+"</td>"
                error+="<td>"+str(16)+"</td>"
                error+="<td"+error_color+"Date gresite"+error_color2+"</td>"
                error+="<td>"+"po/pa/B"+"</td>"
                error+="<td>"+"Gasit: "+list_data[15]+"</td>"
                error+="</tr>"
                print error
            prev_error=prev_error+1
            return prev_error
        i=i+1
        #remove spacesa
    index=0
    for value in list_data:

        if isinstance(value, unicode):#check data type
            list_data[index]=value.encode("utf-8",'ignore')
             #removes leading and trailing spaces from string types

        elif isinstance(value, str):#check data type
            list_data[index]=value.strip()
        elif isinstance(value, float):
            list_data[index]=int(value)
        index=index+1

    return prev_error
