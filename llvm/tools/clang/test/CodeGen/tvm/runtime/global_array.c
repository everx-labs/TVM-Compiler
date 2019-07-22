// #define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %s
int s [] = { 3,1,4,1,5,9,2,6 };

int wrk () {
    int res = 0;
    for (int i = 0; i < sizeof(s)/sizeof(*s); i++)
        res = res*10 + s[i];
    return res;
}

long __attribute__((optnone)) test() {
    if (wrk () != 31415926) {
#ifndef NATIVE
        // CHECK-NOT: custom error
        __builtin_tvm_throw(13);
#endif
        return -1;
    }

    return 0;
}

#ifdef NATIVE
#include <stdio.h>

int main () {
    long res = test();
    printf("Finishing with exit code %d\n", res);
    return (int)res;
}
#endif