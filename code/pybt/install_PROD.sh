#! /bin/bash

servers="grv-events-res02 grv-events-res01"

for server in $servers
do
    rsync -av -e ssh *.py  $server:/home/pgrosul/PROD/pybt
    rsync -av -e ssh *.sh  $server:/home/pgrosul/PROD/pybt
done
