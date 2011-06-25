/**
 * Macros for debugging, sprinkle these everywhere through your code.
 * Use VERBOSE everywhere, DEBUG most places and ERROR sparingly.
 *
 * To compile these into your code run `make cleanall` first then use a
 * command such as:
 *
 *     DEBUG_LEVEL=VERBOSE make all
 *
 * to set the debug level and build all.  Defaults to a level of ERROR.
 */

#if defined VERBOSE_ENABLE
#define VERBOSE(module, msg)          fprintf(&stream_debug, "[" + module + "] => VERBOSE: " + msg + "\n")
#define VERBOSE(module, msg, objs...) fprintf(&stream_debug, "[" + module + "] => VERBOSE: " + msg + "\n", objs)
#else
#define VERBOSE(module, msg)
#define VERBOSE(module, msg, objs...)
#endif

#if defined VERBOSE_ENABLE || defined DEBUG_ENABLE
#define DEBUG(module, msg)            fprintf(&stream_debug, "[" + module + "] => DEBUG: " + msg + "\n")
#define DEBUG(module, msg, objs...)   fprintf(&stream_debug, "[" + module + "] => DEBUG: " + msg + "\n", objs)
#else
#define DEBUG(module, msg)
#define DEBUG(module, msg, objs...)
#endif

#if defined VERBOSE_ENABLE || defined DEBUG_ENABLE || defined ERROR_ENABLE
#define ERROR(module, msg)            fprintf(&stream_debug, "[" + module + "] => ERROR: " + msg + "\n")
#define ERROR(module, msg, objs...)   fprintf(&stream_debug, "[" + module + "] => ERROR: " + msg + "\n", objs)
#else
#define ERROR(module, msg)
#define ERROR(module, msg, objs...)
#endif
