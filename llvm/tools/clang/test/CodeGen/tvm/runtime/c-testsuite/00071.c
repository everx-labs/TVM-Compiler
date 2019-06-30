// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

#define X 1
#undef X

#ifdef X
FAIL
#endif

int
main()
{
	return 0;
}

#include "shared.h"
