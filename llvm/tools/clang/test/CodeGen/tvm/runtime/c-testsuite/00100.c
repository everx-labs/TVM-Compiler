// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
foo(void)
{
	return 0;
}

int
main()
{
	return foo();
}

#include "shared.h"
