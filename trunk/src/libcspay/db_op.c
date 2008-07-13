#include <mysql.h>

#include "db_op.h"
#include "debug.h"

MYSQL *
db_connect()
{
	MYSQL *conn;
	void *ret = NULL;

	conn = mysql_init(NULL);
	ret = mysql_real_connect(
		conn,
		"localhost", "cspay", "cspay_passwd",
		"cspay",
		0, NULL, 0);
	if (!ret) {
		Dprintf("%s\n", mysql_error(conn));
		mysql_close(conn);
		return NULL;
	}
	return conn;
}


