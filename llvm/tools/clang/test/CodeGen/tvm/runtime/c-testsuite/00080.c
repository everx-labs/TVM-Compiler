// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test_entry_point | FileCheck %S/shared.h

void
voidfn()
{
    return;
}

int
main()
{
    voidfn();
    return 0;
}

#include "shared.h"
