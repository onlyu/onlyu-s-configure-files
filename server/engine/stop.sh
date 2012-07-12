#!/bin/sh
echo "Stop engine ....."
kill -2 `cat engine.pid`
echo "Sleep....."
sleep 1
echo "Stop netd....."
kill -2 `cat netd.pid` 
echo "Sleep....."
sleep 1
echo "Stop dbd....."
kill -2 `cat dbd.pid` 


