#!/bin/bash

strategy=$1

to_kill=$(ps -ef | grep $strategy | grep -v grep | awk '{print $2}')
for d in $to_kill; do kill -9 $d; done
