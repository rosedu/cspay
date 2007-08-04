#ifndef _CSPAY_H_
#define _CSPAY_H_

#define MAXR	20	/*maximum number of restrictions*/
#define MAXF	13
#define MAXD	10	/*maximum number of departments*/

/* TODO doxygen! */
struct cspay_config {
	char *univ_name;	/*university name*/
	struct interval *sem;	/* semester interval */
	struct interval *vac[MAXR];	/* vacation */
	int vac_no;		/*number of vacations*/
	struct faculty *fac[MAXF];
	int fac_no;			/*number of faculties*/
};

struct faculty {
	char *name;		/*faculty name*/
	char *short_name;	/* short fac name*/
	char *dean;	/* :) */	
	struct department *depts[MAXD];	/*departments of that faculty*/
	int dept_no;	/*number of departments*/
};

struct interval {
	time_t start;
	time_t end;
};

struct department {
	char *name;
	char *chief;
};

struct cspay_file_list {
	int nr;
	char **names;
};

/*
 * functia intoarce informatiile dintr-un fisier xml
 * primit ca parametru
 */
struct cspay_config *cspay_get_config(char *);

/*
 * functia intoarce un set de fisere create, in urma
 * procesarii fisierul ini, primit ca parametru, si a 
 * unei structuri de configurare, (deobicei itnoarsa de functia)
 * de mai sus.
 */
struct cspay_file_list *cspay_convert_options(struct cspay_config *, char *);

/*
 * functii pentru eliberare celor doua structuri
 */
void cspay_free_file_list(struct cspay_file_list *);
void cspay_free_config(struct cspay_config *);

#endif
