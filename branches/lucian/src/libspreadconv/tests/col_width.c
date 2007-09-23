#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spreadconv.h"

/*
 * se teseteaza conversia latimii coloanelor
 * Daca fisierul ods este identic cu cel xls
 */

int
main(void)
{
	struct spreadconv_data *doc;
	struct spreadconv_rc_style *cs;

	int cs_[50];
	char *cn;
	char *sz;
	char *fn;
	float rlsz;
	int i;
	doc = spreadconv_new_spreadconv_data("Data", 2, 51);
	#define START	5
	#define I	(i - START)
	for (i = START; i < 50; ++ i) {
		cn = malloc(512);
		snprintf(cn, 512, "co%d", i);
		cs = calloc(1, sizeof (struct spreadconv_rc_style));
		cs->name = cn;
		cs->type = LSC_STYLE_COL;
		sz = malloc(512);
		rlsz = (float) i / 10;
		snprintf(sz, 512, "%2.1fcm", rlsz);
		cs->size = sz;
		doc->cells[0][I].text = strdup(sz);
		cs_[I] = spreadconv_add_unique_rc_style(cs, doc);
	}
	for (i = START; i < 50; ++ i) {
		spreadconv_set_col_style(I, cs_[I], doc);
	}

	spreadconv_dir_name = NULL;
	fn = spreadconv_create_spreadsheet(doc, LSC_FILE_ODS);
	if (!fn) {
		perror("ods");
		return -1;
	}
	printf("%s\n", fn);
	free(fn);
	fn = spreadconv_create_spreadsheet(doc, LSC_FILE_XLS);
	if (!fn) {
		perror("xls");
		return -1;
	}
	printf("%s\n", fn);
	free(fn);
	spreadconv_free_spreadconv_data(doc);
	return 0;
}


