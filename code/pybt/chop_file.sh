#!/bin/bash

/usr/bin/gunzip $1
fn=$(echo $1 | awk -F "." '{print $1}')
python chop_file.py $fn
/usr/bin/gzip $fn




