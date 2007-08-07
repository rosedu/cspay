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
struct sum_hours {
	int prof;
	int conf;
	int sl;
	int as;
};
struct total_hours {
	struct sum_hours course;
	struct sum_hours aplic;
};
struct table_styles {
	int ce_table;	/* normal cell_table*/
	int up_table;	/* up bold line */
	int down_table;	/* bottom bold line*/
	int right_table;	/* right bold line */
	int left_table;	/* left bold line */
	int up_left_corner;	/* up left corner bold*/
	int up_right_corner;	/* up right corner bold*/
	int down_left_corner;	/* down left corner bold*/
	int down_right_corner;	/* down right corner bold*/
};

static dictionary *ini;
static struct spreadconv_data *doc;
static struct cspay_config *cfg;
static struct tm *month_date;
static struct table_styles *ts;
static struct total_hours result;


static size_t get_first_work_day(time_t start, time_t end, 
							struct cspay_config *cfg, struct class_info *ci);
/*
 * citeste configuratia unei ore.
 * In caz ca este gresita configuratia,
 * inexistenta, sau incompleta, este returnat NULL.
 * Altfel, este returnat un pointer catre o
 * structura de tipul class_info;
 */								
static struct class_info *read_class_info (size_t index)
{
	struct class_info *ci;
	char class_data[32];
	char *timeline = NULL;	/* Nu trebuie eliberat! */

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

	#define STR_LEN_ORE_	4	/* lungimea lui "ore/" */
	strcpy(class_data, "ore/");
	ci->section_len = STR_LEN_ORE_;
	ci->section_len += sprintf(class_data + STR_LEN_ORE_, "%d", (int) index);
	strcpy (ci->section_name, class_data);
	strcat(class_data + ci->section_len, ":");
	++ ci->section_len;

	/* get faculty name */
	strcpy(class_data + ci->section_len, "facultate");
	ci->class_faculty = iniparser_getstr(ini, class_data);
	if (!ci->class_faculty) {
		fprintf(stderr, "Error reading \"facultate\" variable.\n");
		goto READ_ERR;
	}

	/* get course name */
	strcpy(class_data + ci->section_len, "disciplina");
	ci->class_course = iniparser_getstr(ini, class_data);
	if (!ci->class_course) {
		fprintf(stderr, "Error reading \"disciplina\" variable.\n");
		goto READ_ERR;
	}

	/* get role for that class */
	strcpy(class_data + ci->section_len, "rol");
	ci->class_role_type = iniparser_getint(ini, class_data, -1);
	if (ci->class_role_type < 0) {
		fprintf(stderr, "Error reading \"rol\" variable.\n");
		goto READ_ERR;
	}

	/* get role number for class */
	strcpy(class_data + ci->section_len, "numar_post");
	ci->class_role_num = iniparser_getint(ini, class_data, -1);
	if (ci->class_role_num < 0) {
		fprintf(stderr, "Error reading \"numar_post\" variable.\n");
		goto READ_ERR;
	}

	/* get class type (course/lab) */
	strcpy(class_data + ci->section_len, "tip_post");
	ci->class_type = iniparser_getint(ini, class_data, -1);
	if (ci->class_type < 0) {
		fprintf(stderr, "Error reading \"tip_post\" variable.\n");
		goto READ_ERR;
	}

	/* get group for that class */
	strcpy(class_data + ci->section_len, "grupa");
	ci->class_group = iniparser_getstr(ini, class_data);
	if (!ci->class_group) {
		fprintf(stderr, "Error reading \"grupa\" variable.\n");
		goto READ_ERR;
	}

	/* get class day */
	strcpy(class_data + ci->section_len, "zi");
	ci->class_day = iniparser_getint(ini, class_data, -1);
	if (ci->class_day < 0){
		fprintf(stderr, "Error reading \"zi\" variable.\n");
		goto READ_ERR;
	}

	/* get class timeline */
	strcpy(class_data + ci->section_len, "ore");
	timeline = iniparser_getstr(ini, class_data);
	if (!timeline) {
		fprintf(stderr, "Error reading \"ore\" variable.\n");
		goto READ_ERR;
	}
	sscanf(timeline, "%d-%d", &ci->class_h_start, &ci->class_h_end);

	/* get class parity */
	strcpy(class_data + ci->section_len, "paritate");
	ci->class_parity = iniparser_getint(ini, class_data, -1);
	if (ci->class_parity < 0){
		fprintf(stderr, "Error reading \"paritate\" variable.\n");
		goto READ_ERR;
	}

	/* first week */
	strcpy(class_data + ci->section_len, "paritate_start");
	ci->class_first_week = iniparser_getint(ini, class_data, -1);
	if (ci->class_first_week < 0){
		fprintf(stderr, "Error reading \"paritate_start\" variable.\n");
		goto READ_ERR;
	}
	return ci;

	READ_ERR:
	free(ci);
	return NULL;
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
	
	Dprintf("freed cspay_file_list\n");
}		       

struct cspay_config *
cspay_get_config(char *xml_file_name)
{
	if (xml_file_name == NULL)
		xml_file_name = strdup("cspay.xml");
	return read_cspay_xml(xml_file_name);
}

static int config_styles (void)
{
	/* configure columns */
	struct spreadconv_cell_style *curr;

	struct spreadconv_rc_style *wide, *narrow;
	int co_wide, co_narrow;

	Dprintf("Begin columns styles\n");

	wide = calloc(1, sizeof (struct spreadconv_rc_style));
	wide->name = strdup("wide_col");
	wide->size = strdup("5cm");
	co_wide = spreadconv_add_unique_rc_style(wide, doc);

	narrow = calloc(1, sizeof (struct spreadconv_rc_style));
	narrow->name = strdup("narrow_col");
	narrow->size = strdup("0.6cm");
	co_narrow = spreadconv_add_unique_rc_style(narrow, doc);

	spreadconv_set_col_style(0, co_narrow, doc);
	spreadconv_set_col_style(3, co_wide, doc);

	/* stiluri pentru casute*/
	/* TABLE STYLES */
	Dprintf("Begin cells styles\n");
	ts = calloc(1, sizeof (struct table_styles));

	/* calloc vs. malloc, calloc wins! */

	curr = calloc(1, sizeof (struct spreadconv_cell_style));
	curr->name = strdup("table_normal_cell");
	curr->border = strdup("1pt solid #000000");
	ts->ce_table = spreadconv_add_unique_cell_style(curr, doc);
	if (ts->ce_table < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}
	curr = calloc(1, sizeof (struct spreadconv_cell_style));
	curr->name = strdup("table_up_cell");
	curr->border = strdup("1pt solid #000000");
	curr->border_top = strdup("2pt solid #000000");
	ts->up_table = spreadconv_add_unique_cell_style(curr, doc);
	if (ts->up_table < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}
	curr = calloc(1, sizeof (struct spreadconv_cell_style));
	curr->name = strdup("table_down_cell");
	curr->border = strdup("1pt solid #000000");
	curr->border_bottom = strdup("2pt solid #000000");
	ts->down_table = spreadconv_add_unique_cell_style(curr, doc);
	if (ts->down_table < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}
	curr = calloc(1, sizeof (struct spreadconv_cell_style));
	curr->name = strdup("table_left_cell");
	curr->border = strdup("1pt solid #000000");
	curr->border_left = strdup("2pt solid #000000");
	ts->left_table = spreadconv_add_unique_cell_style(curr, doc);
	if (ts->left_table < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}
	curr = calloc(1, sizeof (struct spreadconv_cell_style));
	curr->name = strdup("table_right_cell");
	curr->border = strdup("1pt solid #000000");
	curr->border_right = strdup("2pt solid #000000");
	ts->right_table = spreadconv_add_unique_cell_style(curr, doc);
	if (ts->right_table < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}

	curr = calloc(1, sizeof (struct spreadconv_cell_style));
	curr->name = strdup("table_up_left_cell");
	curr->border = strdup("1pt solid #000000");
	curr->border_top = strdup("2pt solid #000000");
	curr->border_left = strdup("2pt solid #000000");
	ts->up_left_corner = spreadconv_add_unique_cell_style(curr, doc);
	if (ts->up_left_corner < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}
	curr = calloc(1, sizeof (struct spreadconv_cell_style));
	curr->name = strdup("table_up_right_cell");
	curr->border = strdup("1pt solid #000000");
	curr->border_top = strdup("2pt solid #000000");
	curr->border_right = strdup("2pt solid #000000");
	ts->up_right_corner = spreadconv_add_unique_cell_style(curr, doc);
	if (ts->up_right_corner < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}
	curr = calloc(1, sizeof (struct spreadconv_cell_style));
	curr->name = strdup("table_down_left_cell");
	curr->border = strdup("1pt solid #000000");
	curr->border_bottom = strdup("2pt solid #000000");
	curr->border_left = strdup("2pt solid #000000");
	ts->down_left_corner = spreadconv_add_unique_cell_style(curr, doc);
	if (ts->down_left_corner < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}
	curr = calloc(1, sizeof (struct spreadconv_cell_style));
	curr->name = strdup("table_down_right_cell");
	curr->border = strdup("1pt solid #000000");
	curr->border_bottom = strdup("2pt solid #000000");
	curr->border_right = strdup("2pt solid #000000");
	ts->down_right_corner = spreadconv_add_unique_cell_style(curr, doc);
	if (ts->down_right_corner < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}
	return 0;

	ERR_TS:
	free(ts);
	return -1;

}

/*
 * create header data for spreadsheet
 */

static int create_header (void)
{
	char *val = NULL;		
	/* TODO cred ca se poate si cu un singur char* */
	/* universitatea si faculateatea  */
	Dprintf("I look for university name\n");
	val = iniparser_getstr(ini, "antet:universitate");
	if (!val){
		fprintf(stderr, "Error obatining university name.\n");
		goto ERR_;
	}
	doc->cells[0][0].text = strdup(val);

	Dprintf("I look for faculty name\n");
	val = iniparser_getstr(ini, "antet:facultate");
	if (!val){
		fprintf(stderr, "Error obatining university name.\n");
		goto ERR_;
	}
	doc->cells[1][0].text = strdup(val);

	/* catedra */
	#define DEPT_LIMIT	30	/* nr. max. de car. ptr catedra */
	#define STR_DEPT_SIZE	7	/* strlen("Catedra ") */
	Dprintf("I look for department\n");
	val = iniparser_getstr(ini, "antet:catedra");
	if (!val) {
		fprintf(stderr, "Error obatining department.\n");
		goto ERR_;
	}

	doc->cells[0][6].text = malloc(DEPT_LIMIT);
	strncpy(doc->cells[0][6].text, "Catedra ", STR_DEPT_SIZE);
	strncat(doc->cells[0][6].text, val, DEPT_LIMIT - STR_DEPT_SIZE - 1);

	/* luna */
	static char months[12][32] = {
		{"Ianuarie"},
		{"Februarie"},
		{"Martie"},
		{"Aprilie"},
		{"Mai"},
		{"Iunie"},
		{"Iulie"},
		{"August"},
		{"Septembrie"},
		{"Octombrie"},
		{"Noiembrie"},
		{"Decembrie"}
	};
	Dprintf("I look for month\n");
	month_date = calloc(1, sizeof (struct tm));
	month_date->tm_mon = iniparser_getint(ini, "antet:luna", -1);
	if (month_date->tm_mon < 0) {
		fprintf(stderr, "Error obtaining month\n");
		goto ERR_;
	}
	doc->cells[1][6].text = strdup("Luna ");
	doc->cells[1][7].text = strdup(months[month_date->tm_mon]);

	Dprintf("I wrote the month\n");

	/* Salvam in month_date data de start a perioadei*/ 
	if (!localtime(&cfg->sem->start)) {
		fprintf(stderr, "Error converting date.\n");
		goto ERR_;     
	}
	month_date->tm_year = localtime(&cfg->sem->start)->tm_year;
	month_date->tm_mday = 1;

	Dprintf("I have calculated the year from semester's limits\n");

	/* some header data */
	doc->cells[3][2].text = strdup("Situatia orelor efectuate de");
	doc->cells[4][2].text = strdup("cu functia de baza la");
	val = iniparser_getstr(ini, "antet:nume");
	if (!val) {
		fprintf(stderr, "Error obtaining name.\n");
		goto ERR_;
	}
	doc->cells[3][5].text = strdup(val);



	/* table head */
	doc->cells[6][0].text = strdup("Nr.");
	doc->cells[6][1].text = strdup("Felul si");
	doc->cells[6][2].text = strdup("Facultatea");
	doc->cells[6][3].text = strdup("Disciplina");
	doc->cells[6][4].text = strdup("Curs");
	doc->cells[6][5].text = strdup("Aplicatii");
	doc->cells[6][6].text = strdup("An/Gr");
	doc->cells[6][7].text = strdup("Data");
	doc->cells[6][8].text = strdup("Ore");

	doc->cells[7][0].text = strdup("crt.");
	doc->cells[7][1].text = strdup("nr. post");

	return 0;
	ERR_:
	return -1;
}

/*
 * create footer data for spreadsheet
 */

static int create_footer (size_t last_row)
{
	char formula[32];
	char *tmp_ini_val;
	sprintf(formula, "=sum(E9:F%d)", (int) last_row - 1);
	doc->cells[last_row][7].text = strdup("Total:");
	doc->cells[last_row][8].value_type = strdup("formula");
	doc->cells[last_row][8].text= strdup(formula);
	last_row += 2;

	doc->cells[last_row][1].text = strdup("TOTAL ore:");
	doc->cells[last_row][2].text = strdup("Curs");
	doc->cells[last_row][3].text = strdup("Nr. ore");
	doc->cells[last_row][4].text = strdup("Aplicatii");
	doc->cells[last_row][5].text = strdup("Nr. ore");
	last_row++;

	doc->cells[last_row][2].text = strdup("Prof.");
	doc->cells[last_row][3].text = malloc(5);
	sprintf(doc->cells[last_row][3].text, "%d", result.course.prof);
	doc->cells[last_row][4].text = strdup("Prof.");
	doc->cells[last_row][5].text = malloc(5);
	sprintf(doc->cells[last_row][5].text, "%d", result.aplic.prof);
	last_row++;

	doc->cells[last_row][2].text = strdup("Conf.");
	doc->cells[last_row][3].text = malloc(5);
	sprintf(doc->cells[last_row][3].text, "%d", result.course.conf);
	doc->cells[last_row][4].text = strdup("Conf.");
	doc->cells[last_row][5].text = malloc(5);
	sprintf(doc->cells[last_row][5].text, "%d", result.aplic.conf);
	last_row++;

	doc->cells[last_row][2].text = strdup("S.l.");
	doc->cells[last_row][3].text = malloc(5);
	sprintf(doc->cells[last_row][3].text, "%d", result.course.sl);
	doc->cells[last_row][4].text = strdup("S.l.");
	doc->cells[last_row][5].text = malloc(5);
	sprintf(doc->cells[last_row][5].text, "%d", result.aplic.sl);
	last_row++;

	doc->cells[last_row][2].text =	 strdup("As.");
	doc->cells[last_row][3].text = malloc(5);
	sprintf(doc->cells[last_row][3].text, "%d", result.course.as);
	doc->cells[last_row][4].text = strdup("As.");
	doc->cells[last_row][5].text = malloc(5);
	sprintf(doc->cells[last_row][5].text, "%d", result.aplic.as);
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
	 *
	 * De ce, informatia vine din personal.ini?
	 */

#if 0
	doc->cells[last_row+7][4].text = strdup(cfg->fac[f_index]->dean);
	doc->cells[last_row+7][6].text =
		strdup(cfg->fac[f_index]->depts[d_index].chief);
#endif
/*
 * TODO titular curs? ce-i cu el?
 */
	tmp_ini_val = iniparser_getstr(ini, "antet:decan");
	if (!tmp_ini_val) {
		fprintf(stderr, "Nu am gasit decanul.\n");
	} else {
		doc->cells[last_row][6].text = strdup(tmp_ini_val);
	}

	tmp_ini_val = iniparser_getstr(ini, "antet:sef_catedra");
	if (!tmp_ini_val) {
		fprintf(stderr, "Nu am gasit sefcatedra.\n");
	} else {
		doc->cells[last_row][4].text = strdup(tmp_ini_val);
	}
	return 0;
}

/*
 * Asta este cea mai importanta din biblioteca
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
	size_t table_crt;	/* int vs. size_t ???*/
	char *tmp_str;
	int ccs;	/* current cell style */

	int tmp_sum_prof;
	int tmp_sum_conf;
	int tmp_sum_sl;
	int tmp_sum_as;
	int tmp_sum;

	const char roles[4][14] = {
			{"as"},
			{"conf"},
			{"sl"},
			{"prof"}
	};

	Dprintf("Processing ini file\n");


	/* init ini file parsing */
	ini = iniparser_load(fname);
	if (!ini) {
		fprintf(stderr, ".ini file not found, try personal.ini\n");
		ini = iniparser_load("personal.ini");
		if (!ini){
			fprintf(stderr, "personal.ini not found, bye!\n");
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
		Dprintf("Begin rule number%d\n", class_index);

		ci = read_class_info (class_index);
		if (ci == NULL)
			break;

		index = get_first_work_day(month_start, month_end, cfg, ci);
		
		Dprintf("First working day is: %d\n",
					localtime(&index)->tm_mday);
		
		/*
		 * iterate through the same day of different weeks
		 */

		tmp_sum_prof = tmp_sum_conf = tmp_sum_sl = tmp_sum_as = 0;
		tmp_sum = 0;
		for (/* no init */; index < month_end;
				index += ci->class_parity * WEEK) {
			if (!is_work(cfg, index))
				continue;
			/* row index in table */
			if (table_crt == 0) {
				ccs = ts->up_left_corner;
			} else {
				ccs = ts->left_table;
			}
			doc->cells[8 + table_crt][0].text = malloc(4);
			sprintf(doc->cells[8 + table_crt][0].text, "%d", (int) table_crt + 1);
			spreadconv_set_cell_style(8 + table_crt, 0, ccs, doc);

			/* role type and number */
			if (table_crt == 0){
				ccs = ts->up_table;
			} else {
				ccs = ts->ce_table;
			}
			doc->cells[8 + table_crt][1].text = malloc(10);
			sprintf(doc->cells[8 + table_crt][1].text, "%s%d", roles[ci->class_role_type], ci->class_role_num);
			spreadconv_set_cell_style(8 + table_crt, 1, ccs, doc);


			/* faculty */
			doc->cells[8 + table_crt][2].text = strdup(ci->class_faculty);
			spreadconv_set_cell_style(8 + table_crt, 2, ccs, doc);

			/* course */
			doc->cells[8 + table_crt][3].text = strdup(ci->class_course);
			spreadconv_set_cell_style(8 + table_crt, 3, ccs, doc);

			if (ci->class_type) {/* e aplicatie */
				doc->cells[8 + table_crt][5].value_type = strdup("float");
				doc->cells[8 + table_crt][5].text = malloc(4);
				sprintf(doc->cells[8 + table_crt][5].text, "%d", ci->class_h_end - ci->class_h_start);
			} else { /* e curs */
				doc->cells[8 + table_crt][4].value_type = strdup("float");
				doc->cells[8 + table_crt][4].text = malloc(4);
				sprintf(doc->cells[8 + table_crt][4].text, "%d", ci->class_h_end - ci->class_h_start);
			}
			spreadconv_set_cell_style(8 + table_crt, 4, ccs, doc);
			spreadconv_set_cell_style(8 + table_crt, 5, ccs, doc);

			/* group */
			doc->cells[8 + table_crt][6].text = strdup(ci->class_group);
			spreadconv_set_cell_style(8 + table_crt, 6, ccs, doc);

			/* date */
			doc->cells[8 + table_crt][7].text = malloc(20);
			strftime(doc->cells[8 + table_crt][7].text, 19, "%d-%b", localtime(&index));
			spreadconv_set_cell_style(8 + table_crt, 7, ccs, doc);
				
			/* FIXME: no validation on class timeline */
			if (table_crt == 0) {
				ccs = ts->up_right_corner;
			} else {
				ccs = ts->right_table;
			}
			tmp_str = malloc (10);
			sprintf (tmp_str, "%d-%d", ci->class_h_start,
					ci->class_h_end);
			doc->cells[8 + table_crt][8].text = tmp_str;
			spreadconv_set_cell_style(8 + table_crt, 8, ccs, doc);

			++ table_crt;
			tmp_sum +=  ci->class_h_end - ci->class_h_start;
		}

		Dprintf("End rule number: %d\n", class_index);
		if (ci->class_type) {/* e aplicatie*/
			if (ci->class_role_type == 0)
				result.aplic.as += tmp_sum;
			if (ci->class_role_type == 1)
				result.aplic.conf += tmp_sum;
			if (ci->class_role_type == 2)
				result.aplic.sl += tmp_sum;
			if (ci->class_role_type == 3)
				result.aplic.prof += tmp_sum;
		} else {	/* e curs */
			if (ci->class_role_type == 0)
				result.course.as += tmp_sum;
			if (ci->class_role_type == 1)
				result.course.conf += tmp_sum;
			if (ci->class_role_type == 2)
				result.course.sl += tmp_sum;
			if (ci->class_role_type == 3)
				result.course.prof += tmp_sum;
		}
		++ class_index;
		free (ci);
	}
	Dprintf("End of all rules.\n");
	spreadconv_set_cell_style(8 + table_crt - 1, 0, ts->down_left_corner, doc);
	spreadconv_set_cell_style(8 + table_crt - 1, 1, ts->down_table, doc);
	spreadconv_set_cell_style(8 + table_crt - 1, 2, ts->down_table, doc);
	spreadconv_set_cell_style(8 + table_crt - 1, 3, ts->down_table, doc);
	spreadconv_set_cell_style(8 + table_crt - 1, 4, ts->down_table, doc);
	spreadconv_set_cell_style(8 + table_crt - 1, 5, ts->down_table, doc);
	spreadconv_set_cell_style(8 + table_crt - 1, 6, ts->down_table, doc);
	spreadconv_set_cell_style(8 + table_crt - 1, 7, ts->down_table, doc);
	spreadconv_set_cell_style(8 + table_crt - 1, 8, ts->down_right_corner, doc);
	/* create spreadsheet footer */
	create_footer (table_crt + 8);

	spreadconv_dir_name = strdup("./out/");

	Dprintf("Begin output file\n");

	ods_fname = spreadconv_create_spreadsheet(doc, LSC_FILE_ODS);
	if (!ods_fname) {
		fprintf(stderr, "Error creating .ods file\n");
		return NULL;
	} else {
		Dprintf("Output: %s\n\n", ods_fname);
	}

	spreadconv_free_spreadconv_data(doc);
	iniparser_freedict(ini);
	free(spreadconv_dir_name);

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
cspay_convert_options(struct cspay_config *config, char *fname)
{
	/* tipul functiei cred ca e gresit */
	/*
	 * In ce caz se returneaza mai multe fisiere? 
	 * RD: In cazul in care se realizeaza plata cu ora pe mai multe luni.
	 * Se extrage un fisier .ods pentru fiecare luna.
	 */

	struct cspay_file_list *ret;
	char *temp;
	
	cfg = config; /* FIXME ugly! cfg is global */
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
	
	Dprintf("freed cspay_config\n");
}

/*
 * afla prima zi de lucru
 */
static size_t get_first_work_day(time_t start, time_t end, 
							struct cspay_config *cfg, struct class_info *ci)
{
	struct tm *tmp_date;
	time_t ret;
	tmp_date = localtime(&start);
	tmp_date->tm_mday = 1;
	ret = mktime(tmp_date);
	while (ret < end) {
		if (localtime(&ret)->tm_wday == ci->class_day) {
			-- ci->class_first_week;
			if (is_work(cfg, ret)) {
				if (ci->class_first_week <= 0)
					break;
			}
		}
		ret += DAY;
	}
	if (ret >= end) {
		fprintf(stderr, "Impossible error\n");
		return -1;
	}
	return ret;
}
