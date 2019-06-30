// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

typedef struct { int x; int y; } s;

s v;

int
main()
{
	v.x = 1;
	v.y = 2;
	return 3 - v.x - v.y;
}


#include "shared.h"
