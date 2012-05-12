

var http = require('http');

console.error(process.argv[2])

http.createServer(function(req, res) {
  console.log(req.socket.remoteAddress || req.connection.remoteAddress);
  res.end(JSON.stringify(req.headers, true, 2));
}).listen(8080);


