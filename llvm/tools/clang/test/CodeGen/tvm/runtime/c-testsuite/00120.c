// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

struct {
	enum { X } x;
} s;


int
main()
{
	return X;
}

#include "shared.h"
