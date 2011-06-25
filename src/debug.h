/**
 * Macros for debugging, sprinkle these everywhere through your code.
 * Use VERBOSE everywhere, DEBUG most places and ERROR sparingly.
 *
 * Example:
 *
 *     VERBOSE("commander", "I was commanded to do [%i]", cmd)
 *
 * To compile these into your code run `make cleanall` first then use a
 * command such as:
 *
 *     DEBUG_LEVEL=VERBOSE make all
 *
 * to set the debug level and build all.  Defaults to a level of ERROR.
 */

#ifndef debug_h
#define debug_h

#include "uart_common.h"

#if defined VERBOSE_ENABLE
#define VERBOSE(module, msg, ...) fprintf(stream_debug, "[" module "] => VERBOSE: " msg "\n", __VA_ARGS__)
#else
#define VERBOSE(module, msg, ...)
#endif

#if defined VERBOSE_ENABLE || defined DEBUG_ENABLE
#define DEBUG(module, msg, ...)   fprintf(stream_debug, "[" module "] => DEBUG: " msg "\n", __VA_ARGS__)
#else
#define DEBUG(module, msg, ...)
#endif

#if defined VERBOSE_ENABLE || defined DEBUG_ENABLE || defined ERROR_ENABLE
#define ERROR(module, msg, ...)   fprintf(stream_debug, "[" module "] => ERROR: " msg "\n", __VA_ARGS__)
#else
#define ERROR(module, msg, ...)
#endif

#endif /* debug_h */
