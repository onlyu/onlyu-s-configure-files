
function packet(pid)
{
	this.arr = new Array();
	this.length = 0;
	this.pid = pid;
	this.push = function(val) {
		this.arr[ this.arr.length ] = val;			
		if (typeof val == "number") {
			this.length += 4;
		} else if (typeof val == "string") {
			this.length += 4;
			this.length += val.length;
		}
	}

	function push_int(buff, ptr, val) {
		buff[ptr ++] = val % 256;
		val /= 256;
		buff[ptr ++] = val % 256;
		val /= 256;
		buff[ptr ++] = val % 256;
		val /= 256;
		buff[ptr ++] = val % 256; 
		return 4;
	}
	
	function push_string(buff, ptr, str) {
		var len = push_int(buff, ptr, str.length);
		ptr += len;
		buff.write(str, ptr, "binary");	
		return len + str.length;
	}
	function push_buffer(buff, ptr, buff2) {
		var len = push_int(buff, ptr, buff2.length);
		
		ptr += len;	
		for (var i = 0; i < buff2.length; i++) {
			buff[i + ptr] = buff2[i];	
		}
		return len + buff2.length; 
	}

	this.data = function(val) {
		var buff = new Buffer(this.length + 4);	
		var ptr = 0;
		ptr += push_int(buff, ptr, this.pid);
		for (var i = 0; i < this.arr.length; i++) {
			if (typeof this.arr[i] == "number") {
				ptr += 	push_int(buff, ptr, this.arr[i]);
			} else if (typeof this.arr[i] == "string") {
				ptr += push_string(buff, ptr, this.arr[i]);
			} else if (typeof this.arr[i] == "Buffer") {	
				ptr += push_buffer(buff, ptr, this.arr[i]);
			}
		}
		return buff;
	}
}

exports.packet = packet;

