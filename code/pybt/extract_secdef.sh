#!/bin/bash

SOURCE=$1
ROOT=$2
DATE=$(echo $SOURCE | awk -F "_" '{print $4 }')
OUTPUT=/local/disk1/cme_derived/$ROOT/$ROOT.secdef.$DATE

cmd=$(echo zgrep -P "'35=d'" $SOURCE \| grep -P "'107=$ROOT[[:print:]]{2}[^[:print:]]'" \> $OUTPUT ) 
eval $cmd
