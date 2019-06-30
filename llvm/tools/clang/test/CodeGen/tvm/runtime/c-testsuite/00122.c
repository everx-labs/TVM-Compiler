// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

#define F(a, b) a
int
main()
{
	return F(, 1) 0;
}

#include "shared.h"
