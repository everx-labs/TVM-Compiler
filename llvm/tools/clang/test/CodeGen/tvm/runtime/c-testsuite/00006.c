// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	int x;

	x = 50;
	while (x)
		x = x - 1;
	return x;
}

#include "shared.h"
