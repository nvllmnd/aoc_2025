#pragma once

#include "attributes.h"
#include "error.h"

/// Only configures mimalloc options for now. Will also contain variants for
/// language runtime and compile time options eventually.
typedef enum InitOptions {
  /// Falls through to [InitOptions__Default] for now. Tentative
  InitOptions__None = 0,
  /// sets mi_option_show_errors = true and mi_option_max_errors = 3
  InitOptions__ShowErrors = 1 << 1,
  /// sets mi_option_show_stats = true
  InitOptions__ShowMemoryStatsOnExit = 1 << 2,
  /// sets mimalloc options to verbose, which shows errors and prints memory stats on exit
  InitOptions__Default = InitOptions__ShowMemoryStatsOnExit | InitOptions__ShowErrors,
} InitOptions;

/// Initialize core resources that are needed across our system
/// - Inits our pre-hashed builtin symols
/// - Calls [init_memory] to ensure mimalloc is properly initialized prior to any allocations
RETURNS_ERROR
Error core_init(InitOptions opts);
