#include <stdlib.h>	/* system headers go before user headers */
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "debug.h"
#include "getopt.h"
#include "cspay.h"

#include "../libspreadconv/spreadconv.h"
#include "../iniparser-2.17/src/iniparser.h"

#define HOUR	3600
#define DAY		(24 * HOUR)
#define WEEK	(7 * DAY)


/*
 * record containing all the information about a specific class
 */

struct class_info {
	char section_name[8];
	int section_len;
	int class_day;
	int class_parity;
	int class_role_type;
	int class_role_num;
	char *class_course;
	char *class_faculty;
	char *class_timeline;
	char *class_group;
	int class_type;
	int class_h_start;
	int class_h_end;
	int class_first_week;
};

static dictionary *ini;
static struct spreadconv_data *doc;
static struct cspay_config *cfg;
static int ce_table;
static struct tm *month_date;

static struct class_info *read_class_info (size_t index)
{
	struct class_info *ci;
	char class_data[32];
	char *timeline;

	ci = malloc (sizeof (*ci));
	if (ci == NULL) {
		perror ("malloc");
		return NULL;
	}

	/*
	 * class data contains the section name (ore/[number]),
	 * followed by a colon sign and the variable name;
	 * this is how iniparser works
	 */

	strcpy(class_data, "ore/");
	sprintf(class_data + strlen("ore/"), "%d", index);
	strcpy (ci->section_name, class_data);
	ci->section_len = strlen(class_data);	/* FIXME double strlen*/
	strcat(class_data + ci->section_len, ":");
	++ ci->section_len;

	/* get faculty name */
	strcpy(class_data + ci->section_len, "facultate");
	ci->class_faculty = iniparser_getstr(ini, class_data);
	if (!ci->class_faculty) {
		fprintf(stderr, "Nu am gasit facultatea");
		return NULL;
	}

	/* get course name */
	strcpy(class_data + ci->section_len, "disciplina");
	ci->class_course = iniparser_getstr(ini, class_data);
	if (!ci->class_course) {
		fprintf(stderr, "Nu am gasit disciplina");
		return NULL;
	}

	/* get role for that class */
	strcpy(class_data + ci->section_len, "rol");
	ci->class_role_type = iniparser_getint(ini, class_data, -1);
	if (ci->class_role_type < 0) {
		fprintf(stderr, "Nu am gasit rol\n");
		return NULL;
	}

	/* get role number for class */
	strcpy(class_data + ci->section_len, "numar_post");
	ci->class_role_num = iniparser_getint(ini, class_data, -1);
	if (ci->class_role_num < 0) {
		fprintf(stderr, "Nu am gasit rol\n");
		return NULL;
	}

	/* get class type (course/lab) */
	strcpy(class_data + ci->section_len, "tip_post");
	ci->class_type = iniparser_getint(ini, class_data, -1);
	if (ci->class_type < 0) {
		fprintf(stderr, "Nu am gasit tipul post.\n");
		return NULL;
	}

	/* get group for that class */
	strcpy(class_data + ci->section_len, "grupa");
	ci->class_group = iniparser_getstr(ini, class_data);
	if (!ci->class_group) {
		fprintf(stderr, "Nu am gasit grupa anul");
		return NULL;
	}

	/* get class day */
	strcpy(class_data + ci->section_len, "zi");
	ci->class_day = iniparser_getint(ini, class_data, -1);
	if (ci->class_day < 0){
		fprintf(stderr, "Error reading \"zi\" variable.\n");
		return NULL;
	}

	/* get class timeline */
	strcpy(class_data + ci->section_len, "ore");
	timeline = iniparser_getstr(ini, class_data);
	if (!timeline) {
		fprintf(stderr, "Nu am gasit grupa anul");
		return NULL;
	}
	sscanf(timeline, "%d-%d", &ci->class_h_start, &ci->class_h_end);

	/* get class parity */
	strcpy(class_data + ci->section_len, "paritate");
	ci->class_parity = iniparser_getint(ini, class_data, -1);
	if (ci->class_parity < 0){
		fprintf(stderr, "Eroare la paritate\n");
		return NULL;
	}

	/* first week */
	strcpy(class_data + ci->section_len, "paritate_start");
	ci->class_first_week = iniparser_getint(ini, class_data, -1);
	if (ci->class_first_week < 0){
		fprintf(stderr, "Eroare la prima saptamana\n");
		return NULL;
	}

	return ci;
}

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
	
	Dprintf("Am eliberat memoria pentru cspay_file_list\n");
}		       

struct cspay_config *
cspay_get_config(void)
{
	/*TODO poate ceva verificari aici?*/
	return read_cspay_xml("cspay.xml");
}

static int config_styles (void)
{
	/* configure columns */
	struct spreadconv_cell_style *table;

	struct spreadconv_rc_style *wide, *narrow;
	int co_wide, co_narrow;

	Dprintf("Am inceput sa creez stiluri pentru coloane\n");

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
	Dprintf("Am inceput sa creez stiluri pentru casute\n");

	/* calloc vs. malloc, calloc wins! */
	table = calloc(1, sizeof (struct spreadconv_cell_style));
	table->name = strdup("table_celffl");
	table->border = strdup("1pt solid #000000");
	ce_table = spreadconv_add_unique_cell_style(table, doc);
	if (ce_table < 0){
		fprintf(stderr, "Eroare la crearea stilui celulelor\n");
		return -1;
	}

	return 0;
}

/*
 * create header data for spreadsheet
 * FIXME:
 * 	assert iniparser_getstr
 * 	str* hazards
 */

static int create_header (void)
{
	char *val = NULL;

	/* universitatea si faculateatea  */
	Dprintf("Caut universitatea si facultatea\n");
	doc->cells[0][0].text = strdup(iniparser_getstr(ini, "antet:universitate"));
	doc->cells[1][0].text = strdup(iniparser_getstr(ini, "antet:facultate"));

	/* catedra */
	Dprintf("Caut catedra\n");
	val = strdup(iniparser_getstr(ini, "antet:catedra"));
	doc->cells[0][6].text = malloc(strlen(val) + strlen("Catedra ") + 1);
	strcpy(doc->cells[0][6].text, "Catedra ");
	strcat(doc->cells[0][6].text, val);

	/* luna */
	Dprintf("Caut luna\n");
	month_date = calloc(1, sizeof (struct tm));
	month_date->tm_mon = iniparser_getint(ini, "antet:luna", -1);
	if (month_date->tm_mon < 0) {
		fprintf(stderr, "Eroare la obtinerea lunii\n");
	}
	doc->cells[1][6].text = malloc(strlen("Luna") + 1);
	strcpy(doc->cells[1][6].text, "Luna");
	doc->cells[1][7].text = malloc(12);
	strftime(doc->cells[1][7].text, 12, "%B", month_date);
	Dprintf("Am scris luna\n");

	/* Salvam in month_date data de start a perioadei*/ 
	month_date->tm_year = localtime(&cfg->sem->start)->tm_year;
	month_date->tm_mday = 1;

	Dprintf("Am aflat anul din semestru\n");

	/* some header data */
	doc->cells[3][2].text = strdup("Situatia orelor efectuate de");
	doc->cells[4][2].text = strdup("cu functia de baza la");
	doc->cells[3][5].text = strdup(iniparser_getstr(ini, "antet:nume"));
	
	/* table head */
	doc->cells[6][0].text = strdup("Nr.");
	doc->cells[6][1].text = strdup("Felul si");
	doc->cells[6][2].text = strdup("Facultatea");
	doc->cells[6][3].text = strdup("Disciplina");
	doc->cells[6][4].text = strdup("Curs");
	doc->cells[6][5].text = strdup("Aplicatii");
	doc->cells[6][5].text = strdup("An/Gr");
	doc->cells[6][6].text = strdup("Data");
	doc->cells[6][7].text = strdup("Ore");

	doc->cells[7][0].text = strdup("crt.");
	doc->cells[7][1].text = strdup("nr. post");

	return 0;
}

/*
 * create footer data for spreadsheet
 *
 * TODO: computations ...
 */

static int create_footer (size_t last_row)
{
	doc->cells[last_row][7].text = strdup("Total:");
	last_row += 2;

	doc->cells[last_row][1].text = strdup("TOTAL ore:");
	doc->cells[last_row][2].text = strdup("Curs");
	doc->cells[last_row][3].text = strdup("Nr. ore");
	doc->cells[last_row][4].text = strdup("Aplicatii");
	doc->cells[last_row][5].text = strdup("Nr. ore");
	last_row++;

	doc->cells[last_row][2].text = strdup("Prof.");
	doc->cells[last_row][4].text = strdup("Prof.");
	last_row++;

	doc->cells[last_row][2].text = strdup("Conf.");
	doc->cells[last_row][4].text = strdup("Conf.");
	last_row++;

	doc->cells[last_row][2].text = strdup("S.l.");
	doc->cells[last_row][4].text = strdup("S.l.");
	last_row++;

	doc->cells[last_row][2].text = strdup("As.");
	doc->cells[last_row][4].text = strdup("As.");
	last_row += 2;

	doc->cells[last_row][0].text = strdup("Intocmit,");
	doc->cells[last_row][2].text = strdup("Titular curs,");
	doc->cells[last_row][4].text = strdup("Sef catedra,");
	doc->cells[last_row][6].text = strdup("Decan,");
	last_row++;

	doc->cells[last_row][0].text =
		strdup(iniparser_getstr (ini, "antet:nume"));

	/*
	 * TODO: indexes for the faculty in the cfg->fac array
	 * and for the department in the cfg->fac[i]->depts arraty
	 */

#if 0
	doc->cells[last_row+7][4].text = strdup(cfg->fac[f_index]->dean);
	doc->cells[last_row+7][6].text =
		strdup(cfg->fac[f_index]->depts[d_index].chief);
#endif

	return 0;
}

/*
 * TODO:
 *  * eliberari de memorie, in caz de eroare
 *  * initilizarea pointerilor, cu goto
 * FIXME:
 *  * anumite caractere trebuie escapate (&)
 *    sau in libspreadconv trebuie?
 */

char *
cspay_convert_single_file(char *fname)
{
	struct class_info *ci;
	char *ods_fname = NULL;
	size_t class_index;
	time_t month_start;
	time_t month_end;
	time_t index;
	size_t table_crt;
	struct tm *tmp_date;
	char *tmp_str;

	const char roles[4][14] = {
			{"as"},
			{"conf"},
			{"sl"},
			{"prof"}
	};

	Dprintf("Am inceput sa convertesc fisierul\n");

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

	/* configure spreadsheet column and cell styles */
	config_styles ();

	/* use ini file to create spreadsheet header */
	create_header ();

	month_start = mktime(month_date);
	++ month_date->tm_mon;
	if (month_date->tm_mon == 12){
		month_date->tm_mon = 0;
		++ month_date->tm_year;
	}
	month_end = mktime(month_date) - 1;
	
	/*
	 * browse the classes and add data to spreadsheet
	 * FIXME str* hazards
	 */

	class_index = 1;
	table_crt = 0;

	while (1) {
		Dprintf("Tratez regula %d\n", class_index);

		ci = read_class_info (class_index);
		if (ci == NULL)
			break;

		/*
		 * start from first day
		 * RD: could this happen in a new function?
		 */
		tmp_date = localtime(&month_start);
		tmp_date->tm_mday = 1;
		index = mktime(tmp_date);
		while (index < month_end) {
			if (localtime(&index)->tm_wday == ci->class_day)
				if (is_work(cfg, index)) {
					-- ci->class_first_week;
					if (!ci->class_first_week)
						break;
				}
			index += DAY;
		}
		if (index >= month_end) {
			fprintf(stderr, "Nu am gasit o prima zi ?\n");
			return NULL;
		}
		Dprintf("Am gasit prima zi in care se va lucra %d\n",
					localtime(&index)->tm_mday);
		
		/*
		 * iterate through the same day of different weeks
		 */

		for (/* no init */; index < month_end;
				index += ci->class_parity * WEEK) {
			if (!is_work(cfg, index))
				continue;

			/* row index in table */
			doc->cells[8 + table_crt][0].text = malloc(4);
			sprintf(doc->cells[8 + table_crt][0].text, "%d", table_crt + 1);
			spreadconv_set_cell_style(8 + table_crt, 0, ce_table, doc);

			/* role type and number */
			doc->cells[8 + table_crt][1].text = malloc(10);
			sprintf(doc->cells[8 + table_crt][1].text, "%s%d", roles[ci->class_role_type], ci->class_role_num);
			spreadconv_set_cell_style(8 + table_crt, 1, ce_table, doc);

			/* facultaty */
			doc->cells[8 + table_crt][2].text = strdup(ci->class_faculty);
			spreadconv_set_cell_style(8 + table_crt, 2, ce_table, doc);

			/* course */
			doc->cells[8 + table_crt][3].text = strdup(ci->class_course);
			spreadconv_set_cell_style(8 + table_crt, 3, ce_table, doc);

			/* group */
			doc->cells[8 + table_crt][6].text = strdup(ci->class_group);
			spreadconv_set_cell_style(8 + table_crt, 6, ce_table, doc);

			/* date */
			doc->cells[8 + table_crt][7].text = malloc(20);
			strftime(doc->cells[8 + table_crt][7].text, 19, "%d-%b", localtime(&index));
			spreadconv_set_cell_style(8 + table_crt, 7, ce_table, doc);
				
			/* FIXME: no validation on class timeline */
			tmp_str = malloc (10);
			sprintf (tmp_str, "%d-%d", ci->class_h_start,
					ci->class_h_end);
			doc->cells[8 + table_crt][8].text = tmp_str;
			spreadconv_set_cell_style(8 + table_crt, 8, ce_table, doc);

			if (ci->class_type) {/* e aplicatie */
				doc->cells[8 + table_crt][5].value_type = strdup("float");
				doc->cells[8 + table_crt][5].text = malloc(4);
				sprintf(doc->cells[8 + table_crt][5].text, "%d", ci->class_h_end - ci->class_h_start);
			} else { /* e curs */
				doc->cells[8 + table_crt][4].value_type = strdup("float");
				doc->cells[8 + table_crt][4].text = malloc(4);
				sprintf(doc->cells[8 + table_crt][4].text, "%d", ci->class_h_end - ci->class_h_start);
			}
			spreadconv_set_cell_style(8 + table_crt, 4, ce_table, doc);
			spreadconv_set_cell_style(8 + table_crt, 5, ce_table, doc);

			++ table_crt;
		}

		Dprintf("Am terminat regula %d\n", class_index);

		++ class_index;
		free (ci);
	}

	/* create spreadsheet footer */
	create_footer (table_crt + 8);

	spreadconv_dir_name = strdup("./out/");

	Dprintf("Am inceput sa creez fisierul output\n");

	ods_fname = spreadconv_create_spreadsheet(doc, LSC_FILE_ODS);
	if (!ods_fname) {
		fprintf(stderr, "Eroare la creaarea fisierului ods\n");
		return NULL;
	} else {
		Dprintf("Am creat %s\n\n", name);
	}

	spreadconv_free_spreadconv_data(doc);
	iniparser_freedict(ini);
	free(spreadconv_dir_name);
	cspay_free_config(cfg);

	return ods_fname;
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
	
	Dprintf("Am eliberat memoria pentru cspay_config\n");
}

/*
 * main function for internal testing
 */

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
