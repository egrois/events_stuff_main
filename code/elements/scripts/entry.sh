#!/bin/bash

dt=$(date +%Y%m%d)
source  /home/egadmin/set_env
export PYTHONPATH=$PYTHONPATH:.
nohup python ./scripts/entry.py order_entry log.$dt &

