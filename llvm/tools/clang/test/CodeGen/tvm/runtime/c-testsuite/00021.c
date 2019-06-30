// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
foo(int a, int b)
{
	return 2 + a - b;
}

int
main()
{
	return foo(1, 3);
}


#include "shared.h"
