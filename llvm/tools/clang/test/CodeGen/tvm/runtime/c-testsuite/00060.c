// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

// line comment

int
main()
{
	/*
		multiline
		comment
	*/
	return 0;
}

#include "shared.h"
