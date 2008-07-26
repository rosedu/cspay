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
#include "sheet.h"
#include "spreadconv.h"
#include "header_footer.h"
#include "main.h"
#include "data.h"


/** output document */
static struct spreadconv_data *doc;

/** config from xml file*/
static struct time_config *cfg;

/** results*/
static struct total_hours result;

/** month for wich we generate the output
 * this also contains the year,
 * day is, initially set to 1
 *
 * XXX extern :-(
 */
struct tm *curr_month;

static time_t get_first_work_day(struct interval t, struct class_info *ci);
static int is_work(time_t t);
static char *build_file_name(void);
static void free_class_info(struct class_info *ci);


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
 * load and parse options
 * \remarks this is the most important function from libcspay
 * \return 0 on succes
 * \return -1 on error
 */
static int
load_and_parse_options()
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

	Dprintf("Processing options\n");




	/* create new spreadsheet */
	doc = spreadconv_new_spreadconv_data("Date", 100, 9);

	/* configure spreadsheet column and cell styles */
	if (config_styles(doc)){
		spreadconv_free_spreadconv_data(doc);
		return -1;
	}

	/* use ini file to create spreadsheet header */
	if (create_header()) {
		spreadconv_free_spreadconv_data(doc);
		return -1;
	}

	int year_offset;
	year_offset = 0;
	/* FIXME Ugly hack
	 */
	switch (curr_month->tm_mon) {
		case 0:
		case 1:
			year_offset = 1;
		default:
			year_offset = 0;
	}
	Dprintf("year offset: %d\n", year_offset);
	curr_month->tm_year += year_offset;

	/**
	 * set month_start and month_end
	 * this is absolute
	 */
	m.start = mktime(curr_month);
	++ curr_month->tm_mon;
	if (curr_month->tm_mon == 12){
		curr_month->tm_mon = 0;
		++ curr_month->tm_year;
	}
	m.end = mktime(curr_month) - 1;
	
	-- curr_month->tm_mon;
	if (curr_month->tm_mon == -1) {
		curr_month->tm_mon = 11;
		-- curr_month->tm_year;
	}
	Dprintf("Caut intre %d:%d:%d si %d:%d:%d\n",
		localtime(&m.start)->tm_mday, localtime(&m.start)->tm_mon, localtime(&m.start)->tm_year,
		localtime(&m.end)->tm_mday, localtime(&m.end)->tm_mon, localtime(&m.end)->tm_year);
	/*
	 * browse the classes and add data to spreadsheet
	 * FIXME str* hazards
	 */

	class_index = 1;
	table_crt = 0;

	memset(&result, 0, sizeof (result));

	/*
	 * se face o interogare si se afla cate tipuri de ore tine respectiva persoana
	 */
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	unsigned int row_n;
	char query[256];
	snprintf(query, 256, "SELECT\
		facultate,\
		materie,\
		grad,\
		post,\
		tip_curs2,\
		an_grupa,\
		zi,\
		ora,\
		paritate,\
		paritate_start\
		FROM `orar` WHERE `acoperit_efect`=\'%s\'", Name);
	/* XXX Name should be sanitized! */
	if (mysql_query(Conn, query)) {
		printf("Eroare la interogare: %s\n", mysql_error(Conn));
		return 1;
	} else {
		Dprintf("Interogare cu succes\n");
		Dprintf("%s\n", query);
	}
	res = mysql_use_result(Conn);
	
	//row_n = mysql_num_rows(res);
	//Dprintf("Interogarea a intors %d linii\n", row_n);

	row_n = 0;
	while ((row = mysql_fetch_row(res)) != NULL) {
		Dprintf("Begin row number: %d\n", row_n ++);
		/*
		 * se itereaza prin aceste ore (linii)
		 */
		
		ci = read_class_info(row);
		/*
		if (ci == NULL)
			break;
		*/

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
		free_class_info(ci);
	}
	Dprintf("End of all rules.\n");

	Dprintf("Begin set styles\n");
	spreadconv_set_cell_style(TC + table_crt - 1, 0, ds->table_b[0], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 1, ds->table_b[1], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 2, ds->table_b[1], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 3, ds->table_b[1], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 4, ds->table_b[1], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 5, ds->table_b[1], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 6, ds->table_b[2], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 7, ds->table_b[2], doc);
	spreadconv_set_cell_style(TC + table_crt - 1, 8, ds->table_b[2], doc);
	Dprintf("End set styles\n");
	
	/* create spreadsheet footer */
	create_footer (TC + table_crt, &result);
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
	free(ds);
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
	if ((cfg->sem->start > t) || (cfg->sem->end <= t))
		return 0;
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
 * iterate through months
 * \param config config structure, returned by \a cspay_get_config
 * \param fname ini file name
 * \return a file list
 */
struct cspay_file_list *
cspay_convert_options()
{
	struct cspay_file_list *ret;
	char *temp;
	char mv_comm[512];
	char *file_name;
	

	int i;
	int n_months;
	struct tm *months[12];
	static char *extension[LSC_FILE_XLS | LSC_FILE_ODS];

	ret = malloc(sizeof (struct cspay_file_list));
	ret->nr = 0;
	ret->names = malloc(24 * sizeof (char *));

	cfg = read_time_config();

	n_months = load_months(months);
	extension[LSC_FILE_XLS] = ".xls";
	extension[LSC_FILE_ODS] = ".ods";


	for (i = 0; i < n_months; ++ i) {
		curr_month = months[i];
		curr_month->tm_year = localtime(&cfg->sem->start)->tm_year;
		curr_month->tm_mday = 1;
		
		Dprintf("I have calculated the year from semester's limits\n");
		Dprintf("Year: %u\n", curr_month->tm_year);

		load_and_parse_options();
		file_name = build_file_name();
		/* free(month);*/
		temp = save_document(Format);
		if (!temp) {
			fprintf(stderr, "%d err\n", Format);
		} else {
			snprintf(mv_comm, 512, "mv %s %s/%s%s", temp, 
				spreadconv_dir_name, file_name, extension[Format]);
			system(mv_comm);
			free(temp);
			ret->names[ret->nr] = calloc(1, 512);
			snprintf(ret->names[ret->nr ++], 512, "%s%s%s",
				spreadconv_dir_name, file_name, extension[Format]);
		}
		free(curr_month);
		free(file_name);
		spreadconv_free_spreadconv_data(doc);
	}
	free_parsed_data();
	if (!ret->nr) {
		free(ret);
		return NULL;
	}
	return ret;
}

/**
 * free config
 * \param cfg config to be freed
 */
void 
cspay_free_config(struct time_config *cfg)
{
	int i;
	free(cfg->sem);
	for (i = 0; i < cfg->vac_no; ++ i)
		free(cfg->vac[i]);
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
		return ret;
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
	disc = strdup(BaseCourse);
	for (i = 0; disc[i] != '\0'; ++ i)
		if (!isalpha(disc[i])) {
			disc[i] = '_';
		}
		
	name = strdup(Name);
	for (i = 0; name[i] != '\0'; ++ i) {
		if (!isalpha(name[i])) {
			name[i] = '_';
		}
	}
	snprintf(ret, 512, "%d_%02d_%s_%s", curr_month->tm_year + 1900, curr_month->tm_mon + 1,
			disc, name);
	free(disc);
	free(name);
	return ret;
}
static void
free_class_info(struct class_info *ci)
{
	if (!ci)
		return;
	free(ci);
}
