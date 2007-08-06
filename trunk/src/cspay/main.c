/*
	program in consola (cspay), foloseste biblioteca libcspay. Este fie 
    interactiv, fie apelat cu un fisier ca parametru.
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "../libcspay/cspay.h"

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
     return 1;
}
int
main( int argc, char **argv )
{	
    int i;
    struct cspay_file_list *lista;
	struct cspay_config *cf;
	if( argv[0]=="-n")
    { 
        //mod neinteractiv 
        if (argv[1]=='-x' && argv[2][0]!='-')
        {
            cf=cspay_get_config(argv[1]);
                    if(argv[3]=='-x' && argv[4][0]!='-')
                       {
                        lista = cspay_convert_options( argv[1] , cf);
		    		          for( i = 0; i < lista->nr; i++)
		                           printf( lista->names[i] );
                       }
                       else usage();
        }
        else usage();
	}
	else 
	     {
         if( argv[0]=="-i")
         {
         //mod interactiv
         if (argv[1]=='-x' && argv[2][0]!='-')
        {
            cf=cspay_get_config(argv[1]);
        }
         else usage();
		 printf("Not yet");
		 exit(-1);
	     }
	     else usage();
         }
	return 0;
}
