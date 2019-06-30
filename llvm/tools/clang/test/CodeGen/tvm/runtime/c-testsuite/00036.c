// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	int x;
	
	x = 0;
	x += 2;
	x += 2;
	if (x != 4)
		return 1;
	x -= 1;
	if (x != 3)
		return 2;
	x *= 2;
	if (x != 6)
		return 3;
		
	return 0;
}

#include "shared.h"
