// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

#define x f
#define y() f

typedef struct { int f; } S;

int
main()
{
	S s;

	s.x = 0;
	return s.y();
}

#include "shared.h"
