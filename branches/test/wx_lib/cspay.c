#include <wchar.h>
#include "cspay.h"

#ifdef __cplusplus
#	error "Aici este cod C"
#endif

static char mystr[] = "Ana sparge cana";

float
media(int a, int b, int c)
{
	return (float)(a + b + c) / 3;
}

char *
mesaj(void)
{
	return mystr;
	/*
	 * RD: don't do this; string will be stored in a read-only are
	 * behavior will be undefined when trying to modify it when
	 * outside the function
	 */
	/*return "Ana sparge cana";*/
}
