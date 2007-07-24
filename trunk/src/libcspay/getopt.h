/*
	Cojocar Lucian <cojocar .at. gmail .dot. com>
	Citeste informatiile din cspay.xml si din personal.ini
	
	Mon Jul 23 22:38:26 EEST 2007
*/
#ifndef __GETOPT_H__
#define __GETOPT_H__ 1


#include "cspay.h"

/*read data from cspay.xml, or from char * */
struct cspay_config *read_cspay_xml(char *);

#endif /* __GETOPT_H__ */
