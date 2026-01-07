// SPDX-FileCopyrightText: 2025 Matthew McDade <nvllmnd@pm.me>
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#if defined(__cplusplus)
#error __FILE__ is not compatible with C++. use your fancy Destructors instead!!
#else

#include <stdio.h>
#include <stdlib.h>

#include "typedefs.h"
#define AUTOFREE_NAME(T) /** T* destructor name for type T */ free_##T

#define AUTOFREE_PTR_TYPEDEF(T) /** Name of typedef for T* */ T##Ptr

#define AUTOFREE_PTR_FN_NAME(                                                                     \
    T) /** Name of dtor for the pointer to pointer to type; Destructor Pointer Destructor Name */ \
  CONCAT3(free_, T, _ptr)

#define AUTOFREE_FN(T) /** signature for destructor for type T */ void AUTOFREE_NAME(T)(__typeof__(T*) self)

#define AUTOFREE_PTR_FN(T) /** signature for destructor for type T* */ \
  void AUTOFREE_PTR_FN_NAME(T)(__typeof__(T**) self)

#define AUTOFREE_FN_IMPL(T) /** generates a dtor that simply calls free on (pointer to) T  */ \
  AUTOFREE_FN(T) { free(self); }

#define AUTOFREE_IMPL_BEGIN(T)                 \
  \ /** Beginning chunk of  dtor for type T */ \
      AUTOFREE_FN(T) {
#define AUTOFREE_IMPL_END               \
  \ /** End chunk of dtor for type T */ \
      free(self);                       \
  }

#define _AUTOFREE_PTR_FWD_IMPL(T) \
  AUTOFREE_PTR_FN(T) {            \
    AUTOFREE_NAME(T)(*self);      \
    *self = nullptr;              \
  }

#define AUTOFREE_PTR_FWD_IMPL(T) /** dtor impl for type T* that forwards to dtor for type T */ _AUTOFREE_PTR_FWD_IMPL(T)

#define AUTOFREE_PTR_FWD_IMPL_BEGIN(                                                                                  \
    T) /** begin chunk of dtor impl for type T* that forwards to dtor for type T, as well as sandwitched user code */ \
  AUTOFREE_PTR_FN(T) {                                                                                                \
    AUTOFREE_NAME(T)(*self);

#define AUTOFREE_PTR_FWD_IMPL_END }

#define AUTOFREE_IMPL(T) AUTOFREE_FN_IMPL(T) AUTOFREE_PTR_FWD_IMPL(T)

#define AUTOFREE_DTOR_BEGIN(T) AUTOFREE_FN(T)
#define AUTOFREE_DTOR_END(T) AUTOFREE_PTR_FWD_IMPL(T)

// TODO: Should probably have some #ifs or something to check that compiler supports __attribute__((cleanup(func))),
// so more friendly errors are reported with compilers that do not support cleanup (like MVCC for instance, grrr
// micro$hit)
#define AUTOFREE_CLEANUP_ATTR(                                                                 \
    dtor) /** attribute that decorates variable decl/def, passing dtor to cleanup attribute */ \
  __attribute__((cleanup(dtor)))

#define AUTOFREE_DECL(T) /** Type declaration for a variable that will run a cleanup function upon going out of scope. \
          ex:                                                                                                          \
          ```cpp                                                                                                       \
          {                                                                                                            \
            // Make sure you call DTOR_FN(ScopedArena) (or DTOR_SIMPLE_IMPL) before the following line will compile    \
            //                                                                                                         \
            DEFER(ScopedArena) arena = {};                                                                             \
            // some other ish...                                                                                       \
                                                                                                                 \     \
                                                                                                                 j     \
                                                                                                                 \     \
            // arena cleanup function gets ran here                                                                    \
          }                                                                                                            \
                                                                                                                 \     \
          ```                                                                                                          \
                                                                                                               \       \
 */                                                                                                                    \
  T AUTOFREE_CLEANUP_ATTR(AUTOFREE_NAME(T))

#define AUTOFREE_PTR_DECL(                                                                          \
    T) /** Same as DEFER, but for destructors for pointers to pointers to T (T**) use DTOR_PTR_* */ \
  T* AUTOFREE_CLEANUP_ATTR(AUTOFREE_PTR_FN_NAME(T))

#define AUTOFREE_SCOPED_IMPL(                                                                                         \
    T) /** same as DTOR_IMPL, but calls a special cleanup function pointer field on type T. does not work if T is not \
          a struct and does not have a cleanup field. see SCOPED_RESOURCE macro */                                    \
  AUTOFREE_FN(T) { self->cleanup(self); }
#define AUTOFREE_PTR_SCOPED_IMPL(T) /** same as DTOR_SCOPED_IMPL, but operates on pointers to pointers to T (T**) */ \
  AUTOFREE_PTR_FN(T) { (*self)->cleanup(*self); }

#define AUTOFREE_SCOPED_BEGIN(                                                                       \
    T) /** Same as DTOR_SCOPED_IMPL, but acts as a help for users to define their own scoped dtor */ \
  AUTOFREE_FN(T) {                                                                                   \
    self->cleanup(self);
#define AUTOFREE_SCOPED_END /** End DTOR_SCOPED_BEGIN */ }

#define AUTOFREE_PTR_SCOPED_BEGIN(                                                        \
    T) /** Same as DTOR_SCOPED_BEGIN, but operates on pointers to pointeres to T (T**) */ \
  AUTOFREE_PTR_FN(T) {                                                                    \
    (*self)->cleanup(*self);
#define AUTOFREE_PTR_SCOPED_END /** End DTR_PTR_SCOPED_BEGIN */ \
  free(*self);                                                  \
  *self = nullptr;                                              \
  }

// ALIASES
#define autofree AUTOFREE_DECL
#define autofree_ptr AUTOFREE_PTR_DECL
#define DESTRUCT autofree
#define DESTRUCT_PTR autofree_ptr

// Use to easily make structs have a cleanup callback function pointer field t hat will be executed when
// said variable goes out of scope
#define AUTOFREE_METHOD(T) void (*cleanup)(__typeof__(T*))
#define AUTOFREE_METHOD_VOID AUTOFREE_METHOD(void)

#define AUTOFREE_PTR_WRAP_WITH_FREER(T, _freer) \
  typedef typeof_unqual(T) * T##PtrWrap;        \
  AUTOFREE_FN(T) { _freer(*self); }

#define AUTOFREE_PTR_WRAP(T) AUTOFREE_PTR_WRAP_WITH_FREER(T, free)

#endif
