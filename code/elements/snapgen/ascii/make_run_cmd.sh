#!/bin/bash

FILES=$(ls params/*)
for f in $FILES;
do
    echo ./snapgen $f
done
