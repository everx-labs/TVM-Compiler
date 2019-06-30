// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int
main()
{
	struct T { int x; };
	{
		struct T s;
		s.x = 0;
		return s.x;
	}
}

#include "shared.h"
