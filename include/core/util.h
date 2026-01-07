#pragma once

#include "system/alloc.h"
#include "typedefs.h"


#if defined(KB1)
#undef KB1
#endif
#if defined(KILOBYTES)
#undef KILOBYTES
#endif

#if defined(MEGABYTES)
#undef MEGABYTES
#endif


#if defined(GIGABYTES)
#undef GIGABYTES
#endif

#if defined(TERABYTES)
#undef TERABYTES
#endif


static constexpr u64 KB1 = 1024;

#define KILOBYTES(n) ((n) * cast(__typeof__(n), KB1))
#define MEGABYTES(n) (KILOBYTES((n)) * cast(__typeof__(n), KB1))
#define GIGABYTES(n) (MEGABYTES((n)) * cast(__typeof__(n), KB1))
#define TERABYTES(n) (GIGABYTES((n)) * cast(__typeof__(n), KB1))


/// @brief Determines length of a null-terminated string
///
/// @param string string to determine length of
/// @param max number of characters to examine
///
/// @details Reads up to @param max characters of @param string, returning
/// early upon encountering a null byte.
/// @returns length of characters up to null byte, or max if no null byte is found
///
FN_PURE
usize stringlen(const char* string, usize max);

FN_PURE
u64 fnv_hash_stringn(const char* string, usize len);

FN_PURE
u64 fnv_hash_string(const char* string);

#define FASTMOD(n, limit) ((n) & ((limit) - 1))

/// Reads a file from @param path to a string.
/// Returned String must be freed by caller
/// Allocates with global allocator using [malloc] (which in our case will be [mi_malloc], since we override [malloc]
/// with [mi_malloc])
const char* read_file_to_string(const char* path);

/// Same as [read_file_to_string] but allocates using input @param alloc
const char* read_file_to_string_in(const char* path, Allocator alloc);
