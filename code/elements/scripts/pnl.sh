#!/bin/bash

dt=$(date +%Y%m%d)

source  /home/egadmin/set_env
nohup python scripts/status.py oob.$dt MSG_PNL_STATUS TEST &

