// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int (*fptr)() = 0;


int
main()
{
	if (fptr)
		return 1;
	return 0;
}


#include "shared.h"
