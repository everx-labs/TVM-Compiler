// XFAIL: *
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int a[] = {5, [2] = 2, 3};

int
main()
{
	if (sizeof(a) != 4*sizeof(int))
		return 1;
		
	if (a[0] != 5)
		return 2;
	if (a[1] != 0)
		return 3;
	if (a[2] != 2)
		return 4;
	if (a[3] != 3)
		return 5;
	
	return 0;
}

#include "shared.h"
