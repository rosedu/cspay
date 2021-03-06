/**
 * \file main.c
 * \brief main export file
 * \author Cojocar Lucian
 */
#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <popt.h>
#include <ctype.h>

#include "spreadconv.h"
#include "cspay.h"
#include "debug.h"

/**
 * extern variables
 */
char *Name;		/** for who we generate? */
char *FacultyName;	/** Name's faculty */
int Format;		/** zip or ods */
char *BaseCourse;	
int Month;		/** mask, 1 ->january, 3->jan & feb */
MYSQL *Conn;		/** MySQL db connection */

static int db_connect();
static int parse_cmd(int, char **);

int
main(int argc, char *argv[])
{
	struct cspay_file_list *ret;
	int i;

	ret = NULL;

	if (parse_cmd(argc, argv)) {
		return EXIT_FAILURE;
	}
	Dprintf("name:%s\n", Name);

	if (db_connect()) {
		printf("Error connecting to db!\n");
		goto ERR_CONNECT;
	}
	Dprintf("m-am conectat la baza de date!\n");

	ret = cspay_convert_options();	/**< in cspay.h */
	if (ret) {
		for (i = 0; i < ret->nr; ++ i)
			printf("%s\n", ret->names[i]);
		cspay_free_file_list(ret);
		ret = NULL;
	}
	return EXIT_SUCCESS;
	ERR_CONNECT:
	free(Name);
	free(FacultyName);
	free(BaseCourse);
	return EXIT_FAILURE;
}

/**
 * connect to database
 * \return 0 if ok
 * 	!= 0 else
 * \brief it sets Conn variable
 */
static int
db_connect()
{
	void *ret = NULL;

	Conn = mysql_init(NULL);
	ret = mysql_real_connect(
		Conn,
		"127.0.0.1", "cspay", "C2p4yp422",
		"cspay",
		0, NULL, 0);
	if (!ret) {
		printf("%s\n", mysql_error(Conn));
		mysql_close(Conn);
		return 1;
	}
	return 0;
}

/**
 * parse command line
 * \return 0 if ok
 * 	!0 else
 * \brief it sets Name, FacultyName, Format, BaseCourse and
 * Month mask from cmd
 */
static int
parse_cmd(int argc, char *argv[])
{
	char *tmp_format;
	char op;
	struct poptOption options_table[] = {
		{"nume", 'n', POPT_ARG_STRING, &Name, 0,
			"Numele celui caruia ii generam foile", "NAME"},
		{"facultate", 'f', POPT_ARG_STRING, &FacultyName, 0,
			"Facultatea la care preda respectivul", "FAC"},
		{"curs", 'c', POPT_ARG_STRING, &BaseCourse, 0,
			"Cursul de baza", "COURSE"},
		{"save", 's', POPT_ARG_STRING, &tmp_format, 0,
			"Formatul in care se scoate fisierul (ods|xls)", "FMT"},
		{"luna", 'l', POPT_ARG_INT, &Month, 0,
			"O masca ce desemneaza lunile pe care se generaza", "MASK"},
		POPT_AUTOHELP
		POPT_TABLEEND
	};
	poptContext opt_con;

	tmp_format = NULL;
	
	opt_con = poptGetContext("cspay", argc, (const char**) argv, options_table, 0);
	op = poptGetNextOpt(opt_con);
	if (op != -1 || !Month || !FacultyName || !tmp_format || !Name || !BaseCourse) {
		if (op != -1)
			fprintf(stderr, "%s\n", poptStrerror(op));
		poptPrintUsage(opt_con, stderr, 0);
		return 1;
	}

	Format = LSC_FILE_XLS;
	if (_tolower(tmp_format[0]) == 'o')
		Format = LSC_FILE_ODS;
	
	poptFreeContext(opt_con);
	free(tmp_format);

	return 0;
}
