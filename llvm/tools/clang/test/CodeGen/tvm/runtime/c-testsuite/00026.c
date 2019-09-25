// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	char *p;
	
	p = "hello";
	return p[0] - 104;
}

#include "shared.h"
