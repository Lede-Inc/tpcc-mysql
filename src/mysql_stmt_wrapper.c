#include <mysql.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mysql_stmt_wrapper.h"


typedef struct _my_stmt_ {
    MYSQL_STMT stmt;
    MYSQL * mysql;
    MYSQL_BIND * bind_param;
    MYSQL_BIND * bind_result;
    char prep_sql[4000];
	char exec_sql[4000];
    MYSQL_RES * res;
	int num_fields;
} MY_MYSQL_STMT;

#define MY_PGET(a, b) (((MY_MYSQL_STMT *)(a))->b)

my_bool _my_mysql_stmt_bind_param(MYSQL_STMT *stmt, MYSQL_BIND *bind) {
	char * prep_sql = MY_PGET(stmt, prep_sql);
	char * exec_sql = MY_PGET(stmt, exec_sql);
	char buf[1024];

	MY_PGET(stmt, bind_param) = bind;

	unsigned int offset = 0;
	unsigned int tocopy = 0;
	int index = 0;
	char * p = NULL;
	char * exec_pos = exec_sql;
	if (!exec_sql[0]) {
		memset(exec_sql, 0, sizeof(MY_PGET(stmt, exec_sql)));
	}
	p = strchr(prep_sql+offset, '?');
	while (p) {
		tocopy = p - prep_sql - offset;

		strncpy(exec_pos, prep_sql + offset, tocopy);
		offset += tocopy;
		exec_pos += tocopy;

		memset(buf, 0, sizeof(buf));
		switch(bind[index].buffer_type) {
			case MYSQL_TYPE_FLOAT:
				sprintf(buf, "%f", *(float *)bind[index].buffer);
				break;
			case MYSQL_TYPE_STRING:
				sprintf(buf, "'%s'", bind[index].buffer);
				break;
			case MYSQL_TYPE_LONG:
				if (bind[index].is_unsigned) {
					sprintf(buf, "%ud", *(unsigned int*)bind[index].buffer);
				} else {
					sprintf(buf, "%d", *(int*)bind[index].buffer);
				}
				break;
			default:
				abort();
		}

		strcpy(exec_pos, buf);
		exec_pos += strlen(buf);
		offset ++;
		index ++;

		p = strchr(prep_sql+offset, '?');
	}
	strcpy(exec_pos, prep_sql+offset);
	
	fprintf(stderr, "Prep_SQL:%s, Exec_SQL:%s @ %s", prep_sql, exec_sql, __func__);

	return 0;
};

my_bool _my_mysql_stmt_bind_result(MYSQL_STMT *stmt, MYSQL_BIND *bind) {
	MY_PGET(stmt, bind_result) = bind;
	return 0;
};

my_bool _my_mysql_stmt_close(MYSQL_STMT * stmt) {
	if (MY_PGET(stmt, res)) {
		free(MY_PGET(stmt, res));
		MY_PGET(stmt, res) = NULL;
	}
	free(stmt);
	return 0;
}

unsigned int _my_mysql_stmt_errno(MYSQL_STMT * stmt) {
	return mysql_errno(MY_PGET(stmt, mysql));
};

const char * _my_mysql_stmt_error(MYSQL_STMT * stmt) {
	return mysql_error(MY_PGET(stmt, mysql));
};

int _my_mysql_stmt_execute(MYSQL_STMT * stmt) {
	char * SQL = MY_PGET(stmt, exec_sql);
	return mysql_query(MY_PGET(stmt, mysql), SQL);
};

int _my_mysql_stmt_fetch(MYSQL_STMT * stmt) {
	if (!MY_PGET(stmt, num_fields)) {
		MY_PGET(stmt, num_fields) = mysql_num_fields(MY_PGET(stmt, res));
	}
	MYSQL_ROW row = mysql_fetch_row(MY_PGET(stmt, res));
	if (!row) {
		if (mysql_errno(MY_PGET(stmt, mysql))) {
			return 1;
		} else {
			return MYSQL_NO_DATA;
		}
	}

	MYSQL_BIND * bind_result = MY_PGET(stmt, bind_result);

	int i = 0;
	for (; i < MY_PGET(stmt, num_fields); i ++) {
		switch (bind_result[i].buffer_type) {
			case MYSQL_TYPE_FLOAT:
				if (row[i]) {
					*(float *)(bind_result[i].buffer) = (float) atof(row[i]);
				} else {
					bind_result[i].buffer = NULL;
				}
				break;
			case MYSQL_TYPE_LONG:
				//code don't use is_unsigned. so we don't consider it  in our convert.
				if (row[0]) {
					*(int*)(bind_result[i].buffer) = (int) atol(row[i]);
				} else {
					bind_result[i].buffer = NULL;
				}
				break;
			case MYSQL_TYPE_STRING:
				if (row[i]) {
					 strncpy(bind_result[i].buffer, row[i], bind_result[i].buffer_length);
				} else {
					bind_result[i].buffer = NULL;
				}
				break;
			default:
				abort();
		}	
	}
	return 0;
};

my_bool _my_mysql_stmt_free_result(MYSQL_STMT * stmt) {
	mysql_free_result(MY_PGET(stmt, res));
	MY_PGET(stmt, res) = NULL;
	return 0;
};

MYSQL_STMT * _my_mysql_stmt_init(MYSQL * mysql) {
    MY_MYSQL_STMT * stmt = calloc(sizeof(MY_MYSQL_STMT), 1); 
    stmt->mysql = mysql;
	
    return (MYSQL_STMT *) stmt;
}

int _my_mysql_stmt_prepare(MYSQL_STMT * stmt, const char * stmt_str, unsigned long length) {
	strncpy(MY_PGET(stmt, prep_sql), stmt_str, length);
 	
/* num_fields is used to store datas.
	int n = 0;
	char * p = MY_PGET(stmt, prep_sql);
	while(*p) {
		if (*p == '?') {
			n ++;
		}
		p ++;
	}
	MY_PGET(stmt, num_fields) = n;
*/
	return 0;
};

const char * _my_mysql_stmt_sqlstate(MYSQL_STMT * stmt) {
	return mysql_sqlstate(MY_PGET(stmt, mysql));
};

int _my_mysql_stmt_store_result(MYSQL_STMT * stmt) {
	MY_PGET(stmt, res) = mysql_store_result(MY_PGET(stmt, mysql));
	if (MY_PGET(stmt, res)) {
		return 0; 
	} else {
		return 1;
	}
};
