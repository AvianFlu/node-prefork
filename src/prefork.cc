#include <v8.h>
#include <node.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include "../deps/aeternum/aeternum.h"

using namespace v8;
using namespace node;

static Handle<Value> Prefork(const Arguments& args) {
  HandleScope scope;

  aeternum_fork();
  ev_default_fork();
  aeternum_redirect("/dev/null", STDIN_FILENO);
  aeternum_redirect("/dev/null", STDOUT_FILENO);
  aeternum_redirect("/dev/null", STDERR_FILENO);

  return scope.Close(Null());
}


extern "C" void init(Handle<Object> target) {
  HandleScope scope;

  NODE_SET_METHOD(target, "prefork", Prefork);
}
