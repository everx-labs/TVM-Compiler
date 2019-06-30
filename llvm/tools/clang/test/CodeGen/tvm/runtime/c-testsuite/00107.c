// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

typedef int myint;
myint x = (myint)1;

int
main(void)
{
	return x-1;
}

#include "shared.h"
