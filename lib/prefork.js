
var prefork = require('../build/Release/prefork').prefork,
    fs = require('fs');

module.exports = function (options) {
  options = options || {};
  var outfd, errfd;

  if (Array.isArray(options.customFds) && options.customFds.length < 3) {
    options.customFds.unshift(-1); 
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
  if (outfd && errfd) {
    options.customFds = [
      -1,
      outfd,
      errfd
    ];
  }
  prefork(options);
};
