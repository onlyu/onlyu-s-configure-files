var http = require("http");
var fs = require("fs");
var path = require("path");
var ws = require("websocket-server");

var spawn = require("child_process").spawn;
var iostat = spawn("iostat", ["-c", "2"]);

console.log("start");

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

var server = ws.createServer({debug:true, server:httpServer});
server.addListener("connection", function(conn){
	console.log("stat %d", conn._state);
	iostat.stdout.on("data", function(data){
		console.log(data);
		if(conn._stat == 4){
			conn.send(data);
		}
	});
	/*
	if (conn._state == 4){
		console.log("set interval");
		setInterval(function(){
			console.log("send hello");
			conn.send("hello\n");
		}, 1000);
	}
	*/
});

server.addListener("error", function(){
	console.log(Array.prototype.join.call(arguments, ","));
});

server.addListener("disconnected", function(conn){
	console.log("disconnected");
});

server.listen(8888)

