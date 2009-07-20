import sys
import random
import commands
from db_gather import gather_data
import time

#calling example:
# call.py <year> <univ> <fac> <desk> <name1> [ name2 ... ]


if __name__ == "__main__":

    basepath="/home/cspay/public_html/out/"
    letters=['a','b','c','d','e','f','g','h','1','2','3']
    random.shuffle(letters)
    path=basepath+"".join(letters)
    print commands.getstatusoutput("mkdir path")
    path += "/"
    for pic in sys.argv:
        print pic
    users = sys.argv[5:]
    for user in users:
        gather_data(user,sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4],path)
    zipname = basepath+str(time.localtime())+".zip"
    print zipname
    

    commands.getstatusoutput("zip -r path zipname")
    commands.getstatusoutput("rm -r path")


