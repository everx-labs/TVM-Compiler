// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	int x;
	int *p;
	
	x = 0;
	p = &x;
	return p[0];
}

#include "shared.h"
