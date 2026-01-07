#pragma once

#include "attributes.h"
#include "typedefs.h"

constexpr const usize SMALL_SIZE_MAX = 1024;  // MI_SMALL_SIZE_MAX;
struct mi_heap_s;

//
typedef struct mi_heap_s* Heap;
//
// #define heap_new mi_heap_new
// #define heap_new_ex mi_heap_new_ex
// #define smalloc mi_malloc_small
// #define heap_smalloc mi_heap_malloc_small
// #define heap_malloc mi_heap_malloc_aligned
// #define heap_calloc mi_heap_calloc_aligned
// #define heap_realloc mi_heap_realloc_aligned
// #define heap_recalloc mi_heap_recalloc_aligned
// #define heap_destroy mi_heap_destroy
// #define heap_delete mi_heap_delete
//
#define VT_ALLOC_FN_NAME(name) vtable_##name##_alloc_fn

#define VT_ZALLOC_FN_NAME(name) vtable_##name##_alloc_zeroed_fn

#define VT_EXPAND_FN_NAME(name) vtable_##name##_expand_fn
#define VT_REALLOC_FN_NAME(name) vtable_##name##_realloc_fn

#define VT_REZALLOC_FN_NAME(name) vtable_##name##_realloc_zeroed_fn

#define VT_STRDUP_FN_NAME(name) vtable_##name##_strdup_fn
#define VT_DEALLOC_FN_NAME(name) vtable_##name##_dealloc_fn

#define ALLOC_SIG (void* ctx, usize size, usize align)
#define ZALLOC_SIG ALLOC_SIG
#define EXPAND_SIG (void* ctx, void* ptr, usize newsize)

#define REALLOC_SIG (void* ctx, void* ptr, usize newsize, usize align)
#define REZALLOC_SIG REALLOC_SIG
#define STRDUP_SIG (void* ctx, const char* string, usize n)
#define DEALLOC_SIG (void* ctx, void* ptr)

#define DEFINE_ALLOC_FN(name) static void* VT_ALLOC_FN_NAME(name) ALLOC_SIG
#define DEFINE_ZALLOC_FN(name) static void* VT_ZALLOC_FN_NAME(name) ZALLOC_SIG
#define DEFINE_EXPAND_FN(name) static void* VT_EXPAND_FN_NAME(name) EXPAND_SIG
#define DEFINE_REALLOC_FN(name) static void* VT_REALLOC_FN_NAME(name) REALLOC_SIG
#define DEFINE_REZALLOC_FN(name) static void* VT_REZALLOC_FN_NAME(name) REZALLOC_SIG
#define DEFINE_STRDUP_FN(name) static char* VT_STRDUP_FN_NAME(name) STRDUP_SIG
#define DEFINE_DEALLOC_FN(name) static void VT_DEALLOC_FN_NAME(name) DEALLOC_SIG

typedef void*(*AllocBytesFn)ALLOC_SIG;
typedef void*(*AllocZeroedFn)ZALLOC_SIG;
typedef void*(*AllocExpandFn)EXPAND_SIG;
typedef void*(*AllocReallocFn)REALLOC_SIG;
typedef void*(*AllocRezallocFn)REZALLOC_SIG;
typedef char*(*AllocStrDupFn)STRDUP_SIG;
typedef void(*AllocDeallocFn) DEALLOC_SIG;

typedef struct AllocVTable AllocVTable;

/// A zig-inspired Allocator Interface
typedef struct Allocator Allocator;
struct Allocator {
  /// type-erased pointer to Allocator state. Maybe null
  /// in case of stateless allocator
  void* ctx;

  /// Constant (non-owning) pointer to allocation functions vtable.
  /// Prefer to make a const(expr) static instance of each variety/implementation
  /// of any Allocator's Vtable.
  /// The idea is to keep all vtables constant and static to hopefully end up with
  /// any dynamic dispatch optimized away and/or function pointer calls inlined, ect...
  ///
  /// For reference: [alloc_vtable_noop_instance], [alloc_vtable_gpa_instance], [alloc_vtable_string_heap_instance],
  /// ect...
  ///
  const AllocVTable* vt;
};

struct AllocVTable {
  AllocBytesFn alloc_bytes;
  AllocZeroedFn alloc_zeroed;
  /// Attempt to expand memory at ptr in-place. @see [mi_expand]
  AllocExpandFn expand;
  AllocReallocFn realloc_bytes;
  AllocRezallocFn realloc_zeroed;
  AllocStrDupFn strdup;
  AllocDeallocFn dealloc;
};

/// Returns pointer to a const(expr) static instance of
/// an [AllocVTable] with all function pointer fields pointing to a noop function
FN_CONST
const AllocVTable* alloc_vtable_noop_instance(void);

FN_CONST
const AllocVTable* alloc_vtable_global_malloc_instance(void);

FN_CONST
const AllocVTable* alloc_vtable_heap_instance(void);

// #ifndef EXPAND
// #define EXPAND(x) x
// #endif

// #define alloc_vtable_new4(ab, ex, rea, fr) ((AllocVTable){ .alloc_bytes = &(ab), .expand = &(ab), . })
// #define alloc_vtable_new3()
// #define alloc_vtable_new2()
// #define alloc_vtable_new1()

// #define GET_MACRO4(_1, _2, _3, _4, name, ...) name

// #define alloc_vtable_new(...) EXPAND(GET_MACRO4(__VA_ARGS__,alloc_vtable_new4, alloc_vtable_new3, alloc_vtable_new2,
// alloc_vtable_new1)(__VA_ARGS__))

FN_PURE
Allocator allocator_heap_new(Heap heap);

FN_CONST
Allocator allocator_noop_new(void);

FN_CONST
Allocator allocator_global_new(void);

#define allocator_new(state, static_vtable) ({ ((Allocator){.ctx = cast(void*, (state)), .vt = (static_vtable)}); })

#define allocator_stateless_new(static_vtable) allocator_new(nullptr, (static_vtable))

HEDLEY_MALLOC
static inline void* allocator_alloc_bytes(Allocator self, usize size, usize align) {
  return self.vt->alloc_bytes(self.ctx, size, align);
}

HEDLEY_MALLOC
static inline void* allocator_alloc_zeroed(Allocator self, usize size, usize align) {
  return self.vt->alloc_zeroed(self.ctx, size, align);
}

static inline void* allocator_expand(Allocator self, void* ptr, usize newsize) {
  return self.vt->expand(self.ctx, ptr, newsize);
}

static inline void* allocator_realloc_bytes(Allocator self, void* ptr, usize newsize, usize align) {
  return self.vt->realloc_bytes(self.ctx, ptr, newsize, align);
}

static inline void* allocator_realloc_zeroed(Allocator self, void* ptr, usize newsize, usize align) {
  return self.vt->realloc_zeroed(self.ctx, ptr, newsize, align);
}

static inline void* allocator_strdup(Allocator self, const char* string, usize len) {
  return self.vt->strdup(self.ctx, string, len);
}

#define allocator_context_as(T, alloc) (cast(T*, alloc.ctx))

static inline void allocator_dealloc(Allocator self, void* ptr) { self.vt->dealloc(self.ctx, ptr); }

#define VTABLE_NEW(name)                           \
  ((AllocVTable){                                  \
      .alloc_bytes = VT_ALLOC_FN_NAME(name),       \
      .alloc_zeroed = VT_ZALLOC_FN_NAME(name),     \
      .expand = VT_EXPAND_FN_NAME(name),           \
      .realloc_bytes = VT_REALLOC_FN_NAME(name),   \
      .realloc_zeroed = VT_REZALLOC_FN_NAME(name), \
      .strdup = VT_STRDUP_FN_NAME(name),           \
      .dealloc = VT_DEALLOC_FN_NAME(name),         \
  })

/// Allocate a null-terminated c-style string of length @param len + 1 in a string-only heap
/// To allocate a string in a different allocator, use [alloc_string_in]
HEDLEY_MALLOC
char* alloc_string(usize len);

/// Same as [alloc_string] but allocates @param len + 1 bytes using @param alloc
HEDLEY_MALLOC
char* alloc_string_in(usize len, Allocator alloc);

HEDLEY_MALLOC
char* alloc_strndup(const char* string, usize n);

HEDLEY_MALLOC
char* alloc_strdup(const char* string);

HEDLEY_MALLOC
char* alloc_strndup_in(const char* string, usize n, Allocator alloc);

HEDLEY_MALLOC
char* alloc_strdup_in(const char* string, Allocator alloc);
