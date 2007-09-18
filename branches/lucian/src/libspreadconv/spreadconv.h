/*
 * libspreadconv header
 * First draft: 16.05.2007, Vlad Dogaru
 */

/**
 * \ingroup libspreadconv
 * \file spreadconv.h
 * libspreadconv header file
 * \author Vlad Dogaru
 * \defgroup libspreadconv Library for creating spreadsheet files
 */

#ifndef _SPREADCONV_H_
#define _SPREADCONV_H_

/**
 * ODF spreadsheet
 * \remarks Initially, only ODS will be implemented. Others will be 
 * included via external converters.
 */
#define LSC_FILE_ODS		1
#define LSC_FILE_XLSX		2 /**< Office Open XML spreadsheet */
#define LSC_FILE_XLS		4 /**< Classic MS Excel spreadsheet */

#define LSC_STYLE_ROW		1 /**< specifies a row style */
#define LSC_STYLE_COL		2 /**< specifies a column style */

/** Application name */
#define LSC_NAME 		"libspreadconv"
/** Application version */
#define LSC_VERSION		"0.1"

/**
 * \struct spreadconv_rc_style
 * style information for a spreadsheet row or column
 */
struct spreadconv_rc_style {
	/** style name */
	char *name;
	/** stores whether the style is for a row or for a column */
	char type;
	/** 
	 * size information -- row height or column width. This is a
	 * char* because the user can specify a measure unit right after
	 * the value, such as "5cm".
	 * WARNING for xls file, uso only cm.
	 */
	char *size;
};

/**
 * \struct spreadconv_cell_style
 * style information for a spreadsheet cell
 */
struct spreadconv_cell_style {
	/** style name */
	char *name;
	/** vertical alignment
	 * \remarks Possible values include: top, middle, bottom,
	 * automatic */
	char *valign;
	/** horizontal alignment
	 * \remarks Possible values: center, start, end,
	 * left, right, justify */
	char *halign;
	/** number of rows on wich the cell is spanned
	 * \remarks Possible values: "1", "2", "3", etc.
	 * default is "1";
	 */
	char *row_span;
	/** number of columns on wich the cell is spanned
	 * \remarks Possible values: "1", "2", "3", etc.
	 * default is "1";
	 */
	char *col_span;
	/** 
	 * general border specifier
	 * \remarks If this is set, all other border specificators are
	 * ignored and the cell is identically bordered on all four
	 * sides.
	 *
	 * Borders are specified as strings with three space-separated
	 * entities as follows:
	 *  - width (e.g. 0.05in, might also be possible to use cm)
	 *  	WARNING	for xls file uso *only* pt
	 *  - line style (solid should be the only one needed)
	 *  - color, in HTML code (#000000 for black)
	 */
	char *border;
	/** top border specifier */
	char *border_top;
	/** bottom border specifier */
	char *border_bottom;
	/** left border specifier */
	char *border_left;
	/** right border specifier */
	char *border_right;
};

/**
 * \struct spreadconv_cell
 * spreadsheet cell structure
 * \remarks suggested by Razvan. Thank you :-)
 */
struct spreadconv_cell {
	/** 
	 * The value type of the cell.
	 * \remarks Numbers should get "float", regardless if they are
	 * float or integer. Other possible values are date, percentage,
	 * currency, time, boolean or string, although they might not
	 * all be implemented. String is assumed if this is NULL. A
	 * special value (not in the standard) is ``formula'', which
	 * will cause libspreadconv to generate code for a cell
	 * containing the formula specified in \a text.
	 */
	char *value_type;
	/**
	 * the text contained in the cell.
	 * \remarks Although \a value_type may be numerical, its value is
	 * stored as a string.
	 */
	char *text;
	/**
	 * the cell's style
	 *
	 * This references a \a spreadconv_cell_style contained within
	 * the same \a spreadconv_data structure.
	 */
	struct spreadconv_cell_style *style;
};

/**
 * \struct spreadconv_data
 * generic spreadsheet data container
 * \remarks for the moment, only single-sheet spreadsheets are supported
 */
struct spreadconv_data {
	/** name of the sheet */
	char *name;
	/** number of rows */
	int n_rows;
	/** number of columns */
	int n_cols;

	/** number of unique row/column styles */
	int n_unique_rc_styles;
	/** unique row/column style specification */
	struct spreadconv_rc_style *unique_rc_styles;

	/** 
	 * row styles. Contains a pointer to a unique row style for each
	 * row. So the length is always \a nrows.
	 */
	struct spreadconv_rc_style **row_styles;
	/** column styles. Contains a pointer to a unique row/column style
	 * for each column.  So the length is always \a ncols.
	 */
	struct spreadconv_rc_style **col_styles;

	/** number of unique cell styles */
	int n_unique_cell_styles;
	/** unique cell styles */
	struct spreadconv_cell_style *unique_cell_styles;

	/**
	 * spreadsheet cells. A \a nrows by \a ncols matrix of cells.
	 */
	struct spreadconv_cell **cells;
};

/**
 * Name of the directory in which to store generated files.
 * \remarks If it is NULL or not found, "/tmp" is used
 */
char *spreadconv_dir_name;

/**
 * Converts a generic \a spreadconv_data structure into the associated
 * file(s).
 */
char * spreadconv_create_spreadsheet(struct spreadconv_data *, int);


/**
 * Creates a new \a spreadconv_data structure, with the specified
 * dimensions. This function also allocates the needed space for 
 * the arrays contained in the structure.
 */
struct spreadconv_data * spreadconv_new_spreadconv_data(char *, int, int);

/**
 * Frees all memory used by a \a spreadconv_data structure.
 */
void spreadconv_free_spreadconv_data(struct spreadconv_data *data);

/**
 * Adds a new unique row or column style to a spreadsheet.
 */
int spreadconv_add_unique_rc_style(struct spreadconv_rc_style *, struct spreadconv_data *);

/**
 * Adds a new unique cell style to a spreadsheet.
 */
int spreadconv_add_unique_cell_style(struct spreadconv_cell_style *, struct spreadconv_data *);

/**
 * Associates a row with a unique style
 */
int spreadconv_set_row_style(int, int, struct spreadconv_data *);

/**
 * Associates a column with a unique style
 */
int spreadconv_set_col_style(int, int, struct spreadconv_data *);

/**
 * Associates a cell with a unique cell style
 */
int spreadconv_set_cell_style(int, int, int, struct spreadconv_data *);

/**
 * Changes the dimensions of a \a spreadconv_data structure.
 * \remarks Not implemented yet. The spreadsheet structure is passed as
 * last parameter for uniformity (?) with the other functions.
 */
int spreadconv_change_dimenstions(int, int, struct spreadconv_data *);

#endif /* _SPREADCONV_H_ */
