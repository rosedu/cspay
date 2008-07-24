/*
	Cojocar Lucian <cojocar .at. gmail .dot. com>
	Citeste informatiile din cspay.xml si din personal.ini
	
	Mon Jul 23 22:38:26 EEST 2007
*/
/**
 * config load header file
 * \ingroup libcspay
 * \file load_cfg.h
 * \author Cojocar Lucian
 */
#ifndef __LOAD_CFG_H__
#define __LOAD_CFG_H__ 

#include <mysql.h>

/** read data from cspay.xml, or from char * */
struct cspay_config *read_cspay_xml(char *);

/** read months from ini file */
int load_months(struct tm **);

/** read information of a class */
struct class_info read_class_info(MYSQL_ROW *);

#endif /* __LOAD_CFG_H__ */
