#! /bin/bash

while true;
do
    sleep 10
    rsync  -avz -e ssh /local/disk1/staging/*  grv-events-res02:/local/disk1/staging/
done
