//! Macros and functions related to typed slices of memory
//! Sl is short for (Sl)ice (Slice).
//!
//! Types are prefixed with 'Sl' to say a type is a generic variant of
//! this modules defnition of Slice
//!
//!
#pragma once

#include "typedefs.h"

#if defined(__cplusplus)
#else

#define Slice(T) Sl##T

#define SL_ALIAS(T, aliased) typedef Slice(T) Slice(aliased)

#define struct_Slice(T)             \
  typedef struct Slice(T) Slice(T); \
  struct Slice(T) {                 \
    T* data;                        \
    i32 len;                        \
  }

// =====================================
// =         Common Definitions        =
// =====================================
typedef u8 Byte;
struct_Slice(Byte);

typedef const char ConstChar;
struct_Slice(ConstChar);
SL_ALIAS(ConstChar, String);

#define slice_new(T, ...) \
  (Slice(T)) { .data = nullptr, .len = 0, __VA_ARGS__ }

struct_Slice(int);

#endif
