#include <stdio.h>
#include <stdlib.h>

#include "spreadconv.h"

int
main(void)
{
	int x;

	do {
		/*
		 * RD: scanf is EVIL - that x variable is a horrible
		 * workaround
		 *
		 * try this:
		 *   http://home.att.net/~jackklein/c/code/strtol.html 
		 * or learn from the experts:
		 * https://www.securecoding.cert.org/confluence/display/seccode/INT05-A.+Do+not+use+functions+that+input+character+data+and+convert+the+data+if+these+functions+cannot+handle+all+possible+inputs
		 */

		printf("x = "); fflush(stdout);
		scanf("%d", &x);
		if (x)
			printf("%s\n\n", 
					spreadconv_convert_column_number(x));
	} while (x);

	return 0;
}
