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
     exit(1);
}

int
main( int argc, char **argv )
{	
    int i;
    char *inif,*xmlf;
    struct cspay_file_list *lista;
	struct cspay_config *config;
	
	if( argc < 4 ) {
		usage();
		exit(-1);
	}
	if(strcmp(argv[1],"-n")==0 && argc==6)
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
         if(strcmp(argv[1],"-i")==0 && argc==4) {
	         printf("interactive\n");
	         //mod interactiv
	         if (strcmp(argv[2],"-x")==0 && argv[3][0]!='-') {
	            config = cspay_get_config(argv[1]);
	         }
	         else usage();
			 printf("Not yet");
			 exit(-1);
	     }
	     else { 
	     	printf("\n\nTEST!!!%d\n\n",argc); usage();
	     }
	}
	return 0;
}
