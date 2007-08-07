/*
	program in consola (cspay), foloseste biblioteca libcspay. Este fie 
    interactiv, fie apelat cu un fisier ca parametru.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "../libcspay/cspay.h"
#if 0	/* win 32 ?*/
#	include "E:\CSpay\trunk\src\libcspay"
#endif
void 
usage(void)
{
     fprintf(stderr,"Incorrect syntax\n\n");
     printf("The CSpay application allows two types of usages:\n");
     printf("The interactive usage in which the user has to answer queries \n");
     printf("so that the personal configuration file(ini file) is generated.\n");
     printf("The correct syntax for the interactive usage is:\n");
     printf("cspay -i -x <xml file name>\n\n");
     printf("The non-interactive usage in which the user does not need to  \n");
     printf("supply any extra information, both the personal configuration \n");
     printf("file(ini file) and the general configuration file(xml file) have\n");
     printf("been previously created through the GUI or the Web interface\n");
     printf("The correct syntax for the non-interactive usage is:\n");
     printf("cspay -n -x <xml file name> -f <ini file name>");
     exit(1);
}
int
main( int argc, char **argv )
{	
    int i;
    char *inif,*xmlf;
    struct cspay_file_list *lista;
	struct cspay_config *cf;
	for(i=0;i<argc;i++)
       printf("%s ",argv[i]);
	if( argv[1]=="-n")
    { 
        //mod neinteractiv 
        if (strcmp(argv[2],"-x")==0 && argv[3][0]!='-')
        {   
            //xmlf=(char*)malloc(strlen(argv[2]));
            //strcpy(xmlf,argv[2]);
            cf=cspay_get_config(xmlf);
            printf("xml functional");
                    if(strcmp(argv[4],"-f")==0 && argv[5][0]!='-')
                       {
                        lista = cspay_convert_options( cf, argv[4]);
		    		          for( i = 0; i < lista->nr; i++)
		                           printf( lista->names[i] );
                        printf("ini functional");
                       }
                       else usage();
        }
        else usage();
	}
	else 
	     {
         if( argv[1]=="-i")
         {
         //mod interactiv
         if (strcmp(argv[2],"-x")==0 && argv[3][0]!='-')
        {
            cf=cspay_get_config(argv[1]);
        }
         else usage();
		 printf("Not yet");
		 exit(-1);
	     }
	     else { printf("\n\nTEST!!!\n\n"); usage();}
         }
	return 0;
}
