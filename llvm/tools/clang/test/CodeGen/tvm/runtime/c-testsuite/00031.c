// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
zero()
{
	return 0;
}

int
one()
{
	return 1;
}

int
main()
{
	int x;
	int y;
	
	x = zero();
	y = ++x;
	if (x != 1)
		return 1;
	if (y != 1)
		return 1;
	
	x = one();	
	y = --x;
	if (x != 0)
		return 1;
	if (y != 0)
		return 1;
	
	x = zero();
	y = x++;
	if (x != 1)
		return 1;
	if (y != 0)
		return 1;
	
	x = one();
	y = x--;
	if (x != 0)
		return 1;
	if (y != 1)
		return 1;
	
	return 0;
}

#include "shared.h"
