#!/bin/bash

dt=$(date +%Y%m%d)

source  /home/egadmin/set_env
nohup python scripts/md_trader.py oob.$dt TEST $1 &

