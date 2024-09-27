#!/usr/bin/bash

PARAMS_FILE=params.csv
RESULTS_FILE=results.csv
TEMP_FILE=temp.txt

F_LINES="$(awk -F ',' '{print $1}' $PARAMS_FILE)"
S_LINES="$(awk -F ',' '{print $2}' $PARAMS_FILE)"
N="$(wc -l $PARAMS_FILE | awk '{print $1}')"

if [ ! -f $RESULTS_FILE ]; then
    echo Creating results file
    echo f,s,time > $RESULTS_FILE
fi

while read line; do
    F="$(echo $line | awk -F ',' '{print $1}')"
    S="$(echo $line | awk -F ',' '{print $2}')"
    echo "Running: ./PA02fib -F $F -S $S"
    { time ./PA02fib -F $F -S $S 2> /dev/null > /dev/null ; } 2> $TEMP_FILE
    TIME="$(grep "real" $TEMP_FILE | awk '{print $2}' | sed 's/[m|s]/ /g')"
    MINS="$(echo $TIME  | awk '{print $1}')"
    SECS="$(echo $TIME  | awk '{print $2}')"
    TOTAL_SECS="$(echo "$MINS*60 + $SECS" | bc)"
    echo "Time: $TOTAL_SECS"
    echo $F,$S,$TOTAL_SECS >> $RESULTS_FILE
done < $PARAMS_FILE

echo Deleting temporary file
rm $TEMP_FILE
