#!/bin/sh	
CURDIR=`pwd`
TIMESTAMP=`date +%Y%m%d%H%M%S`
LOGDIR=/home/quants/userdata
LOGBACKUPDIR=/home/quants/backup
LOGFILENAME=quants_userdata_$TIMESTAMP.tar.gz

mkdir -p $LOGBACKUPDIR
cd $LOGDIR

tar -zcvf $LOGFILENAME *
mv $LOGFILENAME $LOGBACKUPDIR 
rm -rf $LOGDIR/*

cd $CURDIR
