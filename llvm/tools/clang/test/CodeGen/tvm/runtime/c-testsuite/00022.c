// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

typedef int x;

int
main()
{
	x v;
	v = 0;
	return v;
}


#include "shared.h"
