#!/bin/bash

root=$1
files=$(ls /mnt/cme2/futures/*/*/*_${root}_*)

for file in $files;
do 
    outf=$(echo $file | awk -F "/" '{print $7}' | awk -F "_" '{print $3"_"$4"_"$5}')
    outf=/local/disk1/cme_derived/$root/$outf 
    echo zgrep -P \'107=$root[[:print:]]{2}[^[:print:]]\' $file \| gzip -9 \> $outf
done
