#pragma once

#include "attributes.h"
#include "error.h"
#include "system/alloc.h"
#include "typedefs.h"

constexpr const i32 KEYWORD_MAX_SIZE = 22;

/// max keyword size is 22 (24, -1 for prefixed length byte, -1 for null terminated byte). Cant imagine us ever needing
/// more than 24 bytes. in fact this can probably be reduced to 8 or 12
typedef struct Keyword Keyword;
/// Keyword packed along with its hash
struct Keyword {
  u64 hash;
  u8 len;
  char name[KEYWORD_MAX_SIZE + 1];
};

typedef struct Keywords Keywords;
struct Keywords {
  Keyword* data;
  isize len;
  isize cap;
};

RETURNS_ERROR
HEDLEY_NON_NULL(1, 2)
Error hash_keywords(const char* __restrict__ input, const char* __restrict__ output);

RETURNS_ERROR
Keyword* init_keywords(Allocator alloc);
