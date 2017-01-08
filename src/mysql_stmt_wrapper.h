#ifndef __mysql_stmt_wrapper__H_
#define __mysql_stmt_wrapper__H_

#define MYSQL_WRAPPER

#include <mysql.h>

#define SHARD 1

#ifdef SHARD
#define mysql_stmt_bind_param(...) _my_mysql_stmt_bind_param(__VA_ARGS__)
#define mysql_stmt_bind_result(...) _my_mysql_stmt_bind_result(__VA_ARGS__)
#define mysql_stmt_close(...) _my_mysql_stmt_close(__VA_ARGS__)
#define mysql_stmt_errno(...) _my_mysql_stmt_errno(__VA_ARGS__)
#define mysql_stmt_error(...) _my_mysql_stmt_error(__VA_ARGS__)
#define mysql_stmt_execute(...) _my_mysql_stmt_execute(__VA_ARGS__)
#define mysql_stmt_fetch(...) _my_mysql_stmt_fetch(__VA_ARGS__)
#define mysql_stmt_free_result(...) _my_mysql_stmt_free_result(__VA_ARGS__)
#define mysql_stmt_init(...) _my_mysql_stmt_init(__VA_ARGS__)
#define mysql_stmt_prepare(...) _my_mysql_stmt_prepare(__VA_ARGS__)
#define mysql_stmt_sqlstate(...) _my_mysql_stmt_sqlstate(__VA_ARGS__)
#define mysql_stmt_store_result(...) _my_mysql_stmt_store_result(__VA_ARGS__)
#define mysql_trans_no_xa(...) _mysql_trans_no_xa(__VA_ARGS__)
#else
#define mysql_trans_no_xa(...)  ((int)0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

my_bool _my_mysql_stmt_bind_param(MYSQL_STMT *stmt, MYSQL_BIND *bind);
my_bool _my_mysql_stmt_bind_result(MYSQL_STMT *stmt, MYSQL_BIND *bind);
my_bool _my_mysql_stmt_close(MYSQL_STMT * stmt);
unsigned int _my_mysql_stmt_errno(MYSQL_STMT * stmt);
const char * _my_mysql_stmt_error(MYSQL_STMT * stmt);
int _my_mysql_stmt_execute(MYSQL_STMT * stmt);
int _my_mysql_stmt_fetch(MYSQL_STMT * stmt);
my_bool _my_mysql_stmt_free_result(MYSQL_STMT * stmt);
MYSQL_STMT * _my_mysql_stmt_init(MYSQL * mysql);
int _my_mysql_stmt_prepare(MYSQL_STMT * stmt, const char * stmt_str, unsigned long length);
const char * _my_mysql_stmt_sqlstate(MYSQL_STMT * stmt);
int _my_mysql_stmt_store_result(MYSQL_STMT * stmt);

int _mysql_trans_no_xa(MYSQL * mysql);

#ifdef __cplusplus
}
#endif

#endif
