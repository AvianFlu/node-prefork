#include <unistd.h>
#include <v8.h>
#include <node.h>
#include <assert.h>
#include <errno.h>
#include "../deps/aeternum/aeternum.h"

using namespace v8;
using namespace node;

static Handle<Value> Prefork(const Arguments& args) {
  HandleScope scope;
  
  int i, stdio_fds[3] = { -1, -1, -1 };

  if (!args[0].IsEmpty() && args[0]->IsArray()) {
    Local<Array> custom_fds = Local<Array>::Cast(args[0]);
    for(i = 0; i < 3; i++) {
      stdio_fds[i] = custom_fds->Get(i)->Int32Value();
    }
  }

  // fork(2) and setsid()
  if (aeternum_fork() < 0) {
    return ThrowException(ErrnoException(errno, "aeternum_fork()"));
  }
  // let the event loop know it happened
  ev_default_fork();

  if (aeternum_dup(stdio_fds[0], STDIN_FILENO) == -1) {
    return ThrowException(ErrnoException(errno, "aeternum_dup()"));
  }
  if (aeternum_dup(stdio_fds[1], STDOUT_FILENO) == -1) {
    return ThrowException(ErrnoException(errno, "aeternum_dup()"));
  }
  if (aeternum_dup(stdio_fds[2], STDERR_FILENO) == -1) {
    return ThrowException(ErrnoException(errno, "aeternum_dup()"));
  }

  // return the new pid of the child process.
  return scope.Close(Integer::New(getpid()));
}



extern "C" void init(Handle<Object> target) {
  HandleScope scope;

  NODE_SET_METHOD(target, "prefork", Prefork);
}
