/*
 * libcspay main file
 */
/**
 * \ingroup libcspay
 * \file cspay.c
 * \brief libcspay main file
 * \author Cojocar Lucian
 */
#include <stdlib.h>	/* system headers go before user headers */
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

#include "debug.h"
#include "load_cfg.h"
#include "cspay.h"

#include "spreadconv.h"
#include "iniparser.h"

#define HOUR	3600		/**< 1 hour in seconds*/
#define DAY	(24 * HOUR)	/**< 1 day in seconds*/
#define WEEK	(7 * DAY)	/**< 1 week in seconds*/

/**
 * role types
 */
#define LCSP_ROLE_ASS	0	/**< assinstant type :-) */
#define LCSP_ROLE_CON	1	/**< "conferentiar" */
#define LCSP_ROLE_SEL	2	/**< "sef lucrari" */
#define LCSP_ROLE_TEA	3	/**< teacher */

/**
 * class types
 */
#define LCSP_CLASS_CRS	0	/**< course */
#define LCSP_CLASS_APP	1	/**< application */
/**
 * \struct class_info
 * contains all the information about a specific class
 */
struct class_info {
	/** name length, see bellow*/
	int name_len;
	/** name of course, usefull in reading information from ini file
	 * eg: "ore/1", "ore/2"
	 */
	char name[8];

	/** day in week of class*/
	int day;

	/** class parity
	 * 1 - every week
	 * 2 - every two weeks
	 * etc.
	 */
	int parity;
	/** first week of class
	 * 1 - the class begin in first *valid* week of the month
	 * 2 - the class begin in second *valid* week of the month
	 * etc.
	 * \todo check if this is implemented corectly
	 */
	int first_week;

	/** role type
	 * 0 - assistant
	 * 1 - "conferentiar"
	 * 2 - "sef lucrari"
	 * 3 - teacher
	 */
	int role_type;
	/** role number/name*/
	char *role_num;

	/** name of class, a.k.a. disciplina*/
	char *class;

	/** short name of faculty*/
	char *faculty;
	
	/** group name of class
	 * eg: 312CC*/
	char *group;

	/** class type
	 * 0 - course
	 * 1 - application
	 */
	int class_type;

	/**
	 * time of class
	 * \remarks interval is used as int
	 */
	struct interval timeline;
	
};

/**
 * \struct sum_hours 
 * contains sums from final table
 */
struct sum_hours {
	/** sum of hours as teacher*/
	int prof;

	/** sum of hours as "conferentiar"*/
	int conf;

	/** sum of hours as "sef lucrari"*/
	int sl;

	/** sum of hours as assistant*/
	int as;
};

/**
 * \struct total_hours
 * contains sum hour
 */
struct total_hours {
	/** for course*/
	struct sum_hours course;
	/** for applications*/
	struct sum_hours aplic;
};

/**
 * \struct defined_styles
 * contains informations used for creating spreadsheets
 */
struct defined_styles {
	/**
	 * header table cell styles
	 * \remark same style for all cell from header
	 */
	int table_h;

	/**
	 * table content styles
	 * table_c[0], first col
	 * table_c[1], 1, 2, 3, 4, 5,  col
	 * table_c[2], 6, 7, 8 col
	 */
	int table_c[3];

	/**
	 * last row styles
	 * table_b[0], 0 col
	 * table_b[1], 1, 2, 3, 4, 5 col
	 * table_b[2], 6, 7, 8 col
	 */
	int table_b[3];

	/**
	 * row/columns styles
	 * rc[0],  col 0.6cm (0)
	 * rc[1],  col 1.5cm (4, 5, 6, 7, 8)
	 * rc[2],  col 2.0cm (2, 3)
	 * rc[3],  row 1cm
	 */
	int rc[4];
};

/** ini file dictionary */
static dictionary *ini;

/** output document */
static struct spreadconv_data *doc;

/** config from xml file*/
static struct cspay_config *cfg;

/** month for wich we generate the output
 * this also contains the year,
 * day is, initially set to 1
 */
static struct tm *month;

/** styles for spreasheets*/
static struct defined_styles *ds;

/** results*/
static struct total_hours result;


static time_t get_first_work_day(struct interval t, struct class_info *ci);
static int is_work(time_t t);
static char *build_file_name(void);

/**
 * Read from ini file "[ore/index]" class
 * \param index class number
 * \return NULL if an error ocured, else
 * a class_info pointer
 */								
static struct class_info *read_class_info (size_t index)
{
	struct class_info *ret;
	char read[32];
	char *timeline;	/* Nu trebuie eliberat! */
	int nl;

	ret = malloc (sizeof (*ret));
	if (ret == NULL) {
		perror ("malloc");
		return NULL;
	}

	/*
	 * class data contains the section name (ore/[number]),
	 * followed by a colon sign and the variable name;
	 * this is how iniparser works
	 */

	#define STR_LEN_ORE_	4	/* lungimea lui "ore/" */
	strcpy(read, "ore/");
	ret->name_len = STR_LEN_ORE_;
	ret->name_len += snprintf(read + STR_LEN_ORE_, 8 - STR_LEN_ORE_, "%d", (int) index);
	strncpy (ret->name, read, 8);
	strcpy(read + ret->name_len, ":");

	nl = ret->name_len + 1;	/* 1 = strlen(":")*/

	/* get faculty name */
	strcpy(read + nl, "facultate");
	ret->faculty = iniparser_getstr(ini, read);
	if (!ret->faculty) {
	/*
		fprintf(stderr, "Error reading \"facultate\" variable.\n");
	*/
		goto READ_ERR;
	}

	/* get course name */
	strcpy(read + nl, "disciplina");
	ret->class = iniparser_getstr(ini, read);
	if (!ret->class) {
		fprintf(stderr, "Error reading \"disciplina\" variable.\n");
		goto READ_ERR;
	}

	/* get role for that class */
	strcpy(read + nl, "rol");
	ret->role_type = iniparser_getint(ini, read, -1);
	if (ret->role_type < 0) {
		fprintf(stderr, "Error reading \"rol\" variable.\n");
		goto READ_ERR;
	}

	/* get role number for class */
	strcpy(read + nl, "numar_post");
	ret->role_num = iniparser_getstr(ini, read);
	if (!ret->role_num) {
		fprintf(stderr, "Error reading \"numar_post\" variable.\n");
		goto READ_ERR;
	}

	/* get class type (course/lab) */
	strcpy(read + nl, "tip_post");
	ret->class_type = iniparser_getint(ini, read, -1);
	if (ret->class_type < 0) {
		fprintf(stderr, "Error reading \"tip_post\" variable.\n");
		goto READ_ERR;
	}

	/* get group for that class */
	strcpy(read + nl, "grupa");
	ret->group = iniparser_getstr(ini, read);
	if (!ret->group) {
		fprintf(stderr, "Error reading \"grupa\" variable.\n");
		goto READ_ERR;
	}

	/* get class day */
	strcpy(read + nl, "zi");
	ret->day = iniparser_getint(ini, read, -1);
	if (ret->day < 0){
		fprintf(stderr, "Error reading \"zi\" variable.\n");
		goto READ_ERR;
	}

	/* get class timeline */
	strcpy(read + nl, "ore");
	timeline = iniparser_getstr(ini, read);
	if (!timeline) {
		fprintf(stderr, "Error reading \"ore\" variable.\n");
		goto READ_ERR;
	}
	if (2 != sscanf(timeline, "%d-%d", (int *)&ret->timeline.start, (int *) &ret->timeline.end)) {
		fprintf(stderr, "Error *parsing* \"ore\" variable.\n");
		goto READ_ERR;
	}

	/* get class parity */
	strcpy(read + nl, "paritate");
	ret->parity = iniparser_getint(ini, read, -1);
	if (ret->parity < 0){
		fprintf(stderr, "Error reading \"paritate\" variable.\n");
		goto READ_ERR;
	}

	/* first week */
	strcpy(read + nl, "paritate_start");
	ret->first_week = iniparser_getint(ini, read, -1);
	if (ret->first_week < 0){
		fprintf(stderr, "Error reading \"paritate_start\" variable.\n");
		goto READ_ERR;
	}
	return ret;

	READ_ERR:
	free(ret);
	return NULL;
}


/**
 * free a file list
 * \param list list to be freed
 */
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

/**
 * read config from xml file
 * \param xml_file_name xml file name
 * \return a config structure from xml file
 */
struct cspay_config *
cspay_get_config(char *xml_file_name)
{
	if (xml_file_name == NULL)
		xml_file_name = strdup("cspay.xml");
	return read_cspay_xml(xml_file_name);
}

/**
 * Config styles for tables, rows and columns
 * \return 0 on succes
 * \return -1 on error
 */
static int 
config_styles (void)
{
	/* configure columns */
	struct spreadconv_cell_style *ccs;
	struct spreadconv_rc_style *rcs;

	ds = calloc(1, sizeof (struct defined_styles));

	Dprintf("Begin row/columns styles\n");

	rcs = calloc(1, sizeof (struct spreadconv_rc_style));
	rcs->type = LSC_STYLE_COL;
	rcs->name = strdup("col_0_6_cm");
	rcs->size = strdup("0.6cm");
	ds->rc[0] = spreadconv_add_unique_rc_style(rcs, doc);

	rcs = calloc(1, sizeof (struct spreadconv_rc_style));
	rcs->type = LSC_STYLE_COL;
	rcs->name = strdup("col_1_5_cm");
	rcs->size = strdup("1.5cm");
	ds->rc[1] = spreadconv_add_unique_rc_style(rcs, doc);

	rcs = calloc(1, sizeof (struct spreadconv_rc_style));
	rcs->type = LSC_STYLE_COL;
	rcs->name = strdup("col_2_0_cm");
	rcs->size = strdup("2cm");
	ds->rc[2] = spreadconv_add_unique_rc_style(rcs, doc);

	rcs = calloc(1, sizeof (struct spreadconv_rc_style));
	rcs->type = LSC_STYLE_ROW;
	rcs->name = strdup("row_1_0_cm");
	rcs->size = strdup("1cm");
	ds->rc[3] = spreadconv_add_unique_rc_style(rcs, doc);
	
	spreadconv_set_col_style(0, ds->rc[0], doc);
	spreadconv_set_col_style(2, ds->rc[2], doc);
	spreadconv_set_col_style(3, ds->rc[2], doc);
	spreadconv_set_col_style(4, ds->rc[1], doc);
	spreadconv_set_col_style(5, ds->rc[1], doc);
	spreadconv_set_col_style(6, ds->rc[1], doc);
	spreadconv_set_col_style(7, ds->rc[1], doc);
	spreadconv_set_col_style(8, ds->rc[1], doc);
	spreadconv_set_row_style(6, ds->rc[3], doc);
	
	/* cell styles */
	Dprintf("Begin cells styles\n");

	/* Table Header */
	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_header_1");
	ccs->halign = strdup("center");
	ccs->valign = strdup("middle");
	ccs->border = strdup("2pt solid #000000");
	ds->table_h = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_h < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}

	/* Table content */
	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_content_1");
	ccs->border_bottom = strdup("1pt solid #000000");
	ccs->border_left = strdup("2pt solid #000000");
	ccs->border_right = strdup("2pt solid #000000");
	ccs->halign = strdup("end");
	ds->table_c[0] = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_c[0] < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}

	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_content_2");
	ccs->border_bottom = strdup("1pt solid #000000");
	ccs->border_left = strdup("2pt solid #000000");
	ccs->border_right = strdup("2pt solid #000000");
	ccs->halign = strdup("center");
	ds->table_c[1] = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_c[1] < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}

	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_content_3");
	ccs->border_bottom = strdup("1pt solid #000000");
	ccs->border_left = strdup("2pt solid #000000");
	ccs->border_right = strdup("2pt solid #000000");
	ccs->halign = strdup("start");
	ds->table_c[2] = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_c[2] < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}
	/* Table bottom */
	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_bottom_1");
	ccs->border_bottom = strdup("2pt solid #000000");
	ccs->border_left = strdup("2pt solid #000000");
	ccs->border_right = strdup("2pt solid #000000");
	ccs->halign = strdup("end");
	ds->table_b[0] = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_b[0] < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}

	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_bottom_2");
	ccs->border_bottom = strdup("2pt solid #000000");
	ccs->border_left = strdup("2pt solid #000000");
	ccs->border_right = strdup("2pt solid #000000");
	ccs->halign = strdup("center");
	ds->table_b[1] = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_b[1] < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}

	ccs = calloc(1, sizeof (struct spreadconv_cell_style));
	ccs->name = strdup("table_bottom_3");
	ccs->border_bottom = strdup("2pt solid #000000");
	ccs->border_left = strdup("2pt solid #000000");
	ccs->border_right = strdup("2pt solid #000000");
	ccs->halign = strdup("start");
	ds->table_b[2] = spreadconv_add_unique_cell_style(ccs, doc);
	if (ds->table_b[2] < 0){
		fprintf(stderr, "Error at cells styles\n");
		goto ERR_TS;
	}
	return 0;

	ERR_TS:
	free(ds);
	return -1;

}

/**
 * create header data for spreadsheet
 * \return 0, on succes
 * \return -1 on error
 * \remarks \a month is allocated here
 */
static int
create_header (void)
{
	char *val = NULL;		

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
	#define DEPT_LIMIT	30	/**< nr. max. de car. ptr catedra */
	#define STR_DEPT_SIZE	8	/**< strlen("Catedra ") */
	Dprintf("I look for department\n");
	val = iniparser_getstr(ini, "antet:catedra");
	if (!val) {
		fprintf(stderr, "Error obatining department.\n");
		goto ERR_;
	}

	doc->cells[0][6].text = malloc(DEPT_LIMIT);
	strcpy(doc->cells[0][6].text, "Catedra ");
	strncat(doc->cells[0][6].text, val, DEPT_LIMIT - STR_DEPT_SIZE - 1);

	/* luna */
	static char str_months[12][32] = {
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
	month = calloc(1, sizeof (struct tm));
	month->tm_mon = iniparser_getint(ini, "antet:luna", -1);
	if (month->tm_mon < 0) {
		fprintf(stderr, "Error obtaining month\n");
		goto ERR_;
	}
	doc->cells[1][6].text = strdup("Luna ");
	doc->cells[1][7].text = strdup(str_months[month->tm_mon]);

	Dprintf("I wrote the month\n");

	/* Salvam in month data de start a perioadei*/ 
	if (!localtime(&cfg->sem->start)) {
		fprintf(stderr, "Error converting date.\n");
		goto ERR_;     
	}
	month->tm_year = localtime(&cfg->sem->start)->tm_year;
	month->tm_mday = 1;

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
	val = iniparser_getstr(ini, "antet:nume_curs");
	if (!val) {
		fprintf(stderr, "Error obtaining course name.\n");
		goto ERR_;
	}
	doc->cells[4][5].text = strdup(val);


	/* table head */
	doc->cells[6][0].text = strdup("Nr.\ncrt.");
	spreadconv_set_cell_style(6, 0, ds->table_h, doc);

	doc->cells[6][1].text = strdup("Felul si\nnr. post");
	spreadconv_set_cell_style(6, 1, ds->table_h, doc);

	doc->cells[6][2].text = strdup("Facultatea");
	spreadconv_set_cell_style(6, 2, ds->table_h, doc);

	doc->cells[6][3].text = strdup("Disciplina");
	spreadconv_set_cell_style(6, 3, ds->table_h, doc);

	doc->cells[6][4].text = strdup("Curs");
	spreadconv_set_cell_style(6, 4, ds->table_h, doc);

	doc->cells[6][5].text = strdup("Aplicatii");
	spreadconv_set_cell_style(6, 5, ds->table_h, doc);

	doc->cells[6][6].text = strdup("An/Gr");
	spreadconv_set_cell_style(6, 6, ds->table_h, doc);

	doc->cells[6][7].text = strdup("Data");
	spreadconv_set_cell_style(6, 7, ds->table_h, doc);

	doc->cells[6][8].text = strdup("Ore");
	spreadconv_set_cell_style(6, 8, ds->table_h, doc);

	return 0;
	ERR_:
	return -1;
}

/**
 * create footer data for spreadsheet
 * \param last_row footer's first row
 * \return 0 on succes
 * \return 01 on error
 */
static int create_footer (size_t last_row)
{
	
	char tot[12];
	char *tmp_ini_val;
	int i;
	sprintf(tot, "%d", result.course.prof + result.aplic.prof +
			result.course.conf + result.aplic.conf +
			result.course.sl + result.aplic.sl +
			result.course.as + result.aplic.as);
	doc->cells[last_row][7].text = strdup("Total:");
	doc->cells[last_row][8].value_type = strdup("float");
	doc->cells[last_row][8].text= strdup(tot);
	last_row += 2;

	/* doc->cells[last_row][1].text = strdup("TOTAL ore:");
	 */
	doc->cells[last_row][3].text = strdup("Curs");
	spreadconv_set_cell_style(last_row, 3, ds->table_h, doc);

/*	doc->cells[last_row][3].text = strdup("Nr. ore");
	spreadconv_set_cell_style(last_row, 3, ds->table_h, doc);*/

	doc->cells[last_row][4].text = strdup("Aplicatii");
	spreadconv_set_cell_style(last_row, 4, ds->table_h, doc);

/*	doc->cells[last_row][5].text = strdup("Nr. ore");
	spreadconv_set_cell_style(last_row, 5, ds->table_h, doc);*/

	last_row++;

	doc->cells[last_row][2].text = strdup("Prof.");
	spreadconv_set_cell_style(last_row, 2, ds->table_h, doc);
	doc->cells[last_row][3].text = malloc(5);
	sprintf(doc->cells[last_row][3].text, "%d", result.course.prof);
	
/*	doc->cells[last_row][4].text = strdup("Prof."); */
	doc->cells[last_row][4].text = malloc(5);
	sprintf(doc->cells[last_row][4].text, "%d", result.aplic.prof);
	last_row++;

	doc->cells[last_row][2].text = strdup("Conf.");
	spreadconv_set_cell_style(last_row, 2, ds->table_h, doc);

	doc->cells[last_row][3].text = malloc(5);
	sprintf(doc->cells[last_row][3].text, "%d", result.course.conf);
/*	doc->cells[last_row][4].text = strdup("Conf.");*/
	doc->cells[last_row][4].text = malloc(5);
	sprintf(doc->cells[last_row][4].text, "%d", result.aplic.conf);
	last_row++;

	doc->cells[last_row][2].text = strdup("S.l.");
	spreadconv_set_cell_style(last_row, 2, ds->table_h, doc);

	doc->cells[last_row][3].text = malloc(5);
	sprintf(doc->cells[last_row][3].text, "%d", result.course.sl);
/*	doc->cells[last_row][4].text = strdup("S.l.");*/
	doc->cells[last_row][4].text = malloc(5);
	sprintf(doc->cells[last_row][4].text, "%d", result.aplic.sl);
	for (i = last_row - 2; i <= last_row; ++ i) {
		spreadconv_set_cell_style(i, 3, ds->table_c[1], doc);
		spreadconv_set_cell_style(i, 4, ds->table_c[1], doc);
	}
	last_row++;

	spreadconv_set_cell_style(last_row, 3, ds->table_b[1], doc);
	spreadconv_set_cell_style(last_row, 4, ds->table_b[1], doc);

	doc->cells[last_row][2].text =	 strdup("As.");
	spreadconv_set_cell_style(last_row, 2, ds->table_h, doc);

	doc->cells[last_row][3].text = malloc(5);
	sprintf(doc->cells[last_row][3].text, "%d", result.course.as);
/*	doc->cells[last_row][4].text = strdup("As.");*/
	doc->cells[last_row][4].text = malloc(5);
	sprintf(doc->cells[last_row][4].text, "%d", result.aplic.as);
	last_row += 2;

	doc->cells[last_row][0].text = strdup("Intocmit,");
	doc->cells[last_row][3].text = strdup("Titular curs,");
	doc->cells[last_row][4].text = strdup("Sef catedra,");
	doc->cells[last_row][6].text = strdup("Decan,");
	last_row++;

	doc->cells[last_row][0].text =
		strdup(iniparser_getstr (ini, "antet:nume"));
	tmp_ini_val = iniparser_getstr(ini, "antet:titular");
	if (tmp_ini_val)
	doc->cells[last_row][3].text = strdup(tmp_ini_val);

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

/**
 * load and parse options
 * \remarks this is the most important function from libcspay
 * \param fname ini file name
 * \return 0 on succes
 * \return -1 on error
 */
static int
load_and_parse_options(char *fname)
{
	struct class_info *ci;
	size_t class_index;
	struct interval m;
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
			return -1;
		}
	}

	/* create new spreadsheet */
	doc = spreadconv_new_spreadconv_data("Date", 60, 9);

	/* configure spreadsheet column and cell styles */
	if (config_styles()){
		iniparser_freedict(ini);
		spreadconv_free_spreadconv_data(doc);
		return -1;
	}

	/* use ini file to create spreadsheet header */
	if (create_header()) {
		iniparser_freedict(ini);
		spreadconv_free_spreadconv_data(doc);
		return -1;
	}

	/**
	 * set month_start and month_end
	 * this is absolute
	 */
	m.start = mktime(month);
	++ month->tm_mon;
	if (month->tm_mon == 12){
		month->tm_mon = 0;
		++ month->tm_year;
	}
	m.end = mktime(month) - 1;
	
	-- month->tm_mon;
	if (month->tm_mon == -1) {
		month->tm_mon = 11;
		-- month->tm_year;
	}

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

		index = get_first_work_day(m, ci);
		
		Dprintf("First working day is: %d\n",
					localtime(&index)->tm_mday);
		
		/*
		 * iterate through the same day of different weeks
		 */

		tmp_sum_prof = tmp_sum_conf = tmp_sum_sl = tmp_sum_as = 0;
		tmp_sum = 0;
		#define TC	7	/**< table content start row*/
		for (/* no init */; index < m.end;
				index += ci->parity * WEEK) {
			if (!is_work(index))
				continue;
			/* NR. crt */
			ccs = ds->table_c[0];
			doc->cells[TC + table_crt][0].text = malloc(4);
			sprintf(doc->cells[TC + table_crt][0].text, "%d", (int) table_crt + 1);
			spreadconv_set_cell_style(TC + table_crt, 0, ccs, doc);

			/* Felul si nr. post*/
			ccs = ds->table_c[1];
			doc->cells[TC + table_crt][1].text = malloc(10);
			sprintf(doc->cells[TC + table_crt][1].text, "%s%s", roles[ci->role_type], ci->role_num);
			spreadconv_set_cell_style(TC + table_crt, 1, ccs, doc);


			/* faculty */
			doc->cells[TC + table_crt][2].text = strdup(ci->faculty);
			spreadconv_set_cell_style(TC + table_crt, 2, ccs, doc);

			/* course */
			doc->cells[TC + table_crt][3].text = strdup(ci->class);
			spreadconv_set_cell_style(TC + table_crt, 3, ccs, doc);

			if (ci->class_type == LCSP_CLASS_APP) {/* e aplicatie */
				doc->cells[TC + table_crt][5].value_type = strdup("float");
				doc->cells[TC + table_crt][5].text = malloc(4);
				sprintf(doc->cells[TC + table_crt][5].text, "%d", (int) (ci->timeline.end - ci->timeline.start));
			} else { /* e curs */
				doc->cells[TC + table_crt][4].value_type = strdup("float");
				doc->cells[TC + table_crt][4].text = malloc(4);
				sprintf(doc->cells[TC + table_crt][4].text, "%d", (int) (ci->timeline.end - ci->timeline.start));
			}
			spreadconv_set_cell_style(TC + table_crt, 4, ccs, doc);
			spreadconv_set_cell_style(TC + table_crt, 5, ccs, doc);

			ccs = ds->table_c[2];
			/* group */
			doc->cells[TC + table_crt][6].text = strdup(ci->group);
			spreadconv_set_cell_style(TC + table_crt, 6, ccs, doc);

			/* date */
			doc->cells[TC + table_crt][7].text = malloc(20);
			strftime(doc->cells[TC + table_crt][7].text, 19, "%d-%b", localtime(&index));
			spreadconv_set_cell_style(TC + table_crt, 7, ccs, doc);
				
			/* FIXME: no validation on class timeline */
			/* Time */
			tmp_str = malloc (10);
			sprintf (tmp_str, "%02d-%02d", (int) ci->timeline.start,
					(int) ci->timeline.end);
			doc->cells[TC + table_crt][8].text = tmp_str;
			spreadconv_set_cell_style(TC + table_crt, 8, ccs, doc);

			++ table_crt;
			tmp_sum +=  ci->timeline.end - ci->timeline.start;
		}

		Dprintf("End rule number: %d\n", class_index);
		/**
		 * \todo simplify this!
		 */
		if (ci->class_type == LCSP_CLASS_APP) {/* e aplicatie*/
			if (ci->role_type == LCSP_ROLE_ASS)
				result.aplic.as += tmp_sum;
			if (ci->role_type == LCSP_ROLE_CON)
				result.aplic.conf += tmp_sum;
			if (ci->role_type == LCSP_ROLE_SEL)
				result.aplic.sl += tmp_sum;
			if (ci->role_type == LCSP_ROLE_TEA)
				result.aplic.prof += tmp_sum;
		} else {	/* e curs */
			if (ci->role_type == LCSP_ROLE_ASS)
				result.course.as += tmp_sum;
			if (ci->role_type == LCSP_ROLE_CON)
				result.course.conf += tmp_sum;
			if (ci->role_type == LCSP_ROLE_SEL)
				result.course.sl += tmp_sum;
			if (ci->role_type == LCSP_ROLE_TEA)
				result.course.prof += tmp_sum;
		}
		++ class_index;
		free (ci);
	}
	Dprintf("End of all rules.\n");
	
	spreadconv_set_cell_style(TC + table_crt - 1, 0, ds->table_b[0], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 1, ds->table_b[1], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 2, ds->table_b[1], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 3, ds->table_b[1], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 4, ds->table_b[1], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 5, ds->table_b[1], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 6, ds->table_b[2], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 7, ds->table_b[2], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 8, ds->table_b[2], doc);
	
	/* create spreadsheet footer */
	create_footer (TC + table_crt);
	return 0;
}

/**
 * save a document in specified format
 * \param ft file type, see libspreadconv
 * \return NULL on error
 * \return outputed complete file path
 */
static char *
save_document(int ft)
{
	char *ret;

	Dprintf("Begin output file\n");
	
	spreadconv_dir_name = NULL;

	ret = spreadconv_create_spreadsheet(doc, ft);
	if (!ret)
		return  NULL;
	Dprintf("End output file\n");
	return ret;
}

/**
 * free ini dictionary
 * and spreadconv data
 */
static void
free_parsed_data()
{
	spreadconv_free_spreadconv_data(doc);
	iniparser_freedict(ini);
}

/**
 * test if \a t is a work day (hour)
 * \param t
 * \return 0, t is in vacation
 * \return 1, t is not in vacation
 */
int
is_work(time_t t)
{
	int i;
	for (i = 0; i < cfg->vac_no; ++ i){
		if ((cfg->vac[i]->start <= t) &&
			(t < cfg->vac[i]->end))
			return 0;
			
	}
	return 1;
}

/**
 * convert an options ini file, and a config structure
 * into desired files
 * \param config config structure, returned by \a cspay_get_config
 * \param fname ini file name
 * \return a file list
 */
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
	char *file_types;
	char *mv_comm;
	char *file_name;

	cfg = config;  
	ret = malloc(sizeof (struct cspay_file_list));
	ret->nr = 0;
	ret->names = malloc(24 * sizeof (char *));
	load_and_parse_options(fname);

	file_types = iniparser_getstr(ini, "antet:tip_fisier");
	mv_comm = calloc(1, 512);
	file_name = build_file_name();
	if (strstr(file_types, "ods")) {
		temp = save_document(LSC_FILE_ODS);
		if (!temp) {
			fprintf(stderr, "ods err\n");
		} else {
			snprintf(mv_comm, 512, "mv %s %s/%s.ods", temp, 
				spreadconv_dir_name, file_name);
			system(mv_comm);
			free(temp);
			ret->names[ret->nr] = calloc(1, 512);
			snprintf(ret->names[ret->nr ++], 512, "%s%s.ods",
				spreadconv_dir_name, file_name);
		}
	}
	
	if (strstr(file_types, "xls")) {

		temp = save_document(LSC_FILE_XLS);
		if (!temp) {
			fprintf(stderr, "xls err\n");
		} else {
			snprintf(mv_comm, 512, "mv %s %s/%s.xls", temp, 
				spreadconv_dir_name, file_name);
			system(mv_comm);
			free(temp);
			ret->names[ret->nr] = calloc(1, 512);
			snprintf(ret->names[ret->nr ++], 512, "%s%s.xls",
				spreadconv_dir_name, file_name);
		}
	}
	free_parsed_data();
	free(mv_comm);
	free(file_name);
	return ret;
}

/**
 * free config
 * \param cfg config to be freed
 */
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

/**
 * find the first work day
 * \param t interval in wich we search
 * \param ci class info
 * \return first absolute day of class 
 */
static time_t 
get_first_work_day(struct interval t, struct class_info *ci)
{
	struct tm *tmp_date;
	time_t ret;
	tmp_date = localtime(&t.start);
	tmp_date->tm_mday = 1;
	ret = mktime(tmp_date);
	while (ret < t.end) {
		if (localtime(&ret)->tm_wday == ci->day) {
			-- ci->first_week;
			if (is_work(ret)) {
				if (ci->first_week <= 0)
					break;
			}
		}
		ret += DAY;
	}
	if (ret >= t.end) {
		fprintf(stderr, "Impossible error\n");
		return -1;
	}
	return ret;
}

/**
 * build file name string
 * output file name should be:
 * 2007_11_PO_Intocmit
 * \return desired file name string
 */
static char *
build_file_name(void)
{
	char *ret, *name;
	char *disc;
	int i;
	ret = calloc(1, 512);
	disc = strdup(iniparser_getstr(ini, "antet:nume_curs"));
	for (i = 0; disc[i] != '\0'; ++ i)
		if (!isalpha(disc[i])) {
			disc[i] = '_';
		}
		
	name = strdup(iniparser_getstr(ini, "antet:nume"));
	for (i = 0; name[i] != '\0'; ++ i) {
		if (!isalpha(name[i])) {
			name[i] = '_';
		}
	}
	snprintf(ret, 512, "%d_%d_%s_%s", month->tm_year + 1901, month->tm_mon + 1,
			disc, name);
	return ret;
}
