//! Module/Header for all our runtime error codes
#pragma once

typedef enum Error : int {
  /// Ok! No Error!
  Error_Ok = 0,
  /// System is exhausted of all resources.
  Error_SystemOutOfMemory,
  /// First parameter is null when it should not be
  Error_NullParam1,
  /// Second parameter is null when it should not be
  Error_NullParam2,
  /// Third parameter is null when it should not be
  Error_NullParam3,
  /// Fourth parameter is null when it should not be
  Error_NullParam4,
  /// Fifth parameter is null when it should not be
  Error_NullParam5,
  /// Sixth parameter is null when it should not be
  Error_NullParam6,
  /// Seventh parameter is null when it should not be
  Error_NullParam7,
  /// Eigth parameter is null when it should not be
  Error_NullParam8,

  Error_IndexOutOfRange,

  /// Used for when heaps or buffers can not add more elements without
  /// having to request more from system memory
  Error_MemoryFull,
  /// First parameter of a method function is nullptr
  Error_MethodSelfIsNull,
  Error_IOWrite,
  Error_IORead,
  /// a constructor or initialization function was passed paramaters that
  /// were unexpected, as such assume the returned value is zeroed/null/random unint bytes (whatever the functions api
  /// specifies)
  Error_InvalidInitParams,
  Error_Unknown,
  Error_Count,
} Error;

/// Stringifies Error enum variant.
const char* error_string(Error err);

/// Can only be used inside functions that return Error
/// Returns from function early if (expr) != Error_Ok
#define try_err(expr)                               \
  do {                                              \
    const Error _ERROR_ = (expr);                   \
    if (_ERROR_ != Error_Ok) {                      \
      fprintf(stderr, "%s", error_string(_ERROR_)); \
      return _ERROR_;                               \
    }                                               \
  } while (0)

/// Check if expr evaluates to Error_Ok,
/// this macro evaluates is essentially expr == Error_Ok
#define try_is_ok(expr) ({ (expr) == Error_Ok; })

///
/// Can only be used inside functions that return Error
///
/// run expr that returns Error, if expr != Error_Ok,
/// call function with signature:
///
/// ```cpp
/// bool orelse(Error err);
/// ```
///
/// if orelse returns true, error is 'caught' and function
/// continues to run statements after this macro, otherwise
/// when orelse returns false, error value from original expr
/// is returned
///
#define try_catch(expr, orelse)             \
  ({                                        \
    const Error _ERROR_ = (expr);           \
    if (_ERROR_ != Error_Ok) {              \
      const bool recover = orelse(_ERROR_); \
      if (!recover) {                       \
        return _ERROR_;                     \
      }                                     \
    }                                       \
  })

#define bail_on_error(expr, msg) \
  ({                             \
    if (!try_is_ok((expr))) {    \
      FLOG(msg);                 \
    }                            \
  })
