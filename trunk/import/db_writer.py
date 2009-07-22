import MySQLdb
import sys


def db_write_line(cursor,line_data,line_index,prev_error):
    
    cursor.execute ("""SELECT fac_id
                           FROM facultati
                           WHERE LOWER(nume_scurt)=%s""",
                        (str.lower(line_data[0])))
    link_fac = cursor.fetchone ()
    
    if not link_fac:
        if(prev_error<3):
            print "\n\n   --------------------------------- \n    "
            print "SQL : SELECT fac_id FROM facultati WHERE LOWER(nume_scurt)=",str.lower(line_data[0])
            print "\nERROR : Line NR ",line_index,"data did not have a coresponding faculty id "
            print "Possible causses : faculty short name changed, table 'facultati' was altered"
        prev_error+=1
        
    else :   
        cursor.execute ("""SELECT disc_id
                           FROM discipline
                           WHERE link_fac=%s AND LOWER(nume)=%s""",
                        (link_fac['fac_id'],str.lower(line_data[2])))
        id_ora = cursor.fetchone ()
        paritate="1"
        paritate_start="1"
        if not id_ora:
            if(prev_error<3):
                print "\n\n   --------------------------------- \n    "
                print "SQL : SELECT disc_id FROM discipline WHERE link_fac=",link_fac['fac_id'],
                " AND LOWER(nume)=",str.lower(line_data[2])
                print "\nERROR : Line NR ",line_index,"data did not have a coresponding course/lect id"
                print "Possible causses : course/lecture name changed, table 'discipline' was altered"
            prev_error+=1
        else:
            if(line_data[19][2:3]=='i'):
                paritate="2"
                paritate_start="1"
            elif(line_data[19][2:3]=='p'):
                paritate="2"
                paritate_start="2"
            db="""INSERT INTO ore (link_disc,tip_ora,forma,cod,an,serie,
	  nr_stud,nr_grupa,tip_grupa_aplicatii,nr_ore_curs,nr_ore_aplicatii,nr_post,
	  grad_post,pers_norma,tip_ocupare,pers_acoperit,pers_acoperit_efect,an_grupa,zi,ora,
	  sala,paritate,paritate_start) VALUES ("""
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
            db+='\''+str(line_data[17])+'\',' #pers_acoperit_efect     <------------- Problema de rezolvat >>>>>>
            db+='\''+str(line_data[18])+'\',' #an_grupa
            db+='\''+str(line_data[19][:2])+'\',' #zi
            db+='\''+str(line_data[20])+'\',' #ora
            db+='\''+str(line_data[21])+'\',' #sala
            db+='\''+paritate+'\','#paritate, #
            db+='\''+paritate_start+'\')'#,paritate_start)
            print db,"\n"            
            db2=(int(id_ora['disc_id']),line_data[1],line_data[3],line_data[4],int(line_data[5]),line_data[6],
                 int(line_data[7]),int(line_data[8]),line_data[9],int(line_data[10]),int(line_data[11]),int(line_data[12]),
                 line_data[13],line_data[14],line_data[15],line_data[16],line_data[17],line_data[18],
                 line_data[19][:2],line_data[20],line_data[21],paritate,paritate_start)
            
            #print "\n\n\n\n\n\n\n"
            #print " SQL final statement ----------------------> "
            #print db
            #print " ------------------------------------------> "
            #print "\n\n\n\n\n\n\n"

            #cursor.execute(db)
            print """INSERT INTO ore (link_disc,tip_ora,forma,cod,an,serie,
	  nr_stud,nr_grupa,tip_grupa_aplicatii,nr_ore_curs,nr_ore_aplicatii,nr_post,
	  grad_post,pers_norma,tip_ocupare,pers_acoperit,pers_acoperit_efect,an_grupa,zi,ora,
	  sala,paritate,paritate_start) VALUES """,str(db2)

            cursor.executemany("""INSERT INTO ore (link_disc,tip_ora,forma,cod,an,serie,
	  nr_stud,nr_grupa,tip_grupa_aplicatii,nr_ore_curs,nr_ore_aplicatii,nr_post,
	  grad_post,pers_norma,tip_ocupare,pers_acoperit,pers_acoperit_efect,an_grupa,zi,ora,
	  sala,paritate,paritate_start) VALUES (%s,%s)""",[db2,db2])
            return prev_error
                        
                            
        
    
