#! /bin/bash

ROOT=/local/disk1/data/snapshots 

for dir in 0 1 2;
do
    for symbol in ES NQ GE 6A 6B 6C 6E 6S 6J 6M CL NG ZN ZB ZF;
    do
        mv /local/disk1/staging/$symbol*\_$dir\_*.h5 $ROOT/$dir/$symbol
    done
done

