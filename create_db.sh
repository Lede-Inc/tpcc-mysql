#!/bin/bash
DBNAME=tpcc1000
DBNAME_HISTORY=$DBNAME"_history"
DBNAME_STOCK=$DBNAME"_stock"
mysqladmin create $DBNAME
mysqladmin create $DBNAME_HISTORY
mysqladmin create $DBNAME_STOCK
