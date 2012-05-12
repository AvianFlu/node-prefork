#include <unistd.h>
#include <v8.h>
#include <node.h>
#include "../deps/aeternum/aeternum.h"

using namespace v8;
using namespace node;

const char* ToCString(const String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

static Handle<Value> Prefork(const Arguments& args) {
  HandleScope scope;
  // Take the options object out of args[0]
  Local<Object> js_options = args[0]->ToObject();

  // Grab stdio filenames from the options object
  Local<Value> stdin_v = js_options->Get(String::NewSymbol("stdin"));
  Local<Value> stdout_v = js_options->Get(String::NewSymbol("stdout"));
  Local<Value> stderr_v = js_options->Get(String::NewSymbol("stderr"));

  // fork(2) and setsid()
  aeternum_fork();
  // let the event loop know it happened
  ev_default_fork();

  // tediously convert javascript strings to C strings, in three steps.
  // redirect stdio to the file provided, if provided.
  if (stdin_v->IsString()) {
    String::Utf8Value infile(stdin_v);
    aeternum_redirect(ToCString(infile), STDIN_FILENO);
  }
  else {
    aeternum_redirect(NULL, STDIN_FILENO);
  }

  if (stdout_v->IsString()) {
    String::Utf8Value outfile(stdout_v);
    aeternum_redirect(ToCString(outfile), STDOUT_FILENO);
  }
  else {
    aeternum_redirect(NULL, STDOUT_FILENO);
  }

  if (stderr_v->IsString()) {
    String::Utf8Value errfile(stderr_v);
    aeternum_redirect(ToCString(errfile), STDERR_FILENO);
  }
  else {
    aeternum_redirect(NULL, STDERR_FILENO);
  }

  // return null, since any error would have made things blow up by now.
  return scope.Close(Null());
}


extern "C" void init(Handle<Object> target) {
  HandleScope scope;

  NODE_SET_METHOD(target, "prefork", Prefork);
}
