

var http = require('http');

http.createServer(function (req, res) {
  res.end(JSON.stringify(process.argv));
}).listen(8080);
