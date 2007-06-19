/*
 * libspreadconv main file
 * First draft: 15.06.2007, Vlad Dogaru
 */

/**
 * \file spreadconv.c
 * \todo Move the auxiliary functions (those at the top of the file --
 * the static ones) into a separate file.
 * \brief libspreadconv main implementation file
 * \author Vlad Dogaru
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>

#include "spreadconv.h"

/** Global external error code */
extern int errno;

/**
 * XML namespace declaration. An XML namespace--URL pair.
 * \todo This type should be hidden. Apparently static has no effect in
 * this context, so I'm all out of suggestions for now. -Vlad
 */
struct spreadconv_xml_namespace {
	/** namespace name */
	char *name;
	/** namespace URL */
	char *url;
};

/** Number of existing XML namespaces */
static const int xml_namespace_count = 8;

/**
 * Existing XML namespaces. These are all written in the root element of
 * all files in an ODS package.
 */
static const struct spreadconv_xml_namespace xml_namespaces[] = {
	{ "office", "urn:oasis:names:tc:opendocument:xmlns:office:1.0" },
	{ "table", "urn:oasis:names:tc:opendocument:xmlns:table:1.0" },
	{ "text", "urn:oasis:names:tc:opendocument:xmlns:text:1.0" },
	{ "meta", "urn:oasis:names:tc:opendocument:xmlns:meta:1.0" },
	{ "style", "urn:oasis:names:tc:opendocument:xmlns:style:1.0" },
	{ "manifest", "urn:oasis:names:tc:opendocument:xmlns:manifest:1.0" },
	{ "fo", "urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0" },
	{ "dc", "http://purl.org/dc/elements/1.1" }
};

char *
spreadconv_convert_column_number(int n)
{
	int max_value, string_length;
	char *text;
	
	if (n <= 0)
		return 0;
		
	for (max_value=26, string_length=1; 
			n > max_value;
			max_value *= 27, string_length++)
		;
	
	text = malloc(string_length+1);
	if (text == 0) 
		return 0;

	text[string_length] = 0;
	for (; string_length>=1; string_length--) {
		text[string_length-1] = (n-1) % 26 + 'A';
		n = (n-1) / 26;
	}

	return text;
}

/**
 * Prints all the needed namespaces to a stream. The namespaces are
 * formatted accordingly and should be attributes to the root element.
 * \param f The stream to write to
 */
static void
print_namespaces(FILE *f)
{
	int i;

	for (i=0; i<xml_namespace_count; i++)
		fprintf(f, "xmlns:%s=\"%s\" ", 
				xml_namespaces[i].name,
				xml_namespaces[i].url);
}

/**
 * Creates the manifest file for a package. The manifest file is
 * basically always the same for standard, minimal, unencrypted packages
 * such as the ones we are creating. Thus, there is no need to pass it
 * a \a spreadconv_data structure.
 * \param data Not currently used, but here for further expansion. We
 * thus avoid changing function prototypes later on.
 * \remarks This function assumes that the current directory is that
 * corresponding to a package and that the user is permitted to create
 * new files and directories. It creates the directory META-INF, with
 * the file manifest.xml in it.
 * \return 0 on success, -1 on error, and errno is set accordingly by
 * the last function call
 */
static int
create_manifest_file(struct spreadconv_data *data)
{
	FILE *f;
	int i;
	const char *filenames[] = { "content", "meta", "settings", "style" };
	const int nfiles = 4;

	/* attempt to create directory */
	if (mkdir("META-INF", S_IFDIR | S_IRUSR | S_IWUSR | S_IXUSR) == -1)
		return -1;

	f = fopen("META-INF/manifest.xml", "w");
	if (f == 0)
		return -1;

	fprintf(f, "<?xml version=\"1.0\"?>\n");
	fprintf(f, "<manifest:manifest ");
	print_namespaces(f);
	fprintf(f, ">\n");
	fprintf(f, "<manifest:file-entry "
			"manifest:media-type=\"application/vnd.oasis.opendocument.spreadsheet\" "
			"manifest:full-path=\"/\" />\n");
	for (i=0; i<nfiles; i++)
		fprintf(f, "<manifest:file-entry "
				"manifest:media-type=\"text/xml\" "
				"manifest:full-path=\"%s.xml\" />\n",
				filenames[i]);
	fprintf(f, "</manifest:manifest>\n");
	fclose(f);
	return 0;
}

/**
 * Creates the mimetype file. This contains a single line, specifying
 * the mimetype of the file. It must not contain an end-of-line
 * character.
 * \return 0 on success, -1 on error, and errno set accordingly.
 */
static int
create_mimetype_file(void)
{
	FILE *f;

	f = fopen("mimetype", "w");
	if (f == 0)
		return -1;

	fprintf(f, "application/vnd.oasis.opendocument.spreadsheet");

	fclose(f);
	return 0;
}

/**
 * Creates the settings file. This is basically an empty file that
 * applications should fill with their specific settings. It is present
 * in the archive for standards conformance.
 * \param data Not currently used
 * \return 0 on success, -1 on error
 */
static int
create_settings_file(struct spreadconv_data *data)
{
	FILE *f;

	f = fopen("settings.xml", "w");
	if (f == 0)
		return -1;

	fprintf(f, "<?xml version=\"1.0\"?>\n");
	fprintf(f, "<office:document-settings ");
	print_namespaces(f);
	fprintf(f, "/>\n");

	fclose(f);
	return 0;
}

/**
 * Creates the style file. This is also empty -- style information goes
 * in content.xml, because, for some unknown reason, including style
 * information in this file crashes both ods readers I have tried.
 * \param data Not currently used
 * \return 0 on success, -1 on error
 */
static int
create_style_file(struct spreadconv_data *data)
{
	FILE *f;

	f = fopen("styles.xml", "w");
	if (f == 0)
		return -1;

	fprintf(f, "<?xml version=\"1.0\"?>\n");
	fprintf(f, "<office:document-styles ");
	print_namespaces(f);
	fprintf(f, "/>\n");

	fclose(f);
	return 0;
}

/**
 * Creates the meta file. This contains information about the file
 * generator, namely libspreadconv.
 * \param data Not currently used
 * \return 0 on success, -1 on error
 */
static int
create_meta_file(struct spreadconv_data *data)
{
	FILE *f;

	f = fopen("meta.xml", "w");
	if (f == 0)
		return -1;

	fprintf(f, "<?xml version=\"1.0\"?>\n");
	fprintf(f, "<office:document-meta ");
	print_namespaces(f);
	fprintf(f, ">\n");
	fprintf(f, "<office:meta>\n");
	fprintf(f, "<meta:generator>%s %s</meta:generator>\n",
			LSC_NAME, LSC_VERSION);
	fprintf(f, "</office:meta>\n");
	fprintf(f, "</office:document-meta>\n");

	fclose(f);
	return 0;
}

/**
 * Prints a row or column style to a file.
 * \param style Pointer to the style to print
 * \param f The stream to print to
 */
static void
print_rc_style(struct spreadconv_rc_style *style, FILE *f)
{
	fprintf(f, "<style:style style:name=\"\%s\" ", style->name);
	if (style->type == LSC_STYLE_ROW) {
		fprintf(f, "style:family=\"table-row\">\n");
		fprintf(f, "<style:table-row-properties style:row-height=\"%s\" />\n",
				style->size);
	} else {
		/* assuming LSC_STYLE_COLUMN */
		fprintf(f, "style:family=\"table-column\">\n");
		fprintf(f, "<style:table-column-properties style:column-width=\"%s\" />\n",
				style->size);
	}
	fprintf(f, "</style:style>\n");
}

/**
 * Prints a cell style to a file.
 * \param style Pointer to the style to print
 * \param f The stream to print to
 */
static void
print_cell_style(struct spreadconv_cell_style *style, FILE *f)
{
	fprintf(f, "<style:style style:name=\"%s\" ", style->name);
	fprintf(f, "style:family=\"table-cell\">\n");
	fprintf(f, "<style:table-cell-properties ");
	if (style->valign != 0) 
		fprintf(f, "style:vertical-align=\"%s\" ", style->valign);
	if (style->border != 0)
		fprintf(f, "fo:border=\"%s\" ", style->border);
	if (style->border_top != 0)
		fprintf(f, "fo:border-top=\"%s\" ", style->border_top);
	if (style->border_bottom != 0)
		fprintf(f, "fo:border-bottom=\"%s\" ", style->border_bottom);
	if (style->border_left != 0)
		fprintf(f, "fo:border-left=\"%s\" ", style->border_left);
	if (style->border_right != 0)
		fprintf(f, "fo:border-right=\"%s\" ", style->border_right);
	fprintf(f, "/>\n");
	fprintf(f, "</style:style>\n");
}

/**
 * Prints a table cell to a file.
 * \remarks Currently only float, string and formula are supported.
 * \param cell Pointer to the cell to print
 * \param f The spream to print to
 */
static void
print_table_cell(struct spreadconv_cell *cell, FILE *f)
{
	/* avoid printing rubbish if text is NULL */
	if (cell->text == 0)
		cell->text = strdup("");

	fprintf(f, "<table:table-cell ");
	if (cell->style != 0) 
		fprintf(f, "table:style-name=\"%s\" ", cell->style->name);

	if ((cell->value_type == NULL) || 
			(strncmp(cell->value_type, "string", strlen("string")) == 0)) {
		fprintf(f, ">\n");
		fprintf(f, "<text:p>%s</text:p>\n", cell->text?cell->text:"");
		fprintf(f, "</table:table-cell>\n");
		return;
	}

	if (strncmp(cell->value_type, "formula", strlen("formula")) == 0) {
		fprintf(f, "table:formula=\"%s\" />\n", cell->text);
		return;
	}

	if (strncmp(cell->value_type, "float", strlen("float")) == 0) {
		fprintf(f, "office:value-type=\"float\" ");
		fprintf(f, "office:value=\"%s\">\n", cell->text);
		fprintf(f, "<text:p>%s</text:p>\n", cell->text);
		fprintf(f, "</table:table-cell>\n");
		return;
	}

	/* If we got here, then the value_type is not supported */
	fprintf(stderr, "Cell value type not supported: %s\n", cell->value_type);
	assert(0);
}

/**
 * Creates the content file. This is where most of the text (and the
 * code) actually resides. It is also the only file creation routine so
 * far that actually uses the data it is supplied with.
 * \param data A \a spreadconv_data structure from which to draw
 * information
 * \return 0 on success, -1 on error
 */
static int
create_content_file(struct spreadconv_data *data)
{
	FILE *f;
	int i, j;

	f = fopen("content.xml", "w");
	if (f == 0)
		return -1;

	fprintf(f, "<?xml version=\"1.0\"?>\n");
	fprintf(f, "<office:document-content ");
	print_namespaces(f);
	fprintf(f, ">\n");

	/* print style information */
	fprintf(f, "<office:automatic-styles>\n");
	for (i=0; i<data->n_unique_rc_styles; i++)
		print_rc_style(&data->unique_rc_styles[i], f);
	for (i=0; i<data->n_unique_cell_styles; i++)
		print_cell_style(&data->unique_cell_styles[i], f);
	fprintf(f, "</office:automatic-styles>\n");

	/* print the actual spreadsheet */
	fprintf(f, "<office:body>\n");
	fprintf(f, "<office:spreadsheet>\n");
	
	/* avoid printing garbage if data->name is NULL */
	if (data->name == 0)
		data->name = strdup("Sheet 1");
	
	fprintf(f, "<table:table table:name=\"%s\">\n", data->name);
	
	/* print the columns */
	for (i=0; i<data->n_cols; i++) {
		fprintf(f, "<table:table-column ");
		if (data->col_styles[i] != 0)
			fprintf(f, "table:style-name=\"%s\" ", 
					data->col_styles[i]->name);
		fprintf(f, "/>\n");
	}

	/* print the rows, each with the associated cells */
	for (i=0; i<data->n_rows; i++) {
		fprintf(f, "<table:table-row ");
		if (data->row_styles[i] != 0)
			fprintf(f, "table:style-name=\"%s\" ",
					data->row_styles[i]->name);
		fprintf(f, ">\n");
		
		for (j=0; j<data->n_cols; j++)
			print_table_cell(&data->cells[i][j], f);
		fprintf(f, "</table:table-row>\n");
	}

	fprintf(f, "</table:table>\n");
	fprintf(f, "</office:spreadsheet>\n");
	fprintf(f, "</office:body>\n");
	fprintf(f, "</office:document-content>\n");
	fclose(f);
	return 0;
}



/**
 * Converts a generic \a spreadconv_data structure into the associated
 * file(s).
 * \param data The data to covert to a spreadsheet
 * \param file_types A bit field indicating which file types to create
 * \remarks Only ods format is currently supported
 * \returns The created file's name, without extension
 */
char * 
spreadconv_create_spreadsheet(struct spreadconv_data *data, int file_types)
{
	char *dirname;
	char *prev_dir;
	char *buffer, *buffer2;

	/* Only ODS files are currently supported. */
	assert(file_types == LSC_FILE_ODS);

	prev_dir = getcwd(prev_dir, 0);
	if (chdir(spreadconv_dir_name) == -1) {
		free(spreadconv_dir_name);
		spreadconv_dir_name = strdup("/tmp");
		chdir(spreadconv_dir_name);
	}

	dirname = strdup("libspreadconvXXXXXX");
	if (mkdtemp(dirname) == 0)
		return 0;

	chdir(dirname);

	/* create the files for the package*/
	create_manifest_file(data);
	create_mimetype_file();
	create_settings_file(data);
	create_style_file(data);
	create_meta_file(data);
	create_content_file(data);

	/* package the files according to the standard */
	buffer = malloc(1000);
	snprintf(buffer, 1000, "zip -r %s.ods mimetype META-INF "
			"content.xml meta.xml styles.xml settings.xml -n mimetype", dirname);
	system(buffer);

	/* 
	 * Move created file a level up and delete the temporary
	 * directory.
	 */
	buffer2 = malloc(1000);
	snprintf(buffer, 1000, "%s.ods", dirname);
	snprintf(buffer2, 1000, "../%s", buffer);
	rename(buffer, buffer2);

	chdir("..");
	remove(dirname);
	
	chdir(prev_dir);
	return dirname;
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
	struct spreadconv_data *data;
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
 * Adds a new unique row or column style to a spreadsheet.
 * \param style The style to add
 * \param data Pointer to the structure that the style should be added
 * to
 * \return The position in \a unique_rc_array where the style has been
 * added or -1 if there was an error
 * \remarks A return value of 0 is perfectly correct. The style array,
 * like any other C array, is zero-indexed.
 */
int spreadconv_add_unique_rc_style(struct spreadconv_rc_style style, 
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

	data->unique_rc_styles[data->n_unique_rc_styles++] = style;
	return data->n_unique_rc_styles-1;
}

/**
 * Adds a new unique cell style to a spreadsheet.
 * \param style The style to add
 * \param data Pointer to the structure that the style should be added
 * to
 * \return The position in \a unique_cell_array where the style has been
 * added or -1 if there was an error
 * \remarks A return value of 0 is perfectly correct. The style array,
 * like any other C array, is zero-indexed.
 */
int spreadconv_add_unique_cell_style(struct spreadconv_cell_style style, 
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

	data->unique_cell_styles[data->n_unique_cell_styles++] = style;
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
