#ifndef _CSPAY_H_
#define _CSPAY_H_

#define MAXR	20	/*maximum number of restrictions*/
#define MAXF	13
/* TODO doxygen! */
struct cspay_config {
	time_t start[MAXR];	/*restrictions start, see wiki*/
	time_t end[MAXR];	/*and end time*/
	int rest_no;		/*number of restrictions*/
	struct faculty *fac[MAXF];
	int fac_no;			/*number of faculties*/
	char *univ_name;	/*university name*/
};
struct faculty {
	char *name;		/*faculty name*/
	char *short_name;/* short fac name*/
	char *decan;	/* :) */	
	char **dept;	/*departments of that faculty*/
	int dept_no;	/*number of departments*/
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
