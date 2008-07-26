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

#define MAXR	20	/**< maximum number of restrictions*/
#define MAXF	13	/**< maximum number of faculties*/
#define MAXD	10	/**< maximum number of departments*/

/**
 * \struct cspay_config
 * all informations from xml file
 */
struct time_config {
	/** semester interval*/
	struct interval *sem;

	/** number of vacations*/
	int vac_no;		
	/** vacation intervals*/
	struct interval *vac[MAXR];
};

/**
 * \struct interval
 * defines a time interval
 */
struct interval {
	/** start*/
	time_t start;
	/** end*/
	time_t end;
};

/**
 * \struct class_info
 * contains all the information about a specific class
 */
struct class_info {
	/** name length, see bellow*/
	int name_len;
	/** name of course, usefull in reading information from ini file
	 * eg: "ore/1", "ore/2"
	 */
	char name[8];

	/** day in week of class; Monday = 1*/
	int day;

	/** class parity
	 * 1 - every week
	 * 2 - every two weeks
	 * etc.
	 */
	int parity;
	/** first week of class
	 * 1 - the class begin in first *valid* week of the month
	 * 2 - the class begin in second *valid* week of the month
	 * etc.
	 * \todo check if this is implemented corectly
	 */
	int first_week;

	/** role type
	 * 0 - assistant
	 * 1 - "conferentiar"
	 * 2 - "sef lucrari"
	 * 3 - teacher
	 */
	int role_type;

	/** role number/name*/
	char *role_num;

	/** name of class, a.k.a. disciplina*/
	char *class;

	/** short name of faculty*/
	char *faculty;
	
	/** group name of class
	 * eg: 312CC*/
	char *group;

	/** class type
	 * c - course
	 * l - application
	 */
	char class_type;

	/**
	 * time of class
	 * \remarks interval is used as int
	 */
	struct interval timeline;
	
};


/** read vacation and semester interval */
struct time_config *read_time_config(void);

/** read months from ini file */
int load_months(struct tm **);

/** read information of a class */
struct class_info *read_class_info(MYSQL_ROW);

#endif /* __LOAD_CFG_H__ */
