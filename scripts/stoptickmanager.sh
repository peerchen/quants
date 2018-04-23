#!/bin/sh
#ps -ef| grep tickmanager | awk '{print $2}' | xargs kill -9
killall tickmanager
