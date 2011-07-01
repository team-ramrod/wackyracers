#include "debug.h"
#include "uart_common.h"
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <util/atomic.h>

#define DEBUG_UART debug

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
            fprintf(stream_DEBUG_UART, "[%s] => %s:", module, verbosity);
            vfprintf(stream_DEBUG_UART, msg, ap);
            fprintf(stream_DEBUG_UART, "\n");
        }
    }

    va_end(ap);
}
