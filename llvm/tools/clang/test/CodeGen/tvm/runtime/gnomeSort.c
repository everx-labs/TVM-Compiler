// XFAIL: *
// #define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h
long ag[] = {1,7,4,3,0,716,74,2,5,9,23,24};

void gnome_sort(long A[], long N) {
    for (long i = 0; i + 1 < N; ++i) {
        if (A[i] > A[i + 1]) {
            long tmp = A[i];
            A[i] = A[i + 1];
            A[i + 1] = tmp;
            if (i != 0)
                i -= 2;
        }
    }
}
long test() {
    long a[] = {1,7,4,3,0,716,74,2,5,9,23,24};
    gnome_sort( a, 12);
    gnome_sort(ag, 12);
    for(long i = 0; i < 12; i++)
        if((i > 0 && a[i] < a[i-1]) || a[i] != ag[i]) {
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