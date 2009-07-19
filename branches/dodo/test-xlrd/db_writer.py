import MySQLdb
import sys


def db_write_line(line_data,line_index):
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
    if not link_fac:
        print "Error -> Line NR ",line_index,"data did not have a coresponding faculty id "
        print "Possible causses : faculty short name changed, table 'facultati' was altered"
    else :   
        cursor.execute ("""SELECT disc_id
                           FROM discipline
                           WHERE link_fac=%s AND nume=%s""",
                        (link_fac['fac_id'],line_data[2]))
        id_ora = cursor.fetchone ()
        if not id_ora:
            print "Error -> Line NR ",line_index,"data did not have a coresponding course/lect id"
            print "Possible causses : course/lecture name changed, table 'discipline' was altered"
        else:
            db='('
            #db+=str(id_ora['disc_id'])+',' #ora
            db+=str(id_ora['disc_id'])+',' #link_disc
            db+='\''+str(line_data[1])+'\',' #tip
            db+='\''+str(line_data[3])+'\',' #forma
            db+='\''+str(line_data[4])+'\',' #cod
            db+=str(line_data[5])+',' #an
            db+='\''+str(line_data[6])+'\',' #serie
            db+=str(line_data[7])+',' #nr_stud
            db+=str(line_data[8])+',' #nr_grup
            db+='\''+str(line_data[9])+'\',' #tip_grup_app
            db+=str(line_data[10])+',' #nr_ore_curs
            db+=str(line_data[11])+',' #nr_ore_aplicatii
            db+=str(line_data[12])+',' #nr_post
            db+='\''+str(line_data[13])+'\',' #grad_post
            db+='\''+str(line_data[14])+'\',' #pers_norma
            db+='\''+str(line_data[15])+'\','
            db+='\''+str(line_data[16])+'\',' #pers_acoperit
            db+='\'0\',' #pers_acoperit_efect     <------------- Problema de rezolvat >>>>>>
            db+='\''+str(line_data[18])+'\',' #an_grupa
            db+='\''+str(line_data[19])+'\',' #zi
            db+='\''+str(line_data[20])+'\',' #ora
            db+='\''+str(line_data[21])+'\',' #sala
            db+='1,'#paritate, #
            db+='1)'#,paritate_start)
            print db
            #tuple_db=(id_ora, #ora
               #         line_data[2], #link_disc
               #         line_data[1], #tip
               #         line_data[3], #forma
               #         line_data[5], #an
               #         line_data[6], #serie
               #         line_data[7], #nr_stud
               #         line_data[8], #nr_grup
               #         line_data[9], #tip_grup_app
               #         line_data[10], #nr_ore_curs
               #         line_data[11], #nr_ore_aplicatii
               #         line_data[12], #nr_post
               #         line_data[13], #grad_post
               #         line_data[14], #pers_norma
               #         line_data[15],
               #         line_data[16], #pers_acoperit
               #         line_data[17], #pers_acoperit_efect
               #         line_data[18], #an_grupa
               #         line_data[19], #zi
               #         line_data[20], #ora
               #         line_data[21], #sala
               #         1,#paritate, #
               #         1)#,paritate_start) #)#,paritate_start) #
           
            #print str(tuple_db)
            #print tuple_db

            cursor.execute("""INSERT INTO ore (link_disc,tip_ora,forma,cod,an,serie,
	  nr_stud,nr_grupa,tip_grupa,tip_grupa_aplicatii,nr_ore_curs,nr_ore_aplicatii,nr_post,
	  grad_post,pers_norma,tip_ocupare,pers_acoperit,pers_acoperit_efect,an_grupa,zi,ora,
	  sala,paritate,paritate_start) VALUES %s""",db)
    cursor.close ()
    conn.close()                        
                        
                            
        
    
