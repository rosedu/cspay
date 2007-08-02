#include <stdlib.h>	/* system headers go before user headers */
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "getopt.h"
#include "cspay.h"

#include "../libspreadconv/spreadconv.h"
#include "./iniparser/src/iniparser.h"

#define HOUR	3600
#define DAY		(24 * HOUR)
#define WEEK	(7 * DAY)

/* verifica daca t este zi (perioada) de lucru*/
/* daca este functia intoarce 1, 0 altfel*/
int is_work(struct cspay_config *cfg, time_t t);

/* intoarce numele unui singur fisier creat */
char *cspay_convert_single_file(char *fname);

void
cspay_free_file_list(struct cspay_file_list *list)
{
	int i;

	/* RD: someone should beware of code readability - use blank */
	for (i = 0; i < list->nr; i++)
	       free(list->names[i]);
	free(list->names);
	free(list);
	
#ifdef __DEBUG__
	printf("Am eliberat memoria pentru cspay_file_list\n");
#endif
}		       

struct cspay_config *
cspay_get_config(void)
{
	/*TODO poate ceva verificari aici?*/
	return read_cspay_xml("cspay.xml");
}

static struct cspay_config *cfg = NULL;
static struct spreadconv_data *doc = NULL;

char *
cspay_convert_single_file(char *fname)
{
	/* TODO eliberari de memorie, in caz de eroare */
	/* TODO cred ca mergeau facute doua functii */
	/* initilizarea pointerilor, cu goto: */
	/* FIXME anumite caractere trebuie escapate (&)*/
	/* sau in libspreadconv trebuie? */
#ifdef __DEBUG__
	printf("Am inceput sa convertesc fisierul\n");
#endif
	
	dictionary *ini;
	char *name = NULL;
	char *val = NULL;
	char get_val[25];
	int i;
	int len;
	int day;
	int crt;
	int role;
	int h_start;
	int h_end;
	struct tm * tmp_date = NULL;
	time_t tmp_start;
	time_t tmp_end;
	time_t index;

	/* load configuration from XML file (cspay.xml) */
	cfg = cspay_get_config();

	/* init ini file parsing */
	ini = iniparser_load(fname);
	if (!ini) {
		fprintf(stderr, "Fisier .ini negasit, incercam altul\n");
		ini = iniparser_load("personal.ini");
		if (!ini){
			fprintf(stderr, "personal.ini nu a fost gasit\n");
			return NULL;
		}
	}

	/* create new spreadsheet */
	doc = spreadconv_new_spreadconv_data("Date", 60, 9);

	/* configure columns */
#ifdef __DEBUG__
	printf("Am inceput sa creez stiluri pentru coloane\n");
#endif
	struct spreadconv_rc_style *wide, *narrow;
	int co_wide, co_narrow;

	wide = calloc(1, sizeof (struct spreadconv_rc_style));
	wide->name = strdup("wide_col");
	wide->size = strdup("5cm");
	co_wide = spreadconv_add_unique_rc_style(wide, doc);

	narrow = malloc(sizeof (struct spreadconv_rc_style));
	narrow->name = strdup("narrow_col");
	narrow->size = strdup("0.6cm");
	co_narrow = spreadconv_add_unique_rc_style(narrow, doc);

	spreadconv_set_col_style(0, co_narrow, doc);
	spreadconv_set_col_style(3, co_wide, doc);

	/* stiluri pentru casute*/
#ifdef __DEBUG__
	printf("Am inceput sa creez stiluri pentru casute\n");
#endif

	struct spreadconv_cell_style *table;
	int ce_table;

	/* calloc vs. malloc, calloc wins! */
	table = calloc(1, sizeof (struct spreadconv_cell_style));
	table->name = strdup("table_celffl");
	table->border = strdup("1pt solid #000000");
	ce_table = spreadconv_add_unique_cell_style(table, doc);
	if (ce_table < 0){
		fprintf(stderr, "Eroare la crearea stilui celulelor\n");
		return NULL;
	}
	
	/* FIXME assert iniparser_getstr */
	/* universitatea si faculateatea  */
#ifdef __DEBUG__
	printf("Caut universitatea si facultatea\n");
#endif
	doc->cells[0][0].text = strdup(iniparser_getstr(ini, "antet:universitate"));
	doc->cells[1][0].text = strdup(iniparser_getstr(ini, "antet:facultate"));
	
	/* FIXME str* hazards */
	/* catedra */
#ifdef __DEBUG__
	printf("Caut catedra\n");
#endif
	val = strdup(iniparser_getstr(ini, "antet:catedra"));
	doc->cells[0][6].text = malloc(strlen(val) + strlen("Catedra ") + 1);
	strcpy(doc->cells[0][6].text, "Catedra ");
	strcat(doc->cells[0][6].text, val);
	
	/* luna */
#ifdef __DEBUG__
	printf("Caut luna\n");
#endif
	
	tmp_date = calloc(1, sizeof (struct tm));
	tmp_date->tm_mon = iniparser_getint(ini, "antet:luna", -1);
	if (tmp_date->tm_mon < 0) {
		fprintf(stderr, "Eroare la obtinerea lunii\n");
	}
	doc->cells[1][6].text = malloc(strlen("Luna") + 1);
	strcpy(doc->cells[1][6].text, "Luna");
	doc->cells[1][7].text = malloc(12);
	strftime(doc->cells[1][7].text, 12, "%B",tmp_date);
	
#ifdef __DEBUG__
	printf("Am scris luna\n");
#endif
	/* Salvam in tmp_date data de start a perioadei*/ 
	tmp_date->tm_year = localtime(&cfg->sem->start)->tm_year;
	tmp_date->tm_mday = 1;
	
#ifdef __DEBUG__
	printf("Am aflat anul din semestru\n");
#endif 
	tmp_start = mktime(tmp_date);
	
	++ tmp_date->tm_mon;
	if (tmp_date->tm_mon == 12){
		tmp_date->tm_mon = 0;
		++ tmp_date->tm_year;
	}
	tmp_end = mktime(tmp_date) - 1;
	
	
	/* procesarea optiunilor */
	/* FIXME str* hazards */
	i = 1;
	crt = 0;
	while (1){
#ifdef __DEBUG__
		printf("Tratez regula %d\n", i);
#endif
		
		/*caut ziua*/
		strcpy(get_val, "ore/");
		sprintf(get_val + strlen("ore/"), "%d", i);
		len = strlen(get_val);	/* FIXME double strlen*/
		strcat(get_val + len, ":");
		++ len;
		strcpy(get_val + len, "zi");
		day = iniparser_getint(ini, get_val, -1);
		
		if (day < 0){
			/* PE AICI IESE*/
			break;
		}
		
		/* PARITATEA */
		int par;
		strcpy(get_val + len, "paritate");
		par = iniparser_getint(ini, get_val, -1);
		if (par < 0){
			fprintf(stderr, "Eroare la paritate\n");
			return NULL;
		}
		
		/* PRIMA saptamana*/
		int fw;
		strcpy(get_val + len, "paritate_start");
		fw = iniparser_getint(ini, get_val, -1);
		if (fw < 0){
			fprintf(stderr, "Eroare la prima saptamana\n");
			return NULL;
		}
		/* incep sa caut prima zi (day din saptamana) valida*/
		tmp_date = localtime(&tmp_start);
		tmp_date->tm_mday = 1;
		index = mktime(tmp_date);
		while (index < tmp_end){
			if (localtime(&index)->tm_wday == day)
				if (is_work(cfg, index)) {
					-- fw;
					if (!fw)
						break;
				}
			index += DAY;
		}
		if (index >= tmp_end){
			fprintf(stderr, "Nu am gasit o prima zi ?\n");
			return NULL;
		}
#ifdef __DEBUG__
		printf("Am gasit prima zi in care se va lucra %d\n",
					localtime(&index)->tm_mday);
#endif
		
		/* Acum ca am gasit prima zi in care se va lucra*/
		/* Pot incepe sa iterez saptamanal */
		static const char roles[4][14] = {
				{"as"},
				{"conf"},
				{"sl"},
				{"prof"}
		};
		
		/* ROLUL */
		strcpy(get_val + len, "rol");
		role = iniparser_getint(ini,  get_val, -1);
		if (role < 0) {
			fprintf(stderr, "Nu am gasit rol\n");
			return NULL;
		}
		
		/* NR POST */
		char *post_no;
		post_no = iniparser_getstr(ini, strcpy(get_val + len, "numar_post") - len);
	
		/* FACUTATEEA*/
		char *faculty_name;
		strcpy(get_val + len, "facultate");
		faculty_name = iniparser_getstr(ini, get_val);
		if (!faculty_name) {
			fprintf(stderr, "Nu am gasit facultatea");
			return NULL;
		}
		
		/* DISCIPLINA */
		char *course;
		strcpy(get_val + len, "disciplina");
		course = iniparser_getstr(ini, get_val);
		if (!course) {
			fprintf(stderr, "Nu am gasit disciplina");
			return NULL;
		}
		
		/* GRUPA */
		char *grp;
		strcpy(get_val + len, "grupa");
		grp = iniparser_getstr(ini, get_val);
		if (!grp) {
			fprintf(stderr, "Nu am gasit grupa anul");
			return NULL;
		}
		
		/* ORELE */
		char *hours;
		strcpy(get_val + len, "ore");
		hours = iniparser_getstr(ini, get_val);
		if (!hours) {
			fprintf(stderr, "Nu am gasit grupa anul");
			return NULL;
		}
		sscanf(hours, "%d-%d", &h_start, &h_end);
		
		/* CURS || APLICATIE */
		int ca;
		strcpy(get_val + len, "tip_post");
		ca = iniparser_getboolean(ini, get_val, 0);		

		while (index < tmp_end) {
			if (is_work(cfg, index)) {
				/* nr crt*/
				doc->cells[6 + crt][0].text = malloc(4);
				sprintf(doc->cells[6 + crt][0].text, "%d", crt + 1);
				spreadconv_set_cell_style(6 + crt, 0, ce_table, doc);
				
				/* felul si nr post */
				doc->cells[6 + crt][1].text = malloc(10);
				sprintf(doc->cells[6 + crt][1].text, "%s%s",
				roles[role],post_no);
				spreadconv_set_cell_style(6 + crt, 1, ce_table, doc);
				
				/* facultatea */
				doc->cells[6 + crt][2].text = strdup(faculty_name);
				spreadconv_set_cell_style(6 + crt, 2, ce_table, doc);
				
				/* disciplina */
				doc->cells[6 + crt][3].text = strdup(course);
				spreadconv_set_cell_style(6 + crt, 3, ce_table, doc);
				
				/* an  grupa*/
				doc->cells[6 + crt][6].text = strdup(grp);
				spreadconv_set_cell_style(6 + crt, 6, ce_table, doc);
				
				/* in sfarsit data */
				doc->cells[6 + crt][7].text = malloc(20);
				strftime(doc->cells[6 + crt][7].text, 19, "%d-%b", localtime(&index));
				spreadconv_set_cell_style(6 + crt, 7, ce_table, doc);
				
				/* orele Atentie nu se verifica nimic */
				doc->cells[6 + crt][8].text = strdup(hours);
				spreadconv_set_cell_style(6 + crt, 8, ce_table, doc);
				
				/* Durata unei aplicatii || curs*/
				
				if (ca){/* e aplicatie */
					doc->cells[6 + crt][5].value_type = strdup("float");
					doc->cells[6 + crt][5].text = malloc(4);
					sprintf(doc->cells[6 + crt][5].text, "%d", h_end - h_start);
				} else { /* e curs */
					doc->cells[6 + crt][4].value_type = strdup("float");
					doc->cells[6 + crt][4].text = malloc(4);
					sprintf(doc->cells[6 + crt][4].text, "%d", h_end - h_start);
				}
				spreadconv_set_cell_style(6 + crt, 4, ce_table, doc);
				spreadconv_set_cell_style(6 + crt, 5, ce_table, doc);
				++ crt;
			}
			index += par * WEEK;
		}
#ifdef __DEBUG__
		printf("Am terminat regula %d\n", i);
#endif
		
		++ i;
	}
	
	spreadconv_dir_name = strdup("./out/");
	
#ifdef __DEBUG__
	printf("Am inceput sa creez fisierul output\n");
#endif
	
	name = spreadconv_create_spreadsheet(doc, LSC_FILE_ODS);
	
	if (!name) {
		fprintf(stderr, "ERoare la creaarea fisierului ods\n");
		return NULL;
	} else {
#ifdef __DEBUG__
		printf("Am creat %s\n\n", name);
#endif
	}
	
	spreadconv_free_spreadconv_data(doc);
	iniparser_freedict(ini);
	free(spreadconv_dir_name);
	cspay_free_config(cfg);
	
	return name;
}

int
is_work(struct cspay_config *cfg, time_t t)
{
	int i;
	for (i = 0; i < cfg->vac_no; ++ i){
		if ((cfg->vac[i]->start <= t) &&
			(t < cfg->vac[i]->end))
			return 0;
			
	}
	return 1;
}

struct cspay_file_list *
cspay_convert_options(char *fname)
{
	/* tipul functiei cred ca e gresit */
	/*
	 * In ce caz se returneaza mai multe fisiere? 
	 * RD: In cazul in care se realizeaza plata cu ora pe mai multe luni.
	 * Se extrage un fisier .ods pentru fiecare luna.
	 */

	struct cspay_file_list *ret;
	char *temp;

	ret = malloc(sizeof (struct cspay_file_list));
	ret->nr = 1;
	ret->names = malloc(sizeof (char *));
	temp = cspay_convert_single_file(fname);
	ret->names[0] = strdup(temp);
	free(temp);

	return ret;
}

void 
cspay_free_config(struct cspay_config *cfg)
{
	int i, j;
	free(cfg->sem);
	for (i = 0; i < cfg->vac_no; ++ i)
		free(cfg->vac[i]);
	for (i = 0; i < cfg->fac_no; ++ i) {
		for (j = 0; j < cfg->fac[i]->dept_no; ++ j) {
			free(cfg->fac[i]->depts[j]->name);
			free(cfg->fac[i]->depts[j]->chief);
		}
		free(cfg->fac[i]->name);
		free(cfg->fac[i]->short_name);
		free(cfg->fac[i]->dean);
	}
	free(cfg->univ_name);				
	free(cfg);
	
#ifdef __DEBUG__
	printf("Am eliberat memoria pentru cspay_config\n");
#endif
}

int 
main(void)
{
	struct cspay_file_list *res;

	res = cspay_convert_options("personal.ini");
	if (res)
		printf("A fost creat fisierul: %s.ods\n", res->names[0]);
	cspay_free_file_list(res);

	return 0;
}
