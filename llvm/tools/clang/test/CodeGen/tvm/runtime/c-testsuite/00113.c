// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h
// XFAIL: *
// TODO: We don't support float - should we generate correct error here?

int
main()
{
	int a = 0;
	float f = a + 1;

	return f == a;
}

#include "shared.h"
