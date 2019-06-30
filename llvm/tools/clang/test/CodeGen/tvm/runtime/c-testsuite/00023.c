// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int x;

int
main()
{
	x = 0;
	return x;
}


#include "shared.h"
