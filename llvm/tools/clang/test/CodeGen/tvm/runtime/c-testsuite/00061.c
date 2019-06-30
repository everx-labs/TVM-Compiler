// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

#define FOO 0

int main()
{
	return FOO;
}


#include "shared.h"
