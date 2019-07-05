// XFAIL: *
//#define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h
long ag[] = {1,3,2,4,9,8,7,5,6,11,10,0};

void bucketSort(long array[], long n) {
    long i, j;
    long count[n];
    for(i = 0; i < n; i++)
        count[i] = 0;
    for(i=0; i < n; i++)
        (count[array[i]])++;
    for(i=0,j=0; i < n; i++)
        for(; count[i]>0;(count[i])--) {
            array[j++] = i;
        }
}
long test() {
    long a[] = {1,3,2,4,9,8,7,5,6,11,10,0};
    bucketSort(a,12);
    bucketSort(ag,12);
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