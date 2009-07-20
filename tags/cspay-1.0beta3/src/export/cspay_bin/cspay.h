/**
 * \ingroup libcspay
 * \file cspay.h
 * licbspay header file
 * \author Cojocar Lucian
 * \defgroup libcspay Library for the main aplication
 */
#ifndef _CSPAY_H_
#define _CSPAY_H_


#ifdef __cplusplus
extern "C" {
#endif
#if 0
/**
 * \struct faculty
 * informations about a faculty
 */
struct faculty {
	/** long faculty name*/
	char *name;
	/** short faculty name*/
	char *short_name;

	/** dean name*/
	char *dean;

	/** number of departments*/
	int dept_no;
	/** departments of faculty*/
	struct department *depts[MAXD];
};


/**
 * \struct department
 * defines a faculty deprtment
 */
struct department {
	/** name of department*/
	char *name;

	/** chief's name*/
	char *chief;
};
#endif 

/**
 * \struct cspay_file_list
 * a structure containing several file names
 */
struct cspay_file_list {
	/** number of file names*/
	int nr;
	/** file names*/
	char **names;
};

/**
 * Read informations from xml file.
 */
struct cspay_config *cspay_get_config(char *);

/**
 * Convert a \a cspay_config and a MySQL db to a ods|xls file
 */
struct cspay_file_list *cspay_convert_options();

/**
 * free \a cspay_file_list structure
 */
void cspay_free_file_list(struct cspay_file_list *);

/**
 * free \a cspay_config structure
 */
struct time_config;
void cspay_free_config(struct time_config *);

struct tm;
extern struct tm *curr_month;

#ifdef __cplusplus
}
#endif
#endif /* _CSPAY_H_ */
