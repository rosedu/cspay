#ifndef _CSPAY_H_
#define _CSPAY_H_

#define MAXR	20	/*maximum number of restrictions*/
#define MAXF	13
#define MAXD	10	/*maximum number of departments*/
/* TODO doxygen! */
struct cspay_config {
	struct interval *sem; /* semester interval */
	struct interval *vac[MAXR];/* vacation */
	int vac_no;		/*number of vacations*/
	struct faculty *fac[MAXF];
	int fac_no;			/*number of faculties*/
	char *univ_name;	/*university name*/
};
struct faculty {
	char *name;		/*faculty name*/
	char *short_name;/* short fac name*/
	char *decan;	/* :) */	
	struct department *depts[MAXD];	/*departments of that faculty*/
	int dept_no;	/*number of departments*/
};
struct interval{
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

struct cspay_config *cspay_get_config(void);
struct cspay_file_list *cspay_convert_options(char *);
void cspay_free_file_list(struct cspay_file_list *);
void cspay_free_config(struct cspay_config *);

#endif
