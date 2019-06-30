// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
f(int f)
{
	return f;
}

int
main()
{
	return f(0);
}

#include "shared.h"
