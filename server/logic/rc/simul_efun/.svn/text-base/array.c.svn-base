

mixed uniq_array( mixed arr ) { 
	mapping mpTmp ;

	foreach( mixed tmp in arr)
		mpTmp[tmp] = 1;

	return keys( mpTmp ); 
}

mixed shuffle( mixed arr ) {
    int i, j, sz;
    mixed tmp;

    if( !pointerp( arr ) )
        return ({ });

    if( ( sz = sizeof( arr ) ) < 2 )
        return arr;

    for( i = 0; i < sz; i++ ) {
       if ( i == ( j = random( i + 1 ) ) )
           continue;
       tmp = arr[ i ];
       arr[ i ] = arr[ j ];
       arr[ j ] = tmp;
    }

    return arr;
} 

int *find_members( mixed target, mixed arr ) 
{
	int i, *ret;    
	ret = ({ });    
	
	while( ( i = member_array( target, arr, i ) ) != -1 )
		ret += ({ i++ }); 
	
	return ret;
}

mixed array_del( mixed arr, int start, int len ) 
{    
	return arr[0..start-1] + arr[start+len..];
}

mixed array_insert( mixed arr, mixed el, int pos ) 
{
	if( stringp(arr) )
		return arr[0..pos-1] + el + arr[pos..];

	return arr[0..pos-1] + ({ el }) + arr[pos..];
} 

mixed rand_array(mixed arr) 
{
	return sizeof(arr)?arr[random(sizeof(arr))]:0;
}

// 从一个队列里面随机出几个元素出来
mixed rand_x_array(mixed arr, int x)
{
	int i;
	mixed *tmp = ({});
	if (x <= 0) return ({});
	if (sizeof(arr) <= x) return arr;
	i = 0;
	while (i < x) {
		mixed val = rand_array(arr);
		tmp += ({ val });
		arr -= ({ val });
		i++;
	}
	return tmp;
}


string serial_array(mixed arr, string split)
{
	int i = 0;
	string str = "";
	if (!arrayp(arr)) return "";
	while (i < sizeof(arr))  {
		str += (sizeof(str) ? split:"") + arr[i];
		i++;
	}
	return str;
}

string serial_mapping(mapping dict)
{
	string str = "";
	if (!mapp(dict) || !sizeof(dict)) return "";
	foreach (string _key, mixed _val in dict) {
		if (sizeof(str)) str += ",";
		if (stringp(_val)) {
			str += sprintf("%s:%s", ""+_key, _val);
		} else if (intp(_val)) {
			str += sprintf("%s:%d", ""+_key, _val);
		} else  if ( arrayp(_val) ) {
			str += sprintf("%s:({%s})", ""+_key, serial_array(_val, ","));
		}
		else if ( mapp(_val) ) {
			str += sprintf("%s:([%s])", ""+_key, serial_mapping(_val));
		}
		else {
			str += sprintf("%s:%O", ""+_key, _val);
		}
	}
	return str;
}

mapping make_map(mixed *key_arr, mixed *value_arr)
{
	int size = sizeof(key_arr) > sizeof(value_arr) ? sizeof(key_arr) : sizeof(value_arr);
	mapping result_map = ([]);
	for (int i=0; i<size; i++) {
		result_map[key_arr[i]] = result_map[value_arr[i]];
	}
	return result_map;
}
