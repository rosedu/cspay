#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "spreadconv.h"
#include "sheet.h"

/** styles for spreasheets*/
struct defined_styles *ds;

/**
 * Config styles for tables, rows and columns
 * \return 0 on succes
 * \return -1 on error
 */
int 
config_styles (struct spreadconv_data *doc)
{
	/* configure columns */
	struct spreadconv_cell_style *ccs;
	struct spreadconv_rc_style *rcs;

	ds = calloc(1, sizeof (struct defined_styles));
	ds->doc = doc;

	Dprintf("Begin row/columns styles\n");

	rcs = calloc(1, sizeof (struct spreadconv_rc_style));
	rcs->type = LSC_STYLE_COL;
	rcs->name = strdup("col_0_6_cm");
	rcs->size = strdup("0.6cm");
	ds->rc[0] = spreadconv_add_unique_rc_style(rcs, doc);

	rcs = calloc(1, sizeof (struct spreadconv_rc_style));
	rcs->type = LSC_STYLE_COL;
	rcs->name = strdup("col_1_5_cm");
	rcs->size = strdup("1.5cm");
	ds->rc[1] = spreadconv_add_unique_rc_style(rcs, doc);

	rcs = calloc(1, sizeof (struct spreadconv_rc_style));
	rcs->type = LSC_STYLE_COL;
	rcs->name = strdup("col_2_0_cm");
	rcs->size = strdup("2cm");
	ds->rc[2] = spreadconv_add_unique_rc_style(rcs, doc);

	rcs = calloc(1, sizeof (struct spreadconv_rc_style));
	rcs->type = LSC_STYLE_ROW;
	rcs->name = strdup("row_1_0_cm");
	rcs->size = strdup("1cm");
	ds->rc[3] = spreadconv_add_unique_rc_style(rcs, doc);
	
	spreadconv_set_col_style(0, ds->rc[0], doc);
	spreadconv_set_col_style(2, ds->rc[2], doc);
	spreadconv_set_col_style(3, ds->rc[2], doc);
	spreadconv_set_col_style(4, ds->rc[1], doc);
	spreadconv_set_col_style(5, ds->rc[1], doc);
	spreadconv_set_col_style(6, ds->rc[1], doc);
	spreadconv_set_col_style(7, ds->rc[1], doc);
	spreadconv_set_col_style(8, ds->rc[1], doc);
	spreadconv_set_row_style(6, ds->rc[3], doc);
	
	/* cell styles */
	Dprintf("Begin cells styles\n");

	/* Table Header */
	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_header_1");
	ccs->halign = strdup("center");
	ccs->valign = strdup("middle");
	ccs->border = strdup("2pt solid #000000");
	ds->table_h = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_h < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}

	/* Table content */
	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_content_1");
	ccs->border_bottom = strdup("1pt solid #000000");
	ccs->border_left = strdup("2pt solid #000000");
	ccs->border_right = strdup("2pt solid #000000");
	ccs->halign = strdup("end");
	ds->table_c[0] = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_c[0] < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}

	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_content_2");
	ccs->border_bottom = strdup("1pt solid #000000");
	ccs->border_left = strdup("2pt solid #000000");
	ccs->border_right = strdup("2pt solid #000000");
	ccs->halign = strdup("center");
	ds->table_c[1] = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_c[1] < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}

	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_content_3");
	ccs->border_bottom = strdup("1pt solid #000000");
	ccs->border_left = strdup("2pt solid #000000");
	ccs->border_right = strdup("2pt solid #000000");
	ccs->halign = strdup("start");
	ds->table_c[2] = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_c[2] < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}
	/* Table bottom */
	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_bottom_1");
	ccs->border_bottom = strdup("2pt solid #000000");
	ccs->border_left = strdup("2pt solid #000000");
	ccs->border_right = strdup("2pt solid #000000");
	ccs->halign = strdup("end");
	ds->table_b[0] = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_b[0] < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}

	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_bottom_2");
	ccs->border_bottom = strdup("2pt solid #000000");
	ccs->border_left = strdup("2pt solid #000000");
	ccs->border_right = strdup("2pt solid #000000");
	ccs->halign = strdup("center");
	ds->table_b[1] = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_b[1] < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}

	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_bottom_3");
	ccs->border_bottom = strdup("2pt solid #000000");
	ccs->border_left = strdup("2pt solid #000000");
	ccs->border_right = strdup("2pt solid #000000");
	ccs->halign = strdup("start");
	ds->table_b[2] = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_b[2] < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}
	return 0;

	ERR_TS:
	free(ds);
	return -1;

}
