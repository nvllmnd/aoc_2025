#pragma once

#include <stddef.h>
#include <stdint.h>

#include "hedley.h"

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef long double f128;
typedef size_t usize;
typedef ptrdiff_t isize;

/// null-terminated string type alias (ment to be used like: str* string)
typedef const char str;

/// Null-terminated string type alias
typedef const char* str_ptr;
/// C-style char buffer pointer type alias
typedef char* cbuf_ptr;
typedef const char* const_cbuf_ptr;




#define CONCAT(a,b) a##b
#define CONCAT3(a, b, c) a##b##c

#define Slice(T) CONCAT(Slice, T)


#define DEFINE_SLICE(T) typedef struct Slice(T) { \
  T* data;\
  i32 len; \
} Slice(T)

typedef u8 Byte;
DEFINE_SLICE(Byte);

typedef const char Str;
DEFINE_SLICE(Str);

typedef char Buf;
DEFINE_SLICE(Buf);

#define slice_new(T, _data, _len) ((Slice(T)){ .data = _data, .len = _len })

#define StaticBuff(N) CONCAT(StaticBuff, N)
#define TStaticBuff(T, N) CONCAT3(T, StaticBuff, N)
  

#define DEFINE_TSTATIC_BUFFER(T, N) typedef struct TStaticBuff(T, N) { T storage[N]; i32 len; } TStaticBuff(T, N)
#define DEFINE_STATIC_BUFFER(N) typedef struct StaticBuff(N) { char storage[N]; i32 len; } StaticBuff(N)


#define UNUSED(var) ((void)var)


#if defined(TODO_STATIC)
#undef TODO_STATIC
#endif

#define TODO_STATIC(msg) static_assert(false, msg)

#if defined(RUNTIME_TRAP)
#undef RUNTIME_TRAP
#endif

#if HEDLEY_HAS_BUILTIN(__builtin_trap)
#define RUNTIME_TRAP() __builtin_trap()
#else
#define RUNTIME_TRAP() (*(HEDLEY_STATIC_CAST(void*, 0)))
#endif

#define MTODO(msg) do { FLOG("%s", msg); } while(0)
#define FTODO(fmt, ...) do { FLOG((fmt), __VA_ARGS__); } while(0)
#define TODO() FTODO("%s not yet implemented!", __FUNCTION__)



#if defined(cast)
#undef cast
#endif

#define cast HEDLEY_STATIC_CAST

