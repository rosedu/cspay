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

        
    cursor.execute ("""INSERT INTO ore VALUES %s""",#id
                        (id_ora['disc_id'], #ora
                        line_data[2], #link_disc
                        line_data[1], #tip
                        line_data[3], #forma
                        line_data[4], #cod
                        line_data[5], #an
                        line_data[6], #serie
                        line_data[7], #nr_stud
                        line_data[8], #nr_grup
                        line_data[9], #tip_grup_app
                        line_data[10], #nr_ore_curs
                        line_data[11], #nr_ore_aplicatii
                        line_data[12], #nr_post
                        line_data[13], #grad_post
                        line_data[14], #pers_norma
                        line_data[16], #pers_acoperit
                        line_data[17], #pers_acoperit_efect
                        line_data[18], #an_grupa
                        line_data[19], #zi
                        line_data[20], #ora
                        line_data[21], #sala
                        1,#paritate, #
                        1))#,paritate_start) #
    cursor.close ()
    conn.close()                        
                        
                            
        
    
