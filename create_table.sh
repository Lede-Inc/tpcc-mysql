#!/bin/bash
DBNAME=tpcc1000
DBNAME_HISTORY=$DBNAME"_history"
DBNAME_STOCK=$DBNAME"_stock"
DBARGS="-u root"

mysql $DBARGS $DBNAME < create_table_main.sql
mysql $DBARGS $DBNAME_HISTORY < create_table_history.sql
mysql $DBARGS $DBNAME_STOCK < create_table_stock.sql

mysql $DBARGS $DBNAME < create_index_main.sql
mysql $DBARGS $DBNAME_STOCK < create_index_stock.sql

