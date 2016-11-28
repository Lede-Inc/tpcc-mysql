#!/bin/bash
DBNAME=tpcc1000
DBNAME_HISTORY=$DBNAME"_history"
DBNAME_STOCK=$DBNAME"_stock"

mysql $DBNAME < create_table_main.sql
mysql $DBNAME_HISTORY < create_table_history.sql
mysql $DBNAME_STOCK < create_table_stock.sql
