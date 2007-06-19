#include <stdio.h>
#include "main.h"
#include "util.h"

int exported(int x)
{
	printf("The exported function: %d\n", x);
	printf("%d\n", not_exported(x));
	return x;
}
