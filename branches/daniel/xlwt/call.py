import sys
from db_gather import gather_data

#calling example:
# call.py <name> <year> <month1> <month2> ...
# where year is in format YYYY1/YYYY2, and monthN is an ordinal number 

if __name__ == "__main__":
    gather_data(sys.argv[1],sys.argv[2],map(int,sys.argv[3:]))

