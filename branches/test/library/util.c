#include <stdio.h>
#include "util.h"

int not_exported(int x)
{
	printf("The not exported function: %d\n", x);
	return x;
}
