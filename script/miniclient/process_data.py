import sys
import os
import types

def usage():
    print "%s filename"%(sys.argv[0])
    sys.exit(-1)

def statistics(data, guid, action, desc):
    cache_guid = {}
    client_install = {}
    
    for record in data:
        if not record.has_key("id"):
            continue
        if not type(action) is types.ListType:
            action = [action]
        if not record.has_key("action") or not record["action"] in action:
                continue
        date = record["date"]
        if guid == "all":
            if cache_guid.has_key(record["id"]):
                continue
            cache_guid[record["id"]] = True
        elif guid == "day":
            if not cache_guid.has_key(date):
                cache_guid[date] = {}
            if cache_guid[date].has_key(record["id"]):
                continue
            cache_guid[date][record["id"]] = True
        else:
            pass


        all_key = "    all    "

        version = record["version"]
        if version.find("1G") != -1:
            version = "1G"
        if version.find("40M") != -1:
            version = "40M"
        if not client_install.has_key(date):
            client_install[date] = {}
        if not client_install.has_key(all_key):
            client_install[all_key] = {}
        try:
            client_install[date][version] += 1
        except:
            client_install[date][version] = 1

        try:
            client_install[date]["total"] += 1
        except:
            client_install[date]["total"] = 1

        try:
            client_install[all_key][version] += 1
        except:
            client_install[all_key][version] = 1
            
    print ""
    print "                   %s                                  " %( desc)
    print "------------------------------------------------------------"
    print "|  date   |\t2M   |\t25M   |\t40m|100M   |1G	|unknown |total  |"
    print "------------------------------------------------------------"
    keys = client_install.keys()
    keys.sort()
    for key in keys:
        info = client_install[key]
        desc = key + "\t"
        '''
        info_keys = info.keys()
        info_keys.sort()
        for version in info.keys():
            desc += version + "=" + str(info.get(version)) + " "
        print desc
        '''
        print "%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d"%(key,
            info.get("2M") or 0,
            info.get("25M") or 0,
            info.get("40M") or 0,
            info.get("100M") or 0,
            info.get("1G") or 0,
            info.get("unknown") or 0,
            info.get("total") or 0)
    
if __name__ == '__main__':
    if len(sys.argv) < 2:
        usage()
    filename = sys.argv[1]
    f = open(filename)
    if not f:
        print "%s can not open file %s"%(sys.argv[0], filename)
        sys.exit(-1)

    data = []
    lines = [ line.split("&") for line in f.readlines() ]
    for arr in lines:
        record = {}
        last_key = ""
        for key_value in arr:
            key_value_arr = key_value.split("=")
            if len(key_value_arr) == 0 or key_value_arr[0] == "\n":
                continue
            elif len(key_value_arr) == 2:
                last_key = key_value_arr[0]
                record[last_key] = key_value_arr[1]
            else:
                record[last_key] += " " + key_value_arr[0]
        data.append(record)
    f.close()
    
    # client install statistics
    # -------------------------------------------------
    # |  date   |   2M   |  25M   | 100M   |   total  |
    # -------------------------------------------------
    # | .
    # |  .
    # |   .
    # -------------------------------------------------
    # |  all    | ...
    # -------------------------------------------------

    statistics(data, "all", "start_launcher", "install")
    statistics(data, "all", ["login", "new_uid"], "login (new client)")
    statistics(data, "day", ["login", "new_uid"], "login")
    statistics(data, "all",  "new_uid", "new uid (new client)")
    statistics(data, "day",  "new_uid", "new uid")
    statistics(data, "all",  "register_ums", "register ums (new client)")
    statistics(data, "day",  "register_ums", "register ums")

    # download speed
    '''
    speed_statistic = {}
    for record in data:
        if record.has_key("action") and record["action"] == "speed" and record.has_key("size") and int(record["size"]) < 300 * 1024 * 1024:
            speed = 0
            try:
                speed = int(record.get("speed") )
            except:
                speed = 0
            speed = (speed / 1024 / 20) * 20
            try:
                speed_statistic[speed] += 1
            except:
                speed_statistic[speed] = 1

    keys = speed_statistic.keys()
    keys.sort()
    for key in keys:
        print key, speed_statistic[key]
        '''
