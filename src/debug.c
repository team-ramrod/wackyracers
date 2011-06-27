#include "debug.h"
#include "uart_common.h"
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <util/atomic.h>

#ifndef DEBUG_ONLY
#define CHOSEN(x) true
#else
#define CHOSEN(x) !strcmp(module, DEBUG_ONLY)
#endif

void debug(char* verbosity, char* module, char* msg, ...) {
    va_list ap;
    va_start(ap, msg);

    if (CHOSEN(module)) {
        ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) {
            fprintf(stream_debug, "[%s] => %s:", module, verbosity);
            vfprintf(stream_debug, msg, ap);
            fprintf(stream_debug, "\n");
        }
    }

    va_end(ap);
}
