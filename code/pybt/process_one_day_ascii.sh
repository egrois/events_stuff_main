#!/bin/bash

DATE=$2
YEAR=$(date --date=$DATE +'%Y')
MONTH=$(date --date=$DATE +'%m')
ROOT=$1
EXCH=$3

INPUT=/mnt/cme/futures/$YEAR/$MONTH/X${EXCH}_MD_${ROOT}_FUT_${DATE}.gz
OUTPUT=/local/disk1/cme_derived/$ROOT/$ROOT.$DATE.gz

cmd=$(echo zgrep -P "'107=$ROOT[[:print:]]{2}[^[:print:]]'" $INPUT \| gzip -9 \> $OUTPUT)
echo $cmd
