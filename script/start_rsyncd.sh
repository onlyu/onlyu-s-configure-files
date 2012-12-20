killall rsync
sleep 1
rsync --config=script/rsyncd.conf --daemon
