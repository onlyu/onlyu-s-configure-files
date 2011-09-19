var http = require("http");
var fs = require("fs");
var path = require("path");
var ws = require("websocket-server");
var net = require("net");
var rsa = require("./rsa");
var z = require("./z");
var fs = require("./fs");

var spawn = require("child_process").spawn;
var iostat = spawn("iostat", ["-c", "2"]);

var verify_ip = "121.10.246.24";
var verify_port = 9970;
/*
var verify_ip = "192.168.0.8";
var verify_port = 9900;
*/

console.log("start");

rsa.rsaSetup();

var httpServer = http.createServer(function(req, res){
	if(req.method == "GET"){
		if( req.url.indexOf("favicon") > -1 ){
			res.writeHead(200, {'Content-Type': 'image/x-icon', 'Connection': 'close'});
			res.end("");
		} else {
			console.log(req.url);
			res.writeHead(200, {'Content-Type': 'text/html', 'Connection': 'close'});
			fs.createReadStream( path.normalize(path.join(__dirname, "iostat_client.html")), {
				'flags': 'r',
				'encoding': 'binary',
				'mode': 0666,
				'bufferSize': 4 * 1024
			}).addListener("data", function(chunk){
				res.write(chunk, 'binary');
			}).addListener("end",function() {
				res.end();
			});
		}
	}else{
		res.writeHead(404);
		res.end();
	}
});

// record all connection
var connectionTable = {
};

function get_challenger()
{
	var buff = new Buffer(8);
	for (var i=0; i<8; i++) {
		buff[i] = 132;
	}
	return buff;
}

function pak_data(data)
{
	// console.log("pak_data");
	// console.log(data);
	var pak = new Buffer(data.length + 4);
	//pak.copy(data, 4, 0, data.length);
	for (var i=0; i < data.length; i++) {
		pak[i+4] = data[i];
	}
	var length = data.length;
	pak[0] = length % 256;
	length /= 256;
	pak[1] = length % 256;
	length /= 256;
	pak[2] = length % 256;
	length /= 256;
	pak[3] = length;
	// console.log(pak);
	return pak;
}

function create_game_socket(port, ip, conn)
{
	var connect = new net.Socket();
	connect.state = "null";
	connect.web_socket = conn;
	connect.zreader = null;
	connect.connect(port, ip);

	connect.send_pkt = function(pkt){
		if (this.rc4_key != null) {
			this.write(pak_data(pkt));
		} else {
			pkt = fs.rc4(this.rc4_key, pkt);
			this.write(pak_data(pkt));
		}
	};
	connect.on("connect", function(){
		console.log("connected");
		connect.zreader = z.create();	
		connect.state = "sign";
	});
	connect.on("data", function(data){
		//console.log(data);
		var pkts = z.decompress(connect.zreader, data, function(pkt) {
			// console.log(pkt);
			// console.log(connect, pkt);
			if (connect.state == "sign") {
				console.log("S->C: challenger ");	
				var res = rsa.rsaPrivateEncrypt(pkt);
				connect.send_pkt(res);
				console.log("C->S: server challenger response");

				var buff = get_challenger();
				connect.send_pkt(res);
				console.log("C->S: send challenger to server ");
				connect.state = "verify";
			}else if(connect.state == "verify") {
				console.log("S->C: recv verify packet");
				console.log("server verify success");
				connect.state = "recv_key";
			} else if (connect.state == "recv_key"){
				console.log("S->C: recv rc4 key");
				var key = rsa.rsaPrivateDecrypt(pkt);
				if (key.length != 4) {
					throw "rc4 key error";
				}
				console.rc4_key = fs.rc4_key(key);
				console.log("key success");
				connect.state = "traffic";	
			} else if (connect.state == "traffic"){
				console.log("S->C: traffic", pkt);
			} else {
				console.log("unknow state", pkt);
			}
		})
	});
	connect.on("error", function(){
		if (connect.zreader != null) {
			z.free(connect.zreader);
			connect.zreader = null;
		}
		if (connect.rc4_key != null) {
			fs.rc4_free(connect.rc4_key);
			connect.rc4_key = null;
		}
		console.log("connect error");
	});
	connect.on('close', function(had_error){
		if (connect.zreader != null) {
			z.free(connect.zreader);
			connect.zreader = null;
		}
		if (connect.rc4_key != null) {
			fs.rc4_free(connect.rc4_key);
			connect.rc4_key = null;
		}
		console.log('connect closed, had_error=', had_error);
	});
	return connect;
}

var server = ws.createServer({debug:true, server:httpServer});
server.addListener("connection", function(conn) {
	console.log("stat %d", conn._state);
	iostat.stdout.on("data", function(data) {
		console.log(data);
		if(conn._stat == 4) {
			conn.send(data);
		}
	});

	// create connect to server
	if (conn._state == 4) {
		var connect = create_game_socket(verify_port, verify_ip, conn);
		connectionTable[conn] = connect;
	}
});

server.addListener("error", function(){
	console.log(Array.prototype.join.call(arguments, ","));
});

server.addListener("disconnected", function(conn){
	console.log("disconnected");
});

server.listen(8888)

