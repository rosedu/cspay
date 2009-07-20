#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mysql.h>

#include "header_footer.h"
#include "sheet.h"
#include "debug.h"
#include "spreadconv.h"
#include "cspay.h"
#include "main.h"

/**
 * create header data for spreadsheet
 * ds is extern
 * \return 0, on succes
 * \return -1 on error
 * \remarks \a month is allocated here
 */
int
create_header (void)
{
	char *val = NULL;		
	struct spreadconv_data *doc;

	doc = ds->doc;

	Dprintf("I look for university name\n");
	val = "UPB";
	if (!val){
		fprintf(stderr, "Error obatining university name.\n");
		goto ERR_;
	}
	doc->cells[0][0].text = strdup(val);

	Dprintf("I look for faculty name\n");
	val = FacultyName;
	if (!val){
		fprintf(stderr, "Error obatining university name.\n");
		goto ERR_;
	}
	doc->cells[1][0].text = strdup(val);

	/* catedra */
	#define DEPT_LIMIT	512	/**< nr. max. de car. ptr catedra */
	#define STR_DEPT_SIZE	8	/**< strlen("Catedra ") */
	Dprintf("I look for department\n");
	val = "antet:catedra";
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

	/*
	Dprintf("I look for month\n");
	month = calloc(1, sizeof (struct tm));
	month->tm_mon = iniparser_getint(ini, "antet:luna", -1);
	if (month->tm_mon < 0) {
		fprintf(stderr, "Error obtaining month\n");
		goto ERR_;
	}
	*/
	doc->cells[1][6].text = strdup("Luna ");
	doc->cells[1][7].text = strdup(str_months[curr_month->tm_mon]);

	Dprintf("I wrote the month\n");


	/* some header data */
	doc->cells[3][2].text = strdup("Situatia orelor efectuate de");
	doc->cells[4][2].text = strdup("cu functia de baza la");
	val = Name;
	if (!val) {
		fprintf(stderr, "Error obtaining name.\n");
		goto ERR_;
	}
	doc->cells[3][5].text = strdup(val);
	val = BaseCourse;
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
int 
create_footer(int last_row, struct total_hours *result)
{
	
	Dprintf("Begin create footer\n");
	char tot[12];
	char *tmp_ini_val;
	struct spreadconv_data *doc;
	int i;
	doc = ds->doc;
	sprintf(tot, "%d", result->course.prof + result->aplic.prof +
			result->course.conf + result->aplic.conf +
			result->course.sl + result->aplic.sl +
			result->course.as + result->aplic.as);
	doc->cells[last_row][7].text = strdup("Total:");
	doc->cells[last_row][8].value_type = strdup("float");
	doc->cells[last_row][8].text= strdup(tot);
	last_row += 2;

	/* doc->cells[last_row][1].text = strdup("TOTAL ore:");
	 */
	Dprintf("Begin print small table\n");
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
	sprintf(doc->cells[last_row][3].text, "%d", result->course.prof);
	
/*	doc->cells[last_row][4].text = strdup("Prof."); */
	doc->cells[last_row][4].text = malloc(5);
	sprintf(doc->cells[last_row][4].text, "%d", result->aplic.prof);
	last_row++;

	doc->cells[last_row][2].text = strdup("Conf.");
	spreadconv_set_cell_style(last_row, 2, ds->table_h, doc);

	doc->cells[last_row][3].text = malloc(5);
	sprintf(doc->cells[last_row][3].text, "%d", result->course.conf);
/*	doc->cells[last_row][4].text = strdup("Conf.");*/
	doc->cells[last_row][4].text = malloc(5);
	sprintf(doc->cells[last_row][4].text, "%d", result->aplic.conf);
	last_row++;

	doc->cells[last_row][2].text = strdup("S.l.");
	spreadconv_set_cell_style(last_row, 2, ds->table_h, doc);

	doc->cells[last_row][3].text = malloc(5);
	sprintf(doc->cells[last_row][3].text, "%d", result->course.sl);
/*	doc->cells[last_row][4].text = strdup("S.l.");*/
	doc->cells[last_row][4].text = malloc(5);
	sprintf(doc->cells[last_row][4].text, "%d", result->aplic.sl);
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
	sprintf(doc->cells[last_row][3].text, "%d", result->course.as);
/*	doc->cells[last_row][4].text = strdup("As.");*/
	doc->cells[last_row][4].text = malloc(5);
	sprintf(doc->cells[last_row][4].text, "%d", result->aplic.as);
	last_row += 2;
	Dprintf("End print small table\n");
	
	Dprintf("Begin print Intocmit, Tc, Sef catedra\n");
	Dprintf("Printing on row : %d\n", last_row);
	doc->cells[last_row][0].text = strdup("Intocmit,");
	doc->cells[last_row][3].text = strdup("Titular curs,");
	doc->cells[last_row][4].text = strdup("Sef catedra,");
	doc->cells[last_row][6].text = strdup("Decan,");
	last_row++;
	Dprintf("End print Intocmit, Tc, Sef catedra\n");
	
	Dprintf("Begin search nume, titular\n");
	doc->cells[last_row][0].text =
		strdup(Name);
	tmp_ini_val = "antet:titular";	/* XXX SELECT */
	if (tmp_ini_val)
		doc->cells[last_row][3].text = strdup(tmp_ini_val);
	Dprintf("End search nume, titular\n");

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
	tmp_ini_val = "antet:decan";
	if (!tmp_ini_val) {
		fprintf(stderr, "Nu am gasit decanul.\n");
	} else {
		doc->cells[last_row][6].text = strdup(tmp_ini_val);
	}

	tmp_ini_val = "antet:sef_catedra";
	if (!tmp_ini_val) {
		fprintf(stderr, "Nu am gasit sefcatedra.\n");
	} else {
		doc->cells[last_row][4].text = strdup(tmp_ini_val);
	}
	Dprintf("End create footer\n");
	return 0;
}
