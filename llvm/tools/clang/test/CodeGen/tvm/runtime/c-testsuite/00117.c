// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int main()
{
	int x[] = { 1, 0 };
	return x[1];
}

#include "shared.h"
