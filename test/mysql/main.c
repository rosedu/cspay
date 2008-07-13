/*
 * mysql
 * gcc `mysql_config --cflags` `mysql_confing --libs` main.c -o m
 *
 * mysql> select * from users;
 * +----+------+--------+
 * | id | nume | parola |
 * +----+------+--------+
 * |  1 | nnn  | ppp    | 
 * |  2 | nnn1 | ppp1   | 
 * |  3 | nnn2 | ppp3   | 
 * +----+------+--------+
 * 3 rows in set (0.00 sec)
 *
 * mysql> quit
 *
 */
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

static MYSQL *conn = NULL;

static int
do_query(const char *q)
{	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	int i;
	unsigned int fields_n;

	if (mysql_query(conn, q)) {
		printf("%s\n", mysql_error(conn));
	} else {
		res = mysql_use_result(conn);
		fields_n = mysql_num_fields(res);
		while ((row = mysql_fetch_row(res))) {
			for (i = 0; i < fields_n; ++ i)
				printf("%s ", row[i]);	
			printf("\n");
		}
		mysql_free_result(res);
	}
	return 0;
}

int
main(void)
{
	void *ret = NULL;


	conn = mysql_init(NULL);
	ret = mysql_real_connect(
		conn,
		"localhost", "cspay", "cspay_passwd",
		"cspay",
		0, NULL, 0);

	if (!ret) {
		printf("%s\n", mysql_error(conn));
		return EXIT_FAILURE;
	}

	#define	Q1	"select * from users"
	do_query(Q1);

	#define	Q2	"select id from users"
	do_query(Q2);

	#define	Q3	"select * from users where id=2"
	do_query(Q3);

	mysql_close(conn);
	return 0;
}
		
