import MySQLdb
import sys


def db_write_line(line_data):
    try:
        conn = MySQLdb.connect (host = "localhost", user = "cspay",
                                        passwd = "Chote3at", db = "rsoc_cspay")
    except MySQLdb.Error, e:
            print "Error %d: %s" % (e.args[0], e.args[1])
            sys.exit (1)
    cursor = conn.cursor (MySQLdb.cursors.DictCursor)


    cursor.execute ("""SELECT fac_id
                           FROM facultati
                           WHERE nume_scurt=%s""",
                        (line_data[0]))
        
    link_fac = cursor.fetchone ()
    print "---->_------------"
    print link_fac['fac_id']    
    cursor.execute ("""SELECT disc_id
                           FROM discipline
                           WHERE link_fac=%s AND nume=%s""",
                        (link_fac['fac_id'],line_data[2]))
    id_ora = cursor.fetchone ()

        
   # cursor.execute ("""INSERT INTO ore VALUES (%s)""",#id
         #               [(id_ora['disc_id'],line_data,1,1)])
    cursor.close ()
    conn.close()                        
                        
                            
        
    
