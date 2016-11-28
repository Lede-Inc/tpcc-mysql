#!/bin/bash
DBNAME=tpcc1000
DBNAME_HISTORY=$DBNAME"_history"
DBNAME_STOCK=$DBNAME"_stock"
CMD="mysqladmin -u root create"
echo $CMD $DBNAME
eval $CMD $DBNAME
echo $CMD $DBNAME_HISTORY
eval $CMD $DBNAME_HISTORY
echo $CMD $DBNAME_STOCK
eval $CMD $DBNAME_STOCK
echo Done
