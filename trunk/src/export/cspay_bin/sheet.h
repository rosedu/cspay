#ifndef _SHEET_H
#define _SHEET_H 1

/**
 * \struct defined_styles
 * contains informations used for creating spreadsheets
 */
struct defined_styles {
	/**
	 * header table cell styles
	 * \remark same style for all cell from header
	 */
	int table_h;

	/**
	 * table content styles
	 * table_c[0], first col
	 * table_c[1], 1, 2, 3, 4, 5,  col
	 * table_c[2], 6, 7, 8 col
	 */
	int table_c[3];

	/**
	 * last row styles
	 * table_b[0], 0 col
	 * table_b[1], 1, 2, 3, 4, 5 col
	 * table_b[2], 6, 7, 8 col
	 */
	int table_b[3];

	/**
	 * row/columns styles
	 * rc[0],  col 0.6cm (0)
	 * rc[1],  col 1.5cm (4, 5, 6, 7, 8)
	 * rc[2],  col 2.0cm (2, 3)
	 * rc[3],  row 1cm
	 */
	int rc[4];
	/**
	 * XXX this is a hack
	 */
	struct spreadconv_data *doc;
};

/**
 * Config styles for tables, rows and columns
 * \return 0 on succes
 * \return -1 on error
 */
int config_styles (struct spreadconv_data *doc);

/** styles for spreasheets*/
extern struct defined_styles *ds;

#endif /* _SHEET_H */

