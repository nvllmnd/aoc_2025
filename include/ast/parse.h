//!
//! Parser
//!
//! Input source code as string or filepath and translate into AST
//! TODO: Async/Threaded Parser
//!
#pragma once

#include "error.h"
#include "stb/stb_c_lexer.h"
#include "typedefs.h"

HEDLEY_BEGIN_C_DECLS

#define _____IGNOREME_THIS_IS_FOR_IDE_SYNTAX_HIGHLIGHTING_BUG_XDXD_____
typedef struct ParserCtx ParserCtx;

Error parser_init_file(ParserCtx* self, const char* filepath);

Error parser_init_memory(ParserCtx* self, const char* source);

HEDLEY_END_C_DECLS
