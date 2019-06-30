// XFAIL: *
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

double x = 100;

int
main()
{
	return x < 1;
}

#include "shared.h"
