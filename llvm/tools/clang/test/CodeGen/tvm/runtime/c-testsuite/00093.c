// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int a[] = {1, 2, 3, 4};

int
main()
{
	if (sizeof(a) != 4*sizeof(int))
		return 1;
	
	return 0;
}

#include "shared.h"
