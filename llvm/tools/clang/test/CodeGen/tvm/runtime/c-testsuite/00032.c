// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	int arr[2];
	int *p;
	
	arr[0] = 2;
	arr[1] = 3;
	p = &arr[0];
	if(*(p++) != 2)
		return 1;
	if(*(p++) != 3)
		return 2;
	
	p = &arr[1];
	if(*(p--) != 3)
		return 1;
	if(*(p--) != 2)
		return 2;
		
	p = &arr[0];
	if(*(++p) != 3)
		return 1;
	
	p = &arr[1];
	if(*(--p) != 2)
		return 1;

	return 0;
}

#include "shared.h"
