
var prefork = require('../lib/prefork');
var http = require('http');

prefork({
  stdout: 'out.log',
  stderr: 'err.log'
});

http.createServer(function (req, res) {
  console.log('stdout still running.');
  console.error('stderr still running.');
  res.end('I am still running');
}).listen(9090);
