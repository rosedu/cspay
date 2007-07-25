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
	
void
cspay_free_file_list(struct cspay_file_list *list)
{
	int i;

	/* RD: someone should beware of code readability - use blank */
	for (i = 0; i <= list->nr; i++)
	       free(list->names[i]);
	free(list->names);
	free(list);
}		       

struct cspay_config *
cspay_get_config(void)
{
	/*TODO poate ceva verificari aici?*/
	return read_cspay_xml("cspay.xml");
}

struct cspay_file_list *
cspay_convert_options(char *fname)
{
	/* TODO eliberari de memorie, in caz de eroare */
	/* initilizarea pointerilor, cu goto: */
	/* FIXME anumite caractere trebuie escapate (&)*/
	#ifdef __DEBUG__
	printf("Am inceptu sa convertesc fisierul\n");
	#endif
	
	struct cspay_file_list *ret;
	struct cspay_config *cfg;
	struct spreadconv_data *doc;
	dictionary *ini;
	char *name;
	char *val;
	char get_val[25];
	int i;
	int len;
	int day;
	int crt;
	int role;
	struct tm *tmp_date;
	time_t tmp_start;
	time_t tmp_end;
	time_t index;
		
	cfg = cspay_get_config();
	ini = iniparser_load(fname);
	if (!ini){
		fprintf(stderr, "Fsier ini negasit, incercam altul\n");
		ini = iniparser_load("personal.ini");
		if (!ini){
			fprintf(stderr, "personal.ini nu a fost gasit\n");
			return NULL;
		}
	}
	
	doc = spreadconv_new_spreadconv_data("Date", 20, 9);
	
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
	doc->cells[1][6].text = malloc(strlen("Luna ") + 12);
	strcpy(doc->cells[1][6].text, "Luna ");
	strftime(doc->cells[1][6].text + strlen("Luna "), 12, "%B",tmp_date);
	
	#ifdef __DEBUG__
	printf("Am scris luna\n");
	#endif
	/* Salvam in tmp_date data de start a perioadei*/ 
	tmp_date->tm_year = localtime(&cfg->sem_start)->tm_year;
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
	while (i <= 2){
		#ifdef __DEBUG__
		printf("Tratez regula %d\n", i);
		#endif
		
		strcpy(get_val, "ore/");
		sprintf(get_val + strlen("ore/"), "%d", i);
		len = strlen(get_val);	/* FIXME double strlen*/
		strcat(get_val + len, ":");
		++ len;
		strcpy(get_val + len, "zi");
		day = iniparser_getint(ini, get_val, -1);
		
		/* incep sa caut prima zi (day din saptamana) valida*/
		tmp_date = localtime(&tmp_start);
		tmp_date->tm_mday = 1;
		index = mktime(tmp_date);
		while (index < tmp_end){
			if (localtime(&index)->tm_wday == day)
				if (is_work(cfg, index))
					break;
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
		static const char roles[3][14] = {
				{"asist"},
				{"conf"},
				{"sef l"}
		};
		while (index < tmp_end) {
			if (is_work(cfg, index)) {
				/* nr crt*/
				doc->cells[6 + crt][0].text = malloc(4);
				sprintf(doc->cells[6 + crt][0].text, "%d", crt + 1);
				
				/* felul si nr post */
				/* SCuze pentru ce urmeaza*/
				doc->cells[6 + crt][1].text = malloc(10);
				strcpy(get_val + len, "rol");
				printf("====>%s\n", get_val);
				role = iniparser_getint(ini,  get_val, -1);
				if (role < 0) {
					fprintf(stderr, "Nu am gasit rol\n");
					return NULL;
				}
				sprintf(doc->cells[6 + crt][1].text, "%s%s",
				roles[role],
				iniparser_getstr(ini, strcpy(get_val + len, "numar_post") - len));
				
				/* facultatea */
				strcpy(get_val + len, "facultate");
				val = iniparser_getstr(ini, get_val);
				printf("====>%s\n", get_val);
				if (!val) {
					fprintf(stderr, "Nu am gasit facultatea");
					return NULL;
				}
				doc->cells[6 + crt][2].text = strdup(val);
				
				/* disciplina */
				strcpy(get_val + len, "disciplina");
				val = iniparser_getstr(ini, get_val);
				if (!val) {
					fprintf(stderr, "Nu am gasit disciplina");
					return NULL;
				}
				doc->cells[6 + crt][3].text = strdup(val);
				
				/* an  grupa*/
				strcpy(get_val + len, "grupa");
				val = iniparser_getstr(ini, get_val);
				if (!val) {
					fprintf(stderr, "Nu am gasit grupa anul");
					return NULL;
				}
				doc->cells[6 + crt][6].text = strdup(val);
				
				/* in sfarsit data */
				doc->cells[6 + crt][7].text = malloc(20);
				strftime(doc->cells[6 + crt][7].text, 19, "%d-%b", localtime(&index));
				
				++ crt;
			}
			index += WEEK;
		}
		#ifdef __DEBUG__
		printf("Am terminat regula %d\n", i);
		#endif
		
		++ i;
	}
	
	
	
	spreadconv_dir_name = strdup("./out/");
	
	name = spreadconv_create_spreadsheet(doc, LSC_FILE_ODS);
	
	if (!name) {
		fprintf(stderr, "ERoare la creaarea fisierului ods\n");
		return NULL;
	} else {
		#ifdef __DEBUG__
		printf("Am creat %s\n\n", name);
		#endif
	}
	free(name);
	spreadconv_free_spreadconv_data(doc);
	free(spreadconv_dir_name);
	return ret;
}

int
is_work(struct cspay_config *cfg, time_t t)
{
	int i;
	for (i = 0; i < cfg->rest_no; ++ i){
		if ((cfg->start[i] <= t) &&
			(t < cfg->end[i]))
			return 0;
			
	}
	return 1;
}

int 
main(void)
{
	cspay_convert_options(NULL);
	/* read_cspay_xml(NULL); */
	return 0;
}
