// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	short s = 1;
	long l = 1;

	s -= l;
	return s;
}

#include "shared.h"
