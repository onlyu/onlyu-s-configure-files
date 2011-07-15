cat ping_1021.log | sed 's/.*fps=\(.*\),last_delay=\(.*\),average_delay=\(.*\).*/\1,\2,\3/g' | awk -F, '{fps_count[$1]++; last_delay[$2]++; average_delay[$3] ++;} END{ for(fps in fps_count) print fps,fps_count[fps]; for(delay in last_delay) print delay,last_delay[delay]; for(delay in average_delay) print delay,average_delay[delay]}'

