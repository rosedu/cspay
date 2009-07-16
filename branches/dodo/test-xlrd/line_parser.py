#For parsing lists of data

#Curently checks for
#1 empty lines
#2 incomplete lines
#3 data coruption

#Does not change attempt to correct data errors
#If the data is inconsistent it gives an error and halts execution

def parse(list_data,line_number):
    if not list_data:
        return str(line_number)

    
    for value in list_data:
        if not value:
            return str(line_number)

        #remove spaces
        if type(value)==type('string'): #check data type
            value.strip() #removes leading and trailing spaces from string types
        #to remove only leading (lstrip) and only trailing (rstrip)

    return "Ok"
        



