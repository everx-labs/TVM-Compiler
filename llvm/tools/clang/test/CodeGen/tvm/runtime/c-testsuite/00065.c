// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

#define ADD(X, Y) (X + Y)


int
main()
{
	return ADD(1, 2) - 3;
}

#include "shared.h"
