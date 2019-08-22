// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	int x, *p;

	if (sizeof(0) != 1)
		return 1;
	if (sizeof 0 != 1)
		return 1;
	if (sizeof(char) != 1)
		return 1;
	if (sizeof(int) != 1)
		return 1;
	if (sizeof(&x) != sizeof p)
		return 1;
	return 0;
}

#include "shared.h"
