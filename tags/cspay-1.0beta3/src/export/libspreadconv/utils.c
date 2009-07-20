#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "utils.h"

/**
 * \ingroup libspreadconv
 * \file utils.c
 * \brief libspreadconv utility functions file
 * \author Vlad Dogaru
 * \author Cojocar Lucian
 */

/**
 * Converts a number to its representation in `spreadsheet speak' :-P
 * \remarks Since we number columns and rows beginning with 0, this
 * should just be a base 26 conversion of the number, with `digits'
 * being A-Z.
 * TODO: Test this out. Alas, I must see which car passes through the
 * intersection second :-(
 */
char *
spreadconv_convert_column_number(int n)
{
	int len, i;
	char *temp = malloc (100); /* sufficiently big */
	char *result = 0;

	len = 0;
	while (n) {
		temp[len++] = 'A' + (n % 26);
		n /= 26;
	}

	result = malloc(len + 1);
	for (i = 0; i < len; i++)
		result[i] = temp[len-i-1];
	result[len] = 0;

	free(temp);
	return result;
}

/**
 * Convert ("2pt solid #000000") to 2 (int).
 * convert a ods style format to an xls style 
 * format.
 * \remarks only the line size is used
 * \remarks the line size must be in "pt"
 * \todo support other units
 * \param str contains "2pt solid #000000" like string
 * \return atoi("2pt");
 */
int
bord_str2i(char *str)
{
	char colour[10], style[10], size[10];
	int i_size;
	if (!str)
		return 0;
	/*
	 * FIXME str hazards
	 */
	/*
	 * assume that the size is in pt
	 */
	sscanf(str, "%s %s %s", size, style, colour);
	i_size = atoi(size);
	return i_size;
}

/**
 * print escaped string in a python script
 * \param f script file descriptor 
 * \param str string to print (it ends wiht '\\0')
 */
void
py_print_esc(FILE *f, char *str)
{
	while (*str) {
		switch (*str) {
			case '\n':
				fprintf(f, " "); break;
			default:
				fputc(*str, f);
		}
		++ str;
	}
}

/**
 * Convert a column width from ods size 
 * to xls size
 * \remarks size must pe set in "cm"
 * \return converted size
 */
int
col_str2i(char *str)
{
	return atof(str) * (float) 1340;
}

/**
 * Convert a row height from ods size 
 * to xls size
 * \remarks size must pe set in "cm"
 * \return converted size
 */
int
row_str2i(char *str)
{
	return atof(str) * (float) 470;
}
