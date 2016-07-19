#!/bin/bash

for symbol in ES NQ GE 6A 6B 6C 6E 6S 6J 6M CL NG ZN ZF ZB;
do
    mkdir $symbol
    ./extract.sh $symbol > extract_$symbol.txt
done
