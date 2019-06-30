// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

struct S1 { int x; };
struct S2 { struct S1 s1; };

int
main()
{
	struct S2 s2;
	s2.s1.x = 1;
	return 0;
}

#include "shared.h"
