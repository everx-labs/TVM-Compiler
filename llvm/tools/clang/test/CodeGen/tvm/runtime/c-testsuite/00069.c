// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

#if 0
X
#elif 0
X
#elif 1
int x = 0;
#endif

int
main()
{
	return x;
}

#include "shared.h"
