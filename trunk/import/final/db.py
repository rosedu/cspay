import MySQLdb
import sys

error_color="""><FONT COLOR="#CC0000">"""
error_color2="""</FONT>"""
def db_write_line(cursor,line_data,line_index,prev_error):
    
    cursor.execute ("""SELECT fac_id
                           FROM facultati
                           WHERE nume_scurt=%s""",
                        (line_data[0]))
    link_fac = cursor.fetchone ()
    
    if not link_fac:
        if(prev_error<1000):
            error="<tr>"
            error+="<td>"+str(line_index+1)+"</td>"
            error+="<td>"+"Facultate"+"</td>"
            error+="<td>"+str(1)+"</td>"
            error+="<td"+error_color+"Date gresite"+error_color2+"</td>"
            error+="<td>"+"eroare BD"+"</td>"
            error+="<td>"+"Gasit: "+line_data[0]+"</td>"
            error+="</tr>"
            print error
        prev_error+=1
        
    else :   
        cursor.execute ("""SELECT disc_id
                           FROM discipline
                           WHERE link_fac=%s AND nume=%s""",
                        (link_fac['fac_id'],line_data[2]))
        id_ora = cursor.fetchone ()
        paritate="1"
        paritate_start="1"
        if not id_ora:
            if(prev_error<1000):
                error="<tr>"
                error+="<td>"+str(line_index+1)+"</td>"
                error+="<td>"+"Discipline"+"</td>"
                error+="<td>"+str(3)+"</td>"
                error+="<td"+error_color+"Date gresite"+error_color2+"</td>"
                error+="<td>"+"eroare BD"+"</td>"
                error+="<td>"+"Gasit: "+line_data[2]+"</td>"
                error+="</tr>"
                print error
            prev_error=prev_error+1
        else:
            if str.lower(line_data[19][0:2]) in ["lu","ma","mi","jo","vi","sa","du"]:
                if(str.lower(line_data[19][2:3])=='i'):
                    paritate="2"
                    paritate_start="1"
                elif(str.lower(line_data[19][2:3])=='p'):
                    paritate="2"
                    paritate_start="2"
                
            else:
                if(prev_error<1000):
                    error="<tr>"
                    error+="<td>"+str(line_index+1)+"</td>"
                    error+="<td>"+"Paritate"+"</td>"
                    error+="<td>"+str(20)+"</td>"
                    error+="<td"+error_color+"Date gresite"+error_color2+"</td>"
                    error+="<td>"+"lu(p/i),ma(p/i)..."+"</td>"
                    error+="<td>"+"Gasit: "+line_data[19][0:3]+"</td>"
                    error+="</tr>"
                    print error
                    
                prev_error=prev_error+1
                
            db="""INSERT INTO ore (link_disc,tip_ora,forma,cod,an,serie,
	  nr_stud,nr_grupa,tip_grupa_aplicatii,nr_ore_curs,nr_ore_aplicatii,nr_post,
	  grad_post,pers_norma,tip_ocupare,pers_acoperit,pers_acoperit_efect,an_grupa,zi,ora,
	  sala,paritate,paritate_start) VALUES ("""
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
            
            cursor.execute(db)
    return prev_error
                        
                            
        
    
