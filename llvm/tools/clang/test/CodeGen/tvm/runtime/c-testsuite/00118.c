// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	struct { int x; } s = { 0 };
	return s.x;
}

#include "shared.h"
