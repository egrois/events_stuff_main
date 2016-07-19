#!/bin/bash

SYMBOL=$1
for year in 2011 2012 2013;
do
    FILES=$(ls /mnt/cme2/futures/$year/*/X*_MD_${SYMBOL}_*.gz)
    for file in $FILES;
    do
        echo ./extract_secdef.sh $file $SYMBOL
    done
done
