#ifndef _CSPAY_H_
#define _CSPAY_H_

struct cspay_config {
	/* cand avem wiki */
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
