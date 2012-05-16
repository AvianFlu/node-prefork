
var prefork = require('../build/Release/prefork').prefork,
    fs = require('fs');

module.exports = function (options) {
  options = options || {};
  var infd = -1, outfd = -1, errfd = -1,
      customFds;

  if (options.stdin && typeof options.stdin === 'string') {
    infd = fs.openSync(options.stdin, 'r');
  }
  if (options.stdout && typeof options.stdout === 'string') {
    outfd = fs.openSync(options.stdout, 'a');
  }
  if (options.stderr && typeof options.stderr === 'string') {
    errfd = fs.openSync(options.stderr, 'a');
  }
  else if (outfd) {
    errfd = outfd;
  }
  if (Array.isArray(options.customFds)) {
    customFds = options.customFds;
  }
  else {
    customFds = [
      infd,
      outfd,
      errfd
    ];
  }
  prefork(customFds);
};
