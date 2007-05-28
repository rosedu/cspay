#include "cspay.h"
#include <stdlib.h>

void
cspay_free_file_list(struct cspay_file_list *list)
{
	int i;

	for (i=0; i<=list->nr; i++)
	       free(list->names[i]);
	free(list->names);
	free(list);
}		       
