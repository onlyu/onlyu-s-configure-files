#!/bin/sh

nohup ./dbd -d > dbd.log 2>&1 & echo $! > dbd.pid
nohup ./netd -n > netd.log 2>&1  & echo $! > netd.pid
nohup ./engine  > gamed.log 2>&1 & echo $! > engine.pid
