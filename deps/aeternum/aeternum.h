#ifdef __cplusplus
extern "C" {
#endif

#ifndef _aeternum_h
#define _aeternum_h

#include "options.h"

extern options_t aeternum_options(int argc, char *argv[]);

extern void aeternum_start(options_t opts);

extern void aeternum_fork();

extern void aeternum_redirect(const char *filename, int fileno);

extern void aeternum_exec(const char *filename, char *args[]);

#endif

#ifdef __cplusplus
}
#endif
