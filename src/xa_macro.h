#ifndef __TPCC_XA_MACRO_H__
#define __TPCC_XA_MACRO_H__

extern int * transaction_flag;
extern char * XA_FLAG;
#define START_TRANS(t_num, table, key) \
do {\
if (transaction_flag[t_num] != 1) {\
	if(strlen(table) > 0) {\
		mysql_query(ctx[t_num],"START TRANSACTION"); \
		mysql_trans_no_xa(ctx[t_num], table, key);\
	} else {\
		mysql_query(ctx[t_num],"START TRANSACTION"); \
	}\
	transaction_flag[t_num] = 1;\
} } while(0)

#define AFTER_TRANS(t_num) do { transaction_flag[t_num] = 0;} while(0)


#endif

