// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	int x;
	
	x = 1;
	if ((x << 1) != 2)
		return 1;
	
	return 0;
}

#include "shared.h"
