/*
	program in consola (cspay), foloseste biblioteca libcspay. Este fie interactiv, fie apelat cu un fisier ca parametru
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "../libcspay/cspay.h"

int
main( int argc, char **argv )
{
	struct cspay_file_list *lista;
	
	if( argc < 2 ) {
		//mod interactiv
		printf("Not yet");
		exit(-1);
	}
	else {
		//am primit `personal.ini'
		lista = cspay_convert_options( argv[1] );
		int i;
		for( i = 0; i < lista->nr; i++)
			printf( lista->names[i] );
	}
	return 0;
}
