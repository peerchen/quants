#!/bin/sh
source /home/quants/scripts/base.sh
mkdir -p /home/quants/userdata/kbarmanager
touch /home/quants/userdata/kbarmanager/runtime.log
nohup /home/quants/bin/kbarmanager > /home/quants/userdata/kbarmanager/runtime.log 2>&1 &
