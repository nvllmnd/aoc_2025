#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "attributes.h"
#include "hedley.h"

/// Separators for input
static constexpr const char SEPS[] = "\n";

static const char INPUT[] = {
#embed "../../inputs/day1.txt"
};

HEDLEY_NON_NULL(1, 2)
static const char* parse_next(const char* restrict str, const char* restrict sep) {
  str = strpbrk(str, sep);  // find separator
  if (str) {
    str += strspn(str, sep);  // skip separator
  }
  return str;
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  (void)INPUT;

  const char* iter = "A\nB\n";
  do {
    iter = parse_next(iter, SEPS);
    printf("%s", iter);
  } while (iter);

  puts("day1 solution");
  return 0;
}
