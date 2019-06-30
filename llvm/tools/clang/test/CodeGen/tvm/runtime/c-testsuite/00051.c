// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

int x = 0;

int
main()
{
	switch(x)
		case 0:
			;
	switch(x)
		case 0:
			switch(x) {
				case 0:
					goto next;
				default:
					return 1;
			}
	return 1;
	next:
	switch(x)
		case 1:
			return 1;
	switch(x) {
		{
			x = 1 + 1;
			foo:
			case 1:
				return 1;
		}
	}
	switch(x) {
		case 0:
			return x;
		case 1:
			return 1;
		default:
			return 1;
	}
}

#include "shared.h"
