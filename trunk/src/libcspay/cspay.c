#include <stdlib.h>	/* system headers go before user headers */

#include "getopt.h"
#include "cspay.h"

void
cspay_free_file_list(struct cspay_file_list *list)
{
	int i;

	/* RD: someone should beware of code readability - use blank */
	for (i = 0; i <= list->nr; i++)
	       free(list->names[i]);
	free(list->names);
	free(list);
}		       

struct cspay_config *
cspay_get_config(void)
{
	/*TODO poate ceva verificari aici?*/
	return read_cspay_xml("cspay.xml");
}

int 
main(void)
{
	read_cspay_xml(NULL);
	return 0;
}
