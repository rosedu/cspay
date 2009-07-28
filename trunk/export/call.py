import sys
import random
import commands
from db_gather import gather_data
from time import gmtime, strftime


#calling example:
# call.py <type> <univ> <fac> <desk> <name1> [ name2 ... ]


if __name__ == "__main__":

    basepath = "/home/cspay/public_html/out/"
    commands.getstatusoutput("rm -r " + basepath+"*")
    letters = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', '1', '2', '3']
    random.shuffle(letters)
    path = basepath + "".join(letters)
    commands.getstatusoutput("mkdir "+path)
    path += "/"

    users = sys.argv[5:]

   # for args in sys.argv:print args
    for user in users:
        gather_data(user, sys.argv[2], sys.argv[3], sys.argv[4], path,
                    int(sys.argv[1]))

    tik = strftime("%d_%b_%Y",  gmtime())
	
    if int(sys.argv[1]):
	    tik = "Orare_" + tik
    tik +="_"+"".join(letters)

    zipname = basepath + tik + ".zip"
    
    commands.getstatusoutput("zip -r -j " + zipname + " " + path)
    commands.getstatusoutput("rm -r " + path)

    print "/~cspay/out/" + tik + ".zip"


