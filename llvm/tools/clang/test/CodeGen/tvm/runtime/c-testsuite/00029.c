// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	int x;
	
	x = 1;
	x = x ^ 3;
	return x - 2;
}


#include "shared.h"
