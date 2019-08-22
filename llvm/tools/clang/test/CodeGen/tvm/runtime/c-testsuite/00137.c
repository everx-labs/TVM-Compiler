// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h
// XFAIL: *

#define x(y) #y

int
main(void)
{
	char *p;
	p = x(hello)  " is better than bye";

	return (*p == 'h') ? 0 : 1;
}

#include "shared.h"
