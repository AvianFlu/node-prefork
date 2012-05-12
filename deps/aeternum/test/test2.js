

var childProcess = require('child_process');

childProcess.execFile('./aeternum', ['node', 'test.js', 'hello'], function (err, stdout, stderr) {
  if (err) {
    return console.error(err.stack);
  }
  console.log(stdout);
  console.error(stderr);
});

