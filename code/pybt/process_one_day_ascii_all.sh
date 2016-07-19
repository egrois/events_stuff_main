#!/bin/bash


#for symbol in ES NQ 6A 6B 6C 6E 6S 6J 6M GE; 
#do
#    ./process_one_day_ascii.sh $symbol $1 CME
#done

for symbol in CL NG;
do
    ./process_one_day_ascii.sh $symbol $1 NYM 
done

for symbol in ZN ZB ZF;
do
    ./process_one_day_ascii.sh $symbol $1 CBT 
done
