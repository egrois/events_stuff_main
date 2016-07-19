#!/bin/bash
dt=$(date +%Y%m%d)
./scripts/stop.sh

cp $1.param $1.bak.param.$dt
cat $1.param | sed 's/trading_enabled\:.\+$/trading_enabled: false/g' > tmp
mv tmp $1.param 

nohup sudo cset proc -s egadmin --user=egadmin -e ./scripts/run.sh $1 >> log.$dt &
