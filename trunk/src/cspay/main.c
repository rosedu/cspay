
/*
	program in consola (cspay), foloseste biblioteca libcspay. Este fie 
    interactiv, fie apelat cu un fisier ca parametru.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "../common/debug.h"
#include "../libcspay/cspay.h"
#include "../iniparser-2.17/src/iniparser.h"
#if 0	/* win 32 ?*/
#	include "E:\CSpay\trunk\src\libcspay"
#endif
int 
read_num()
{
char *line;
char *endp;
long nr;
     fflush(stdin);
     fflush(stdout);
     line=malloc(255);
     fgets(line,255,stdin);
     	if (line[0] == '\n')
		fgets(line,255,stdin);
  nr = strtol (line, &endp, 10);
  if (*endp != '\0' && *endp != '\n')
     {
      printf("Nu ati introdus un numar!");
      return read_num();
     }
  return (int)nr;
}
void
read_string(char ** str)
{
char *line;
     fflush(stdin);
     fflush(stdout);
     line=malloc(255);
     fgets(line,255,stdin);
     if (line[0] == '\n')
	     fgets(line,255,stdin);
     line[strlen(line)-1]='\0';
     * str = line;
}
void 
usage(void)
{
     //printf("Incorrect syntax\n");
     printf("The CSpay application allows two types of usages:\n");
     printf(" The interactive usage in which the user has to answer queries \n");
     printf(" so that the personal configuration file(ini file) is generated.\n");
     printf(" The correct syntax for the interactive usage is:\n");
     printf("\tcspay -i -x <xml file name>\n\n");
     printf(" The non-interactive usage in which the user does not need to  \n");
     printf(" supply any extra information, both the personal configuration \n");
     printf(" file(ini file) and the general configuration file(xml file) have\n");
     printf(" been previously created through the GUI or the Web interface\n");
     printf(" The correct syntax for the non-interactive usage is:\n");
     printf("\tcspay -n -x <xml file name> -f <ini file name>\n");
     exit(-1);
}

int
main( int argc, char **argv )
{	
    int i,no,nl,rol,nrp,tip_post,zi,oi,of,par,pari,j,l;
    char *nume, intocmit[40],facultate[80],decan[40],sef[40],catedra[40];
    char *inif,*xmlf,*tip,next[3],fac[6],dis[7],grupa[8],*luna, *month;
    char *s,m[12][3],tmpstr[80],buffer[512],bn[40],*universitate,*nc;
    char *titular;
    FILE *f;
    struct cspay_file_list *lista;
    struct cspay_config *config;
    dictionary *d;
	if( argc < 4 ) {
		usage();
	}
	if(argc==6 && strcmp(argv[1],"-n")==0)
    { 

        //mod neinteractiv 
        if (strcmp(argv[2],"-x")==0 && argv[3][0]!='-')
        {   
            xmlf = argv[3];
            inif = argv[5];
	    d = iniparser_load(inif);
	    month = malloc(30);
	    strcpy(month, iniparser_getstr(d, "antet:luna"));
	    l=strlen(month);
	    if(l<3)
	    {
	    config = cspay_get_config( argv[3] );
            if (strcmp(argv[4],"-f")==0 && argv[5][0]!='-') 
	    	{
            	lista = cspay_convert_options( config, inif);
            	cspay_free_config(config);
            	for( i = 0; i < lista->nr; i++)
		        	printf( "%s\n", lista->names[i] );

                exit(0);
	    	}
            }
        }
        // orice fel de eroare, iese si arata detalii
        if (l<3) usage();
	}
	else {
         if(argc==4 && strcmp(argv[1],"-i")==0) {
	         //mod interactiv
		if (strcmp(argv[2],"-x")==0 && argv[3][0]!='-') {
			 config = cspay_get_config(argv[3]);
	         }
	         else usage();
	     inif=malloc(20);
	     strcpy(inif,"personal.ini");
             f=fopen("personal.ini","wt");
             //se introduc datele pentru antet
             printf("\n Introduceti numele: ");
             read_string(&nume);
	     fprintf(f,"[antet]\n nume = %s",nume);
             printf("\n Intocmit de: ");
             scanf("%s",intocmit);
             fprintf(f,"\n intocmit = %s",intocmit);
	     printf("\n Introduceti universitatea: ");
             read_string(&universitate);
	     fprintf(f,"\n universitate = %s",universitate);
             printf("\n Introduceti facultatea: ");
             scanf("%s",facultate);
             fprintf(f,"\n facultate = %s",facultate);
	     printf("\n Introduceti numele cursului: ");
             scanf("%s",nc);
             fprintf(f,"\n nume_curs = %s",nc);
             printf("\n Introduceti numele decanului facultatii: ");
             scanf("%s",decan);
	     printf("\n Introduceti numele titularului cursului: ");
             read_string(&titular);
	     fprintf(f,"\n titular = %s",titular);
             fprintf(f,"\n decan = %s",decan);
             printf("\n Introduceti numele sefului de catedra: ");
             scanf("%s",sef);
             fprintf(f,"\n sef_catedra = %s",sef);
             printf("\n Introduceti numele catedrei: ");
             scanf("%s",catedra);
             fprintf(f,"\n catedra = %s",catedra);
             printf("\n Introduceti numarul lunii: ");
             read_string(&luna);
	     l=strlen(luna);
	     month=malloc(30);
	     strcpy(month,luna);
             fprintf(f,"\n luna = %s",luna);
             printf("\n Introduceti tipul fisierul ce urmeaza fi generat: ");
             read_string(&tip);
             fprintf(f,"\n tip_fisier = %s",tip);
             // se introduc datele pentru ore
             strcpy(next,"Da");
             no=0;
             while(strcmp(next,"Da")==0)
             {
             no++;
             fprintf(f,"\n\n[ore/%d]",no);
             printf("\n Introduceti facultatea(abreviat): ");
             scanf("%s",fac);
             fprintf(f,"\n facultate = %s",fac);
             printf("\n Introduceti disciplina(abreviat): ");
             scanf("%s",dis);
             fprintf(f,"\n disciplina = %s",dis);
             printf("\n Introduceti rolul(0=asistent,1=conferentiar,2=sef lucrari,3=profesor): ");
             scanf("%d",&rol);
             fprintf(f,"\n rol = %d",rol);
             printf("\n Introduceti numarul postului: ");
             scanf("%d",&nrp);
             fprintf(f,"\n numar_post = %d",nrp);
             printf("\n Introduceti tipul postului(0=curs,1=aplicatii): ");
             scanf("%d",&tip_post);
             fprintf(f,"\n tip_post = %d",tip_post);
             printf("\n Introduceti grupa: ");
             scanf("%s",grupa);
             fprintf(f,"\n grupa = %s",grupa);
             printf("\n Introduceti ziua saptamanii(1 = luni, 2 = marti, etc.): ");
             scanf("%d",&zi);
             fprintf(f,"\n zi = %d",zi);
             printf("\n Introduceti intervalul orar: ");
             scanf("%d-%d",&oi,&of);
             fprintf(f,"\n ore = %d-%d",oi,of);
             printf("\n Odata la cate saptamani se tine cursul/aplicatia: ");
             scanf("%d",&par);
             fprintf(f,"\n paritate = %d",par);
             printf("\n In ce saptamana se tine cursul/aplicatia prima data:");
             scanf("%d",&pari);
             fprintf(f,"\n paritate_start = %d",pari);
             printf("\n Doriti sa mai introduceti inca un set de ore?(Da/Nu)");
             scanf("%s",next);
             }
	     fclose(f);
	     d = iniparser_load("personal.ini");
	     }
	     else 
	    usage();
	}
	if(strlen(month)>=3)
	{
		s = strtok(month," ,");
		nl=1;
		while(s!=NULL)
			{
			strcpy(m[nl-1],s);
			m[nl-1][strlen(s)]='\0';
			nl++;
			s = strtok(NULL, " ,");
			}
		nl--;
		strncpy(bn,inif,strlen(inif)-4);
		bn[strlen(inif)-4]='\0';
		for(i=0;i<nl;i++)
			{
				strcpy(tmpstr,bn);
				strcat(tmpstr,m[i]);
				strcat(tmpstr,".ini");
				snprintf(buffer,512,"cp %s %s",inif,tmpstr);
				system(buffer);
				f=fopen(tmpstr,"r+");
				fprintf(f,"[antet]\n nume = %s",iniparser_getstr(d, "antet:nume"));
				fprintf(f,"\n intocmit = %s",iniparser_getstr(d, "antet:intocmit"));
				fprintf(f,"\n universitate = %s",iniparser_getstr(d, "antet:universitate"));
				fprintf(f,"\n facultate = %s",iniparser_getstr(d, "antet:facultate"));
				fprintf(f,"\n nume_curs = %s",iniparser_getstr(d, "antet:nume_curs"));
				fprintf(f,"\n titular = %s",iniparser_getstr(d, "antet:titular"));
				fprintf(f,"\n decan = %s",iniparser_getstr(d, "antet:decan"));
				fprintf(f,"\n sef_catedra = %s",iniparser_getstr(d, "antet:sef_catedra"));
				fprintf(f,"\n catedra = %s",iniparser_getstr(d, "antet:catedra"));
				fprintf(f,"\n luna = %s%*c",m[i],l-strlen(m[i]),' ');
				fprintf(f,"\n tip_fisier = %s",iniparser_getstr(d, "antet:tip_fisier"));
				fclose(f);
				config = cspay_get_config( argv[3] );
            			//lista = cspay_convert_options( config, tmpstr);
            			cspay_free_config(config);
            			//for( j = 0; j < lista->nr; j++)
		        	//	printf( "%s\n", lista->names[j] );

			}
	}
	iniparser_freedict(d);
	return 0;
}
