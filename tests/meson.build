

lex_parse_exe = executable(
  'lex_and_parse',
  files('lex_parse.c'),
  dependencies: [aoc_dep],
  include_directories: inc,
  install: true,
  c_pch: '../pch/aoc_pch.h'
)

test('parsing, lexing and some lite AST tests', lex_parse_exe)
