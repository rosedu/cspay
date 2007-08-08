/*
 * for testing libcspay
 */
#include <stdio.h>
#include <time.h>

#include "cspay.h"

int
main(int argc, char **argv)
{
	struct cspay_config *cfg;
	struct cspay_file_list *fl;
	if (argc != 3) {
		printf("%s xml_file ini_file\n", argv[0]);
		return -1;
	}
	cfg = cspay_get_config(argv[1]);
	fl = cspay_convert_options(cfg, argv[2]);
	cspay_free_config(cfg);
	int i;
	for (i = 0; i < fl->nr; ++ i)
		printf("%s.ods\n", fl->names[i]);
	cspay_free_file_list(fl);
	return 0;
}
