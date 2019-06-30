// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

#if 0 != (0 && (0/0))
   #error 0 != (0 && (0/0))
#endif

#if 1 != (-1 || (0/0))
   #error 1 != (-1 || (0/0))
#endif

#if 3 != (-1 ? 3 : (0/0))
   #error 3 != (-1 ? 3 : (0/0))
#endif

int
main()
{
	return 0;
}

#include "shared.h"
