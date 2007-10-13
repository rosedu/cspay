
/*
	program in consola (cspay), foloseste biblioteca libcspay. Este fie 
    interactiv, fie apelat cu un fisier ca parametru.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "debug.h"
#include "cspay.h"
#include "iniparser.h"
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
      printf("Nu ati introdus un numar! Introduceti un numar:");
      free(line);
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
	char *nume, *intocmit,*facultate,*decan,*sef,*catedra;
	char *inif,*xmlf,*tip,*next,*fac,*dis,*grupa,*luna, *month;
 	char *s,m[12][3],tmpstr[80],buffer[512],bn[40],*universitate,*nc;
    	char *titular;
    	FILE *f;
    	struct cspay_file_list *lista;
    	struct cspay_config *config;
	if( argc < 4 ) 
	{
		usage();
	}
	if(argc==6 && strcmp(argv[1],"-n")==0)
    	{ 
        //mod neinteractiv 
        	if (strcmp(argv[2],"-x")==0 && argv[3][0]!='-')
        	{   
            		xmlf = argv[3];
	    		config = cspay_get_config( argv[3] );
            		if (strcmp(argv[4],"-f")==0 && argv[5][0]!='-') 
	    		{
		            	inif = argv[5]; 
	    		}
            	}
        }
	else 
	{
        if(argc==4 && strcmp(argv[1],"-i")==0) 
		{
	         //mod interactiv
		if (strcmp(argv[2],"-x")==0 && argv[3][0]!='-') 
			{
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
		read_string(&intocmit);
		fprintf(f,"\n intocmit = %s",intocmit);
		printf("\n Introduceti universitatea: ");
		read_string(&universitate);
		fprintf(f,"\n universitate = %s",universitate);
		printf("\n Introduceti facultatea: ");
		read_string(&facultate);
		fprintf(f,"\n facultate = %s",facultate);
		printf("\n Introduceti numele cursului: ");
		read_string(&nc);
		fprintf(f,"\n nume_curs = %s",nc);
		printf("\n Introduceti numele titularului cursului: ");
		read_string(&titular);
		fprintf(f,"\n titular = %s",titular);
		printf("\n Introduceti numele decanului facultatii: ");
		read_string(&decan);
		fprintf(f,"\n decan = %s",decan);
		printf("\n Introduceti numele sefului de catedra: ");
		read_string(&sef);
		fprintf(f,"\n sef_catedra = %s",sef);
		printf("\n Introduceti numele catedrei: ");
		read_string(&catedra);
		fprintf(f,"\n catedra = %s",catedra);
		printf("\n Introduceti numarul lunii: ");
		read_string(&luna);
		fprintf(f,"\n luna = %s",luna);
		printf("\n Introduceti tipul fisierul ce urmeaza fi generat: ");
		read_string(&tip);
		fprintf(f,"\n tip_fisier = %s",tip);
		// se introduc datele pentru ore
		next=malloc(3);
		strcpy(next,"Da");
		no=0;
		while(strcmp(next,"Da")==0)
			{
			no++;
			fprintf(f,"\n\n[ore/%d]",no);
			printf("\n Introduceti facultatea(abreviat): ");
			read_string(&fac);
			fprintf(f,"\n facultate = %s",fac);
			printf("\n Introduceti disciplina(abreviat): ");
			read_string(&dis);
			fprintf(f,"\n disciplina = %s",dis);
			printf("\n Introduceti rolul(0=asistent,1=conferentiar,2=sef lucrari,3=profesor): ");
			rol=read_num();
			fprintf(f,"\n rol = %d",rol);
			printf("\n Introduceti numarul postului: ");
			nrp=read_num();
			fprintf(f,"\n numar_post = %d",nrp);
			printf("\n Introduceti tipul postului(0=curs,1=aplicatii): ");
			tip_post=read_num();
			fprintf(f,"\n tip_post = %d",tip_post);
			printf("\n Introduceti grupa: ");
			read_string(&grupa);
			fprintf(f,"\n grupa = %s",grupa);
			printf("\n Introduceti ziua saptamanii(1 = luni, 2 = marti, etc.): ");
			zi=read_num();
			fprintf(f,"\n zi = %d",zi);
			printf("\n Introduceti intervalul orar: ");
			scanf("%d-%d",&oi,&of);
			fprintf(f,"\n ore = %d-%d",oi,of);
			printf("\n Odata la cate saptamani se tine cursul/aplicatia: ");
			par=read_num();
			fprintf(f,"\n paritate = %d",par);
			printf("\n In ce saptamana se tine cursul/aplicatia prima data:");
			pari=read_num();
			fprintf(f,"\n paritate_start = %d",pari);
			printf("\n Doriti sa mai introduceti inca un set de ore?(Da/Nu)");
			read_string(&next);
			}
		fclose(f);
		}
		else usage();
	}
	lista = cspay_convert_options( config, inif);
	cspay_free_config(config);
	for( i = 0; i < lista->nr; i++)
		printf( "%s\n", lista->names[i] );
	return 0;
}
