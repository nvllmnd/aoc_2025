#pragma once
#include <stdarg.h>
#include <stdio.h>

#include "hedley.h"
#include "typedefs.h"

#define PRINTF_IMPL()            \
  va_list args;                  \
  va_start(args, format);        \
  fprintf(stream, format, args); \
  va_end(args);

/// wraps fprintf, forwarding @param stream, @param format and __VA_ARGS__ to fprintf(stream, format, __VA_ARGS__)
HEDLEY_SENTINEL()
static inline void log_stream(FILE* __restrict__ stream, const char* __restrict__ format, ...){PRINTF_IMPL()}

/// wraps fprintf, forwarding @param stream, @param format and __VA_ARGS__ to fprintf(stream, format, __VA_ARGS__),
/// then calls [RUNTIME_TRAP] (__builtin_trap() if available in compiler host build env)
HEDLEY_SENTINEL()
    [[noreturn]] static inline void log_fatal(FILE* __restrict__ stream, const char* __restrict__ format, ...) {
  PRINTF_IMPL()

  RUNTIME_TRAP();
}

HEDLEY_SENTINEL()
static inline void _println(const char* format, ...) {
  FILE* stream = stdout;
  PRINTF_IMPL();
  putchar('\n');
}

HEDLEY_SENTINEL()
static inline void _print(const char* format, ...) {
  FILE* stream = stdout;
  PRINTF_IMPL();
  fflush(stream);
}

#define fprintln(_format, ...) _println(_format, __VA_ARGS__, nullptr)
#define fprint(_format, ...) _print(_format, __VA_ARGS__, nullptr)

#define println(_m) fprintln("%s", _m)
#define print(_m) fprint("%s", _m)

HEDLEY_SENTINEL()
static inline void _eprintln(const char* format, ...) {
  FILE* stream = stderr;
  PRINTF_IMPL();
  putchar('\n');
}

HEDLEY_SENTINEL()
static inline void _eprint(const char* format, ...) {
  FILE* stream = stderr;
  PRINTF_IMPL();
}

#define efprintln(_format, ...) _println(_format, __VA_ARGS__, nullptr)
#define efprint(_format, ...) _print(_format, __VA_ARGS__, nullptr)

#undef PRINTF_IMPL

#define LOG_STREAM stderr

#define FLOG3(stream, fmt, ...) log_fatal(stream, fmt, __VA_ARGS__, nullptr)
#define FLOG2(fmt, ...) log_fatal(LOG_STREAM, fmt, __VA_ARGS__, nullptr)

#define EXPAND(x) x
#define GET_MACRO3(_1, _2, _3, name, ...) name
#define FLOG(...) EXPAND(GET_MACRO3(__VA_ARGS__, FLOG3, FLOG2)(__VA_ARGS__))

#define LOG3(stream, fmt, ...) log_stream(stream, fmt, __VA_ARGS__, nullptr)
#define LOG2(fmt, ...) log_stream(LOG_STREAM, fmt, __VA_ARGS__, nullptr)
#define LOG(...) EXPAND(GET_MACRO3(__VA_ARGS__, LOG3, LOG2)(__VA_ARGS__))

#define ELOG(...) LOG(stderr, __VA_ARGS__)

#define STDLOG(...) LOG(stdout, __VA_ARGS__)
#define DLOG ELOG

#define LOG_ASSERT(condition, fmt, ...) \
  do {                                  \
    if ((condition)) {                  \
      FLOG((fmt), __VA_ARGS__);         \
    }                                   \
  } while (0)

#if defined(NDEBUG)
#define DBG_ASSERT(cond, fmt, ...)

#else

#define DBG_ASSERT(cond, fmt, ...) LOG_ASSERT((cond), (fmt), __VA_ARGS__)

#endif
