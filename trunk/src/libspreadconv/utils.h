#ifndef __UTILS_H__
#define __UTILS_H__
/**
 * Utils for libspradcovn
 * \ingroup libspreadconv
 * \file utils.h
 * \author Vlad Dogaru
 * \author Cojocar Lucian
 */
char *spreadconv_convert_column_number(int);
int bord_str2i(char *);
int col_str2i(char *);
int row_str2i(char *);
void py_print_esc(FILE *, char *);

#endif
