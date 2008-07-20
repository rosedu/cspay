/*
 * ods file implementation
 * First draft: 15.06.2007, Vlad Dogaru
 */

/**
 * \ingroup libspreadconv
 * \file ods.c
 * \brief ods file implementation
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
#include "ods.h"
#include "utils.h"
#include "debug.h"

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



/**
 * Inline function to print an escaped character sequence to a stream.
 * \param f The file stream
 * \param s The string
 * \remarks Hopefully I have understood the use of \c inline.
 */
static inline void print_escaped(FILE *f, char *s)
{
	while (*s) {
		/* The commented out cases appear at my reference
		 * point[1], but don't work with the test file.
		 * http://www.codecodex.com/wiki/index.php?title=Escape_sequences_and_escape_characters#XML
		 * What's more, backslashes and newlines seem to be
		 * correctly handled without being escaped. Leaving this
		 * here only serves hypothetical further inquiry.
		 */
		switch (*s) {
			case '\'': fprintf(f, "&apos;"); break;
			case '\"': fprintf(f, "&quot;"); break;
			/* case '\\': fprintf(f, "\\\\"); break; */
			case '\n': fprintf(f, "&#13;"); break; 
			case '&':  fprintf(f, "&amp;"); break;
			case '<':  fprintf(f, "&lt;"); break;
			case '>':  fprintf(f, "&gt;"); break;
			default: fprintf(f, "%c", *s);
		}
		s++;
	}
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

	for (i = 0; i < xml_namespace_count; i++)
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
	/*
	 * RD: beware of portability issue; mkdir is only available on
	 * POSIX-compliant systems/
	 */
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
	fprintf(f, "<style:style style:name=\"");
	print_escaped(f, style->name);
	fprintf(f, "\" ");
	if (style->type == LSC_STYLE_ROW) {
		fprintf(f, "style:family=\"table-row\">\n");
		fprintf(f, "<style:table-row-properties style:row-height=\"");
	} else {
		/* assuming LSC_STYLE_COLUMN */
		fprintf(f, "style:family=\"table-column\">\n");
		fprintf(f, "<style:table-column-properties style:column-width=\"");
	}
	print_escaped(f, style->size);
	fprintf(f, "\" />\n");
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
	/*
	 * TODO
	 * if only row_span || col_span are != NULL then 
	 * we have nothing to print
	 */
	fprintf(f, "<style:style style:name=\"");
	print_escaped(f, style->name);
	fprintf(f, "\" ");
	fprintf(f, "style:family=\"table-cell\">\n");
	fprintf(f, "<style:table-cell-properties ");
	if (style->valign != 0) {
		fprintf(f, "style:vertical-align=\"");
		print_escaped(f, style->valign);
		fprintf(f, "\" ");
	}
	if (style->border != 0) {
		fprintf(f, "fo:border=\"");
		print_escaped(f, style->border);
		fprintf(f, "\" ");
	}
	if (style->border_top != 0) {
		fprintf(f, "fo:border-top=\"");
		print_escaped(f, style->border_top);
		fprintf(f, "\" ");
	}
	if (style->border_bottom != 0) {
		fprintf(f, "fo:border-bottom=\"");
		print_escaped(f, style->border_bottom);
		fprintf(f, "\" ");
	}
	if (style->border_left != 0) {
		fprintf(f, "fo:border-left=\"");
		print_escaped(f, style->border_left);
		fprintf(f, "\" ");
	}
	if (style->border_right != 0) {
		fprintf(f, "fo:border-right=\"");
		print_escaped(f, style->border_right);
		fprintf(f, "\" ");
	}
	fprintf(f, "/>\n");
	if (style->halign != 0) {
		fprintf(f, "<style:paragraph-properties ");
		fprintf(f, "fo:text-align=\"");
		print_escaped(f, style->halign);
		fprintf(f, "\"");
		fprintf(f, "/>\n");
	}
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
	if (cell->style != 0) {
		fprintf(f, "table:style-name=\"");
		print_escaped(f, cell->style->name);
		fprintf(f, "\" ");
	}

	if ((cell->value_type == NULL) || 
			(strncmp(cell->value_type, "string", strlen("string")) == 0)) {
		if (cell->style) 
			if (cell->style->row_span || cell->style->col_span) {
				if (!cell->style->row_span) {
					cell->style->row_span = strdup("1");
				} else {
					if (!cell->style->col_span) {
						cell->style->col_span = strdup("1");
					}
				}
				fprintf(f, "table:number-columns-spanned=\"%s\" ", cell->style->col_span);
				fprintf(f, "table:number-rows-spanned=\"%s\"", cell->style->row_span);
			}
			
		
		fprintf(f, ">\n");
		fprintf(f, "<text:p>");
		print_escaped(f, cell->text);
		fprintf(f, "</text:p>\n");
		fprintf(f, "</table:table-cell>\n");
		return;
	}

	if (strncmp(cell->value_type, "formula", strlen("formula")) == 0) {
		fprintf(f, "table:formula=\"");
		print_escaped(f, cell->text);
		fprintf(f, "\" />\n");
		return;
	}

	if (strncmp(cell->value_type, "float", strlen("float")) == 0) {
		fprintf(f, "office:value-type=\"float\" ");
		fprintf(f, "office:value=\"%s\">\n", cell->text);
		fprintf(f, "office:value=\"");
		print_escaped(f, cell->text);
		fprintf(f, "\" ");
		/* redundant code*/
		if (cell->style) 
			if (cell->style->row_span || cell->style->col_span) {
				if (!cell->style->row_span) {
					cell->style->row_span = strdup("1");
				} else {
					if (!cell->style->col_span) {
						cell->style->col_span = strdup("1");
					}
				}
				fprintf(f, "table:number-columns-spanned=\"%s\" ", cell->style->col_span);
				fprintf(f, "table:number-rows-spanned=\"%s\"", cell->style->row_span);
			}
			
		fprintf(f, ">\n<text:p>");
		print_escaped(f, cell->text);
		fprintf(f, "</text:p>\n</table:table-cell>\n");
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
		if (data->col_styles[i] != 0) {
			fprintf(f, "table:style-name=\"");
			print_escaped(f, data->col_styles[i]->name);
			fprintf(f, "\" ");
		}
		fprintf(f, "/>\n");
	}

	/* print the rows, each with the associated cells */
	for (i=0; i<data->n_rows; i++) {
		fprintf(f, "<table:table-row ");
		if (data->row_styles[i] != 0) {
			fprintf(f, "table:style-name=\"");
			print_escaped(f, data->row_styles[i]->name);
			fprintf(f, "\" ");
		}
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
 * Converts a generic \a spreadconv_data structure into an ods
 * file.
 * \param data The data to covert to a spreadsheet
 * \returns The created file's name, with extension and full path
 */
char * 
ods_create_spreadsheet(struct spreadconv_data *data)
{
	char *dirname;
	char *file_name;
	char *prev_dir = 0;
	char *buffer, *buffer2;

	prev_dir = getcwd(prev_dir, 0);
	if (chdir(spreadconv_dir_name) == -1) {
		free(spreadconv_dir_name);
		spreadconv_dir_name = strdup("/tmp/");
		chdir(spreadconv_dir_name);
	}

	dirname = strdup("libspreadconvXXXXXX");
	if (mkdtemp(dirname) == 0)
		return 0;

	chdir(dirname);

	/* create the files for the package*/
	if (create_manifest_file(data)) {
		goto ERR;
	}
	if (create_mimetype_file()) {
		goto ERR;
	}
	if (create_settings_file(data)) {
		goto ERR;
	}
	if (create_style_file(data)) {
		goto ERR;
	}
	if (create_meta_file(data)) {
		goto ERR;
	}
	if (create_content_file(data)) {
		goto ERR;
	}

	/* package the files according to the standard */
	buffer = malloc(1000);
	snprintf(buffer, 1000, "zip -q -r %s.ods mimetype META-INF "
			"content.xml meta.xml styles.xml settings.xml -n mimetype", dirname);
	system(buffer);

	/* 
	 * Move created file a level up and delete the temporary
	 * directory.
	 */
	buffer2 = malloc(1000);
	snprintf(buffer, 1000, "%s.ods", dirname);

	file_name = malloc(512);
	snprintf(file_name, 512, "%s%s", spreadconv_dir_name, buffer);

	snprintf(buffer2, 1000, "../%s", buffer);
	rename(buffer, buffer2);

	chdir("..");
	/* 
	 * This doesn't work. Directory remains there. Dammit.
	 */
	/*
	if (remove(dirname) != 0)
		perror("libspreadconv: create_spreadsheet");
	*/
#ifndef __DEBUG__
	/*
	 * portabilitate 0
	 */
	char *rm_command;
	rm_command = malloc(512);
	snprintf(rm_command, 512, "rm -rf %s", dirname);
	system(rm_command);
	free(rm_command);
#endif 
	chdir(prev_dir);
	free(buffer);
	free(buffer2);
	free(prev_dir);
	free(dirname);
	
	return file_name;

	ERR:
	free(prev_dir);
	free(dirname);
	return NULL;
}



