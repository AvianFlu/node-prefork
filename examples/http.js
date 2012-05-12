
var prefork = require('../lib/prefork');
var http = require('http');

prefork();

http.createServer(function (req, res) {
  res.end('I am still running');
}).listen(9090);
