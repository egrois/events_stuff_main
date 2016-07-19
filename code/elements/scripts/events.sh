#!/bin/bash

dt=$(date +%Y%m%d)
python scripts/events_extract.py $dt
