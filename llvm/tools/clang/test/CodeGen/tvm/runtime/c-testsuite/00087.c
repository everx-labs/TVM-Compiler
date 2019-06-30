// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

struct S
{
	int	(*fptr)();
};

int
foo()
{
	return 0;
}

int
main()
{
	struct S v;
	
	v.fptr = foo;
	return v.fptr();
}


#include "shared.h"
