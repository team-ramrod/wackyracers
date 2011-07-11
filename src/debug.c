#include "debug.h"
#include "uart_common.h"
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <util/atomic.h>

#ifdef DEBUG_UART
#define STREAM_DEBUG DEBUG_UART
#else
#define STREAM_DEBUG stream_debug
#endif

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
            fprintf(STREAM_DEBUG, "[%s] => %s:", module, verbosity);
            vfprintf(STREAM_DEBUG, msg, ap);
            fprintf(STREAM_DEBUG, "\r\n");
        }
    }

    va_end(ap);
}
