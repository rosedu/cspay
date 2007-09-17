#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "spreadconv.h"
/*
 * span test
 */
int
main(void)
{
	struct spreadconv_data *doc;
	struct spreadconv_cell_style *cs_tall, *cs_wide, *cs_square;
	int tall, wide, square;
	char *name;

	doc = spreadconv_new_spreadconv_data("Align_Test", 50, 5);

	cs_tall = calloc(1, sizeof (struct spreadconv_cell_style));
	cs_tall->name = strdup("tall");
	cs_tall->row_span = strdup("3");
	cs_tall->valign = strdup("middle");
	cs_tall->halign = strdup("center");
	cs_tall->border_bottom = strdup("2pt solid #000000");
	cs_tall->border_right = strdup("1pt solid #000000");
	assert((tall = spreadconv_add_unique_cell_style(cs_tall, doc)) >= 0);
	doc->cells[0][0].text = strdup("Inalta");

	cs_wide = calloc(1, sizeof (struct spreadconv_cell_style));
	cs_wide->name = strdup("wide");
	cs_wide->halign = strdup("center");
	cs_wide->valign = strdup("middle");
	cs_wide->col_span = strdup("3");
	cs_wide->border_right = strdup("2pt solid #000000");
	wide = spreadconv_add_unique_cell_style(cs_wide, doc);
	doc->cells[0][1].text = strdup("Lata");

	cs_square = calloc(1, sizeof (struct spreadconv_cell_style));
	cs_square->name = strdup("square");
	cs_square->halign = strdup("center");
	cs_square->valign = strdup("middle");
	cs_square->row_span = strdup("3");
	cs_square->col_span = strdup("3");

	square = spreadconv_add_unique_cell_style(cs_square, doc);
	doc->cells[1][1].text = strdup("Patrat");

	spreadconv_set_cell_style(0, 0, tall, doc);
	spreadconv_set_cell_style(0, 1, wide, doc);
	spreadconv_set_cell_style(1, 1, square, doc);
	name = spreadconv_create_spreadsheet(doc, LSC_FILE_ODS);
	printf("Created file is: \n\t\t%s/%s.ods\n\n", spreadconv_dir_name, name);
	free(name);
	spreadconv_free_spreadconv_data(doc);
	free(spreadconv_dir_name);
	return 0;
}
