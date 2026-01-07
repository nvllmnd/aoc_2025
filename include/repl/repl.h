#pragma once


/// Placeholder for now
typedef struct AstNode AstNode;

/// Read in source code as a string
/// Output parsed AST
AstNode* repl_read(const char* source);

/// Input Parsed AST
/// Output reduced AST after evaluation
AstNode* repl_eval(AstNode root);

/// Input any AST
/// Output S-Expression String representation of AST
const char* repl_print(AstNode root);



