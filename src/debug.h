/**
 * Macros for debugging, sprinkle these everywhere through your code.
 * Use VERBOSE everywhere, DEBUG most places and ERROR sparingly.
 *
 * All these macros should be used
 *
 *     MACRO_NAME(module, format, objs)
 *
 * similar to normal printf with just the module name prepended.
 *
 * Example:
 *     VERBOSE("commander", "I was commanded to do [%i]", cmd)
 *
 * To compile these into your code run `make cleanall` first then use a
 * command such as:
 *
 *     DEBUG_LEVEL=VERBOSE make all
 *
 * to set the debug level and build all.  Defaults to a level of ERROR.
 *
 * To limit the debug output to just one interesting module again you must
 * clean your build with `make cleanall` then run a command such as:
 * 
 *      DEBUG_MODULE=led make all
 *
 * to set the output to only come from the `led` module.  This can be combined
 * with the debug level, e.g.:
 *
 *     DEBUG_LEVEL=VERBOSE DEBUG_MODULE=led make all
 *
 * To use the board-to-board UART for debugging instead of the debug UART use:
 *
 *     DEBUG_UART=board
 *
 */

#ifndef debug_h
#define debug_h

#include <stdarg.h>

#if defined VERBOSE_ENABLE
#define VERBOSE(...) debug("VERBOSE", __VA_ARGS__)
#else
#define VERBOSE(...)
#endif

#if defined VERBOSE_ENABLE || defined DEBUG_ENABLE
#define DEBUG(...) debug("DEBUG", __VA_ARGS__)
#else
#define DEBUG(...)
#endif

#if defined VERBOSE_ENABLE || defined DEBUG_ENABLE || defined ERROR_ENABLE
#define ERROR(...) debug("ERROR", __VA_ARGS__)
#else
#define ERROR(...)
#endif

void debug(char* verbosity, char* module, char* msg, ...);

#endif /* debug_h */
