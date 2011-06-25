#include "debug.h"
#include <stdarg.h>

void debug(char* verbosity, char* module, char* msg, ...) {
    va_list ap;
    va_start(ap, msg);

    fprintf(stream_debug, "[%s] => %s:", module, verbosity);
    vfprintf(stream_debug, msg, ap);
    fprintf(stream_debug, "\n");

    va_end(ap);
}
