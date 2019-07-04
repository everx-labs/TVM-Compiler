// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

struct { int a; int b; int c; } s = {1, 2, 3};

int
main()
{
	if (s.a != 1)
		return 1;
	if (s.b != 2)
		return 2;
	if (s.c != 3)
		return 3;

	return 0;
}

#include "shared.h"
