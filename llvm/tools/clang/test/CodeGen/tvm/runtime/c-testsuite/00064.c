// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

#define X 6 / 2

int
main()
{
	return X - 3;
}

#include "shared.h"
