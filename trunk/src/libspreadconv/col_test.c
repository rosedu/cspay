#include <stdio.h>
#include <stdlib.h>

#include "spreadconv.h"

int
main(void)
{
	int x;

	do {
		printf("x = "); fflush(stdout);
		scanf("%d", &x);
		if (x)
			printf("%s\n\n", 
					spreadconv_convert_column_number(x));
	} while (x);

	return 0;
}
