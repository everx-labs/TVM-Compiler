// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
  int c;
  c = 0;
  do
    ;
  while (0);
  return c;
}

#include "shared.h"
