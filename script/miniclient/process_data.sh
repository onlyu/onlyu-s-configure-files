do_fetch_all_data() {
    for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
    do
	date=`date -v-${i}d "+%Y%m%d"`
	if [ -f mini_stat_access.${date} ]
	then
	    echo "file mini_stat_access.${date} exist!"
	else
	    scp -P 32222 onlyu@183.60.41.251:/home/miniclient/backups/miniclient-www/miniclient-www-183.60.41.251/logs/mini_stat_access${date}.tar.gz /tmp/mini_stat_access${date}.tar.gz
	    tar -xf /tmp/mini_stat_access${date}.tar.gz
	    mv mini_stat_access.run mini_stat_access.${date}
	fi
    done
}

do_preprocess_all_data() {
    rm -rf /tmp/mini_stat_access.log.processed
    for i in 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
    do
	date=`date -v-${i}d "+%Y%m%d"`
	if [ -f mini_stat_access.${date} ]
	then
	    grep -v "183\.62\.45\.7[5-8]" mini_stat_access.${date} | sed 's/[-&?]/ /g' | sed 's/HTTP.*$//g' | awk '{printf "ip=%s&date=%s&time=%s&",$1,substr($2, 2, index($2,":")-2), substr($2, index($2,":")+1); for(i=6;i<=NF;i++) printf "%s&", $i; printf "\n";}' >> /tmp/mini_stat_access.log.processed
	fi
    done
}

do_fetch_data() {
    echo "getting the latest log data for 183.60.41.251"
    scp -P 32222 onlyu@183.60.41.251:/home/miniclient/logs/mini_stat_access.log /tmp/mini_stat_access.log
    grep -v "183\.62\.45\.7[5-8]" /tmp/mini_stat_access.log | sed 's/[-&?]/ /g' | sed 's/HTTP.*$//g' | awk '{printf "ip=%s&date=%s&time=%s&",$1,substr($2, 2, index($2,":")-2), substr($2, index($2,":")+1); for(i=6;i<=NF;i++) printf "%s&", $i; printf "\n";}' > /tmp/mini_stat_access.log.processed
    echo "fetch mini_stat_access.log success"
}

usage() {
    echo "$0 options"
    echo "--fetch_data\t\tfetch mini_stat_access.log from server"
    echo "--fetch_all_data"
    echo "--preprocess_all_data"
    echo "--process"
}

while [ -n "$1" ]
do
    case "$1" in
    --fetch_all_data)
	    do_fetch_all_data
	    ;;
    --fetch_data)
	    do_fetch_data
	    ;;
    --preprocess_all_data)
	    do_preprocess_all_data
	    ;;
    --process)
	    python process_data.py /tmp/mini_stat_access.log.processed
	    ;;
    --help)
	    usage
	    ;;
    esac
    shift
done

