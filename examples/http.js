
var prefork = require('../../build/Release/prefork').prefork;

prefork();

var http = require('http');

http.createServer(function (req, res) {
  res.end('I am still running');
}).listen(9090);
