#!/bin/bash

dt=$(date +%Y%m%d)

source  /home/egadmin/set_env
nohup python scripts/orders.py oob.$dt MSG_ORDER_EVENT TEST &

