#!/bin/sh
starttime=`date +%Y-%m-%d_%H:%M:%S`
export MALLOC_CHECK_=0
export releasepath=/home/quants/bin
export LD_LIBRARY_PATH=/home/quants/lib:$LD_LIBRARY_PATH 
cd ${releasepath}
