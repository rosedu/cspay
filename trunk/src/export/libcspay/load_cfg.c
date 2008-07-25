/**
 * config load functions implementation
 * \ingroup libcspay
 * \author Cojocar Lucian
 * \file load_cfg.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "debug.h"
#include "cspay.h"
#include "load_cfg.h"
#include "main.h"
#include "data.h"

struct time_config *
read_time_config(void)
{
	/*TODO
	 * validarea fisierului
	 */
	struct time_config *ret;
	struct tm conv;
	int cit;
	/*aaaa.ll.zz*/
	char *sem_start = "2008-01-01";	/* XXX obtained from database */
	char *sem_end = "2008-06-01";

	ret = malloc(sizeof *ret);
	memset(ret, 0, sizeof *ret);
	ret->sem = malloc(sizeof *ret->sem);

	sscanf(sem_start, "%d-%d-%d", &conv.tm_year, &conv.tm_mon, &conv.tm_mday);
	/*fix data*/
	-- conv.tm_mon;	/* january is 0*/
	conv.tm_year -= 1900;	/* 1900 is 0*/
	ret->sem->start = mktime(&conv);

	sscanf(sem_end, "%d-%d-%d", &conv.tm_year, &conv.tm_mon, &conv.tm_mday);
	/*fix data*/
	-- conv.tm_mon;	/* january is 0*/
	conv.tm_year -= 1900;	/* 1900 is 0*/
	ret->sem->end = mktime(&conv);

	ret->vac_no = 0;
	Dprintf("sem interval: %d -- %d\n", ret->sem->start, ret->sem->end);
	return ret;
}

/**
 *
 * \param months array wich will be filled
 * \return number of monts from Months variable
 */
int
load_months(struct tm *months[])
{
	int ret, month_mask;
	int i;

	ret = 0;
	month_mask = Month;

	for (i = 0; month_mask; ++ i, month_mask >>= 1) {
		if (month_mask & 0x1) {
			months[ret] = calloc(1, sizeof (struct tm));
			months[ret]->tm_mon = i;
			months[ret]->tm_year = 0;
			++ ret;
		}
	}
	#ifdef __DEBUG__
	Dprintf("Am gasit %d luni: ", ret);
	for (i = 0; i < ret; ++ i)
		printf("%d ", months[i]->tm_mon);
	printf("\n");
	#endif /*__DEBUG__ */
	return ret;
}
/**
 * Read from a database row the info 
 * \param row database row
 * \return NULL if an error ocured, else
 * a class_info pointer
 */								
struct class_info *
read_class_info (MYSQL_ROW row)
{
	struct class_info *ret;
	int i;

	ret = malloc (sizeof (*ret));
	if (ret == NULL) {
		perror ("malloc");
		return NULL;
	}

	i = 0;
	/* get faculty name */
	ret->faculty = strdup(row[i++]);

	/* get course name */
	ret->class = strdup(row[i++]); /**< materie field */

	/* get role for that class */
	ret->role_type = row[i++][0] - '0' - 1; /**< LSCP_ROLE_TEA */
	if (ret->role_type < 0 || ret->role_type > 3) {
		fprintf(stderr, "Error reading \"grad\" variable.\n");
		goto READ_ERR;
	}

	/* get role number for class */
	ret->role_num = strdup(row[i++]); /**< post field */

	/* get class type (course/lab) */
	/**< tip_curs[1|2] field */
	/* possible values: 'l' 'c' */
	switch (tolower(row[i++][0])) {
		case 'l': ret->class_type = LCSP_CLASS_APP; break;
		case 'c': ret->class_type = LCSP_CLASS_CRS; break;
		default:
			fprintf(stderr, "Error reading \"tip_curs\" variable.\n");
			goto READ_ERR;
	}

	/* get group for that class */
	ret->group = strdup(row[i++]); /**< an_grupa field */

	/* get class day */
	Dprintf("class_day:%s\n", row[i]);
	switch (tolower(row[i][0])) {
		case 'l': ret->day = 1; break;
		case 'm': ret->day = (row[i][1] == 'a') ? 2 : 3; break;
		case 'j': ret->day = 4; break;
		case 'v': ret->day = 5; break;
		default:
			fprintf(stderr, "Error reading \"zi\" variable.\n");
			goto READ_ERR;
	}
	++ i;

	/* get class timeline */
	/**< ora field */
	if (2 != sscanf(row[i++], "%d-%d", (int *)&ret->timeline.start, (int *) &ret->timeline.end)) {
		fprintf(stderr, "Error *parsing* \"ore\" variable.\n");
		goto READ_ERR;
	}

	/* get class parity */
	ret->parity = atoi(row[i++]); /**< paritate field */

	/* first week */
	ret->first_week = atoi(row[i++]); /**< paritate_start field */
	return ret;

	READ_ERR:
	free(ret);
	return NULL;
}


