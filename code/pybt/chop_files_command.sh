#!/bin/bash

files=$(ls /local/disk1/cme_derived/$1/$1_*_*.gz)

for file in $files;
do
    echo ./chop_file.sh $file
done
