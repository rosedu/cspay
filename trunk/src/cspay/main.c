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
#if 0	/* win 32 ?*/
#	include "E:\CSpay\trunk\src\libcspay"
#endif
void 
usage(void)
{
     Dprintf("Incorrect syntax\n");
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
    int i,luna,no,rol,nrp,tip_post,zi,oi,of,par,pari;
    char nume[40], intocmit[40],facultate[80],decan[40],sef[40],catedra[40];
    char tip[3],next[2],fac[5],dis[6],grupa[7];
    FILE *f;
    struct cspay_file_list *lista;
	struct cspay_config *config;
	
	if( argc < 4 ) {
		usage();
	}
	if(argc==6 && strcmp(argv[1],"-n")==0)
    { 
        Dprintf("non-interactive\n");
        //mod neinteractiv 
        if (strcmp(argv[2],"-x")==0 && argv[3][0]!='-')
        {   
            xmlf = argv[3];
            inif = argv[5];
            config = cspay_get_config( argv[3] );
            Dprintf("xml functional\n");
            if (strcmp(argv[4],"-f")==0 && argv[5][0]!='-') {
            	lista = cspay_convert_options( config, inif);
            	cspay_free_config(config);
            	for( i = 0; i < lista->nr; i++)
		        	printf( "%s.ods\n", lista->names[i] );
                Dprintf("ini functional\n");
                exit(0);
            }
        }
        // orice fel de eroare, iese si arata detalii
        usage();
	}
	else {
         if(argc==4 && strcmp(argv[1],"-i")==0) {
	         printf("interactive\n");
	         //mod interactiv
	         if (strcmp(argv[2],"-x")==0 && argv[3][0]!='-') {
	            config = cspay_get_config(argv[1]);
	         }
	         else usage();
             f=fopen("personal.ini","wt");
             //se introduc datele pentru antet
             printf("\n Introduceti numele: ");
             scanf("%s",nume);
             fprintf(f,"[antet]\n nume = %s",nume);
             printf("\n Intocmit de: ");
             scanf("%s",intocmit);
             fprintf(f,"\n intocmit = %s",intocmit);
             printf("\n Introduceti facultatea: ");
             scanf("%s",facultate);
             fprintf(f,"\n facultate = %s",facultate);
             printf("\n Introduceti numele decanului facultatii: ");
             scanf("%s",decan);
             fprintf(f,"\n decan = %s",decan);
             printf("\n Introduceti numele sefului de catedra: ");
             scanf("%s",sef);
             fprintf(f,"\n sef_catedra = %s",sef);
             printf("\n Introduceti numele catedrei: ");
             scanf("%s",catedra);
             fprintf(f,"\n catedra = %s",catedra);
             printf("\n Introduceti numarul lunii: ");
             scanf("%d",&luna);
             fprintf(f,"\n luna = %d",luna);
             printf("\n Introduceti tipul fisierul ce urmeaza fi generat: ");
             scanf("%s",tip);
             fprintf(f,"\n tip_fisier = %s",tip);
             // se introduc datele pentru ore
             next="Da";
             no=0;
             while(strcmp(next,"Da")==0)
             {
             no++;
             fprintf(f,"\n\n[ore/%d]",no);
             printf("\n Introduceti facultatea(abreviat): ");
             scanf("%s",fac);
             fprintf(f,"\n facultate = %s",fac);
             printf("\n Introduceti disciplina(abreviat): ");
             scanf("%s",&dis);
             fprintf(f,"\n facultate = %s",dis);
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
             fprintf(f,"\n facultate = %s",zi);
             printf("\n Introduceti intervalul orar: ");
             scanf("%d-%d",&oi,&of);
             fprintf(f,"\n ore = %d-%d",oi,of);
             printf("\n Odata la cate saptamani se tine cursul/aplicatia: ");
             scanf("%d",par);
             fprintf(f,"\n paritate = %s",par);
             printf("\n In ce saptamana se tine cursul/aplicatia prima data:");
             scanf("%d",pari);
             fprintf(f,"\n paritate_start = %s",pari);
             printf("\n Doriti sa mai introduceti inca un set de ore?(Da/Nu)")
             scanf("%s",next);
             }    
	     }
	     else 
              usage();
	}
	return 0;
}
