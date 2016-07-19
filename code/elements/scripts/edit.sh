#!/bin/bash

source  /home/egadmin/set_env
export PYTHONPATH=$PYTHONPATH:.
nohup python ./scripts/param_edit.py  $1 &

