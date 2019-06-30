// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	int x;
	
	x = 1;
	x = x | 4;
	return x - 5;
}


#include "shared.h"
