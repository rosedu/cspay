#include "main.h"

int main(void)
{
	int x = 3;
	exported(x);
	not_exported(x);
	return 0;
}
