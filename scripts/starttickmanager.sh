#!/bin/sh
source /home/quants/scripts/base.sh
mkdir -p /home/quants/userdata/tickmanager
touch /home/quants/userdata/tickmanager/runtime.log
nohup /home/quants/bin/tickmanager > /home/quants/userdata/tickmanager/runtime.log 2>&1 &
