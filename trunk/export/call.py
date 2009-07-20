import sys
from db_gather import gather_data

#calling example:
# call.py <year> <univ> <fac> <desk> <name1> [ name2 ... ]


if __name__ == "__main__":

    path="/home/cspay/public_html/out"
    for pic in sys.argv:
        print pic
    users = sys.argv[5:]
    for user in users:
        gather_data(user,sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4],path)

