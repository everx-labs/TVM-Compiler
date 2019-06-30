// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int strlen(char *);

int
main()
{
	char *p;
	
	p = "hello";
	return strlen(p) - 5;
}

#include "shared.h"
