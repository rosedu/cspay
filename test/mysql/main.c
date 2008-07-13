/*
 * mysql
 * gcc `mysql_config --cflags` `mysql_confing --libs` main.c -o m
 */
#include <mysql.h>
#include <stdio.h>

int
main(void)
{
	MYSQL *conn;
	void *ret;
	conn = mysql_init(NULL);
	ret = mysql_real_connect(
		conn,
		"localhost", "cspay", "cspay_passwd",
		"cspay",
		0, NULL, 0);
	if (!ret) {
		printf("%s\n", mysql_error(conn));
	}
	mysql_close(conn);
	return 0;
}
		
