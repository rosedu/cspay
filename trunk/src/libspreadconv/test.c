#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "spreadconv.h"

/*
 * RD: $ cat /usr/include/errno.h | grep errno
 * No need for variable declaration
 */
/*extern int errno;*/

int
main(void)
{
	struct spreadconv_data *d;
	struct spreadconv_cell_style *cell_style;
	struct spreadconv_rc_style *col_style;
	int ce1, ce2, co1;
	char *name;

	d = spreadconv_new_spreadconv_data("Date", 50, 5);
	if (d == 0) {
		perror("spreadconv_new_spreadconv_data");
		return errno;
	}

	cell_style = malloc(sizeof *cell_style);
	cell_style->name = strdup("ce1");
	cell_style->border = strdup("2pt solid #0000FF");
	ce1 = spreadconv_add_unique_cell_style(cell_style, d);
	if (ce1 == -1) {
		perror("spreadconv_add_unique_cell_style");
		return errno;
	}

	cell_style = malloc(sizeof *cell_style);
	cell_style->name = strdup("ce2");
	cell_style->border_bottom = strdup("2pt solid #FF00FF");
	ce2 = spreadconv_add_unique_cell_style(cell_style, d);
	if (ce2 == -1) {
		perror("spreadconv_add_unique_cell_style");
		return errno;
	}

	col_style = malloc(sizeof *col_style);
	col_style->name = strdup("co1");
	col_style->type = LSC_STYLE_COL;
	col_style->size = strdup("5cm");
	co1 = spreadconv_add_unique_rc_style(col_style, d);
	if (co1 == -1) {
		perror("spreadconv_add_unique_rc_style");
		return errno;
	}

	spreadconv_set_cell_style(0, 0, ce1, d);
	spreadconv_set_cell_style(0, 1, ce2, d);
	spreadconv_set_col_style(0, co1, d);

	/*
	 * Lines like the one below are EVIL! Always use strdup(),
	 * because the free routine attempts to free all strings.
	 * d->cells[0][0].value_type = "float";
	 */
	d->cells[0][0].value_type = strdup("float");
	d->cells[0][0].text = strdup("12");
	d->cells[0][1].value_type = strdup("float");
	d->cells[0][1].text = strdup("1.5");
	d->cells[0][2].value_type = strdup("formula");
	d->cells[0][2].text = strdup("=sum(A1:B1)");

	d->cells[0][3].text = strdup("ceva maaaaaareeeeeeeeeeee");
	d->cells[0][4].text = strdup("altceva");

	d->cells[1][0].text = strdup("Testing escape sequences:");
	d->cells[2][0].text = strdup("\'");
	d->cells[3][0].text = strdup("\"");
	d->cells[4][0].text = strdup("\\");
	d->cells[5][0].text = strdup("a\nb");
	d->cells[6][0].text = strdup("a&c");
	d->cells[7][0].text = strdup("<test>");

	spreadconv_dir_name = strdup("/tmp/libspreadconv");
	name = spreadconv_create_spreadsheet(d, LSC_FILE_ODS);
	if (name == 0) {
		perror("spreadconv_create_spreadsheet");
		return errno;
	}

	printf("Created file is: \n\t\t%s/%s.ods\n\n", spreadconv_dir_name, name);

	printf("Size of spreadconv data structure is: %d bytes.\n", sizeof *d);
	printf("Date structura:\n\tNume foaie:\t\t%s\n\tNr linii:\t\t%d\n\tNr coloane:\t\t%d\n",
			d->name, d->n_rows, d->n_cols);

	free(name);
	spreadconv_free_spreadconv_data(d);
	free(spreadconv_dir_name);
	return 0;
}
