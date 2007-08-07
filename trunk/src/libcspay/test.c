/*
 * for testing libcspay
 */
#include <stdio.h>
#include <time.h>

#include "cspay.h"

int
main()
{
	struct cspay_config *cfg;
	struct cspay_file_list *fl;
	cfg = cspay_get_config(NULL);
	fl = cspay_convert_options(cfg, "personal.ini");
	cspay_free_config(cfg);
	cspay_free_file_list(fl);
	return 0;
}
