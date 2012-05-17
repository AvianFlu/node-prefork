# node-prefork

## Start a node.js process in the background

`node-prefork` seeks to provide an easy API for starting a node.js process in
the background, independent of any controlling terminal session.

## Limitations

This module will not currently work on Windows, at all - but it will, soon.
We're working on it.

This module is called `prefork` for a reason.  When using `fork(2)` on a unix
system, the current process is cloned, file descriptors and all - however, the
new child process is single-threaded.  If you open a file asynchronously in
node.js, it gets opened by a thread pool; if you fork after doing this, you
cannot rely upon that file descriptor to still exist, since the new forked
child is now single-threaded.

This module does not provide any sort of process-watching functionality.  While
it could certainly be used to build a process watcher, your application is on
its own after being started with `prefork()`.

This leaves us with a few simple rules:

- Use `prefork()` as close to the start of your program as is possible.
- If you need to perform I/O beforehand, do it synchronously.
- Nothing created on the thread pool before `prefork()` is called will reliably
  exist afterwards.
- Without a separate solution to restart your process when it crashes, there's
  no guarantee of uptime.  If your process is stable, it could run for months,
  but the first time it crashes, it's done.

If these rules are followed, everything should be nice and stable.

## Installation

Installation is easy via `npm`:

     npm install prefork

If you want the git repo:

     git clone https://github.com/AvianFlu/node-prefork
     cd node-prefork
     node-gyp configure build

If you do not have `node-gyp` installed, simply substitute `node-waf` in the
command above.

## Usage

Using `prefork` is easy - just require it, and call it before your main
application logic starts.

```js

var http = require('http'),
    prefork = require('prefork');

prefork();

http.createServer(function (req, res) {
  res.end('I am a backgrounded HTTP server.');
}).listen(8080);

```

### stdio redirection

`prefork` also allows for various configurations of redirected `stdio`.

To simply log all output to a file:

```js

prefork({
  stdout: 'output.log'
});

```

If only `stdout` is provided, `stderr` will be redirected to the same place.
To separate `stderr` from `stdout`, provide a `stderr` option as well.

```js

prefork({
  stdout: 'output.log',
  stderr: 'errors.log'
});

```

If a `stdin` option is provided, then the file is opened, read, and its data
becomes available as data events on the `process.stdin` stream.  This simple 
example would log the data from `inputfile` to the process' new `stdout`, which
in this case is redirected to `outputfile`.

```js

prefork({
  stdin: 'inputfile',
  stdout: 'outputfile'
});

process.stdin.resume();
process.stdin.on('data', function (data) {
  console.log(data.toString());
});

```

### custom fds

If you're feeling adventurous, an array of integer file descriptors can also be
passed to `prefork()`.  Note that this will override any filename options that
have been passed - note also that passing random integers that aren't real
file descriptors is not a good idea.

```js
var fd = require('fs').openSync('output.log', 'a');

prefork({
  customFds: [ -1, fd, -1 ]
});

```

If a value of `-1` is passed for any of the file descriptors, as in the example
above, that file descriptor will be nulled.  This example nulls `stdin` and
`stderr`, but redirects `stdout` to a file.


