// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int x;
int x = 3;
int x;

int main();

void *
foo()
{
	return &main;
}

int
main()
{
	if (x != 3)
		return 0;

	x = 0;
	return x;
}


#include "shared.h"
