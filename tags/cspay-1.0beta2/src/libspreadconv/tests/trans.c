#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include "utils.h"

/*
 * print intro
 */

static void
print_run_info (void)
{
	puts ("CSpay test:\n\tConvert column number in spreadsheet format\n"
			"\t\t(A, B, ..., Z, AA, AB, ...)\n");
}

/*
 * read an integer from standard input
 *
 * return values:
 * 	-1: error processing number
 * 	0: OK
 * 	1: return only pressed (end the nightmare)
 */

static int
read_int (const char *prompt, int *number)
{
	char buff[25];
	char *endp;
	long long_tmp;

	printf ("%s", prompt); fflush (stdout);
	fgets (buff, sizeof (buff), stdin);

	/* enter (return) key pressed */
	if (buff[0] == '\n')
		return 1;

	errno = 0;
	long_tmp = strtol (buff, &endp, 0);
	if (errno == ERANGE) {
		fputs ("number out of range\n", stderr);
		goto out_err;
	}
	if (long_tmp > INT_MAX) {
		fputs ("value too large\n", stderr);
		goto out_err;
	}
	if (long_tmp < INT_MIN) {
		fputs ("value too small\n", stderr);
		goto out_err;
	}
	if (*endp != '\0' && *endp != '\n') {
		fputs ("not a numeric value\n", stderr);
		goto out_err;
	}

	*number = (int) long_tmp;
	return 0;

out_err:
	return -1;
}

int
main(void)
{
	int col_num;
	int ret;

	print_run_info ();

	while (1) {
		ret = read_int ("Enter a column number (RETURN only to quit): ",
				&col_num);
		if (ret == 0) {
			printf("%s\n", spreadconv_convert_column_number (col_num));
			continue;
		}
		if (ret == 1)
			break;
	}

	return 0;
}
