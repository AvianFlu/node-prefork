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
  
  int stdio_fds[3] = { -1, -1, -1 };
  // Take the options object out of args[0]
  Local<Object> js_options = args[0]->ToObject();
  // Trying to ->Get() properties of empty objects will segfault.
  if (!js_options.IsEmpty()) {
    Local<Value> custom_fds_v = js_options->Get(String::NewSymbol("customFds"));
    if (!custom_fds_v.IsEmpty() && custom_fds_v->IsArray()) {
      Local<Array> custom_fds = Local<Array>::Cast(custom_fds_v);
      int i;
      for(i = 0; i < 3; i++) {
        stdio_fds[i] = custom_fds->Get(i)->Int32Value();
      }
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

  // return null, since any error would have made things blow up by now.
  return scope.Close(Null());
}



extern "C" void init(Handle<Object> target) {
  HandleScope scope;

  NODE_SET_METHOD(target, "prefork", Prefork);
}
