#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "spreadconv.h"
/*
 * align test
 */
int
main(void)
{
	struct spreadconv_data *doc;
	struct spreadconv_cell_style *cs_left, *cs_center, *cs_right;
	int left, right, center;
	char *name;

	doc = spreadconv_new_spreadconv_data("Align_Test", 50, 5);

	cs_left = calloc(1, sizeof (struct spreadconv_cell_style));
	cs_left->name = strdup("align_to_left");
	cs_left->halign = strdup("start");
	assert((left = spreadconv_add_unique_cell_style(cs_left, doc)) >= 0);
	doc->cells[0][0].text = strdup("<-");

	cs_right = calloc(1, sizeof (struct spreadconv_cell_style));
	cs_right->name = strdup("align_to_right");
	cs_right->halign = strdup("end");
	right = spreadconv_add_unique_cell_style(cs_right, doc);
	doc->cells[0][2].text = strdup("->");

	cs_center = calloc(1, sizeof (struct spreadconv_cell_style));
	cs_center->name = strdup("align_to_center");
	cs_center->halign = strdup("center");
	center = spreadconv_add_unique_cell_style(cs_center, doc);
	doc->cells[0][1].text = strdup("<->");

/*
 * Atentie!
 * spreadconv_set_cell_style au voie sa fie apelate *doar*
 * dupa ce s-au adaugat toate stilurile.
 *
 * Aceasta se datoreza faptului ca in timpul apelului lui
 * spread_conv_add_unique_rc_style adresele pe care le-am
 * setat cu spreadconv_set_cell_style se pot modifica, 
 * nemaindicand ce trebuie (pentru ca se apeleaza realloc).
 */
	spreadconv_set_cell_style(0, 1, center, doc);
	spreadconv_set_cell_style(0, 2, right, doc);
	spreadconv_set_cell_style(0, 0, left, doc);

	name = spreadconv_create_spreadsheet(doc, LSC_FILE_ODS);
	printf("Created file is: \n\t\t%s/%s.ods\n\n", spreadconv_dir_name, name);
	free(name);
	spreadconv_free_spreadconv_data(doc);
	free(spreadconv_dir_name);
	return 0;
}
