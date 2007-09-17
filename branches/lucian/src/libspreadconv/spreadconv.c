/*
 * libspreadconv main file
 * First draft: 15.06.2007, Vlad Dogaru
 * Mon Sep 17 21:04:52 EEST 2007, Cojocar Lucian
 */

/**
 * \ingroup libspreadconv
 * \file spreadconv.c
 * \brief libspreadconv main implementation file
 * \author Vlad Dogaru, Cojocar Lucian
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>

#include "spreadconv.h"
#include "ods.h"
#include "xls.h"
#include "debug.h"

char *
spreadconv_create_spreadsheet(struct spreadconv_data *data, int file_type)
{ 
	switch (file_type) {
		case LSC_FILE_ODS:
			return ods_create_spreadsheet(data);
		case LSC_FILE_XLS:
			return xls_create_spreadsheet(data);
		default:
			return NULL;
	}
}

/**
 * Creates a new \a spreadconv_data structure, with the specified
 * dimensions. This function also allocates the needed space for 
 * the arrays contained in the structure.
 * \param sheet_name the sheet name (will be duplicated, so a constant
 * can be supplied)
 * \param rows number of rows in the table
 * \param cols number of columns in the table
 * \return a pointer to the newly-created structure or NULL if not
 * enough memory
 */
struct spreadconv_data *
spreadconv_new_spreadconv_data(char *sheet_name, int rows, int cols)
{
	struct spreadconv_data *data = 0;
	int i, j;
	
	data = malloc(sizeof *data);
	if (data == 0)
		return 0;

	if (sheet_name == 0)
		data->name = strdup("Sheet 1");
	else
		data->name = strdup(sheet_name);
	data->n_rows = rows;
	data->n_cols = cols;

	/* 
	 * Unique styles arrays are (re)allocated as needed by the
	 * style-adding functions.
	 */
	data->n_unique_rc_styles = data->n_unique_cell_styles = 0;
	data->unique_rc_styles = 0;
	data->unique_cell_styles = 0;

	/*
	 * Row and column styles, as well as the cell array, have a
	 * fixed size, so they are allocated here.
	 *
	 * Our own soul is sold to the error-fairy Satan for the sole
	 * reason of treating errors elegantly and not leaving allocated
	 * memory around in case of an error.
	 *
	 * calloc() is used for row_styles and col_styles, since we need
	 * the pointers to be NULL if they are not explicitly set by the
	 * user.
	 */
	data->row_styles = calloc(data->n_rows, sizeof *data->row_styles);
	if (data->row_styles == 0) {
		free(data);
		return 0;
	}

	data->col_styles = calloc(data->n_cols, sizeof *data->col_styles);
	if (data->col_styles == 0) {
		free(data->row_styles);
		free(data);
		return 0;
	}

	data->cells = malloc(data->n_rows * sizeof *data->cells);
	if (data->cells == 0) {
		free(data->row_styles);
		free(data->col_styles);
		free(data);
		return 0;
	}

	for (i=0; i<data->n_rows; i++) {
		/*
		 * Again, using calloc for the cells, since we need
		 * zeroes where fields are not explicitly set by the
		 * user.
		 */
		data->cells[i] = calloc(data->n_cols, sizeof *data->cells[i]);
		if (data->cells[i] == 0) {
			for (j=0; j<i; j++)
				free(data->cells[j]);
			free(data->cells);
			free(data->row_styles);
			free(data->col_styles);
			free(data);
		}
	}

	/* And just when you thought it would never end... */
	return data;

	/* 
	 * PS: Pe cazul general, as folosi o stiva de pointeri. Cand as
	 * aloca un pointer, l-as pune in stiva. Daca pe parcursul celor
	 * n alocari una merge prost, as scoate din stiva si dealoca
	 * pana cand stiva ar fi goala, apoi as iesi cu eroare. 
	 *
	 * Useless? Da. Functional? Voi ce ziceti?
	 * -Vlad
	 */
}

/**
 * Frees all memory used by a \a spreadconv_data structure.
 * \param data The structure to free
 */
void spreadconv_free_spreadconv_data(struct spreadconv_data *data)
{
	int i, j;

	/* 
	 * Have no idea why, but this doesn't work. I shamelessly copied
	 * Razvan's snippet from an older email into debug.h, but it's a
	 * no-go.
	 * Dprintf("in spreadconv_free_spreadconv_data\n");
	 */

	/* Free all cell data */
	for (i=0; i<data->n_rows; i++) {
		for (j=0; j<data->n_cols;j++) {
			if (data->cells[i][j].value_type != 0)
				free(data->cells[i][j].value_type);
			if (data->cells[i][j].text != 0)
				free(data->cells[i][j].text);
			data->cells[i][j].style = 0;
		}
		free(data->cells[i]);
	}
	free(data->cells);

	/* Free all style data */
	free(data->col_styles);
	free(data->row_styles);
	for (i = 0 ;i < data->n_unique_rc_styles; i++) {
		struct spreadconv_rc_style *style = &data->unique_rc_styles[i];
		if (style->name != 0)
			free(style->name);
		if (style->size != 0)
			free(style->size);
	}
	if (data->unique_rc_styles != 0) 
		free(data->unique_rc_styles);
	for (i = 0; i < data->n_unique_cell_styles; i++) {
		struct spreadconv_cell_style *style = &data->unique_cell_styles[i];
		if (style->name != 0)
			free(style->name);
		if (style->valign != 0)
			free(style->valign);
		if (style->halign != 0)
			free(style->halign);
		if (style->border != 0)
			free(style->border);
		if (style->border_top != 0)
			free(style->border_top);
		if (style->border_bottom != 0)
			free(style->border_bottom);
		if (style->border_left != 0)
			free(style->border_left);
		if (style->border_right != 0)
			free(style->border_right);
		if (style->row_span != 0)
			free(style->row_span);
		if (style->col_span != 0)
			free(style->col_span);
	}
	if (data->unique_cell_styles != 0)
		free(data->unique_cell_styles);

	/* Free the sheet name and finally the entire structure */
	free(data->name);
	free(data);
}


/**
 * Adds a new unique row or column style to a spreadsheet. The style
 * pointer is afterwards freed and set to NULL, so that it can be used
 * again for adding another style.
 * \param style The style to add
 * \param data Pointer to the structure that the style should be added
 * to
 * \return The position in \a unique_rc_array where the style has been
 * added or -1 if there was an error
 * \remarks A return value of 0 is perfectly correct. The style array,
 * like any other C array, is zero-indexed.
 */
int spreadconv_add_unique_rc_style(struct spreadconv_rc_style *style, 
		struct spreadconv_data *data)
{
	if (data->unique_rc_styles == 0) 
		data->unique_rc_styles = malloc(sizeof *data->unique_rc_styles);
	else
		data->unique_rc_styles = realloc(data->unique_rc_styles,
				(data->n_unique_rc_styles + 1) * 
					sizeof *data->unique_rc_styles);

	if (data->unique_rc_styles == 0)
		return -1;

	data->unique_rc_styles[data->n_unique_rc_styles++] = *style;
	style->name = style->size = 0;
	style->type = 0;
	free(style);
	return data->n_unique_rc_styles-1;
}

/**
 * Adds a new unique cell style to a spreadsheet. The style
 * pointer is afterwards freed and set to NULL, so that it can be used
 * again for adding another style.
 * \param style The style to add
 * \param data Pointer to the structure that the style should be added
 * to
 * \return The position in \a unique_cell_array where the style has been
 * added or -1 if there was an error
 * \remarks A return value of 0 is perfectly correct. The style array,
 * like any other C array, is zero-indexed.
 */
int spreadconv_add_unique_cell_style(struct spreadconv_cell_style *style, 
		struct spreadconv_data *data)
{
	if (data->unique_cell_styles == 0) 
		data->unique_cell_styles = malloc(sizeof *data->unique_cell_styles);
	else
		data->unique_cell_styles = realloc(data->unique_cell_styles,
				(data->n_unique_cell_styles + 1) * 
					sizeof *data->unique_cell_styles);

	if (data->unique_cell_styles == 0)
		return -1;

	data->unique_cell_styles[data->n_unique_cell_styles++] = *style;
	/* 
	 * Clumsy as it seems, we set all fields to NULL here so that if
	 * another style of the same type gets malloc'ed over it it
	 * doesn't get its old properties.
	 *
	 * Note that we cannot free the fields, as they values to which
	 * they had pointed are now pointed to by the unique_cell_styles
	 * fields.
	 */
	style->name = style->valign = style->halign = style->border = style->border_top = 
		style->border_bottom = style->border_left = style->border_right = 
		style->row_span = style->col_span = 0;
	free(style);
	return data->n_unique_cell_styles-1;
}

/**
 * Associates a row with a unique style
 * \param row the row number
 * \param style the style number
 * \param data the associated data structure
 * \return 0 on success, -1 on error
 */
int spreadconv_set_row_style(int row, int style, struct spreadconv_data *data)
{
	if (row >= data->n_rows)
		return -1;
	if (style >= data->n_unique_rc_styles)
		return -1;
	if (data->unique_rc_styles[style].type == LSC_STYLE_COL)
		return -1;
	data->row_styles[row] = &data->unique_rc_styles[style];
	return 0;
}

/**
 * Associates a column with a unique style
 * \param col the column number
 * \param style the style number
 * \param data the asociated data structure
 * \return 0 on success, -1 on error
 */
int spreadconv_set_col_style(int col, int style, struct spreadconv_data *data)
{
	if (col >= data->n_cols)
		return -1;
	if (style >= data->n_unique_rc_styles)
		return -1;
	if (data->unique_rc_styles[style].type == LSC_STYLE_ROW)
		return -1;
	data->col_styles[col] = &data->unique_rc_styles[style];
	return 0;
}

/**
 * Associates a cell with a unique cell style
 * \param row the row of the cell
 * \param col the column of the cell
 * \param style the style number
 * \param data the associated data structure
 * \return 0 on success, -1 on error
 */
int spreadconv_set_cell_style(int row, int col, 
		int style, struct spreadconv_data *data)
{
	if ((row >= data->n_rows) || (col >= data->n_cols))
		return -1;
	if (style >= data->n_unique_cell_styles)
		return -1;
	data->cells[row][col].style = &data->unique_cell_styles[style];
	return 0;
}

