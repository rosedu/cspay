/**
 * \ingroup libcspay
 * \file cspay.h
 * licbspay header file
 * \author Cojocar Lucian
 * \defgroup libcspay Library for the main aplication
 */
#ifndef _CSPAY_H_
#define _CSPAY_H_

#define MAXR	20	/**< maximum number of restrictions*/
#define MAXF	13	/**< maximum number of faculties*/
#define MAXD	10	/**< maximum number of departments*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \struct cspay_config
 * all informations from xml file
 */
struct cspay_config {
	/** university name*/
	char *univ_name;

	/** semester interval*/
	struct interval *sem;

	/** number of vacations*/
	int vac_no;		
	/** vacation intervals*/
	struct interval *vac[MAXR];

	/** number of faculties*/
	int fac_no;
	/** faculties*/
	struct faculty *fac[MAXF];
};

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
 * \struct department
 * defines a faculty deprtment
 */
struct department {
	/** name of department*/
	char *name;

	/** chief's name*/
	char *chief;
};

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
struct cspay_file_list *cspay_convert_options(struct cspay_config *, int argc, char **argv);

/**
 * free \a cspay_file_list structure
 */
void cspay_free_file_list(struct cspay_file_list *);

/**
 * free \a cspay_config structure
 */
void cspay_free_config(struct cspay_config *);

#ifdef __cplusplus
}
#endif
#endif /* _CSPAY_H_ */
