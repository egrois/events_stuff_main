#!/bin/bash
dt=$(date +%Y%m%d)
year=$(date +%Y)
month=$(date +%m)
scp -r pgrosul@grv-events-res02:/mnt/raw/cme/futures/$year/$month/$dt /home/egadmin/$year/$month/
