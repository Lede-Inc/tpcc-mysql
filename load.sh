export LD_LIBRARY_PATH=/usr/local/mysql/lib/mysql/
DBNAME=$1
WH=$2
HOST=1.2.3.4
PORT=3360
STEP=100
USERNAME=_UR_USER_NAME_
PASS=_UR_PASS_WORD_

./tpcc_load -h $HOST -P ${PORT} -d $DBNAME -u ${USERNAME} -p${PASS} -w $WH -l 1 -m 1 -n $WH >> 1.out &

x=1

while [ $x -le $WH ]
do
 echo $x $(( $x + $STEP - 1 ))
if [ $WH > $(( $x + $STEP - 1 )) ]; then
./tpcc_load -h $HOST -P ${PORT} -d $DBNAME -u ${USERNAME} -p${PASS} -w $WH -l 2 -m $x -n $(( $x + $STEP - 1 ))  >> 2_$x.out &
./tpcc_load -h $HOST -P ${PORT} -d $DBNAME -u ${USERNAME} -p${PASS} -w $WH -l 3 -m $x -n $(( $x + $STEP - 1 ))  >> 3_$x.out &
./tpcc_load -h $HOST -P ${PORT} -d $DBNAME -u ${USERNAME} -p${PASS} -w $WH -l 4 -m $x -n $(( $x + $STEP - 1 ))  >> 4_$x.out &
else
./tpcc_load -h $HOST -P ${PORT} -d $DBNAME -u ${USERNAME} -p${PASS} -w $WH -l 2 -m $x -n $WH  >> 2_$x.out &
./tpcc_load -h $HOST -P ${PORT} -d $DBNAME -u ${USERNAME} -p${PASS} -w $WH -l 3 -m $x -n $WH  >> 3_$x.out &
./tpcc_load -h $HOST -P ${PORT} -d $DBNAME -u ${USERNAME} -p${PASS} -w $WH -l 4 -m $x -n $WH  >> 4_$x.out &
fi
 x=$(( $x + $STEP ))
done

