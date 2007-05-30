/*
 * libspreadconv header
 * First draft: 16.05.2007, Vlad
 */

#ifndef _SPREADCONV_H_
#define _SPREADCONV_H_

/* 
 * file type constants 
 * Initially, only ODS will be implemented. Others will be included via
 * external converters.
 */
#define LSC_FILE_ODS		1 /* OPF spreadsheet */
#define LSC_FILE_XLSX		2 /* Office Open XML spreadsheet */
#define LSC_FILE_XLS		4 /* Classic MS Excel spreadsheet */

/* generic spreadsheet data container */
/* TODO: add metadata, minimal style data eventually */
struct spreadconv_data {
	/* number of rows and columns */
	int nrows, ncols;
	/* spreadsheet cells */
	char ***cells;
};

/*
 * RD: I would like this better (wit
 */
#if 0
struct spreadconv_cell {
	char *info;
	/* metadata/style/etc. could go here */
};

struct spreadconv_data {
	/* number of rows and columns */
	int nrows, ncols;
	/* spreadsheet cells */
	struct spreadconv_cell **cells;
};
#endif

/* 
 * Converts a generic spreadconv_data structure into the associated
 * file(s). The second parameter is a bit field which determines which
 * formats are to be used for the conversion. The new file's name,
 * without an extension, is returned.
 */
char* spreadconv_create_spreadsheet(struct spreadconv_data *, int);

#endif /* _SPREADCONV_H_ */
