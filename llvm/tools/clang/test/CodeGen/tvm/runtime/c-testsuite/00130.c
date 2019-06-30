// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	char arr[2][4], (*p)[4], *q;
	int v[4];

	p = arr;
	q = &arr[1][3];
	arr[1][3] = 2;
	v[0] = 2;

	if (arr[1][3] != 2)
		return 1;
	if (p[1][3] != 2)
		return 1;
	if (*q != 2)
		return 1;
	if (*v != 2)
		return 1;

	return 0;
}

#include "shared.h"
