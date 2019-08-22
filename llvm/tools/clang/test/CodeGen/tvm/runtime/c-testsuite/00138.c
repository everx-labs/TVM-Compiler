// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h
// XFAIL: *

#define M(x) x
#define A(a,b) a(b)

int
main(void)
{
	char *a = A(M,"hi");

	return (a[1] == 'i') ? 0 : 1;
}

#include "shared.h"
