#!/bin/bash

ROOT=/local/disk1/staging

FILES=$(ls $ROOT/*.txt)

for file in $FILES;
do
    echo python convert_txt_to_h5.py $file
done
