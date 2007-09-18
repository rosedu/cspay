#ifndef __UTILS_H__
#define __UTILS_H__
/*
 * utils for libspreadconv
 */
char *spreadconv_convert_column_number(int);
/*
 * convert string "0.1cm solid #0000" to an id
 * for pyExcelerator
 */
int bord_str2id(char *str);
#endif
